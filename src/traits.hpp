#ifndef TRAITS_HPP
#define TRAITS_HPP

#include <iostream>
#include <vector>
#include <typeinfo>
#include <utility>


namespace testSFML {
template<class T>
struct void_if_valide {
    using type = void;
};


template <class T, class U = void, class V = void>
struct is_container : std::false_type {};

template <class T>
struct is_container<   T,
                typename void_if_valide<decltype(std::declval<T>().begin())>::type,
                typename void_if_valide<decltype(std::declval<T>().end())>::type
            >  : std::true_type {};




template <class T, class U = void>
struct is_string : std::false_type {};



template <class T>
struct is_string<   T,
                typename void_if_valide<typename std::enable_if<
                                                                std::is_same<T,std::wstring>::value ||
                                                                std::is_same<T,std::string>::value
                                                                >::type
                                        >::type
            >  : std::true_type {};




template <class T, class U = void, class V = void>
struct is_point : std::false_type {

};

// On fournis une version de getX et getY pour toutes classe ayant un x et un y public
template <class T>
inline decltype(std::declval<T>().x) getX(T&& t)  { return t.x ;}

template < class T>
inline decltype(std::declval<T>().y) getY(T&& t)  { return t.y ;}


template <class T>
struct is_point<   T,
                    typename void_if_valide<decltype(getX(std::declval<T>()))>::type,
                    typename void_if_valide<decltype(getY(std::declval<T>()))>::type
            >  : std::true_type {};

}
#endif

