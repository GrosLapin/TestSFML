#ifndef GESTIONOPTION_H
#define GESTIONOPTION_H

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <stdlib.h>     /* atof */

#include "outils.hpp"

#include "fonction_conteneur.hpp"


#include "convertion.hpp"




#include <boost/core/demangle.hpp>
#include <typeinfo>
#include <iostream>



namespace testSFML {

class gestion_option
{

    private :

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

        
        // la version refactor
        // Cette version prends pas en compte les options qui n'ont pas de valeur (erreur ?)
        // et leur donne la valeur "", a fixer si besoin
        struct value_ : public std::string
        {
          std::string value;
          bool is_set = true;
          bool prioritaire = true;
          value_ () = default; // je suis pas bien sur de savoir pourquoi j'ai besoin de ça, mais clairement j'en ai beosin
          value_ (std::string v, bool ok = true , bool prio = true) : value(v), is_set(ok), prioritaire(prio) {}
        };
        
        
        std::map<std::string,value_> options;
        std::vector<std::string> args_raw;
        
        bool add_option_if_empty (std::string id, std::string value, bool set = true , bool prioritaire = true )
        {
            // s'il existe mais qu'il est pas affecté
            if (  contain_key(options,id) && ! options[id].is_set )
            {
                options[id] = value_(value,set,prioritaire);
            }
            
            // s'il existe mais qu'il est affecté mais pas prioritaire
            if (  contain_key(options,id) && not options[id].prioritaire )
            {
                options[id] = value_(value,set,prioritaire);
            }
            
            // s'il existe pas 
            if (  ! contain_key(options,id) )
            {
                options[id] = value_(value,set,prioritaire);
            }
            return true;
        }
        

        /// TODO : le fair pour autre chose que des strings
        //std::map < std::string , std::vector < std::string >> map_option_valide_value;

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
            /*
            // check des option 
            for (auto opt : vec_args_named ) {

               auto if_defaut = std::find_if(vec_option_default.begin(), vec_option_default.end() , [&](auto id_v) { return id_v.id == opt.id ;} );
               if ( if_defaut == vec_option_default.end() &&
                    std::find(vec_option.begin(),vec_option.end(),opt.id) == vec_option.end() )
               {
                   erreur ("L'option \"" + opt.id + "\" n'est pas prévu par le programme.");
               }
            }
            
            // check du fichier
            for (auto opt : vec_option_fichier ) {

               auto if_defaut = std::find_if(vec_option_default.begin(), vec_option_default.end() , [&](auto id_v) { return id_v.id == opt.id ;} );
               if ( if_defaut == vec_option_default.end() &&
                    std::find(vec_option.begin(),vec_option.end(),opt.id) == vec_option.end() )
               {
                   erreur ("L'option \"" + opt.id + "\" n'est pas prévu par le programme.");
               }
            }
            */
        }
        
        // pour les options ayant une list de valeur, on verifi qu'on est bon 
        /*inline void check_liste_value (const std::string & id , const std::string & value) const
        {
            if ( contain_key ( map_option_valide_value , id)  and not contain ( map_option_valide_value.at(id) , value ) )
            {
                erreur ("L'option \"" + id + "\" n'est pas dans les valeurs possible : " + to_string (map_option_valide_value.at(id)));
            }
        }*/
    
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
                make_commentaire (" --load-file"+separateur+nomProgramme+".param");
                make_commentaire ("");
                make_commentaire ("Option ayant deja une valeur par defaut ");
                for (const auto& pair : options)
                {
                    if ( pair.second.is_set)
                    flux << pair.first << separateur <<pair.second.value << std::endl;
                }
                make_commentaire ("Option sans valeur par defaut ( à décomenter et compléter ):");
                
                for (const auto& pair : options)
                {
                    if ( ! pair.second.is_set)
                    make_commentaire(pair.first+separateur);
                    
                }
               
                std::cerr << "Generation du fichier " << nomProgramme+".param" << std::endl;
                exit(5);
            }
            
        }

    public :
        
        inline std::string get_separateur () const { return separateur; }

        gestion_option (int argc, char** argv, std::string sep="=") : separateur(sep)
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

            // on garde les fichiers dans l'ordre, pour les traiter apres, 
            std::vector<std::string> fichiers;
            
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
                        fichiers.push_back(option_temps[1]);
                    }
                    else
                    {
                        options[option_temps[0]] = option_temps[1];
                    }
                }
                else // si c'est un param brute
                {
                    args_raw.push_back(param);
                }
            }
            
            for (const auto& fichier : fichiers)
            {
               load_file(fichier,true); // si le fichier n'existe pas on prévient l'user    
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
                        
                        if (option_temps[0]== "--load-file") 
                        {
                            // on peut faire des includes dans les fichiers, 
                            // danger avec les priorités (mais c'est pas mon probleme)
                            load_file(option_temps[1],true);
                        }
                        else
                        {
                            add_option_if_empty(option_temps[0], option_temps[1] );
                        }
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
            if ( args_raw.size() < min )
            {
                erreur ("Erreur : Le programme prend au moins " + std::to_string(min) + " parametres sans noms" );
            }
        }

        inline void nb_raw_args_equal(size_t val) const { nb_raw_args_between(val,val); }

        inline void nb_raw_args_between (size_t min, size_t max) const {
            if ( args_raw.size() < min || args_raw.size() > max ) {
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
            if ( id >= args_raw.size() ){
                erreur("vous avez demandez l'indice " + std::to_string(id) + " alors qu'il n'y a que " + std::to_string(args_raw.size()) + " params");
            }
            return convert_to<T>(args_raw[id]);
        }

        inline void allow_raw_args(bool allow)
        {
            if (not allow and args_raw.size() > 0 )
            {
                erreur("Le programme ne prend pas de parametre libre" );
            }
        }

        inline std::vector<std::string> get_raw_args (){
            check_help();
            generate ();
            return args_raw;
        }

        inline void must_be_valide_file(const std::vector<std::string>& listTag) const
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
            add_option_if_empty(option,std::to_string(defaultVal),true,false);
        }
        
        inline void add(const std::string& option, const std::string& defaultVal)
        {
			check_can_add ();
            std::cout << "add" << std::endl;
            add_option_if_empty(option,defaultVal,true,false);
        }
        

        inline void add(const std::string& option)
        {
			check_can_add ();
            add_option_if_empty(option,"",false, false);
        }
        
        /*
        // TODO : refactor ça avec un truc plus générique
        template<   class Conteneur = std::vector<std::string>,
                    class = std::enable_if_t<is_container<Conteneur>::value> 
                 >
        inline void add (const std::string& option, const Conteneur& conteneur)
        {
            static_assert(is_string<decltype(get_value<Conteneur>(conteneur.begin()))>::value , "La version actuelle ne prend que des strings :/");
            check_can_add ();
            vec_option.push_back(option);
            map_option_valide_value[option] = {  conteneur.begin(),conteneur.end()   };
        }
        
         template<   class Conteneur = std::vector<std::string>,
                    class = std::enable_if_t<is_container<Conteneur>::value> 
                 >
        inline void add (const std::string& option, const std::string& value, const Conteneur& conteneur)
        {
            static_assert(is_string<decltype(get_value<Conteneur>(conteneur.begin()))>::value , "La version actuelle ne prend que des strings :/");
            check_can_add ();
            vec_option_default.push_back({option,value});
            map_option_valide_value[option] = {  conteneur.begin(),conteneur.end()   };
        }
        */

        
        // permet d'avoir une valeur avant la fin de la lecture des params
        template<class T = std::string>
        inline T get_val_unsafe(const std::string& id) const {
			check_help();
            generate ();

            if ( ! contain_key (options,id) )
            {
                erreur("Le programme demande l'option "+ id +" qui n'est pas dans les options prévue par le programme... Dev tu t'es chier..");
            }
            if ( options.at(id).is_set == false )
            {
                erreur("Le programme demande l'option "+ id +" qui n'a pas d'option par defaut, ni de valeur assignée");
            }
            return convert_to<T>(options.at(id).value);
            // il faudra check les valeurs des opitons ayant un champs quand ça sera la 
		}
        
        template<class T = std::string>
        inline T get_val(const std::string& id)  {
			check_typo();
			can_add_param =false;
			return get_val_unsafe<T>(id);
            
        }

        
        inline std::string usage() const {
            std::string retour =  "Usage : \n./"+get_name_file(nomProgramme)+" ";
            for (const auto& opt : options)
            {
                retour += opt.first+separateur+"valeur (default:"+opt.second.value+") ";
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


