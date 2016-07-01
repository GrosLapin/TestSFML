#ifndef FONCTION_STRING_H
#define FONCTION_STRING_H

#include <string>
#include <stdexcept>
#include <ostream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <regex>
#include <type_traits>
#include <cctype>


#include "traits.hpp"

namespace testSFML {
	
		template<
			class T,
			class V = typename std::enable_if< is_container<T>::value >::type,
			class U = typename std::enable_if< !is_string<T>::value >::type
		>
		inline std::ostream & operator<< (std::ostream& out, const T& container)
		{
			for (const auto& val : container)
			{
			out << val << " " ;
			}
			return out;
		}




		template<   class Point,
					class V = typename std::enable_if< is_point<Point>::value >::type
				>
		inline std::ostream& operator<< (std::ostream& out, const Point& p)
		{

			out << "(" << getX(p)  << "," << getY(p) << ")" ;

			return out;
		}
		
		
// la version ou  la limite est un string;
// l'entre est prise en copie, on peut l'utiliser directement
// la limite n'est pas modifiée.
inline std::vector<std::string> decoupe (std::string entre,const std::string& limite)
{
    std::vector<std::string> retour;

    size_t pos = entre.find(limite);
    while (pos !=std::string::npos )
    {
        std::string temps =  entre.substr(0,pos);
        retour.push_back(temps);
        entre = entre.substr(pos+limite.size());
        pos = entre.find(limite);
    }
    retour.push_back(entre);
    return retour;
}

// decoupe avec "je-suis-ton-pere" en " je suis ton pere"
inline std::vector<std::string>  decoupe (const std::string& chaine, const char* separateur)
{
    return decoupe(chaine,std::string(separateur));
}


}
/*
// decoupe avec plusieurs ligmite
inline std::vector<std::string> decoupe (std::string entre,std::vector<std::string> limites)
{
    if ( limites.size() == 0 ) return {entre};

    std::vector<std::string> retour;

    auto premiereCoupe = [&] ()
    {
        auto pos = std::string::npos;
        int taille = 0;
        for (auto& limite : limites)
        {
            auto pos_limite = entre.find(limite);
            // si on a trouvÃ©, et qu'on est plus petit
            if ( pos_limite != std::string::npos && pos_limite < pos )
            {
                pos = pos_limite;
                taille = limite.size();
            }
        }
        return std::pair<decltype(pos),int>(pos,taille);

    };
    auto pairPositionTaille = premiereCoupe();
    while (pairPositionTaille.first !=std::string::npos )
    {
        std::string temps =  entre.substr(0,pairPositionTaille.first);
        retour.push_back(trim(temps));
        entre = entre.substr(pairPositionTaille.first+pairPositionTaille.second);
        pairPositionTaille = premiereCoupe();
    }
    retour.push_back(entre);
    return retour;
}



template <class T>
std::string to_string (T data)
{
    std::ostringstream ostr; //output string stream
    ostr << data; //use the string stream just like cout,
    //except the stream prints not to stdout but to a string.

   return ostr.str();
}

inline std::string toUpper (const std::string& chaine)
{
    std::string retour;
    for (char c : chaine )
    {
        retour.push_back(toupper(c));
    }
    return retour;
}
/// copier coller google :D
// trim from start
inline std::string& ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
inline std::string& rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
inline std::string& trim(std::string &s) {
        return ltrim(rtrim(s));
}

// clairement optimisable ET DAFUCK j'ai besoin de Ã§a :/
//#ifdef BENCH
void inline cleanSpace (std::string & unString)
{

    auto aDelete = [](char c ) {
        // 9 c'est une tabulation
        // 13 je sais pas ce que c'est mais c'est invisible et c'est de la saloperie
        return (c  == ' ' ||  (int)c ==9 ||  (int)c==13 );
    };

    if ( unString.empty () ) return ;


    while ( !unString.empty () &&  aDelete(unString[0]) )
        unString = std::string (unString.begin()+1, unString.end());

    if ( unString.empty () ) return ;

    while ( !unString.empty () &&  aDelete(unString[unString.size()-1]) )
        unString = std::string (unString.begin(), unString.end()-1);

    if ( unString.empty () ) return ;
    if ( unString.size() == 1 ) return;
    // Supprime les doublons d'espace ou de \t
    bool aVerifier = true;
    while (aVerifier)
    {
        aVerifier = false;
        for (size_t i = 0 ; (unString.size() >= 2) && i < unString.size()-2 && !unString.empty() ; i++)
        {
            char enCour = unString[i];
            char suivant = unString[i+1];
            if( aDelete (enCour) && aDelete(suivant) )
            {
                unString=std::string(unString.begin(),unString.begin()+i)+ std::string(unString.begin()+i+1,unString.end());
                aVerifier = true;
            }
        }
    }

    if ( unString.empty () ) return ;

    // remplace les \t par des espaces
    std::string retour;
    for(size_t i = 0 ; i < unString.size() ; i++)
    {

        if(unString[i] == '\t' || unString[i] == 9 )
        {
            retour += ' ';
        }
        else
        {
            retour += unString[i];
        }

    }
    unString = retour;


//std::cout <<"apres" <<  unString << "\n"std::cout.flush();
}
//#else
//   void cleanSpace (std::string & unString);
//#endif

namespace std
{
    template <class T, class U = typename T::first_type , class V = typename T::second_type>
    ostream& operator << (ostream& out, T unePaire)
    {
        out << "[" << unePaire.first << "," << unePaire.second << "]";
        return out;
    }

    template<class T,
              class U = typename std::enable_if<
                                                    !std::is_same<T,std::wstring>::value
                                                 >::type,
              class V = typename std::enable_if<
                                                    !std::is_same<T,std::string>::value
                                                 >::type,
              class W = decltype (std::declval<T>().begin()),
              class X = decltype (std::declval<T>().end())
            >
    std::ostream& operator<< (std::ostream& out, const T& container)
    {
        for (const auto& val : container)
        {
           // out << "\t" << val << "\n" ;
           out <<val << " " ;
        }
        return out;
    }



    inline ostream& operator << (ostream& out, clause c) {
        out << "clause (true:" << c.enzPos << " , false:" << c.enzNeg << ")" ;
        return out;
    }


}

template<class T>
void display (const T& conteneur,std::string separateur =" ")
{
    for (const auto& val : conteneur)
    {
        std::cout << val << separateur ;
    }
}




template<class T,class U>
void ecrire (const T& conteneur,U& flux, std::string separateur =" ")
{

    for (const auto& val : conteneur)
    {
        flux << val << separateur ;
    }
}

std::string remplace (const std::string base, std::string motif, std::string nouveau);

std::string coupeString(std::string chaine, std::string limite , bool debut = true);
std::string coupeLastString(std::string chaine, std::string limite , bool debut = true);





// decoupe " salut (mon ami) Ã§a va ?" en [salut] [mon ami] [Ã§a va ?]
std::vector<std::string> inline decoupe (std::string chaine, const std::string& debut, const std::string& fin)
{
        #define DEBUG false
    // on travaille sur une copie de chaine on peut y allez comme un connard
        std::vector<std::string> retour;
        auto indice_debut = chaine.find(debut);
        auto indice_fin = chaine.find(fin);
        debug("_________________________" ,"\n");
        debug(chaine,"\n");
        debug("          ---            " ,"\n");

        while ( indice_debut != std::string::npos && indice_fin != std::string::npos )
        {
            debug("Position debut " ,indice_debut , "\n");
            debug( "Position fin " , indice_fin  , "\n");

            // si y a qqch avant
            if (indice_debut != 0)
            {
                std::string mot (chaine.begin(),chaine.begin()+indice_debut);
                cleanSpace(mot);
                if ( ! mot.empty() )
                {
                    retour.push_back(mot);
                }

            }

            std::string mot (chaine.begin()+indice_debut+debut.size(),chaine.begin()+indice_fin);
            chaine = std::string(chaine.begin()+indice_fin+debut.size(),chaine.end());
            debug( "mot : " ,mot ,  "\n" ," chaine " , chaine ,"\n");
            retour.push_back(mot);



            indice_debut = chaine.find(debut);
            indice_fin = chaine.find(fin);


        }
        retour.push_back(chaine);
        return retour;
}


inline bool is_numeric (std::string chaine)
{
    #warning gere les . et les , comme etant du numeric
    cleanSpace(chaine);
    auto isSeparateurNumerique = [](char c){
        return (c =='.' || c ==',' );
    };
    if ( chaine.size() == 0 ) return false;
    if ( !(chaine[0] == '-') && !(isdigit(chaine[0]))) return false;

    bool ok = true;
    int nbPoint = 0;
    for (size_t i = 1 ; i < chaine.size() ; i++)
    {
        // si on est un digit pas de probleme
        if ( isdigit(chaine[i]) ) { continue ;}
        if ( isSeparateurNumerique(chaine[i])  && nbPoint < 1 ) {
            nbPoint ++;
            continue;
        }
        // si on arrive la c'est qu'on a fait
        ok = false;
        break;
    }
    return ok;
}

// je sais pas bien pourquoi je pouvais pas le mettre dans "fonction" sans que Ã§a plante (Ã§a me disais que Ã§a voyais pas decoupe)
// Permet de faire depuis une chaine de string un tableau
// vector<double> vd_grand = creation_vector("14.2 1 2.2" , [](string s) { return atof(s.c_str()); });
template<class T>
auto creation_vector(std::string chaine, T fn)
{
    auto vect_string = decoupe(chaine,' ');
    std::vector<decltype(fn(std::string()))> vecRetour;
    for (const auto & s : vect_string )
    {
        vecRetour.push_back(fn(s));
    }
    return vecRetour;
}


inline std::string getDate()
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::string date_espace ( std::ctime(&end_time));
    return remplace(remplace(date_espace," ","_"),"?","");
}
*/
#endif
