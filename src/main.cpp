
#include <SFML/Graphics.hpp>
#include <iostream>
#include "fonctions_grapiphiques.hpp"

#include "fonctions_math.hpp"
#include "gestion_option.hpp"

using namespace testSFML;
int main(int argc, char** argv)
{

    gestion_option param(argc, argv);
    param.add("--ecart-type-centre","1");
    param.add("--ecart-type-maison","1");
    param.add("--nb-maison",1000);
    
    param.allow_raw_args(false);
    param.load_file("./exe.param");

    double ecart_type_centre = param.get_val<double>("--ecart-type-centre");
    double ecart_type_maison = param.get_val<double>("--ecart-type-maison");
    int nb_maison = param.get_val<int>("--nb-maison");

    normal normal_centre(0,ecart_type_centre); // (default mean = zero, and standard deviation = unity)
    normal normal_maison(0,ecart_type_maison);

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
    test.repartition = normal_centre;
    centres.push_back(test);


    centres.push_back({sf::Vector2f(10,10),normal_centre});
    centres.push_back({sf::Vector2f(200,200),normal_centre});
    // Pourquoi ça marche pas ?
    // centres.emplace_back(sf::Vector2f(10,200),s);


      // centres.emplace_back();

	for (const auto& centre : centres )
	{
			cercles.emplace_back(5);
			cercles.back().setPosition(centre.centre);
	}

    auto create_maison = [&] ()
    {
        cercles.emplace_back(2);
        cercles.back().setPosition(random_point(centres,{0,0},{taille,taille}));
        centres.push_back({cercles.back().getPosition(),normal_maison,1});
    };
    for (int i= 0 ; i < nb_maison ; i ++ )
    {
        create_maison();
    }

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
				auto mousePos = convert_to<sf::Vector2f> ( sf::Mouse::getPosition(window) );
				points.emplace_back(mousePos);

               /* double dist =  distance(cercles.front().getPosition(),sf::Mouse::getPosition(window) ) ;
                std::cout << "distance : " <<dist << std::endl;
				std::cout << getGaussianValue(s,dist/100) << std::endl;*/

			    double value = sum_valule(mousePos,centres);
				double coef_total = sum_coef (centres);

				std::cout << "le coef total est : " << coef_total << " ; la valeur du point est : " << value << " le ratio est : " << value/coef_total << std::endl;
                std::cout << "il y a " << centres.size() << " centres "<< std::endl;
            }

            if ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return )
			{

				std::cout << "entrer" << std::endl;
				poly.emplace_back(createPolygone(points));
				points.clear();
                create_maison();



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
