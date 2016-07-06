#ifndef FONCTION_CONTENEUR_HPP
#define FONCTION_CONTENEUR_HPP


#include <iostream>
#include <algorithm>
#include <vector>
#include <typeinfo>
#include <utility>
#include <fstream>

#include "traits.hpp"
namespace std 
{
    // TODO Comprendre pourquoi ça marche pas :/
    // la recherche a coup de find if dans uen map demande la comparaison entre pair 
    template <class Pair,
              class Pair2, 
              class  = std::enable_if_t < 
                            std::is_same < 
                                        std::decay_t < typename Pair::first_type > ,
                                        std::decay_t < typename Pair2::first_type >
                                         > ::value
                                        >,
              class  = std::enable_if_t < 
                            std::is_same < 
                                        std::decay_t < typename Pair::second_type > ,
                                        std::decay_t < typename Pair2::second_type >
                                         >::value
                                        >
              >
    bool operator==(const Pair& p1, const Pair2&  p2 )
    {
        return p1.first == p2.first && p1.second == p2.second;
    } 
}
namespace testSFML 
{


    template < 	bool is_map , 
				class T>
	struct get_value_s
	{
		auto& get_value (const T& ite) { return *ite; }
		//const auto& get_value (const T& ite) { return *ite; }
	};
	
	template <class T>
	struct get_value_s < true, T >
	{
		auto& get_value (const T& ite) { return ite->second;; }
		//const auto& get_value (const T& ite) { return ite->second;; }
	};
	
	
    template<class U , class T>
	auto& get_value (const T& ite)
	{
        get_value_s<is_map<U>::value,T> gvs;
        return gvs.get_value(ite);
	}
	
	template<class U, class T>
	auto& get_value (const T& ite,const U&)
	{
        get_value_s<is_map<U>::value,T> gvs;
        return gvs.get_value(ite);
	}

	
	

    //
    // TODO : decltype de is_same devrait etre sur le get_value
    // la version ou U et T contiennent le meme type
    template <	class Conteneur1, class Conteneur2 > 
    auto contain (const Conteneur1& conteneur1,  const Conteneur2& conteneur2) 
         ->  last_t <  enable_if_all_t <is_container, Conteneur1, Conteneur2 >,
                       std::enable_if_t<
                                          std::is_same < std::decay_t<decltype ( get_value<Conteneur1>(std::declval<Conteneur1>().begin() ))>,
                                                         std::decay_t<decltype ( get_value<Conteneur2>(std::declval<Conteneur2>().begin() ))>
                                                        >::value
                                        >,
                       bool>
                       
    {
        if ( conteneur1.size() < conteneur2.size() ) { return false;}
        bool ok = true;
        
        for ( auto it = conteneur2.begin() ; it != conteneur2.end() && ok ; it = std::next(it) )
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
                    if ( get_value<Conteneur1>(it1) == get_value<Conteneur2>(it) )
                    {
                        fini = true;
                    }
                }
                it1 = std::next(it1);
            }          
        }
        return ok;
    }
    
 
    template < class Conteneur1,
			   class U> 
	auto contain (const Conteneur1& conteneur1 , const U& value)
         -> last_t <    std::enable_if_t <is_container<Conteneur1>::value>,
                       std::enable_if_t<
                                          std::is_same < std::decay_t<decltype (get_value<Conteneur1>(std::declval<Conteneur1>().begin()))>,
                                                         std::decay_t<U>
                                                        >::value
                                        >,
                       bool>
	{
        auto it1 = conteneur1.begin();
        bool trouve = false;
        while ( not trouve and (it1 != conteneur1.end()) ) 
        {
            if ( get_value<Conteneur1>(it1) == value )
            {
                trouve = true;
            }
            it1 = std::next(it1);
        }      

        return trouve;
	}
	
	

    

	
	// si on demande une map<key,val> et une pair<key,val> on fait une recherche
	// normal :)
	// le probleme est que le decay sur une pair ne match pas si la clée est const
	// du coup je dois eclater la pair pour faire le decay
	/*
     *      cout << demangle (typeid(decltype (*m.begin())).name()) << endl;
            cout << demangle ( typeid(std::decay_t<decltype (*m.begin())>).name())<< "   decay"  << endl;
            cout << demangle ( typeid(decltype (std::pair<std::string,int>(std::string("d"),1))).name() ) << endl;
     * 
     *      std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, int>
            std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, int>   decay
            std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int>

     * 
     */
    

	template < class Map,
			   class U> 
	auto contain (const Map& map1 , const U& value)
         -> last_t <   std::enable_if_t <is_map<Map>::value>,
                       std::enable_if_t<
                                          std::is_same < std::decay_t<decltype (map1.begin()->first)>,
                                                         std::decay_t<decltype (value.first)>
                                                        >::value
                                        >,
                       std::enable_if_t<
                                          std::is_same < std::decay_t<decltype (map1.begin()->second)>,
                                                         std::decay_t<decltype (value.second)>
                                                        >::value
                                        >,
                       bool>
	{
        return std::find(map1.begin(),map1.end(),value) != map1.end() ;
	}
    
    
    template<class Map, class K>
    bool contain_key(const Map& map1 , const K& key)
	{
        static_assert(is_map<Map>::value,"La recherche de clée ne peut se faire que dans une map");
        return map1.find(key) != map1.end() ;
	}
    


}
#endif

