#ifndef GESTIONOPTION_H
#define GESTIONOPTION_H

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <stdlib.h>     /* atof */

#include "fonction_string.hpp"
#include "outils.hpp"
#include "convertion.hpp"


namespace testSFML {

class gestion_option
{

    private :

        struct IdValeur {
            std::string id;
            std::string valeur;

            inline bool operator == (const IdValeur& other) const
            {
                return id == other.id;
            }

            inline bool operator == (const std::string & o_id) const
            {
                return o_id == id;
            }
            IdValeur (std::string unId, std::string uneValeur) : id(unId),valeur(uneValeur) {}
        };

        const std::string separateur = "=";
        const std::string commentaire= "#";

        std::string nomProgramme;
        size_t nbArg;

        // si l'utilisateur a fait --help
        bool helpNeeded=false;
        // si l'utilisateur a fait --generate
        bool generate_file = false;
		// on ne peut ajouter des params uniquement avant d'avoir commencer
		// a lire les valeurs
		bool can_add_param = true;
		void check_can_add () const {
			if ( ! can_add_param )
			{
				std::cerr << "On ne peut pas faire de .add(..) apres avoir fait un .get_val, utiliser get_val_unsafe si besoin" << std::endl;
				exit(5);
			}
		}
        
        std::string message_aide,message_fichier;


        std::vector<IdValeur> vec_args_named;
        std::vector<std::string> vec_args_raw;

        std::vector<std::pair<std::string,std::string>> vec_option_default, vec_option_fichier;
        std::vector<std::string> vec_option;

        /// TODO : le fair pour autre chose que des strings
        std::map < std::string , std::vector < std::string >> map_option_valide_value;

        bool hasSeparateur (std::string param)
        {
            return param.find(separateur) != std::string::npos;
        }

        std::string ligne_sans_commentaire (const std::string ligne)
        {
            return decoupe(ligne,commentaire)[0];
        }

        void inline erreur(const char* message) const  { erreur(std::string(message)); }
        void inline erreur(const std::string message) const
        {
            std::cerr << message << std::endl;
            std::cerr << usage() <<std::endl;
            exit(5);
        }

        inline void check_help() const
        {
            if (helpNeeded) { std::cerr << usage() << std::endl; exit(5) ;}
        }

        //fonction qui empeche de croire qu'on a mis un param alors qu'on a fait une faute de frappe :D
        inline void check_typo() const {
            
            // check des option 
            for (auto opt : vec_args_named ) {

               auto if_defaut = std::find_if(vec_option_default.begin(), vec_option_default.end() , [&](auto paire) { return paire.first == opt.id ;} );
               if ( if_defaut == vec_option_default.end() &&
                    std::find(vec_option.begin(),vec_option.end(),opt.id) == vec_option.end() )
               {
                   erreur ("L'option \"" + opt.id + "\" n'est pas prévu par le programme.");
               }
            }
            
            // check du fichier
            for (auto opt : vec_option_fichier ) {

               auto if_defaut = std::find_if(vec_option_default.begin(), vec_option_default.end() , [&](auto paire) { return paire.first == opt.first ;} );
               if ( if_defaut == vec_option_default.end() &&
                    std::find(vec_option.begin(),vec_option.end(),opt.first) == vec_option.end() )
               {
                   erreur ("L'option \"" + opt.first + "\" n'est pas prévu par le programme.");
               }
            }
        }
        
        // pour les options ayant une range
    
        // un peut comme help on peut taper --generate pour generer des fichiers de param 
        // avec les valeurs par defaut
        inline void generate () const {
            if ( generate_file ) {
                std::ofstream flux (nomProgramme+".param");
                auto make_commentaire = [&](const std::string & ligne) 
                {
                    flux << "#  " << ligne << std::endl;  
                };
                make_commentaire ("fichier auto generé");
                make_commentaire (" vous pouvez l'utiliser en ajoutant l'option suivante a votre ligne de commande");
                make_commentaire (" --load-file="+nomProgramme+".param");
                make_commentaire ("");
                make_commentaire ("Option par defaut du programme :");
                for (const auto& pair : vec_option_default )
                {
                    flux << pair.first << "=" <<pair.second << std::endl;
                }
                make_commentaire ("Option sans valeur par defaut ( à décomenter et compléter ):");
                for (const auto& s : vec_option )
                {
                    make_commentaire(s+"=");
                }
                std::cerr << "Generation du fichier " << nomProgramme+".param" << std::endl;
                exit(5);
            }
            
        }

    public :
        
        inline std::string get_separateur () const { return separateur; }

        gestion_option (int argc, char** argv, std::string sep="=") : separateur(sep), vec_args_raw(), vec_option_default (), vec_option()
        {
            if ( argc < 1 ) {
                std::cerr << "Un programme doit avoir au moins un param : son nom. Je sais pas ce que vous avez fait, mais faites pas ca ." << std::endl;
                exit(5);
            }

            if ( separateur.empty() ) {
                std::cerr << "Séparateur vide" << std::endl;
                exit(5);
            }
            
            // on sort le nom du programme des options
            nbArg = (size_t)argc-1; // on peut faire la convertion grace au if du dessus
            nomProgramme = argv[0];

            // Pour toutes les options
            for (int i = 1 ; i < argc ; i ++ )
            {
                std::string param (argv[i]);
                if (param == "--help" ) { helpNeeded = true; continue;}
                if (param == "--generate" ) { generate_file= true; continue;}

                if ( hasSeparateur(param) )
                {
                    auto option_temps = decoupe(param,separateur);
                    if (option_temps[0]== "--load-file") 
                    {
                        load_file(option_temps[1],true); // si le fichier n'existe pas on prévient l'user 
                    }
                    else
                    {
                        vec_args_named.push_back({option_temps[0],option_temps[1]});
                    }
                }
                else // si c'est un param brute
                {
                    vec_args_raw.push_back(param);
                }
            }

        };
        
        // ne retourne pas d'erreur si le fichier n'existe pas, si ça se trouve c'est volontaire et 
        // les options sont deja passé en param
        inline void load_file (const std::string& file, bool assert_file_valide = false)
        {
            std::ifstream flux(file);
            if (flux)
            {
                std::string ligne;
                while (std::getline(flux,ligne))
                {
                    ligne = ligne_sans_commentaire (ligne);
                    if ( hasSeparateur ( ligne) )
                    {
                        auto option_temps = decoupe(ligne,separateur);
                        vec_option_fichier.push_back({option_temps[0],option_temps[1]});
                    }
                }
                
                // gestion du message de fichier
                if ( message_fichier.empty() )
                {
                    message_fichier += "Chargement des options depuis le(s) fichier(s) : " + file +" ";
                }
                else {
                    message_fichier += file + " ";
                }
            }
            else if ( assert_file_valide )
            {
               std::cerr << "Le fichier d'option " << file << " n'est pas un fichier valide." << std::endl;
               exit(4);
            }
        }

        inline void min_raw_args(size_t min) const 
        {

            if ( vec_args_raw.size() < min )
            {
                erreur ("Erreur : Le programme prend au moins " + std::to_string(min) + " parametres sans noms" );
            }

        }

        inline void nb_raw_args_equal(size_t val) const { nb_raw_args_between(val,val); }

        inline void nb_raw_args_between (size_t min, size_t max) const {
            if ( nbArg < min || nbArg > max ) {
                if ( min == max ){
                     erreur ("Erreur : Le programme prend exactement " + std::to_string(min) + " parametres sans noms" );
                }
                else
                {
                    erreur ("Erreur : Le programme prend entre " + std::to_string(min) + " et " + std::to_string(max) + " parametres sans noms" );
                }
            }
        }

        template<class T = std::string>
        inline T get_raw_val(size_t id){
            check_help();
            generate ();
            if ( id < vec_args_raw.size() ){
                erreur("vous avez demandez l'indice " + std::to_string(id) + " alors qu'il n'y a que " + std::to_string(vec_args_raw.size()) + " params");
            }
            return convert_to<T>(vec_args_raw[id]);
        }

        inline void allow_raw_args(bool allow)
        {
            if (not allow and vec_args_raw.size() > 0 )
            {
                erreur("Le programme ne prend pas de parametre libre" );
            }
        }

        inline std::vector<std::string> get_raw_args (){
            check_help();
            generate ();
            return vec_args_raw;
        }

        inline void mustBeValideFile(const std::vector<std::string>& listTag) const
        {
            bool ok = true;
            std::string fichiers_fails = "";
            for(const auto& tag : listTag )
            {
                 auto path = get_val_unsafe(tag);
                 if ( ! file_exists(path) )
                 {
                     ok = false;
                     fichiers_fails += path + " ";
                 }

            }
            if ( not ok )
            {
                erreur("Le(s) fichier(s) : " + fichiers_fails +"ne sont pas valides");
            }
        }

        template<class T,
                 class U = typename std::enable_if< !is_string<T>::value >::type,
                 class V = decltype(std::to_string(std::declval<T>())) >
        inline void add(const std::string& option, const T& defaultVal)
        {
			check_can_add ();
            vec_option_default.push_back({option,std::to_string(defaultVal)});
        }
        
        inline void add(const std::string& option, const std::string& defaultVal)
        {
			check_can_add ();
            vec_option_default.push_back({option,defaultVal});
        }
        
        inline void add(const std::string& option)
        {
			check_can_add ();
            vec_option.push_back(option);
        }
        
        template<   class Conteneur,
                    class = std::enable_if_t<is_container<Conteneur>::value> 
                 >
        inline void add (const std::string& option, const Conteneur& conteneur)
        {
            static_assert(is_string<decltype(*conteneur.begin())>::value , "La version actuelle ne prend que des strings :/");
            check_can_add ();
            vec_option.push_back(option);
            map_option_valide_value[option].insert  (   map_option_valide_value[option].begin(),
                                                        conteneur.begin(),
                                                        conteneur.end()
                                                    );
        }

        
        // permet d'avoir une valeur avant la fin de la lecture des params
        template<class T = std::string>
        inline T get_val_unsafe(const std::string& id) const {
			check_help();
            generate ();

            // est ce qu'on a l'option dans les param ? 
            auto it = std::find(vec_args_named.begin(), vec_args_named.end(), id);
            if ( it != vec_args_named.end() ) 
            {
                
                // si oui il a la priorité 
                return convert_to<T>(it->valeur);
            }
            
            // si non, est ce qu'il est dans le fichier ? 
            auto it_fichier = std::find_if(vec_option_fichier.begin(), vec_option_fichier.end() , [&](auto paire) { return paire.first == id ;} );
            if ( it_fichier!= vec_option_fichier.end() ) {
                return convert_to<T>(it_fichier->second);
            }
            
            // si toujours pas on regarde les options par defaut
            auto if_defaut = std::find_if(vec_option_default.begin(), vec_option_default.end() , [&](auto paire) { return paire.first == id ;} );
            // si on trouve pas on affiche une erreur
            if ( if_defaut == vec_option_default.end() ) {
                // on affiche le message d'erreur qui va bien
                if (std::find(vec_option.begin(),vec_option.end(),id) != vec_option.end()) {
                    erreur("L'option " +id+" n'a pas de valeur par defaut est n'est pas présente");
                }
                else{
                    erreur("Le programme demande une option qui n'est pas dans les options prévue par le programme... Dev tu t'es chier..");
                }
            }
            
            // si on arrive la c'est qu'on a trouvé donc on est safe,
            // j'ai du inverser la logique pour que le compilateur ne stress pas de ne pas avoir de retour;
            return convert_to<T>(if_defaut->second);
		}
        
        template<class T = std::string>
        inline T get_val(const std::string& id)  {
			check_typo();
			can_add_param =false;
			return get_val_unsafe<T>(id);
            
        }

        inline std::string usage() const {
            std::string retour =  "Usage : \n./"+get_name_file(nomProgramme)+" ";
            for (auto& opt : vec_option)
            {
                retour += opt+"=valeur ";
            }

            for (auto& opt : vec_option_default)
            {
                retour += opt.first+"=valeur (default:"+opt.second+") ";
            }
            retour += "\n";
            retour +=  message_fichier;
            retour +=  message_aide;
            retour += "\nl'ordre de priorité des options est : Ligne de commande > fichier > option par defaut \n";
            return retour;
        }

        inline void set_message (const std::string& mes) { message_aide = mes;}




};
}
#endif // GESTIONOPTION_H


