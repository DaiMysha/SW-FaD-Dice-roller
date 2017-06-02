#include <iostream>
#include <sstream>
#include <array>

#include <SFML/Graphics.hpp>

#define WIDTH   640
#define HEIGHT  480


int main(int argc, char** argv)
{
    /** SFML STUFF **/

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SAT test");

    //the loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            /*else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                mouseClick = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };

                for (size_t i = 0; i < boxes.size(); ++i)
                {
                    if (affectedBox == -1 && boxes[i].getGlobalBounds().contains(mouseClick.x, mouseClick.y ))
                    {
                        affectedBox = i;
                    }
                }
                if (affectedBox != -1)
                {
                    switch (event.mouseButton.button)
                    {
                    case sf::Mouse::Left://move
                        dragging = true;
                        break;
                    case sf::Mouse::Right://move
                        rotating = true;
                        break;
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                affectedBox = -1;
                dragging = false;
                rotating = false;
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                if (affectedBox != -1)
                {
                    if (dragging)
                    {
                        sf::Vector2f newPos = boxes[affectedBox].getPosition();
                        newPos = { newPos.x + event.mouseMove.x - mouseClick.x, newPos.y + event.mouseMove.y - mouseClick.y };
                        boxes[affectedBox].setPosition(newPos);
                        mouseClick = { (float)event.mouseMove.x, (float)event.mouseMove.y };
                    }
                    else if (rotating)
                    {
                        mouseClick = { (float)event.mouseMove.x, (float)event.mouseMove.y };
                        float angle = getAngleBetweenVectors(xVector, mouseClick - boxes[affectedBox].getPosition());
                        boxes[affectedBox].setRotation(angle * 180.0f / (float)PI);
                    }
                    calcNormals(boxes, normals);
                    calcProjections(projections, boxes, normals);
                    for (size_t i = 0; i < collisions.size(); ++i)
                    {
                        collisions[i] = projections[i].collides(projections[i+normals.size()]);
                    }
                }
            }*/
        }
    }

    return 0;
}
