#ifndef MATH_HPP
#define MATH_HPP

#include<cmath>

#include <boost/math/distributions/normal.hpp> // for normal_distribution

#include "fonctions_grapiphiques.hpp"
#include "traits.hpp"
#include <type_traits>


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
		using typeRetour = typename std::common_type<decltype(getX(p1)),
											decltype(getX(p2)),
											decltype(getY(p1)),
											decltype(getY(p2))
										   >::type ;
		
		
        return sqrt(
                        pow (typeRetour( getX(p1) ) - typeRetour( getX(p2) ), 2)
                        +
                        pow (typeRetour( getY(p1) ) - typeRetour( getY(p2) ), 2)
                    );
    }


}

#endif
