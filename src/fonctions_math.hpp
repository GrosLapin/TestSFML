#ifndef MATH_HPP
#define MATH_HPP

#include<cmath>

#include <boost/math/distributions/normal.hpp> // for normal_distribution

#include "traits.hpp"

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
        return sqrt(
                        pow (getX(p1) -getX(p2), 2)
                        +
                        pow (getY(p1) -getY(p2), 2)
                    );
    }


}

#endif
