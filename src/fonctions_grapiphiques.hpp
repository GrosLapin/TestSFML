#ifndef FONCTIONS_GRAPHIQUES
#define FONCTIONS_GRAPHIQUES

#include <utility>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <typeinfo>

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





template<
        class T,
        class V = typename std::enable_if< is_container<T>::value >::type,
        class U = typename std::enable_if< !is_string<T>::value >::type
      >
std::ostream& operator<< (std::ostream& out, const T& container)
{
    for (const auto& val : container)
    {
       out << val << " " ;
    }
    return out;
}

// On fournis une version de getX et getY pour toutes classe ayant un x et un y public
template <class T>
decltype(std::declval<T>().x) getX(T&& t)  { return t.x ;}

template < class T>
decltype(std::declval<T>().y) getY(T&& t)  { return t.y ;}



template <class T, class U = void, class V = void>
struct is_point : std::false_type {

};

template <class T>
struct is_point<   T,
                    typename void_if_valide<decltype(getX(std::declval<T>()))>::type,
                    typename void_if_valide<decltype(getY(std::declval<T>()))>::type
            >  : std::true_type {};

template<   class Point,
            class V = typename std::enable_if< is_point<Point>::value >::type
        >
std::ostream& operator<< (std::ostream& out, const Point& p)
{

       out << "(" << getX(p)  << "," << getY(p) << ")" ;

    return out;
}

template<class Conteneur = std::vector<sf::Vector2f> >
sf::ConvexShape createPolygone (Conteneur && points)
{
	std::cout << typeid(points).name() << std::endl;
	std::cout << typeid(points[0]).name ()<< std::endl;
    sf::ConvexShape convex;
    std::cout << points.size() << std::endl;
    convex.setPointCount(points.size());

    // définit les points
    size_t cpt = 0 ;
    for (auto && point : points )
    {
        std::cout << point << std::endl;
        convex.setPoint(cpt, sf::Vector2f(getX(point), getY(point)));
        cpt++;
    }

    return convex;

}


struct lapin {
    float xx;
    float yy;
};

float getX(const lapin & l) { return l.xx;}
float getY(const lapin & l) { return l.yy;}
#endif
