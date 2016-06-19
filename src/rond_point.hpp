#ifndef ROND_POINT_HPP
#define ROND_POINT_HPP


#include <SFML/Graphics.hpp>
#include <vector>
#include "route.hpp"
#include "fonctions_math.hpp"

namespace testSFML 
{
class rond_point 
{
	double min_angle = 30; // 360 / 30 = 12; par 45 = 8
	int nb_route;
	int min_route = 4;
	std::vector<route<sf::Vector2f>> routes;
	
public : 
	
	rond_point() 
	{
		nb_route =random(min_route,(int)(360/min_angle));
	}
	
	
};
}

#endif