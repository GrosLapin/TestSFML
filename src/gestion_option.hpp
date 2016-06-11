#ifndef GESTIONOPTION_H
#define GESTIONOPTION_H

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <stdlib.h>     /* atof */

#include "fonction_string.hpp"
#include "outils.hpp"

namespace testSFML {

template <class T>
struct convert_struct {

};

template <class T>
T convert_to(const std::string& str) {
    static_assert(std::is_same<T,T>::value && false, "Conversion vers un type inconnu.");
    return T();
}

template <>
double convert_to<double> (const std::string& str) {
    return std::atof(str.c_str());
}

template <>
std::string convert_to<std::string> (const std::string& str) {
    return str;
}
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
            for (auto opt : vec_args_named ) {

               auto itDefaut = std::find_if(vecOptionDefault.begin(), vecOptionDefault.end() , [&](auto paire) { return paire.first == opt.id ;} );
               if ( itDefaut == vecOptionDefault.end() &&
                    std::find(vecOption.begin(),vecOption.end(),opt.id) == vecOption.end() )
               {
                   erreur ("L'option " + opt.id + " n'est pas prÃ©vu par le programme.");
               }
            }
        }

    public :
        gestion_option (int argc, char** argv) : vec_args_named(), vec_args_raw(), vecOptionDefault (), vecOption()
        {
            if ( argc < 1 ) {
                std::cerr << "Un programme doit avoir au moins un param : son nom. Je sais pas ce que vous avez fait, mais faites pas ca ." << std::endl;
                exit(5);
            }

            // on sort le nom du programme des options
            nbArg = (size_t)argc-1; // on peut faire la convertion grace au if du dessus
            nomProgramme = argv[0];

            // Pour toutes les options
            for (int i = 1 ; i < argc ; i ++ )
            {
                std::string param (argv[i]);
                if (param == "--help" ) { helpNeeded = true;}

                if ( hasSeparateur(param) )
                {
                    auto option_temps = decoupe(param,separateur);
                    vec_args_named.push_back({option_temps[0],option_temps[1]});
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


        inline void mustBeValideFile(const std::vector<std::string>& listTag) const
        {
            bool ok = true;
            std::string fichiers_fails = "";
            for(const auto& tag : listTag )
            {
                 auto path = get_val(tag);
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
                 class U = typename std::enable_if< !is_string<T>::value >::type>
        inline void add(std::string option, T defaultVal)
        {
            vecOptionDefault.push_back({option,std::to_string(defaultVal)});
        }
        inline void add(std::string option, std::string defaultVal)
        {
            vecOptionDefault.push_back({option,defaultVal});
        }
        inline void add(std::string option)
        {
            vecOption.push_back(option);
        }

        template<class T = std::string>
        inline T get_val(const std::string& id) const {
            check_help();
            check_typo();
            auto it = std::find(vec_args_named.begin(), vec_args_named.end(), id);
            if ( it == vec_args_named.end() ){
                auto itDefaut = std::find_if(vecOptionDefault.begin(), vecOptionDefault.end() , [&](auto paire) { return paire.first == id ;} );
                if ( itDefaut != vecOptionDefault.end() ) {
                    return convert_to<T>(itDefaut->second);
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
            return convert_to<T>(it->valeur);
        }

        inline std::string get_raw_val(size_t id){
            check_help();
            if ( id < vec_args_raw.size() ){
                erreur("vous avez demandez l'indice " + std::to_string(id) + " alors qu'il n'y a que " + std::to_string(vec_args_raw.size()) + " params");
            }
            return vec_args_raw[id];
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
            std::string retour =  "Usage : \n./"+get_name_file(nomProgramme)+" ";
            for (auto& opt : vecOption)
            {
                retour += opt+"=valeur ";
            }

            for (auto& opt : vecOptionDefault)
            {
                retour += opt.first+"=valeur (default:"+opt.second+") ";
            }

            retour +=  message_aide;
            return retour;
        }

        inline void set_message (const std::string& mes) { message_aide = mes;}




};
}
#endif // GESTIONOPTION_H


