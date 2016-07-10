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
	
	
	// TODO la version ici ne prend pas de "route" mais juste des points, a voir plus tard
	// fonction qui essais de rendre la route moins "droite" en la cassant autant que possible avec
	// les points disponibles
	template < class PointRetour = default_point,
               class point_route = default_point,
               class ConteneurPoint = std::vector<default_point>
             >
	std::vector<PointRetour> update_route (std::vector<point_route> route, const ConteneurPoint& points, double angle)
    {
        static_assert(apply_on_all<is_container,ConteneurPoint>::value, " la fonctions prends deux conteneurs ");
        static_assert(apply_on_all<is_point,point_route, decltype(*(points.begin())) > ::value , " les conteneurs attendent des points");
        
        assert( route.size() >= 2 );
        
    
        
        // decay_t + const & = ne sert a rien, mais je trouve ça plus lisible 
        //using point_route = std::decay_t<decltype(*(route.begin()))>;
        auto brise_ligne = [&] (auto it_debut)
        {
            const point_route& ref_debut  = *(it_debut);
            const point_route& ref_fin    = *(std::next(it_debut));
            
            PointRetour retour_point;
            double distance_max = -1;
            bool exist = false; 
            for ( const auto& p : points )
            {
                auto dist = distance(ref_debut,ref_fin,p);
                if ( dist > distance_max // on veux le point le plus loins qui passe ce if, autant check au debut
                    && angle_deg( ref_debut, ref_fin , p )  < angle //  est ce que les deux angles sont bon ? 
                    && angle_deg( ref_fin , ref_debut, p )  < angle 
                )
                {
                    distance_max = dist;
                    // TODO : check si il y pas mieux :/
                    // l'opérateur = doit etre dans la classe point 
                    // mais c'est un concepte donc bon ...
                    assign ( retour_point,  p );
                    // retour.push_back(convert_to<PointRetour>(p));
                    exist = true;
                }
            }
            
            return std::pair<bool,PointRetour> ( exist,retour_point );
            
        };
        
       long int debut = 0;
        while ( size_t(debut) < route.size() -1 )
        {
            std::pair<bool,PointRetour> new_point = brise_ligne(route.begin() + debut);
            if ( new_point.first )
            {
                route.insert( route.begin() + debut + 1,new_point.second);
            }
            else 
            {
                debut ++;
            }
            
        }
        
        
        return route;
    
        
    }
}
#endif