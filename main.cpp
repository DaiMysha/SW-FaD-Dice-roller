#include <iostream>
#include <sstream>
#include <array>
#include <list>

#include <SFML/Graphics.hpp>

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define WIDTH   640
#define HEIGHT  480

//french :			   fortune, aptitude, maitrise,   infortune, difficulte, defi
enum DiceType   { D100, BOOST,   ABILITY, PROFICIENCY, SETBACK,   DIFFICULTY, CHALLENGE, TYPESIZE};
enum ResultType {EMPTY, MINOR, MAJOR, MASSIVE, DOUBLE_MINOR, DOUBLE_MAJOR, MIXED};

//these tables are used to correspond to the actual placement in the real dices
//                              1         2             3             4             5             6			7               8		 9            10             11     12
const int boostResult[] =       {EMPTY,   DOUBLE_MINOR, MINOR,        MAJOR,        MIXED,        EMPTY};
const int abilityResult[] =     {EMPTY,   DOUBLE_MAJOR, MINOR,        MAJOR,        MAJOR,        MINOR,	DOUBLE_MINOR,	MIXED};
const int proficiencyResult[] = {EMPTY,   MIXED,        DOUBLE_MINOR, MIXED,        MAJOR,        MAJOR,	MAJOR,			 MAJOR,	 MINOR,        DOUBLE_MINOR, MIXED, MASSIVE};
const int setbackResult[] =     {EMPTY,   MAJOR,        MINOR,        MINOR,        MAJOR,	      EMPTY};
const int difficultyResult[] =  {EMPTY,   MINOR,        MIXED,        DOUBLE_MAJOR, MINOR,        MINOR,	DOUBLE_MINOR,	MAJOR};
const int challengeResult[] =   {MASSIVE, DOUBLE_MINOR, DOUBLE_MAJOR, MAJOR,        DOUBLE_MAJOR, MINOR,	MAJOR,			 MINOR,	 DOUBLE_MINOR, MIXED,        MIXED, EMPTY};

const int* resultTable[] = { boostResult, abilityResult, proficiencyResult, setbackResult, difficultyResult, challengeResult };

const int diceSize = 139;

const sf::Color diceColors[] = { { 250, 250, 180 }, {194,227,238}, {65,167,21}, {248,240,53}, {53,53,53}, {38,0,70}, {234,0,0} };
const int diceSizes[] = { 100, 6, 8, 12, 6, 8, 12 };

const float uiScale = 0.5f;
const sf::Vector2f delta(10, 10);

const sf::Color buttonColor(190, 150, 70);

sf::IntRect buttons[2];
sf::IntRect arrowsUp[TYPESIZE];
sf::IntRect arrowsDown[TYPESIZE];

struct DiceResult
{
	DiceType type;
	int value; // can be converted to ResultType
};

void drawDice(sf::RenderTarget& target, int x, int y, sf::RectangleShape& dice, DiceType type, ResultType result, float scale = 1.0f)
{
	dice.setScale(scale, scale);
	dice.setPosition((float)x, (float)y);
	dice.setTextureRect(sf::IntRect((type-1)*diceSize, result*diceSize, diceSize, diceSize));
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
	for (int i = 0; i < TYPESIZE - 1; ++i)
	{
		drawDice(target, (int)((i+1.5f)*(diceSize*uiScale + delta.x)), (int)(diceSize*uiScale + delta.y*2), dices, DiceType(i+1), EMPTY, uiScale);
	}

	drawArrow(target, arrow, (int)(diceSize*uiScale / 2.0f + delta.x), (int)(delta.y - arrow.getPoint(0).y), diceColors[D100], true);
	drawArrow(target, arrow, (int)(diceSize*uiScale / 2.0f + delta.x), (int)(3*delta.y - arrow.getPoint(0).y + diceSize*uiScale), diceColors[D100], false);
	for (int i = 1; i < TYPESIZE; ++i)
	{
		drawArrow(target, arrow, (int)((i + 1.5f - 1)*(diceSize*uiScale + delta.x)), (int)(delta.y - arrow.getPoint(0).y), diceColors[i], true);
		drawArrow(target, arrow, (int)((i + 1.5f - 1)*(diceSize*uiScale + delta.x)), (int)(3 * delta.y - arrow.getPoint(0).y + diceSize*uiScale), diceColors[i], false);
	}

	sf::RectangleShape button;
	button.setSize(sf::Vector2f(139.0f, 139.0f / 2.0f));
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

	for (int i = 0; i < TYPESIZE - 1; ++i)
	{
		sprintf(ctext, "%02d", dicesToThrow[i+1]);
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
	button.setPosition(buttons[0].left, buttons[0].top);
	target.draw(button);
	text.setString("RESET");
	text.setPosition(button.getPosition().x - button.getOrigin().x/2 + 3, button.getPosition().y - button.getOrigin().y/2 + 6);
	target.draw(text);

	//button Roll
	button.setPosition(buttons[1].left, buttons[1].top);
	target.draw(button);
	text.setString("ROLL");
	text.setPosition(button.getPosition().x - button.getOrigin().x / 2 + 10, button.getPosition().y - button.getOrigin().y / 2 + 6);
	target.draw(text);
}

void roll(int diceToThrow[], std::list<DiceResult>& results)
{
	for (int i = 0; i < diceToThrow[0]; ++i)
	{
		DiceResult r;
		r.type = D100;
		r.value = rand() % 100 + 1;
		results.emplace_back(r);
	}

	for (int i = 1; i < TYPESIZE; ++i)
	{
		for (int j = 0; j < diceToThrow[i]; ++j)
		{
			DiceResult r;
			r.type = DiceType(i);
			r.value = resultTable[i - 1][rand() % diceSizes[i]];
			results.emplace_back(r);
		}
	}
}

void drawResults(sf::RenderTarget& target, const std::list<DiceResult>& results, sf::RectangleShape& dices, sf::ConvexShape& d100, sf::Font& font)
{
	float scale = 0.5; //9 max with scale 0.5
	int dy = arrowsDown[0].top + arrowsDown[0].height + 5 + diceSize * scale;
	int dx = diceSize * scale / 2;
	int x = dx;
	int y = dy;

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.setColor(sf::Color::Black);

	for (const DiceResult& r : results)
	{
		if (r.type == D100)
		{
			drawD100(target, x, y, d100, scale);
			char ctext[5];
			sprintf(ctext, "%02d", r.value);
			text.setString(ctext);
			text.setPosition(x - diceSize*scale/2 + text.getCharacterSize(), y - text.getCharacterSize() / 2 - 3);
			target.draw(text);
		}
		else
		{
			drawDice(target, x, y, dices, r.type, (ResultType)r.value, scale);
		}
		x += diceSize*scale;
		if (x >= WIDTH)
		{
			x = dx;
			y += diceSize*scale;
		}
	}
}


int main(int argc, char** argv)
{
    /** SFML STUFF **/
	srand(time(nullptr));

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

	buttons[0].width = 139.0f * uiScale;
	buttons[0].height = 139.0f / 2.0f * uiScale;
	buttons[0].left = WIDTH - delta.x - buttons[0].width;
	buttons[0].top = (int)(diceSize*uiScale - delta.y*(1 + uiScale) - buttons[0].height / 2);

	buttons[1].width = buttons[0].width;
	buttons[1].height = buttons[0].height;
	buttons[1].left = buttons[0].left;
	buttons[1].top = (int)(diceSize*uiScale + delta.y*(5 + uiScale) - buttons[0].height / 2);

	for (int i = 0; i < TYPESIZE; ++i)
	{
		arrowsUp[i].width = 40;
		arrowsUp[i].height = 33;
		arrowsUp[i].left = (int)((i + 1.5f - 1)*(diceSize*uiScale + delta.x)) - arrowsUp[i].width/2;
		arrowsUp[i].top = (int)(delta.y - arrow.getPoint(0).y) - arrowsUp[i].height;

		arrowsDown[i].width = 40;
		arrowsDown[i].height = 33;
		arrowsDown[i].left = arrowsUp[i].left;
		arrowsDown[i].top = (int)(3 * delta.y - arrow.getPoint(0).y + diceSize*uiScale);
	}
	arrowsUp[0].left += 5;
	arrowsDown[0].left = arrowsUp[0].left;

	int dicesToThrow[TYPESIZE] = { 0 };
	std::list<DiceResult> resultList;

	float uiScale = 0.5f;
	sf::Vector2u mouse;

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
			else if (event.type == sf::Event::MouseMoved)
			{
				mouse.x = event.mouseMove.x;
				mouse.y = event.mouseMove.y;
			}
            else if (event.type == sf::Event::MouseButtonReleased)
            {
				if (buttons[0].contains(mouse.x, mouse.y))
				{
					resultList.clear();
					for (int i = 0; i < TYPESIZE; ++i)
					{
						dicesToThrow[i] = 0;
					}
				}
				else if(buttons[1].contains(mouse.x, mouse.y))
				{
					resultList.clear();
					roll(dicesToThrow, resultList);
				}
				else
				{
					for (int i = 0; i < TYPESIZE; ++i)
					{
						if (arrowsUp[i].contains(mouse.x, mouse.y))
						{
							if (dicesToThrow[i] < 100)
							{
								++dicesToThrow[i];
							}
						}
						else if (arrowsDown[i].contains(mouse.x, mouse.y))
						{
							if (dicesToThrow[i] > 0)
							{
								--dicesToThrow[i];
							}
						}
					}
				}
            }
        }

        window.clear(sf::Color(40,45,100));
		drawUi(window, d100Shape, diceSheet, arrow, font, dicesToThrow);
		drawResults(window, resultList, diceSheet, d100Shape, font);
		window.display();

        sf::sleep(sf::milliseconds(16));
    }

    return 0;
}
