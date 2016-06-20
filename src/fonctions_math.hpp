#ifndef MATH_HPP
#define MATH_HPP

#include<cmath>

#include <boost/math/distributions/normal.hpp> // for normal_distribution

#include "fonctions_grapiphiques.hpp"
#include "traits.hpp"
#include "convertion.hpp"

#include <type_traits>
#include <chrono>
#include <numeric>
#include <random>
#include <cassert>
#include <cmath>
namespace testSFML {

	constexpr double PI = 3.141592653589793;
    using boost::math::normal; // typedef provides default type is double.

    
	template <class T, class U = void >
	struct random_dispatch {
		using distribution = typename std::uniform_int_distribution<T>;
	};

	template <class T>
	struct random_dispatch	<    T,
								typename std::enable_if<std::is_floating_point<T>::value>::type
							>  
	{
		using distribution = typename std::uniform_real_distribution<T>;
		
	};
    // DANGER il semblerait que ça marche int int :D
    // je me fout du std forward parce que ça sera de type double int... et dans tous les cas
    // un forward sur un type base c'est une copie
    template < class N,
			   class N2>
    inline std::common_type_t<N,N2> random (N&& min, N2&& max)
	{
            using max_type = std::common_type_t<N,N2>;
			
            static std::random_device rd;
            static std::mt19937 mt(rd());
            static typename random_dispatch<max_type>::distribution rand (min,max); 
            // (max_type(min) , max_type(max)) ;  marche pas, etrange à tester avec clang
            return rand(mt);
	}
    
    
    
    inline double getGaussianValue(const normal& norm, const double& distance ){
        return pdf(norm,distance);
    }

    /***
	 *
	 * Je sais que la conversion peut faire des trucs moches (comme des warnings)
	 * Je pourrais ecrire un code qui ne fait jamais de warnings mais je considaire que si
	 * l'utilisateur compile en -WConversion c'est qu'il veut les avoirs
	 * donc je les bloques pas
	 * **/
    template<   class Point,
                class Point2, // on match le meme concept mais pas forcmeent la meme vrais classe
                class = typename std::enable_if< is_point<Point>::value >::type,
                class = typename std::enable_if< is_point<Point2>::value >::type
            >
    inline double distance ( Point&& p1,  Point2& p2)
    {

        return sqrt(
                        pow (( getX(p1) ) - ( getX(p2) ), 2)
                        +
                        pow (( getY(p1) ) - ( getY(p2) ), 2)
                    );

    }



    template < 	class Point,
				class Point2,
				class Point3,
				class = typename std::enable_if< is_point<Point>::value >::type,
				class = typename std::enable_if< is_point<Point2>::value >::type,
				class = typename std::enable_if< is_point<Point3>::value >::type
			 >
    inline double angle_rad ( Point&& p1,Point2&& centre,Point3&& p2)
	{
		double distance_1_centre = distance (p1,centre);
		double distance_2_centre = distance (p2,centre);
		double distance_1_2 	 = distance (p1,p2);
		
		return acos(  ( pow ( distance_1_centre ,2) + pow ( distance_2_centre ,2) - pow ( distance_1_2 ,2))  
					  /
					  (2 * distance_1_centre * distance_2_centre)
					);
	}

	template < class N >
	inline double convertion_rad_deg ( N&& value ) 
	{
		return (value *180 ) / PI;
	}
	    
	
	template < 	class Point,
				class Point2,
				class Point3,
				class = typename std::enable_if< is_point<Point>::value >::type,
				class = typename std::enable_if< is_point<Point2>::value >::type,
				class = typename std::enable_if< is_point<Point3>::value >::type
			>
	inline double angle_deg ( Point&& p1,Point2&& centre,Point3&& p2)
	{
		return convertion_rad_deg ( angle_rad ( std::forward<Point>(p1 ), 
												std::forward<Point2> (centre),
												std::forward<Point3> (p2 )
											  )
								  );
	}
	
	/***
	 *
	 * Creation d'un point dans le rectangle defini par les deux autres points
	 * distributions lineaire.
	 *
	 **/
    template< class PointRetour = sf::Vector2f,
              class Point1 = sf::Vector2f,
              class Point2 = sf::Vector2f,
              class =  typename std::enable_if< is_point<PointRetour>::value >::type,
              class = typename std::enable_if< is_point<Point1>::value >::type,
              class = typename std::enable_if< is_point<Point2>::value >::type
            >
    inline PointRetour random_point(const Point1& p1,const Point2& p2 )
    {
        return 	{ random(getX(p1), getX(p2)),
			      random(getY(p1), getY(p2))
				};
    }


    template <  class Point = sf::Vector2f,
               class X = typename std::enable_if< is_point<Point>::value >::type>
    struct centre_influence
    {
        Point centre;
        normal repartition;
        double coef = 100 ;
        template <  class Point2 = sf::Vector2f,
                    class WW = typename std::enable_if< is_point<Point2>::value >::type>
        double getValue(const Point2 & pt) const
        {
            // la division par 100 est du que je réduit 100 pixel à "1" unité pour la gaussien,
            // revois ça apres
			return coef * getGaussianValue(repartition,distance(centre,pt) / 100 ) ;
        }
    };


    template <  class conteneur ,
                class Point = sf::Vector2f,
                class = typename std::enable_if< is_point<Point>::value >::type,
                class = typename std::enable_if< is_container<conteneur>::value >::type>
    double sum_valule (const Point& p , const conteneur& cont_centre_influence )
    {
		return std::accumulate	(	cont_centre_influence.begin(),
									cont_centre_influence.end(),
									0,
									[&](const double & somme , const auto centre) { return somme + centre.getValue(p); }
								);
    }

	template <  class conteneur ,
				class = typename std::enable_if< is_container<conteneur>::value >::type >
    double sum_coef( const conteneur& cont_centre_influence )
    {
        return std::accumulate	(	cont_centre_influence.begin(),
									cont_centre_influence.end(),
									0,
									[](const double & somme , const auto centre) { return centre.coef+somme; }
								);
    }



    /****
	 *
	 *
	 *
	 **/
	template <  class conteneur ,
                class PointRetour = sf::Vector2f,
                class Point1 = sf::Vector2f,
                class Point2 = sf::Vector2f,
                class =  typename std::enable_if< is_point<PointRetour>::value >::type,
                class =  typename std::enable_if< is_point<Point1>::value >::type,
                class =  typename std::enable_if< is_point<Point2>::value >::type,
                class = typename std::enable_if< is_container<conteneur>::value >::type>
    PointRetour random_point (const conteneur& cont_centre_influence, const Point1& p1,const Point2& p2)
    {
        PointRetour retour;

		std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> linear_rand (0,1);

		bool fini = false;
        
        while (!fini)
        {
            // on prend un point au piff dans l'interval
            PointRetour new_point = random_point(p1,p2);

            double value = sum_valule(new_point,cont_centre_influence);
			double coef_total = sum_coef (cont_centre_influence);
            if (coef_total == 0 )
            {
                retour = new_point;
				fini = true;
            }
			if ( linear_rand(mt) < (value/coef_total))
			{
				retour = new_point;
				fini = true;
			}
        }
        return retour;
    }

}

#endif
