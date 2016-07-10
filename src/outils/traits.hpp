#ifndef TRAITS_HPP
#define TRAITS_HPP

#include <iostream>
#include <vector>
#include <typeinfo>
#include <utility>
#include <map>
#include <unordered_map>
#include <type_traits>

namespace testSFML {

	
	
template <class U, class... T>
struct last_type
{
    using type = U;
};

template <class U, class UU , class... T>
struct last_type<U,UU, T...> {
  using type =  typename last_type<UU,T...>::type;
};

template <class U, class... T>
using last_t = typename last_type<U,T...>::type;


template <class T>
struct is_const
{
    static constexpr bool value = std::is_const < std::remove_reference_t< T >  >::value;
};
    
	
template <template<class...> class Traits, 
		  class... T>
struct apply_on_all 
{
	static constexpr bool value = true;
};

template <template<class...> class Traits, 
		  class TT,
		  class... T>
struct apply_on_all <Traits,TT,T...>
{
	static constexpr bool value = Traits<TT>::value && apply_on_all<Traits,T...>::value;
};

template <template<class...> class Traits, 
		  class... T>
using enable_if_all_t = std::enable_if_t < apply_on_all<Traits,T...>::value >;


template<class T>
struct void_if_valide {
    using type = void;
};

template<class T>
using void_if_valide_t = typename void_if_valide<T>::type;

template<class T>
struct true_if_valide 
{
	static constexpr bool value = true;
};



template <class T, class U = void, class V = void , class W = void>
struct is_container : std::false_type {};

template <class T>
struct is_container<    T,
                        typename void_if_valide<decltype(std::declval<T>().begin())>::type,
                        typename void_if_valide<decltype(std::declval<T>().end())>::type,
                        typename void_if_valide<decltype(std::declval<T>().size())>::type
                    >  : std::true_type {};



// verifique que l'on peut demander un "std::to_string" m
template <class T, class U = void >
struct std_to_stringable : std::false_type {};

template <class T>
struct std_to_stringable<   T,
							void_if_valide_t <decltype( std::to_string( std::declval<T>() ))>
						>  : std::true_type {};

template<class T>
using std_to_stringable_t = typename std_to_stringable<T>::type;
					


template <class T, class U = void>
struct is_string : std::false_type {};

template <class T>
struct is_string<   T,
                typename void_if_valide<typename std::enable_if<
                                                                std::is_same<std::decay_t<T>,std::wstring>::value ||
                                                                std::is_same<T,const char*>::value ||
                                                                std::is_same<std::decay_t<T>,std::string>::value
                                                                >::type
                                        >::type
            >  : std::true_type {};

			

template <class T, class = void >
struct is_map_impl : std::false_type {};


template <class T >
struct is_map_impl<     T,
						last_t < 	std::enable_if_t< is_container<T>::value > ,
									void_if_valide_t<typename T::key_type>,
									void_if_valide_t<typename T::mapped_type>
								>
                    >  : std::true_type {};

template<class T>
using is_map = is_map_impl<std::decay_t<T>>;


template <class T, class U = void, class V = void>
struct is_point : std::false_type {

};

// On fournis une version de getX et getY pour toutes classe ayant un x et un y public
template <class T>
inline decltype(std::declval<T>().x) getX(T&& t)  { return t.x ;}

template < class T>
inline decltype(std::declval<T>().y) getY(T&& t)  { return t.y ;}

template <class T>
inline void setX(T& t, decltype(std::declval<T>().x) value )  { t.x = value ;}

template < class T>
inline void setY(T& t, decltype(std::declval<T>().y) value )  { t.y = value ;}

// TODO upgrade ça avec le set
template <class T>
struct is_point<   T,
                    typename void_if_valide<decltype(getX(std::declval<T>()))>::type,
                    typename void_if_valide<decltype(getY(std::declval<T>()))>::type
            >  : std::true_type {};

}





#endif

