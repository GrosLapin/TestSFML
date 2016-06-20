#ifndef ROND_POINT_HPP
#define ROND_POINT_HPP


#include <SFML/Graphics.hpp>
#include <vector>
#include "route.hpp"
#include "fonctions_math.hpp"

namespace testSFML 
{
	
	template <class Point = sf::Vector2f,
			class  = typename std::enable_if< is_point<Point>::value >::type >
	class rond_point 
	{
		double min_angle = 30; // 360 / 30 = 12; par 45 = 8
		int nb_route;
		int min_route = 4;
		std::vector<route<Point>> routes;
		Point centre;
	public : 
		
		rond_point(Point&& pos) 
		{
			centre = pos;
			nb_route =random(min_route,(int)(360/min_angle));
		}
		
		rond_point(Point&& pos, Point&& premiere_route) 
		{
			centre = pos;
			nb_route =random(min_route,(int)(360/min_angle));
			create_route(std::forward<Point>(premiere_route));
		}
		
		void create_route(Point&& p)
		{
			if (routes.size() == 0)
			{
				routes.emplace_back(centre);
				routes.back().add(p);
			}
			else {
				assert(can_add_route(p));
				routes.emplace_back(centre);
				routes.back().add(p);
			}
		}
		
		bool is_full() const { return routes.size() >= nb_route ; }
		
		
		bool can_add_route  (Point&& p) const
		{
			if (is_full() ) return false;
			
			for(const auto & route : routes) 
			{
				if (!is_angle_ok(route,p,min_angle))
				{
					return false;
				}
			}
			return true;
		}

	};
	template < 	class Point, 
				class Point2, 
				class = typename std::enable_if< is_point<Point>::value >::type,
				class = typename std::enable_if< is_point<Point2>::value >::type>
	bool is_angle_ok(const route<Point>& une_route, Point2 && point, double angles)
	{
		return angles < angle_deg(une_route,std::forward<Point2>(point));
	}
}

#endif