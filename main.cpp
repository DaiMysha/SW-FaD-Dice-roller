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

const sf::Color diceColors[] = { {194,227,238}, {65,167,21}, {248,240,53}, {53,53,53}, {38,0,70}, {234,0,0},  { 250, 250, 180 } };

const float uiScale = 0.5f;
const sf::Vector2f delta(10, 10);

const sf::Color buttonColor(190, 150, 70);

void drawDice(sf::RenderTarget& target, int x, int y, sf::RectangleShape& dice, DiceType type, ResultType result, float scale = 1.0f)
{
	dice.setScale(scale, scale);
	dice.setPosition((float)x, (float)y);
	dice.setTextureRect(sf::IntRect(type*diceSize, result*diceSize, diceSize, diceSize));
	target.draw(dice);
}

void drawD100(sf::RenderTarget& target, int x, int y, sf::ConvexShape& shape, float scale = 1.0f)
{
	shape.setScale(scale, scale);
	shape.setPosition((float)x, (float)y);
	target.draw(shape);
}

void drawArrow(sf::RenderTarget& target, sf::ConvexShape& arrow, int x, int y, sf::Color color, bool up)
{
	arrow.setFillColor(color);
	arrow.setPosition((float)x, (float)y);
	arrow.setScale(1.0f, up ? 1.0f : -1.0f);
	target.draw(arrow);
}

void drawUi(sf::RenderTarget& target, sf::ConvexShape& d100, sf::RectangleShape& dices, sf::ConvexShape& arrow, sf::Font& font, int dicesToThrow[D100 + 1])
{
	drawD100(target, (int)(diceSize*uiScale / 2.0f + delta.x), (int)(diceSize*uiScale + delta.y*2) , d100, uiScale);
	for (int i = 0; i < D100; ++i)
	{
		drawDice(target, (int)((i+1.5f)*(diceSize*uiScale + delta.x)), (int)(diceSize*uiScale + delta.y*2), dices, DiceType(i), EMPTY, uiScale);
	}

	drawArrow(target, arrow, (int)(diceSize*uiScale / 2.0f + delta.x), (int)(delta.y - arrow.getPoint(0).y), diceColors[D100], true);
	drawArrow(target, arrow, (int)(diceSize*uiScale / 2.0f + delta.x), (int)(3*delta.y - arrow.getPoint(0).y + diceSize*uiScale), diceColors[D100], false);
	for (int i = 0; i < D100; ++i)
	{
		drawArrow(target, arrow, (int)((i + 1.5f)*(diceSize*uiScale + delta.x)), (int)(delta.y - arrow.getPoint(0).y), diceColors[i], true);
		drawArrow(target, arrow, (int)((i + 1.5f)*(diceSize*uiScale + delta.x)), (int)(3 * delta.y - arrow.getPoint(0).y + diceSize*uiScale), diceColors[i], false);
	}

	sf::RectangleShape button;
	button.setSize(sf::Vector2f(139.0f, 139.0f / 2.0f));
	button.setOrigin(button.getSize() / 2.0f);
	button.setScale(uiScale, uiScale);
	button.setFillColor(buttonColor);

	sf::Text text;
	text.setFont(font);
	text.setColor(sf::Color::Black);

	//dices amounts
	char ctext[5];

	//D100
	sprintf(ctext, "%02d", dicesToThrow[D100]);
	text.setPosition(d100.getPosition().x - 22 + 3, d100.getPosition().y - 20 + 3);
	text.setString(ctext);
	text.setCharacterSize(25);
	target.draw(text);

	for (int i = 0; i < D100; ++i)
	{
		sprintf(ctext, "%02d", dicesToThrow[i]);
		text.setString(ctext);
		text.setPosition(d100.getPosition().x - 22 + 3 + (diceSize*uiScale + delta.x) * (i + 1) - 5, d100.getPosition().y - 20 + 3);
		if (i > PROFICIENCY)
		{
			text.setColor(sf::Color::White);
		}
		target.draw(text);
	}
	
	text.setCharacterSize(button.getSize().y * uiScale * button.getScale().y);
	text.setColor(sf::Color::Black);

	//button reset
	button.setPosition(WIDTH - delta.x - button.getSize().x / 2.0f * uiScale, (int)(diceSize*uiScale - delta.y*(1 + uiScale)));
	target.draw(button);
	text.setString("RESET");
	text.setPosition(button.getPosition().x - button.getOrigin().x/2 + 3, button.getPosition().y - button.getOrigin().y/2 + 6);
	target.draw(text);

	//button Roll
	button.setPosition(WIDTH - delta.x - button.getSize().x / 2.0f * uiScale, (int)(diceSize*uiScale + delta.y*(5 + uiScale)));
	target.draw(button);
	text.setString("ROLL");
	text.setPosition(button.getPosition().x - button.getOrigin().x / 2 + 10, button.getPosition().y - button.getOrigin().y / 2 + 6);
	target.draw(text);
}

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
    sf::RectangleShape diceSheet;
	diceSheet.setSize(sf::Vector2f((float)diceSize, (float)diceSize));
	diceSheet.setTexture(&diceTexture);
	diceSheet.setTextureRect(sf::IntRect(0,0,diceSize,diceSize));
	diceSheet.setOrigin(sf::Vector2f(diceSize / 2.0f, diceSize / 2.0f));

	sf::ConvexShape d100Shape;
	d100Shape.setPointCount(4);
	d100Shape.setPoint(0, sf::Vector2f(0, -66));
	d100Shape.setPoint(1, sf::Vector2f(66, 0));
	d100Shape.setPoint(2, sf::Vector2f(0, 66));
	d100Shape.setPoint(3, sf::Vector2f(-66, 0));
	d100Shape.setFillColor(sf::Color(250, 250, 180));

	sf::ConvexShape arrow;
	arrow.setPointCount(3);
	arrow.setPoint(0, sf::Vector2f(0, -33));
	arrow.setPoint(1, sf::Vector2f(20, 0));
	arrow.setPoint(2, sf::Vector2f(-20, 0));

	int dicesToThrow[D100 + 1] = { 0 };

	float uiScale = 0.5f;

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
        window.clear(sf::Color(40,45,100));
		drawUi(window, d100Shape, diceSheet, arrow, font, dicesToThrow);
		window.display();

        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
