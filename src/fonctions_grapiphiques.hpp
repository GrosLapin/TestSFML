#ifndef FONCTIONS_GRAPHIQUES
#define FONCTIONS_GRAPHIQUES


#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <typeinfo>

#include "traits.hpp"

namespace testSFML {


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




template<   class Point,
            class V = typename std::enable_if< is_point<Point>::value >::type
        >
inline std::ostream& operator<< (std::ostream& out, const Point& p)
{

       out << "(" << getX(p)  << "," << getY(p) << ")" ;

    return out;
}

template<class Conteneur = std::vector<sf::Vector2f> >
sf::ConvexShape createPolygone (Conteneur && points)
{
	//std::cout << typeid(points).name() << std::endl;
	//std::cout << typeid(points[0]).name ()<< std::endl;
    sf::ConvexShape convex;
  //  std::cout << points.size() << std::endl;
    convex.setPointCount(points.size());

    // définit les points
    size_t cpt = 0 ;
    for (auto && point : points )
    {
      //  std::cout << point << std::endl;
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

}
#endif
