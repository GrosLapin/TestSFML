#ifndef GESTIONOPTION_H
#define GESTIONOPTION_H

#include <vector>
#include <algorithm>
#include <stdexcept>

#include "fonction_string.hpp"


class GestionOption
{
    
    private :

        struct IdValeur {
            std::string id;
            std::string valeur;

            inline bool operator == (const IdValeur& other)
            {
                return id == other.id;
            }
            
            inline bool operator == (const std::string & o_id)
            {
                return o_id = id;
            }
            IdValeur (std::string unId, std::string uneValeur) : id(unId),valeur(uneValeur) {}
        };

        const std::string separateur = "=";
        const std::string commentaire= "#";
        
        std::string nomProgramme;
        size_t nbArg;
        
        // si l'utilisateur à fait --help
        bool helpNeeded=false;
        std::string message_aide;
        
        
        std::vector<IdValeur> vec_args_named;
        std::vector<std::string> vec_args_raw;
        
        std::vector<std::pair<std::string,std::string>> vecOptionDefault;
        std::vector<std::string> vecOption;
        
        
        bool hasSeparateur (std::string param)
        {
            return param.find(separateur) != std::string::npos;
        }
        
        std::string ligne_sans_commentaire (const std::string ligne)
        {
            return decoupe(ligne,commentaire)[0];
        }
        
        void inline erreur(const char& message) const  { erreur(std::string(message)); }
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
            for (auto opt : vec_args ) {

               auto itDefaut = std::find_if(vecOptionDefault.begin(), vecOptionDefault.end() , [&](auto paire) { return paire.first == opt.id ;} );
               if ( itDefaut == vecOptionDefault.end() &&
                    std::find(vecOption.begin(),vecOption.end(),opt.id) == vecOption.end() )
               {
                   erreur ("L'option " + opt.id + " n'est pas prÃ©vu par le programme.");
               }
            }
        }
        
    public :
        GestionOption (int argc, char** argv) :  vec_args(), vecOptionDefault (), vecOption()
        {
            if ( argc < 1 ) {
                std::cerr << "Un programme doit avoir au moins un param : son nom. Je sais pas ce que vous avez fait, mais faites pas ca ." << std::endl;
                exit(5);
            }
            
            // on sort le nom du programme des options
            nbArg = argc-1;
            nomProgramme = argv[0];

            // Pour toutes les options
            for (int i = 1 ; i < argc ; i ++ )
            {
                std::string param (argv[i]);
                if (param == "--help" ) { helpNeeded = true;}
                
                if ( hasSeparateur(param) )
                {
                    auto option_temps = decoupe(param,separateur);
                    vec_args.push_back({option_temps[0],option_temps[1]});
                }
                else // si c'est un param brute
                {
                    vec_args_raw.push_back(param);
                }
            }
            
        };

        inline void min_raw_args(size_t min) const {
            
            if ( vec_args_raw.size() < min )
            {
                erreur ("Erreur : Le programme prend au moins " + std::to_string(min) + " parametres sans noms" );
            }

        }
        
        inline void nb_raw_args_equal(size_t val) const { nb_raw_args_Between(val,val); }
        
        inline void nb_raw_args_between (size_t min, size_t max) const {
            if ( nbArg < min || nbArg > max ) {
                if ( min == max ){
                     erreur ("Erreur : Le programme prend exactement " + std::to_string(min) + " parametres sans noms" );
                }
                else
                {
                    erreur ("Erreur : Le programme prend entre " + std::to_string(min) + " et " << std::to_string(max) << " parametres sans noms" );
                }
            }
        }
        

        inline void mustBeValideFile(const std::vector<std::string>& listTag) const
        {
            bool ok = true;
            std::string fichiers_fails = "";
            for(const auto& tag : listTag )
            {
                 auto path = getVal(tag);
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




        inline void add(std::string option, std::string defaultVal)
        {
            vecOptionDefault.push_back({option,defaultVal});
        }
        inline void add(std::string option)
        {
            vecOption.push_back(option);
        }

        inline std::string getval(std::string id){
            check_help();
            check_typo();
            auto it = std::find(vec_args.begin(), vec_args.end(), id);
            if ( it == vec_args.end() ){
                auto itDefaut = std::find_if(vecOptionDefault.begin(), vecOptionDefault.end() , [&](auto paire) { return paire.first == id ;} );
                if ( itDefaut != vecOptionDefault.end() ) {
                    return itDefaut->second;
                }
                else // On a pas trouvé
                {
                    // on affiche le message d'erreur qui va bien
                    if (std::find(vecOption.begin(),vecOption.end(),id) != vecOption.end()) {
                        erreur("L'option " +id+" n'a pas de valeur par defaut est n'est pas présente");
                    }
                    else{
                        erreur("Le programme demande une option qui n'est pas dans les options prévue par le programme... Dev tu t'es chier..");
                    }
                }
            }
            return it->valeur;
        }

        inline std::string get_raw_val(size_t id){
            check_help();
            if ( id < vec_args_raw.size() ){
                erreur("vous avez demandez l'indice " + to_string(id) + " alors qu'il n'y a que " + to_string(vec_args_raw.size()) + " params");
            }
            return vec_args_raw[id].valeur;
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
            return vec_args_raw;
        }
        
        inline std::string usage() const {
            std::string retour =  "Usage : \n./"+getNomFichier(nomProgramme)+" ";
            for (auto& opt : vecOption)
            {
                retour += opt+"=valeur ";
            }

            for (auto& opt : vecOptionDefault)
            {
                retour += opt.first+"=valeur (default:"+opt.second+") ";
            }

            retour +=  message;
            return retour;
        }
        
        inline void setMessage (const std::string& mes) { message = mes;}




};

#endif // GESTIONOPTION_H


