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
namespace testSFML {

    using boost::math::normal; // typedef provides default type is double.

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
                class X = typename std::enable_if< is_point<Point>::value >::type,
                class XX= typename std::enable_if< is_point<Point2>::value >::type
            >
    inline double distance ( Point&& p1,  Point2& p2)
    {

        return sqrt(
                        pow (( getX(p1) ) - ( getX(p2) ), 2)
                        +
                        pow (( getY(p1) ) - ( getY(p2) ), 2)
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
              class X =  typename std::enable_if< is_point<PointRetour>::value >::type,
              class XX = typename std::enable_if< is_point<Point1>::value >::type,
              class XXX = typename std::enable_if< is_point<Point2>::value >::type
            >
    inline PointRetour random_point(const Point1& p1,const Point2& p2 )
    {
        std::random_device rd;
        std::mt19937 mt(rd());

        // il se fout de savoir qui est le plus petit des deux, ça marche bien :)
        std::uniform_real_distribution<float> random_x (getX(p1), getX(p2));
        std::uniform_real_distribution<float> random_y (getY(p1), getY(p2));

        return {random_x(mt),random_y(mt)};
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
                class X = typename std::enable_if< is_point<Point>::value >::type,
                class XX = typename std::enable_if< is_container<conteneur>::value >::type>
    double sum_valule (const Point& p , const conteneur& cont_centre_influence )
    {
		return std::accumulate	(	cont_centre_influence.begin(),
									cont_centre_influence.end(),
									0,
									[&](const double & somme , const auto centre) { return somme + centre.getValue(p); }
								);
    }

	template <  class conteneur ,
                    class XX = typename std::enable_if< is_container<conteneur>::value >::type >
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
                class X =  typename std::enable_if< is_point<PointRetour>::value >::type,
                class XX=  typename std::enable_if< is_point<Point1>::value >::type,
                class XY=  typename std::enable_if< is_point<Point2>::value >::type,
                class ZX = typename std::enable_if< is_container<conteneur>::value >::type>
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
