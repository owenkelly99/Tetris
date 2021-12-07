/*
* An SFML application that plays the Tetris game.
* SFML Documentation https://www.sfml-dev.org/documentation/2.5.1/
* Set up of SFML VS2019 project https://www.youtube.com/watch?v=lFzpkvrscs4
*
* ==== Group Streamers ====
* Emmet Bloomer
* Owen Kelly
* =========================
*/

#include <iostream>                                                 
#include <SFML/Graphics.hpp>
#include <time.h>					
#include <Windows.h>
#include <SFML/Audio.hpp>
#include <mmsystem.h>
#include <fstream>
#pragma comment(lib, "winmm.lib") // Link to the winmm library






using namespace sf;
using std::to_string;

const int M = 20;						// Height of the tetris board
const int N = 10;						// Width of the tetris board

int field[M][N] = { 0 };
int scoreNumber = 0;					// In game score to be shown to user
int levelNumber = 0;					// In game level number to be shown to the user


struct Point
{int x, y;} a[4], b[4];

int figures[7][4] = //shapes 
{
	1,3,5,7, // I 
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,6, // T
	2,3,4,5, // L 
	3,5,7,6, // J
	2,3,4,5, // O
};


bool check()
{ // Double Check Line
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
		else if (field[a[i].y][a[i].x]) return 0;
	return 1;
};

int main()
{

	srand(time(0));

	int f = 0;

	RenderWindow window(VideoMode(320, 480), "The Tetris Game!");

	Texture t1, t2, t3, t4;	// image file locaters
	t1.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/blue-background.jpg");
	t3.loadFromFile("images/frame.png");
	t4.loadFromFile("images/game-over.jpg");



	sf::Font font;															// Font added to show text on the window
	if (!font.loadFromFile("Include/SFML/Fonts/Mermaid1001.ttf"))
		return EXIT_FAILURE;


	
	

	Sprite s(t1), background(t2), frame(t3), gameOver(t4);

	int dx = 0; bool rotate = 0; int colorNum = 1;
	float timer = 0, delay = 0.3;


	Clock clock1;
	Clock clock2;														// variable for clock to hold game time

	
	{
		while (window.isOpen())
		{

				float time = clock1.getElapsedTime().asSeconds();
				clock1.restart();
				timer += time;

				int showTime = clock2.getElapsedTime().asSeconds();				// Will count up from zero showing game time in seconds


				Event e;
				while (window.pollEvent(e))
				{
					if (e.type == Event::Closed)
						window.close();

					if (e.type == Event::KeyPressed)
						if (e.key.code == Keyboard::Up) rotate = true;
						else if (e.key.code == Keyboard::Left) dx = -1;
						else if (e.key.code == Keyboard::Right) dx = 1;
				}

				if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;		// When down arrow is pressed the shapes will fall very fast

			//Move//
				for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
				if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

				//Rotate//
				if (rotate)
				{
					Point p = a[1];					//center of rotation
					for (int i = 0; i < 4; i++)
					{
						int x = a[i].y - p.y;
						int y = a[i].x - p.x;
						a[i].x = p.x - x;
						a[i].y = p.y + y;
					}
					if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
				}

				//Tick//
				if (timer > delay)
				{
					for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

					if (!check())
					{
						for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

						colorNum = 1 + rand() % 7;
						int n = rand() % 7;
						for (int i = 0; i < 4; i++)
						{
							a[i].x = figures[n][i] % 2;
							a[i].y = figures[n][i] / 2;
						}
					}

					timer = 0;
				}

				dx = 0; rotate = 0; delay = 0.5;									// Delay is the speed in which the shapes will fall



			//Check Lines//
				int k = M - 1;
				for (int i = M - 1; i > 0; i--)
				{
					int count = 0;
					//int count2 = 0;

					for (int j = 0; j < N; j++)
					{

						if (field[i][j]) count++;
						if (count == N) scoreNumber += 50;							// Increases score by 50 points every line which is made
						if (count == i + 1) f = 1;									// Ends the game as the shapes have reached the top of the screen, does this with if statements to show a game over screen at the draw section of the code


						field[k][j] = field[i][j];


					}

					if (count < N) k--;



				}


				if (scoreNumber >= 750) levelNumber = 6, delay = 0.12;				// When a score limit is reached the speed increase and the user goes up a level  
				else if (scoreNumber >= 650) levelNumber = 5, delay = 0.15;
				else if (scoreNumber >= 500) levelNumber = 4, delay = 0.25;
				else if (scoreNumber >= 300) levelNumber = 3, delay = 0.35;
				else if (scoreNumber >= 200) levelNumber = 2, delay = 0.45;
				else levelNumber = 1;




				//Draw//
				if (f == 0)															// If statement which plays the game normally as long as the player doesn't lose
				{

					

				window.clear(Color::White);
				window.draw(background);
				sf::Text scoreLabel("Score: ", font, 20);												// Label for score title
				scoreLabel.setPosition(window.getSize().x - 310, window.getSize().y - 75);
				scoreLabel.setFillColor(Color::Black);
				sf::Text score(to_string(scoreNumber), font, 20);										// Label to display the score the user has
				score.setPosition(window.getSize().x - 240, window.getSize().y - 75);
				score.setFillColor(Color::Black);

				sf::Text levelLabel("Level: ", font, 20);												// Label for level title
				levelLabel.setPosition(window.getSize().x - 310, window.getSize().y - 55);
				levelLabel.setFillColor(Color::Black);
				sf::Text level(to_string(levelNumber), font, 20);										// Label to display the current level the user is on
				level.setPosition(window.getSize().x - 240, window.getSize().y - 55);
				level.setFillColor(Color::Black);

				sf::Text timeLabel("Time: ", font, 20);													// Label for time title
				timeLabel.setPosition(window.getSize().x - 310, window.getSize().y - 35);
				timeLabel.setFillColor(Color::Black);
				sf::Text showTimer(to_string(showTime), font, 20);										// Label to display the amount of time the user has been playing for
				showTimer.setPosition(window.getSize().x - 240, window.getSize().y - 35);
				showTimer.setFillColor(Color::Black);

				window.draw(scoreLabel);																// Drawing the labels above to the window
				window.draw(score);
				window.draw(levelLabel);
				window.draw(level);
				window.draw(timeLabel);
				window.draw(showTimer);

				for (int i = 0; i < M; i++)
					for (int j = 0; j < N; j++)		// for int j
					{
						if (field[i][j] == 0) continue;
						s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
						s.setPosition(j * 18, i * 18);
						s.move(28, 31); //offset
						window.draw(s);
					}

				for (int i = 0; i < 4; i++)			// for int i
				{
					s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
					s.setPosition(a[i].x * 18, a[i].y * 18);
					s.move(28, 31); //offset
					window.draw(s);
				}

				window.draw(frame);
				window.display();
			}
			else if (f == 1) {																	// Will show a game over screen when the player loses
				window.draw(gameOver);
				window.display();
				
					
			}
		}
		


	}

	return 0;
}