#ifndef ROND_POINT_HPP
#define ROND_POINT_HPP


#include <SFML/Graphics.hpp>
#include <vector>
#include "route.hpp"
#include "fonctions_math.hpp"

namespace testSFML 
{
	//  pourquoi le type par defaut ne marhce pas ? 
	template < class Point = sf::Vector2f,
               class  = std::enable_if_t< is_point<Point>::value >
			 >
	class rond_point 
	{
		double min_angle = 30; // 360 / 30 = 12; par 45 = 8
		int nb_route;
		int min_route = 4;
		std::vector<route<Point>> routes;
		Point centre;
	public : 
		// et du coup pourquoi ça rond_point(Point&& pos)  ne marche pas ? 
		rond_point(const Point& pos) 
		{
			centre = pos;
                        // danger ne marche sans doute pas c'est le max théorique mais pas forcement dans mon cas
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
		
		
		// sans doute pas top en therme de construction de code mais je sais pas par ou partir 
		// donc je fais ça 
		template <  class Conteneur,
                    class = typename is_container<Conteneur>::type>        
		void create_routes (Conteneur&& cont_points, double distance_max = 100)
        {
            // pour l'instant j'ia pas d'unité pour mes distance donc je me rends pas compte
            static_assert(is_container<Conteneur>::value, "Le parametre n'est pas un conteneur");
            static_assert(is_point<decltype (cont_points[0])>::value, "Le conteneur ne contient pas de point");
            
            for(const auto& p : cont_points)
            {
                if ( distance(centre,p) < distance_max && can_add_route(p) )
                {
                    create_route(p);
                    if ( is_full () )
                    {
                      break;   
                    }
                }
            }
        }

	};
	template < 	class Point, 
				class Point2, 
				class = typename std::enable_if< is_point<Point>::value >::type,
				class = typename std::enable_if< is_point<Point2>::value >::type>
	bool is_angle_ok(const route<Point>& une_route, Point2 && point, double angles)
	{
		return angles > angle_deg(une_route,std::forward<Point2>(point));
	}
	
	
}

#endif