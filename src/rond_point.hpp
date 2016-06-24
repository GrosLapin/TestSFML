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
		size_t nb_route;
		size_t min_route = 4;
		std::vector<route<Point>> routes;
		Point centre;
	public : 
		
		/// MOCHE DANGER 
		const std::vector<route<Point>>& getRoutes () const { return routes ;}
		std::vector<route<Point>>& getRoutes () { return routes ;}
		
		
		
		
		// et du coup pourquoi ça rond_point(Point&& pos)  ne marche pas ? 
		// il n'y a pas de déduction de type automatique pour les constructeurs
		// avant C++17
		rond_point(const Point& pos) 
		{
			centre = pos;
                        // danger ne marche sans doute pas c'est le max théorique mais pas forcement dans mon cas
			nb_route =random(min_route,(size_t)(360/min_angle));
		}
		
		// a voir si ça marche, mais je pense pas :/
		rond_point(const Point& pos, Point&& premiere_route) 
		{
			centre = pos;
			nb_route =random(min_route,(size_t)(360/min_angle));
			create_route(std::forward<Point>(premiere_route));
		}
		
		void create_route(const Point& p)
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
		
		
		bool can_add_route  (const Point& p) const
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
		void create_routes (const Conteneur& cont_points, double distance_max = 1000)
        {
            // pour l'instant j'ia pas d'unité pour mes distance donc je me rends pas compte
            static_assert(is_container<Conteneur>::value, "Le parametre n'est pas un conteneur");
            static_assert(is_point<decltype (cont_points[0])>::value, "Le conteneur ne contient pas de point");
            
			
			std::cout << centre << std::endl;
			
		
			
			std::vector<sf::Vector2f> vec_points_add;
			if ( routes.empty() )
			{
				auto indice = random((size_t)0,cont_points.size()-1);
				vec_points_add.push_back(convert_to<sf::Vector2f>(cont_points[indice]));
				std::cout  << vec_points_add.back()  << std::endl << " _____________" << std::endl;
 				create_route(cont_points[indice]);
			}
			/*else
			{
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
					else
					{
						
					}
				}
			}*/
		}

	};
	template < 	class Point, 
				class Point2, 
				class = typename std::enable_if< is_point<Point>::value >::type,
				class = typename std::enable_if< is_point<Point2>::value >::type>
	bool is_angle_ok(const route<Point>& une_route, Point2 && point, double angles)
	{
		std::cout << *(une_route.begin()) << "  "<< *(une_route.begin()+1) << " " << point  << " anglge : " << angle_deg(*(une_route.begin()),*(une_route.begin()+1),point);
		std::cout << "   is : " << (angles > angle_deg(*(une_route.begin()),*(une_route.begin()+1),point)) << std::endl;
		return angles > angle_deg(*(une_route.begin()),*(une_route.begin()+1),std::forward<Point2>(point));
	}
	
	
}

#endif