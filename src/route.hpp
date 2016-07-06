#ifndef ROUTE_HPP
#define ROUTE_HPP


#include <vector> 

#include "outils/traits.hpp"
#include "outils/fonctions_math.hpp"

namespace testSFML
{
    template <class Point,
             class = typename std::enable_if< is_point<Point>::value >::type>
    class route 
    {
        std::vector<Point> vec_point;
        
    public: 
        // on match notre définition d'un conteneur
        using iterator          = typename std::vector<Point>::iterator;
        using const_iterator    = typename std::vector<Point>::const_iterator;
        iterator        begin()         { return vec_point.begin() ; }
        const_iterator  begin() const   { return vec_point.begin() ; }
        iterator        end()           { return vec_point.end() ; }
        const_iterator  end()   const   { return vec_point.end() ; }
        size_t          size()  const   { return vec_point.size() ;}
        
        // pour la facilité d'utilisation ( je sais pas si c'est une bonne idée, ça limite ce qu'on peut faire)
        Point back() const { return vec_point.back(); }
        
        
        route (const Point & depart)
        {
            add(depart);
        }
        
        void add(const Point & p)
        {
            vec_point.push_back(p);
        }
        
        template <  class Conteneur,
                    class = typename is_container<Conteneur>::type>        
        bool prolonge ( const Conteneur & cont_points , double angle_max = 15 )
		{
			static_assert(is_container<Conteneur>::value, "Le parametre n'est pas un conteneur");
            static_assert(is_point<decltype (cont_points[0])>::value, "Le conteneur ne contient pas de point");
			static_assert(!std::is_same<decltype (cont_points[0]), Point>::value, "les types sont pas compatibles");			
			// on s'assure que ça soit bon 
			if ( cont_points.empty() ) { return false ; }
			
			auto not_in_route = [&] (const auto& point)
			{
				return std::find_if(vec_point.begin(), vec_point.end(), [&](const auto& p) {
					return getX(p) == getX(point) && getY(p) == getY(point);
					
				})== vec_point.end();
			};
			
			size_t best_indice = 0;
			double best_angle = 360;
			bool ok = false;
			
			for (size_t i = 0 ; i < cont_points.size() ; i ++ )
			{
				
				
				double angle =  angle_deg(*this,cont_points[i]);
				
				if ( angle < best_angle &&  angle > (180-angle_max) &&  not_in_route(cont_points[i]) )
				{
					std::cout << cont_points[i] << " : " <<  angle << "   " <<   " < " << angle_max <<  std::endl;
					best_angle = angle;
					best_indice = i;
					ok = true;
				}
			}
			
			if (ok)
			{
				std::cout << " add " << cont_points[best_indice] << std::endl;
				add(cont_points[best_indice]);
			}
			
			return ok;
			
		}
        
    };
	
	
	template < 	class Point, 
				class Point2, 
				class = typename std::enable_if< is_point<Point>::value >::type,
				class = typename std::enable_if< is_point<Point2>::value >::type>
	double angle_deg (const route<Point>& une_route, Point2 && point)
	{
		assert(une_route.size() >= 2 );
		return angle_deg(*(une_route.end()-2),une_route.back(),std::forward<Point2>(point));
	}
}
#endif