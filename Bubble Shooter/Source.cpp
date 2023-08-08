#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include<SFML/Audio.hpp>
#include<string>

using namespace sf;
using namespace std;

const Color colorArray[] = { Color::Yellow, Color::Blue, Color::Green, Color::Red, Color::Cyan };

const int rowmax = 4, columnmax = 39;

struct ball
{
	FloatRect rect;
	Color color;
	bool draw;
}
grid[rowmax][columnmax]{};

int score = 0;

int EraseSimilarNearby(int row, int column, const sf::Color color, bool add, bool dir);

int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(1920, 1080), "window", Style::Fullscreen);
	window.setFramerateLimit(60);

	//text settings(main menu)
	bool hover2 = false, hover3 = false;
	Font font;
	font.loadFromFile("font.TTF");
	Text text1, text2, text3;
	text1.setFont(font); text2.setFont(font); text3.setFont(font);
	text1.setString("BUBBLE SHOOTER"); text2.setString("START"); text3.setString("EXIT");
	text1.setCharacterSize(100); text2.setCharacterSize(70); text3.setCharacterSize(60);
	text1.setPosition(500, 120); text2.setPosition(860, 540); text3.setPosition(900, 780);


	//score
	Text score_text;
	score_text.setFont(font);
	score_text.setString("SCORE : \n     " + to_string(score));
	score_text.setFillColor(Color::White);
	score_text.setPosition(0, 1000);
	score_text.setCharacterSize(24);

	//back ground
	Texture bg;
	Sprite Bg;
	bg.loadFromFile("12345.jpg");
	Bg.setTexture(bg);

	//sounds
	SoundBuffer buffer;
	SoundBuffer buffer2;
	buffer.loadFromFile("sound1.wav");
	buffer2.loadFromFile("bakht.wav");
	Sound sound;
	Sound music;
	sound.setBuffer(buffer);
	music.setBuffer(buffer2);
	music.play();


	//declarations
	const int ballradius = 25 * 2;
	FloatRect ballrect;


	//drawing balls
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 39; j++)
		{
			grid[i][j].color = colorArray[rand() % 5];
			cout << rand() % 50 << endl;
			ballrect.left = j * ballradius;
			ballrect.top = i * ballradius;
			ballrect.width = ballradius;
			ballrect.height = ballradius;
			grid[i][j].rect = ballrect;
		}
	}

	//some decclarations
	CircleShape ballshape;
	ballshape.setRadius(25);
	CircleShape target;
	bool isShot = false, start = false, exit = false;
	float speedx, speedy, positionx = 960, positiony = 980;
	int x, y;
	double angle;
	
	target.setRadius(25);
	target.setPosition(960, 980);

	//draw single ball(target) 
	for (int i = 0; i < 1; i++)
	{
		target.setFillColor(colorArray[rand() % 5]);
		//cout << rand() % 50 << endl;
	}

	while (window.isOpen())
	{
		Event LOL;
		FloatRect targetpos;
		while (window.pollEvent(LOL))
		{
			if (LOL.type == Event::Closed)window.close();

			if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();

			if (exit)window.close();
		}

		if (hover2)
		{
			text2.setStyle(Text::Underlined);
		}
		else text2.setStyle(Text::Regular);

		if (hover3)
		{
			text3.setStyle(Text::Underlined);
		}
		else text3.setStyle(Text::Regular);
		
		if (start)
		{
			if (target.getPosition().x == 960 && target.getPosition().y == 980)
			{

				if (Mouse::isButtonPressed(Mouse::Left))
				{
					isShot = true;
					x = (Mouse::getPosition(window).x) - 960 - 25; y = (Mouse::getPosition(window).y) - 980 - 25;
					angle = sqrt(pow(x, 2) + pow(y, 2));
					speedx = 20 * (x / angle); speedy = 20 * (y / angle);
				}
				if (!isShot)
				{
					target.setPosition(960, target.getPosition().y);
				}
			}
			if (isShot)
			{
				if (positionx > 1920 - 50 || positionx < 0)speedx *= -1;
				if (positiony < 0)speedy *= -1;
				targetpos = target.getGlobalBounds();
				target.setPosition(positionx, positiony);
				positionx += speedx;
				positiony += speedy;

				for (int i = 0; i < rowmax; i++)
				{
					//collision
					for (int j = 0; j < columnmax; j++)
					{
						ball p = grid[i][j];

						if (!p.draw && p.rect.intersects(targetpos))
						{
							cout << "hit " << (target.getFillColor() == p.color ? "similar" : "cross") << " egg\n";

							if (target.getFillColor() == p.color) {
								grid[i][j].draw = true;
								score++;
								score += EraseSimilarNearby(i, j, p.color, false, false);
								score += EraseSimilarNearby(i, j, p.color, true, false);
								score += EraseSimilarNearby(i, j, p.color, false, true);
								score += EraseSimilarNearby(i, j, p.color, true, true);
								score_text.setString("SCORE : \n     " + to_string(score));
								sound.play();
							}

							//reset pos
							isShot = false;
							target.setPosition(960, 980);
							speedx = 0; speedy = 0;
							positionx = 960;
							positiony = 980;
							for (int i = 0; i < 1; i++)
							{
								target.setFillColor(colorArray[rand() % 5]);
							}
							break;

						}
					}
				}
			}
			if (target.getPosition().y > 1080)
			{
				isShot = false;
				target.setPosition(960, 980);
				speedx = 0; speedy = 0;
				positionx = 960;
				positiony = 980;
				for (int i = 0; i < 1; i++)
				{
					target.setFillColor(colorArray[rand() % 5]);
				}
			}
			
			window.clear();
			window.draw(Bg);
			window.draw(target);
			window.draw(score_text);

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 38; j++)
				{
					ball p = grid[i][j];
					ballshape.setFillColor(p.color);
					ballshape.setPosition(p.rect.left + 7, p.rect.top + 7);
					if (!p.draw)
						window.draw(ballshape);
				}
			}
			window.display();
		}
		else
		{
			Vector2i mousepos(Mouse::getPosition());
			FloatRect mouserect(mousepos.x, mousepos.y, 5, 5);

			if (text2.getGlobalBounds().intersects(mouserect))
			{
				hover2 = true;
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (Event::MouseButtonReleased)
					{
						start = true;
					}
				}
			}
			else
			{
				hover2 = false;

				if (text3.getGlobalBounds().intersects(mouserect))
				{
					hover3 = true;
					if (Mouse::isButtonPressed(Mouse::Left))
						exit = true;
				}
				else
				{
					hover3 = false;
				}
			}

			window.clear();
			window.draw(text1);
			window.draw(text2);
			window.draw(text3);
			window.display();
		}
	}
}

int EraseSimilarNearby(int row, int column, const Color color, bool add, bool dir)
{
	int count = 0;

	if (row >= 0 && row < rowmax && column >= 0 && column < columnmax)
	{
		ball p;

		if (dir)
		{
			row += add ? 1 : -1;

			if (add)
			{
				for (; row < rowmax; row++)
				{
					p = grid[row][column];

					if (!p.draw && p.color == color)
					{
						grid[row][column].draw = true;
						count++;

						count += EraseSimilarNearby(row, column, color, false, false);
						count += EraseSimilarNearby(row, column, color, true, false);
					}
					else
						break;
				}
			}
			else
			{
				for (; row >= 0; row--)
				{
					p = grid[row][column];

					if (!p.draw && p.color == color)
					{
						grid[row][column].draw = true;
						count++;

						count += EraseSimilarNearby(row, column, color, false, false);
						count += EraseSimilarNearby(row, column, color, true, false);
					}
					else
						break;
				}
			}
		}
		else
		{
			column += add ? 1 : -1;

			if (add)
			{
				for (; column < columnmax; column++)
				{
					p = grid[row][column];

					if (!p.draw && p.color == color)
					{
						grid[row][column].draw = true;
						count++;

						count += EraseSimilarNearby(row, column, color, false, true);
						count += EraseSimilarNearby(row, column, color, true, true);
					}
					else
						break;
				}
			}
			else
			{
				for (; column >= 0; column--)
				{
					p = grid[row][column];

					if (!p.draw && p.color == color)
					{
						grid[row][column].draw = true;
						count++;

						count += EraseSimilarNearby(row, column, color, false, true);
						count += EraseSimilarNearby(row, column, color, true, true);
					}
					else
						break;
				}
			}
		}
	}

	return count;
}