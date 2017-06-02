#include <iostream>
#include <sstream>
#include <array>

#include <SFML/Graphics.hpp>

#define WIDTH   640
#define HEIGHT  480

//french :       fortune, aptitude, maitrise,   infortune, difficulte, defi
enum DiceType   {BOOST,   ABILITY, PROFICIENCY, SETBACK,   DIFFICULTY, CHALLENGE, D100};
enum ResultType {EMPTY, MINOR, MAJOR, MASSIVE, DOUBLE_MINOR, DOUBLE_MAJOR, MIXED};

//these tables are used to correspond to the actual placement in the real dices
//                              1         2             3             4             5             6      7             8      9            10             11     12
const int boostResult[] =       {EMPTY,   DOUBLE_MINOR, MINOR,        MAJOR,        MIXED,        EMPTY};
const int abilityResult[] =     {EMPTY,   DOUBLE_MAJOR, MINOR,        MAJOR,        MAJOR,        MINOR, DOUBLE_MINOR, MIXED};
const int proficiencyResult[] = {EMPTY,   MIXED,        DOUBLE_MINOR, MIXED,        MAJOR,        MAJOR, MAJOR,        MAJOR, MINOR,        DOUBLE_MINOR, MIXED, MASSIVE};
const int setbackResult[] =     {EMPTY,                 MAJOR,        MINOR,        MINOR,        MAJOR, EMPTY};
const int difficultyResult[] =  {EMPTY,   MINOR,        MIXED,        DOUBLE_MAJOR, MINOR,        MINOR, DOUBLE_MINOR, MAJOR};
const int challengeResult[] =   {MASSIVE, DOUBLE_MINOR, DOUBLE_MAJOR, MAJOR,        DOUBLE_MAJOR, MINOR, MAJOR,        MINOR, DOUBLE_MINOR, MIXED,        MIXED, EMPTY};

const int diceSize = 139;

int main(int argc, char** argv)
{
    /** SFML STUFF **/

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SW FaD Dice Roller");
    sf::Font font;
    if(!font.loadFromFile("data\\Rebellion.otf"))
    {
        std::cerr << "Impossible to load font" << std::endl;
        return -1;
    }
    sf::Texture diceTexture;
    if(!diceTexture.loadFromFile("data\\dices.png"))
    {
        std::cerr << "Impossible to load dice picture" << std::endl;
        return -1;
    }
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(diceSize, diceSize));
    float scale = 0.3f;
    rectangle.setScale(scale,scale);
    rectangle.setTexture(&diceTexture);
    rectangle.setTextureRect(sf::IntRect(0,0,diceSize,diceSize));
    int x = 0;
    int y = 0;

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
            else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
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
                //event.mouseButton.x, event.mouseButton.y
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
            }
        }
        window.clear(sf::Color(127,127,127));

        rectangle.setPosition(0,0);
        rectangle.setTextureRect(sf::IntRect(x*diceSize,y*diceSize,diceSize,diceSize));
        window.draw(rectangle);

        rectangle.setPosition(diceSize,0);
        rectangle.setTextureRect(sf::IntRect((x+1)*diceSize,y*diceSize,diceSize,diceSize));
        window.draw(rectangle);
        window.display();
        ++x;
        if(x >= CHALLENGE)
        {
            x = 0;
            ++y;
            if(y > MIXED)
            {
                y = 0;
            }
        }
        sf::sleep(sf::milliseconds(200));
    }

    return 0;
}
