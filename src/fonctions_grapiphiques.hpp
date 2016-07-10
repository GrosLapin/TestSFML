#ifndef FONCTIONS_GRAPHIQUES
#define FONCTIONS_GRAPHIQUES


#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <typeinfo>

#include "outils/traits.hpp"

namespace testSFML {





template <class Conteneur = std::vector<sf::Vector2f> >
sf::VertexArray create_lines (Conteneur && points, const sf::Color couleur =  sf::Color::White)
{

    sf::VertexArray lines (sf::LinesStrip, points.size() );

    // définit les points
    size_t cpt = 0 ;
    for (auto && point : points )
    {
        lines[cpt].position = sf::Vector2f(getX(point), getY(point));
        
         // la couleur 
        lines[cpt].color = couleur;
        
        cpt++;
    }
    
   

    return lines;

}

// sempai, static assert moi !!
template<class Conteneur = std::vector<sf::Vector2f> >
sf::ConvexShape create_polygone (Conteneur && points)
{
	static_assert(is_container<Conteneur>::value, "Le parametre n'est pas un conteneur");
	static_assert(is_point<decltype (points[0])>::value, "Le conteneur ne contient pas de point");
	
    sf::ConvexShape convex;
	convex.setPointCount(points.size());

    // définit les points
    size_t cpt = 0 ;
    for (auto && point : points )
    {
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
