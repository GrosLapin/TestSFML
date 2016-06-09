
#include <SFML/Graphics.hpp>
#include <iostream>
#include "fonctions_grapiphiques.hpp"

#include "fonctions_math.hpp"

using namespace testSFML;
int main()
{
     normal s(0,2); // (default mean = zero, and standard deviation = unity)


    const int taille = 500;
    sf::RenderWindow window(sf::VideoMode(taille, taille), "SFML works!");


    std::cout << "v2F " << is_point<sf::Vector2f>::value << std::endl;
    std::cout << "v2i " << is_point<sf::Vector2i>::value << std::endl;
    std::cout << is_point<int>::value << std::endl;
    std::cout << is_point<lapin>::value << std::endl;

    std::vector<sf::Vector2f> points = { {0,0} , {150,10} , {120,90}  , {30,100} , {0,50}};


	std::vector<sf::ConvexShape> poly;
	poly.emplace_back(createPolygone(points));
	poly.emplace_back(createPolygone({ {100,100} , {250,110} , {220,190}  , {130,200} , {100,150}}));

     points.clear();


    std::vector <sf::CircleShape> cercles;


    std::vector<centre_influence<sf::Vector2f> > centres;
    centre_influence<sf::Vector2f> test;
    test.centre = sf::Vector2f(100,100);
    test.repartition = s;
    centres.push_back(test);


    centres.push_back({sf::Vector2f(10,10),s});
    centres.push_back({sf::Vector2f(10,200),s});
    // Pourquoi ça marche pas ?
    // centres.emplace_back(sf::Vector2f(10,200),s);


      // centres.emplace_back();


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

			if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
            {
				/*std::cout << " x : " << event.mouseButton.x << " y : " <<   event.mouseButton.y  << std::endl;
				std::cout << "abs :" << sf::Mouse::getPosition() << std::endl;
				std::cout << "relatif : " <<  sf::Mouse::getPosition(window) << std::endl;*/
				points.emplace_back(sf::Mouse::getPosition(window));

               /* double dist =  distance(cercles.front().getPosition(),sf::Mouse::getPosition(window) ) ;
                std::cout << "distance : " <<dist << std::endl;
				std::cout << getGaussianValue(s,dist/100) << std::endl;*/
            }

            if ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return )
			{

				std::cout << "entrer" << std::endl;
				poly.emplace_back(createPolygone(points));
				points.clear();
				cercles.emplace_back(2);
				cercles.back().setPosition(random_point(centres,{0,0},{taille,taille},10));

				centres.push_back({cercles.back().getPosition(),s,30});




			}
        }

        window.clear();
		/*for (const auto& convex : poly)
		{
			window.draw(convex);
		}*/

        for (const auto& circle : cercles)
        {
            window.draw(circle);
        }

        window.display();
    }

    return 0;
}
