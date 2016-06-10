#ifndef MATH_HPP
#define MATH_HPP

#include<cmath>

#include <boost/math/distributions/normal.hpp> // for normal_distribution

#include "fonctions_grapiphiques.hpp"
#include "traits.hpp"
#include <type_traits>
#include <chrono>
#include <random>

namespace testSFML {

    using boost::math::normal; // typedef provides default type is double.

    inline double getGaussianValue(const normal& norm, const double& distance ){
        return pdf(norm,distance);
    }

    template<   class Point,
                class Point2, // on match le meme concept mais pas forcmeent la meme vrais classe
                class V = typename std::enable_if< is_point<Point>::value >::type,
                class X = typename std::enable_if< is_point<Point2>::value >::type
            >
    inline double distance (Point&& p1, Point2&& p2)
    {
        /// DANGER : COMMENT je fais ça proprement cast du int en float ? ou en double enfin bref comment je cast
        /// en "le plus precis"

		// Je ne sais pas si c'est ce que je veux faire, globalement je dis au compilo, "je sais ce que je fais"
		// mais je sais pas si je devrait pas laisser le warning, en me disant que si le mec a mis -WConversion
		// c'est qu'il veut l'avoir ?
		/*using typeRetour = typename std::common_type<decltype(getX(p1)),
											decltype(getX(p2)),
											decltype(getY(p1)),
											decltype(getY(p2))
										   >::type ;


        return sqrt(
                        pow (typeRetour( getX(p1) ) - typeRetour( getX(p2) ), 2)
                        +
                        pow (typeRetour( getY(p1) ) - typeRetour( getY(p2) ), 2)
                    );*/
        return sqrt(
                        pow (( getX(p1) ) - ( getX(p2) ), 2)
                        +
                        pow (( getY(p1) ) - ( getY(p2) ), 2)
                    );


    }


    template< class PointRetour = sf::Vector2f,
              class Point1 = sf::Vector2f,
              class Point2 = sf::Vector2f,
              class V = typename std::enable_if< is_point<PointRetour>::value >::type,
              class W = typename std::enable_if< is_point<Point1>::value >::type,
              class X = typename std::enable_if< is_point<Point2>::value >::type
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
                class W = typename std::enable_if< is_point<Point>::value >::type>
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
           /* auto max = getGaussianValue(repartition,0);
            return max_value * getGaussianValue(repartition,distance(centre,pt) / 100 ) / max ;*/
		   
		    return coef * getGaussianValue(repartition,distance(centre,pt) / 100 ) ;
		   
        }
    };

	
	// Il faudrait surement que je refonte ça, ça ressemble a un algo de la std !
    template <  class conteneur ,
                class Point = sf::Vector2f,
                class X = typename std::enable_if< is_point<Point>::value >::type>
    double sum_valule (const Point& p , const conteneur& cont_centre_influence )
    {
        double value = 0;
		std::cout << "je calcul la somme de " ;
        for (const auto & centre : cont_centre_influence )
        {
            value += centre.getValue(p);
			std::cout << value << " ";
        }
        std::cout << std::endl;
        return value;
    }
    
	template <  class conteneur >
    double sum_coef( const conteneur& cont_centre_influence )
    {
        double value = 0;
        for (const auto & centre : cont_centre_influence )
        {
            value += centre.coef;
        }
        return value;
    }

    /// VERSION NAZE, il FAUT un param de trop !!
    // il faudrait que je regarde comment m'assurer que c'est bien un conteneur de centre_influence
    template <  class conteneur ,
                class PointRetour = sf::Vector2f,
                class Point1 = sf::Vector2f,
                class Point2 = sf::Vector2f,
                class V = typename std::enable_if< is_point<PointRetour>::value >::type,
                class W = typename std::enable_if< is_point<Point1>::value >::type,
                class X = typename std::enable_if< is_point<Point2>::value >::type>
    PointRetour random_point (const conteneur& cont_centre_influence, const Point1& p1,const Point2& p2, size_t nb_try)
    {
        // nb try + ecrat type de la gaussienne sont a modifier en couple
        PointRetour retour;
        double max_value = -1 ;
        for (size_t i = 0 ; i < nb_try ; i ++ )
        {
            // on prend un point au piff dans l'interval
            PointRetour new_point = random_point(p1,p2);

            double value = sum_valule(new_point,cont_centre_influence);
            if (value > max_value)
            {
                max_value = value;
                retour = new_point;
            }
        }
        return retour;
    }
    
    
    // tentative de faire mieux :)
	template <  class conteneur ,
                class PointRetour = sf::Vector2f,
                class Point1 = sf::Vector2f,
                class Point2 = sf::Vector2f,
                class V = typename std::enable_if< is_point<PointRetour>::value >::type,
                class W = typename std::enable_if< is_point<Point1>::value >::type,
                class X = typename std::enable_if< is_point<Point2>::value >::type>
    PointRetour random_point (const conteneur& cont_centre_influence, const Point1& p1,const Point2& p2)
    {
        // nb try + ecrat type de la gaussienne sont a modifier en couple
        PointRetour retour;
        double max_value = -1 ;
		
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
