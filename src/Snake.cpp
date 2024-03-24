#include<iostream>
#include<deque>
#include<windows.h>
#include<ctime>
#include<random>
#include <vector>
#include <conio.h>

class Position
{
public:
	int x, y;
	Position() 
	{
		this->x = 0;
		this->y = 0;
	}
	Position(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	bool operator == (const Position& pos)
	{
		if (this->x == pos.x && this->y == pos.y)
			return true;
		return false;
	}
};

class Game
{
private:
	int HEIGHT = 30; 
	int WIDTH = 40;
	int score = 0;
	char _key = 0;
	bool isSnakeAlive = true;
	std::random_device rd;
	std::vector<std::string> frame;
	std::vector<std::vector<bool>> validPosition;
	std::deque<Position> snakeBody;
	Position food;

	void printFrame()
	{
		for (std::string s : frame)
		{
			std::cout << "\t\t\t\t" << s << "\n";
		}
	}

	void printSnake()
	{
		getBigger();
		for (auto it = snakeBody.begin(); it != snakeBody.end(); it++)
		{
			Position pos = *it;
			frame[pos.x][pos.y] = (char)254;
		}
	}

	void printFood()
	{
		frame[food.x][food.y] = (char)249;
	}

	void generateSnake()
	{
		snakeBody.push_back(generatePosition());
	}

	void generateFood()
	{
		food = generatePosition();
		while (!validPosition[food.x][food.y])
			food = generatePosition();
	}

	Position generatePosition()
	{
		std::uniform_int_distribution<int> distX(1, HEIGHT - 2);
		std::uniform_int_distribution<int> distY(1, WIDTH - 2);
		return { distX(rd), distY(rd) };
	}

	bool isAlive(Position& pos)
	{
		if (pos.x <= 0 || pos.y <= 0 || pos.x >= HEIGHT - 1 || pos.y >= WIDTH - 1)
		{
			isSnakeAlive = false;
			return false;
		}
		if (!validPosition[pos.x][pos.y])
		{
			isSnakeAlive = false;
			return false;
		}
		return true;
	}

	bool isFoodEaten()
	{
		Position head = snakeBody.back();
		if (head == food)
		{
			score++;
			return true;
		}
		return false;
	}

	void getBigger()
	{
		if (isFoodEaten())
		{
			generateFood();
			Position head = snakeBody.back();
			Position newHead = head;
			switch (_key)
			{
				case 'w':
					newHead.x -= 1;
					break;
				case 'a':
					newHead.y -= 1;
					break;
				case 's':
					newHead.x += 1;
					break;
				case 'd':
					newHead.y += 1;
					break;
			}
			snakeBody.push_back(newHead);
			validPosition[newHead.x][newHead.y] = false;
		}
	}

	char input()
	{
		clock_t start, end;
		start = clock();
		double timetaken = 0;
		while (1)
		{
			if (_kbhit())
			{
				char key = _getch();
				if (_key == 0 || (_key == 'w' || _key == 's') && (key == 'a' || key == 'd'))
				{
					_key = key;
				}
				if (_key == 0 || (_key == 'a' || _key == 'd') && (key == 'w' || key == 's'))
				{
					_key = key;
				}
				return _key;
			}
			end = clock();
			timetaken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
			if (timetaken >= 0)
				break;
		}
		return _key;
	}

	void updateSnake()
	{
		Position head = snakeBody.back();
		Position newHead = head;
		switch (_key)
		{
			case 'w':
				newHead.x -= 1;
				break;
			case 'a':
				newHead.y -= 1;
				break;
			case 's':
				newHead.x += 1;
				break;
			case 'd':
				newHead.y += 1;
				break;
		}
		snakeBody.push_back(newHead);
		if (isAlive(newHead))
		{
			validPosition[newHead.x][newHead.y] = false;

			Position tail = snakeBody.front();

			frame[tail.x][tail.y] = ' ';
			validPosition[tail.x][tail.y] = true;

			snakeBody.pop_front();
		}
	}

	void gameLoop()
	{

		while (isSnakeAlive)
		{
			printSnake();
			printFood();
			printFrame();
			input();
			updateSnake();
			Sleep(100);
			system("cls");
		}
	}

public:
	Game()
	{
		validPosition = std::vector<std::vector<bool>>(HEIGHT, std::vector<bool>(WIDTH, true));
		generateFood();
		generateSnake();
		frame = std::vector<std::string>(HEIGHT);
		
		std::string top, bot, mid;
		for (int i = 0; i < WIDTH; i++)
		{
			if (i == 0 || i == WIDTH - 1)
				mid.push_back((char)186);
			else
				mid.push_back(' ');
			top.push_back((char)205);
		}
		bot = top;
		top[0] = (char)201;
		top[top.size() - 1] = (char)187;
		bot[0] = (char)200;
		bot[bot.size() - 1] = (char)188;

		for (int i = 0; i < HEIGHT; i++)
		{
			if (i == 0)
				frame[i] = top;
			else if (i == HEIGHT - 1)
				frame[i] = bot;
			else
				frame[i] = mid;
		}
		gameLoop();
	}

	int getScore()
	{
		return score;
	}
};

int main()
{
	Game game;
	std::cout << "SCORE -- " << game.getScore() << std::endl;
	return 0;
}
