#ifndef OUITLS_HPP
#define OUITLS_HPP


#include <iostream>
#include <algorithm>
#include <vector>
#include <typeinfo>
#include <utility>
#include <fstream>

#include "fonction_string.hpp"
#include "traits.hpp"

namespace testSFML 
{

    // TODO : mettre à jours avec "get_value" quand je l'aurais fini 
    // TODO : decltype de is_same devrait etre sur le get_value
    // la version ou U et T contiennent le meme type
    template <	class T, class U > 
    auto contain (const T& conteneur1,  const U& conteneur2) 
         -> decltype ( enable_if_all_t <is_container, T, U >,
                       std::enable_if_t<
                                          std::is_same < decltype ( *std::declval<T>().begin() ),
                                                         decltype ( *std::declval<U>().begin() )
                                                        >::value
                                        >,
                       bool)
                       
    {
        if ( conteneur1.size() < conteneur2.size() ) { return false;}
        bool ok = true;
        
        constexpr bool is_map_c1 = is_map<T>::value ;
        constexpr bool is_map_c2 = is_map<U>::value ;
        
        for ( auto it = conteneur2.begin() ; it != conteneur2.end() && ok ; it++ )
        {
            // je ne peux pas utiliser find_if + get_val car find_if passe la valeur
            // et pas l'itérateur 
            // je dois donc faire la boucle à la main
            auto it1 = conteneur1.begin();
            bool fini = false;
            while ( ok and not fini) 
            {
                if ( it1 == conteneur1.end() )
                {
                    ok = false;
                }
                else
                {
                    if ( get_value<is_map_c1>(it1) == get_value<is_map_c2>(it) )
                    {
                        fini = false;
                    }
                }
            }          
        }
        return ok;
    }
    
    template < class T,
			   class U> 
	auto contain (const T& conteneur1 , const U& value)
         -> decltype ( std::enable_if_t <is_container<T>::value>,
                       std::enable_if_t<
                                          std::is_same < decltype ( *std::declval<T>().begin() ),
                                                         U
                                                        >::value
                                        >,
                       bool)
	{
		static_assert(!is_map<T>::value, "je n'ai pas encore fait la version pour les map");
		return std::find(conteneur1.begin(), conteneur1.end(), value) != conteneur1.end();
	}
    
    
    
    
    // TODO get_value sans doute a refaire, je pense pas que la version "non_const" soit utile
    // j'arrive pas a voir quelle est la consteness de l'opérateur de déférancement 
    
    template < 	bool is_map , 
				class T>
	struct get_value_s
	{
		auto& get_value (T& ite) { return *ite; }
		const auto& get_value (const T& ite) { return *ite; }
	};
	
	template <class T>
	struct get_value_s < true, T >
	{
		auto& get_value (T& ite) { return ite->second;; }
		const auto& get_value (const T& ite) { return ite->second;; }
	};
	
	
    template< bool is_map , class T>
	auto& get_value (T& ite)
	{
			get_value_s<is_map,T> gvs;
			return gvs.get_value(ite);
	}
	
	template< bool is_map , class T>
	const auto& get_value (const T& ite)
	{
			get_value_s<is_map,T> gvs;
			return gvs.get_value(ite);
	}

}
#endif

