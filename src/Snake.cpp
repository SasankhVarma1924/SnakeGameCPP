#include<iostream>
#include<chrono>
#include<windows.h>
#include<random>
#include<string>
#include<conio.h>
#include<vector>

const int HEIGHT = 40;
const int WIDTH = 70;


class Game
{
private:
	std::vector<std::string> _frame;
	std::vector<std::string> _lastframe;
	std::vector<short> _bodypos;
	bool died = false,eat = false;
	short _headposx = 4, _headposy = 10;
	short _foodposx = 0, _foodposy = 0;
	char _key=0;
	int _tails = 0, _score = 0;
	std::random_device rd;
public:
	Game()
	{
		std::vector<std::string> frame(HEIGHT);
		std::string fir, las, mid;
		for (int i = 0; i < WIDTH; i++)
		{
			if (i == 0 || i == WIDTH - 1)
			{
				mid += (char)186;
			}
			else
			{
				mid += ' ';
			}
			fir += (char)205;
		}
		las = fir;
		fir[0] = (char)201;
		fir[WIDTH - 1] = (char)187;
		las[0] = (char)200;
		las[WIDTH - 1] = (char)188;

		for (int i = 0; i < HEIGHT; i++)
		{
			if (i == 0)
			{
				frame[i] = fir;
			}
			else if (i == HEIGHT - 1)
			{
				frame[i] = las;
			}
			else
			{
				frame[i] = mid;
			}
		}
		_frame = frame;
		_lastframe = frame;
		frame = { "" };
	}
	int gameLoop()
	{
		_bodypos.reserve(100);
		std::uniform_int_distribution<int> disx(1, HEIGHT - 1);
		std::uniform_int_distribution<int> disy(1, WIDTH - 1);
		_foodposx = disx(rd);
		_foodposy = disy(rd);
		while (!died)
		{
			if (_headposx == 0 || _headposx == HEIGHT - 1 || _headposy == 0 || _headposy == WIDTH - 1)
			{
				died = true;
			}
			printSnake();
			printFrame();
			updateInput();
			Sleep(100);
			system("cls");
		}
		return _score;
	}
	void printSnake()
	{
		std::uniform_int_distribution<int> disx(1, HEIGHT - 1);
		std::uniform_int_distribution<int> disy(1, WIDTH - 1);
		_frame[_headposx][_headposy] = 'O';
		_frame[_foodposx][_foodposy] = (char)3;
		short headposx = _headposx, headposy = _headposy;
		int size = _bodypos.size()-1;
		for (int i = 0; i < _tails; i++)
		{
			if (_bodypos[size] == 0)
			{
				headposx++;
			}
			else if (_bodypos[size] == 1)
			{
				headposy--;
			}
			else if (_bodypos[size] == 2)
			{
				headposx--;
			}
			else if (_bodypos[size] == 3)
			{
				headposy++;
			}
			if (_headposx == headposx && _headposy == headposy)
			{
				died = true;
			}
			up:
			if (_foodposx == headposx && _foodposy == headposy)
			{
				_foodposx = disx(rd);
				_foodposy = disy(rd);
				goto up;
			}
			_frame[headposx][headposy] = 'O';
			size--;
		}

		if (_headposx == _foodposx && _headposy == _foodposy)
		{
			_foodposx = disx(rd);
			_foodposy = disy(rd);
			_score++;
			_tails++;
		}
		if (size - _tails >= 5)
		{
			_bodypos.erase(_bodypos.begin());
		}
	}
	void updateInput()
	{
		input();
		if (_key == 'd')
		{
			_bodypos.push_back(1);
			_headposy++;
		}
		else if (_key == 'a')
		{
			_bodypos.push_back(3);
			_headposy--;
		}
		else if (_key == 's')
		{
			_bodypos.push_back(2);
			_headposx++;
		}
		else if (_key == 'w')
		{
			_bodypos.push_back(0);
			_headposx--;
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
		return 0;
	}
	void printFrame()
	{
		for (std::string s : _frame)
		{
			std::cout <<"\t\t\t\t" << s << std::endl;
		}
		_frame = _lastframe;
	}
	~Game()
	{
		_frame = { "" };
		_lastframe = { "" };
		_bodypos = { 0 };
	}
};


int main()
{
	Game snake;
	std::cout<<"\nSCORE  ::  "<<snake.gameLoop()<<std::endl;

	return 0;
}