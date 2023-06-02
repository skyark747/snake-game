// snake game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <time.h>
using namespace std;

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}

enum direction{LEFT,RIGHT,UP,DOWN};
struct position
{
	int ri, ci;
};
struct snake
{
	position* ps;
	int size;
	int score;
	direction  D;
	string pname;
	char sym;

	int LEFTKEY,RIGHTKEY,UPKEY,DOWNKEY;

};
struct food
{
	position fd;
	int fs;
	bool isalive;
};

void t2b(int c, int rs, int re, char sym)
{
	for (int r = rs; r <= re; r++)
	{
		gotoRowCol(r, c);
		cout << sym;
	}


}
void l2r(int r, int cs, int ce, char sym)
{
	for (int c = cs; c <= ce; c++)
	{
		gotoRowCol(r, c);
		cout << sym;
	}


}
void boundries(int height, int width)
{
	SetClr(3);
	t2b(1, 1, height-2, -37);
	l2r(1, 1, width-2, -37);
	t2b(width-2, 1, height-2, -37);
	l2r(height-2, 1, width-2, -37);

}
void boundries2(int height, int width)
{
	SetClr(12);
	t2b(1, 1, height - 2, -37);
	l2r(1, 1, width - 2, -37);
	t2b(width - 2, 1, height - 2, -37);
	l2r(height - 2, 1, width - 2, -37);
	l2r(height / 2 - 10, width / 2-30, width / 2 + 30, -37);
	l2r(height / 2 + 10, width / 2-30, width / 2 + 30, -37);
}
void boundries3(int height, int width)
{
	SetClr(11);
	t2b(1, 1, height - 2, -37);
	l2r(1, 1, width - 2, -37);
	t2b(width - 2, 1, height - 2, -37);
	l2r(height - 2, 1, width - 2, -37);
	l2r(height / 2 - 10, width / 2 - 30, width / 2 + 30, -37);
	l2r(height / 2 + 10, width / 2 - 30, width / 2 + 30, -37);
	t2b(width/2, 1, height/2-10, -37);
	t2b(width/2, height/2+10, height - 2, -37);
}

bool islegalposition(snake& S,position p)
{
	for (int i = 0; i < S.size; i++)
	{
		if (S.ps[i].ri == p.ri && S.ps[i].ci == p.ci)
		{
			return false;
		}
	}
	return true;
}
bool islegalposition2(snake& S1,snake& S2,snake& S3,snake& S4,position p,int width,int height)
{
	for (int i = 0; i < S1.size; i++)
	{
		if (S1.ps[i].ri == p.ri && S1.ps[i].ci == p.ci)
		{
			return false;
		}		
	}
	for (int i = 0; i < S2.size; i++)
	{
		if (S2.ps[i].ri == p.ri && S2.ps[i].ci == p.ci)
		{
			return false;
		}
	}
	for (int i = 0; i < S3.size; i++)
	{
		if (S3.ps[i].ri == p.ri && S3.ps[i].ci == p.ci)
		{
			return false;
		}
	}
	for (int i = 0; i < S4.size; i++)
	{
		if (S4.ps[i].ri == p.ri && S4.ps[i].ci == p.ci)
		{
			return false;
		}
	}
	if (1 == p.ri || 1 == p.ci || height - 1 == p.ri || width - 1 == p.ci)
	{
	   return false;
	}
	for (int i = width / 2 - 30; i < width / 2 + 30; i++)
	{
		if (i==p.ci && (height/2-10 == p.ri || height / 2 + 10 == p.ri))
		{
			return false;
		}
	}
	for (int i = height / 2 + 10; i < height - 2; i++)
	{
		if (i == p.ri && width/2 == p.ci)
		{
			return false;
		}
	}
	for (int i = 1; i < height / 2 - 10; i++)
	{
		if (i == p.ri && width / 2 == p.ci)
		{
			return false;
		}
	}
	return true;
}

bool snakeisdead(const snake& S1)
{
	for (int i = 1; i < S1.size; i++)
	{
		if (S1.ps[0].ri == S1.ps[i].ri && S1.ps[0].ci == S1.ps[i].ci)
		{
			return true;
		}
	}
	return false;
}
bool snakeisdeadstage1(const snake& S,int height,int width)
{
	if (S.ps[0].ri == 2)
	{
		return true;
	}
	else if (S.ps[0].ri == height - 3)
	{
		return true;
	}
	else if (S.ps[0].ci == 2)
	{
		return true;
	}
	else if (S.ps[0].ci == width - 3)
	{
		return true;
	}
	return false;
}
bool snakeisdeadstage2(const snake& S, int height, int width)
{
	for (int i = width / 2 - 30; i < width / 2 + 30; i++)
	{
		if (S.ps[0].ri == height / 2 - 10 && S.ps[0].ci == i || (S.ps[0].ri == height / 2 + 10 && S.ps[0].ci == i))
		{
			return true;
		}
	}
	return false;
}
bool snakeisdeadstage3(const snake& S, int height, int width)
{
	for (int i = height / 2 + 10; i < height -2; i++)
	{
		if (S.ps[0].ri == i && (S.ps[0].ci == width/ 2-1|| S.ps[0].ci == width / 2+1))
		{
			return true;
		}
	}
	for (int i = 1; i < height/2-10; i++)
	{
		if (S.ps[0].ri == i && (S.ps[0].ci == width / 2-1 || S.ps[0].ci == width / 2+1))
		{
			return true;
		}
	}
	return false;
}
bool snakeisdeadmultiple(const snake& S, const snake& Sn, int height, int width)
{
	for (int i = 0; i < Sn.size; i++)
	{
		if (S.ps[0].ri == Sn.ps[i].ri && S.ps[0].ci == Sn.ps[i].ci)
		{
			return true;
		}		
	}
	return false;
}
bool gameover(snake& S1, snake& S2, snake& S3, snake& S4,int width)
{
	if (S1.score >= 5)
	{
		gotoRowCol(70,width/2);
		cout << S1.pname << " won the game...";
		return true;
	}
	else if (S2.score >= 5)
	{
		gotoRowCol(70, width / 2);
		cout << S2.pname << " won the game...";
		return true;
	}
	else if (S3.score >= 5)
	{
		gotoRowCol(70, width / 2);
		cout << S3.pname << " won the game...";
		return true;
	}
	else if (S4.score >= 5)
	{
		gotoRowCol(70, width / 2);
		cout << S4.pname << " won the game...";
		return true;
	}
	return false;
}
bool winninggame(snake& S)
{
	if (S.score == 5)
	{
		return true;
	}
	return false;
}
void makefood(food& f, int height, int width,snake& S)
{
	do
	{
		f.fd.ri = rand() % height;
		f.fd.ci = rand() % width;
	} 
	while (!islegalposition(S, f.fd));
	f.isalive = true;
	f.fs = 1;

}
void makefood2(food& f, int height, int width,snake& S1,snake& S2,snake& S3,snake& S4)
{
	do
	{
		f.fd.ri = rand() % 50;
		f.fd.ci = rand() % 120;
	} while (!islegalposition2(S1,S2,S3,S4, f.fd,width,height));
	f.isalive = true;
	f.fs = 1;

}
void displayfood(food& f,char sym)
{
	SetClr(4);
	gotoRowCol(f.fd.ri, f.fd.ci);
	cout <<sym;
}
bool foodiseaten(food f,snake& S)
{	
	if(S.ps[0].ri == f.fd.ri && S.ps[0].ci == f.fd.ci)
	{
		return true;
	}
	return false;
}

void input(snake& S1,snake& S2,snake& S3,snake& S4, int height, int width,food& f)
{
	S1.ps = new position[5];	
	
	S1.ps[0].ri = height / 2;
	S1.ps[0].ci = width / 2;

	S1.ps[1].ri = height / 2;
	S1.ps[1].ci = width / 2 - 1;

	S1.ps[2].ri = height / 2;
	S1.ps[2].ci = width / 2 - 2;

	S1.ps[3].ri = height / 2;
	S1.ps[3].ci = width / 2 - 3;

	S1.ps[4].ri = height / 2;
	S1.ps[4].ci = width / 2 - 3;
	
	S1.size = 5;
	S1.score = 0;
	S1.sym = -37;

	S1.pname = "Soban";
	S1.D = RIGHT;
	S1.RIGHTKEY = 77, S1.LEFTKEY = 75, S1.UPKEY = 72, S1.DOWNKEY = 80;

	S2.ps = new position[5];

	S2.ps[0].ri = height / 4;
	S2.ps[0].ci = width / 4;

	S2.ps[1].ri = height / 4;
	S2.ps[1].ci = width / 4 - 1;

	S2.ps[2].ri = height / 4;
	S2.ps[2].ci = width / 4 - 2;

	S2.ps[3].ri = height / 4;
	S2.ps[3].ci = width / 4 - 3;

	S2.ps[4].ri = height / 4;
	S2.ps[4].ci = width / 4 - 3;

	S2.size = 5;
	S2.score = 0;
	S2.sym = -37;

	S2.pname = "Hajra";
	S2.D = RIGHT;
	
	S3.ps = new position[5];

	S3.ps[0].ri = height -10;
	S3.ps[0].ci = width - 20;

	S3.ps[1].ri = height -10;
	S3.ps[1].ci = width -21;

	S3.ps[2].ri = height -10;
	S3.ps[2].ci = width-22;

	S3.ps[3].ri = height -10;
	S3.ps[3].ci = width -23;

	S3.ps[4].ri = height -10;
	S3.ps[4].ci = width - 24;

	S3.size = 5;
	S3.score = 0;
	S3.sym = -37;

	S3.pname = "Aiza";
	S3.D = RIGHT;

	S4.ps = new position[5];

	S4.ps[0].ri = height/6;
	S4.ps[0].ci = 9;

	S4.ps[1].ri = height/6;
	S4.ps[1].ci = 8;

	S4.ps[2].ri = height/6;
	S4.ps[2].ci = 7;

	S4.ps[3].ri = height/6;
	S4.ps[3].ci = 6;

	S4.ps[4].ri = height/6;
	S4.ps[4].ci = 5;

	S4.size = 5;
	S4.score = 0;
	S4.sym = -37;

	S4.pname = "Imsha";
	S4.D = RIGHT;

}
void erasesnake(const snake& S1,const snake& S2,const snake&S3,const snake& S4,int height,int width)
{

	for (int i = 0; i < S1.size; i++)
	{
		gotoRowCol(S1.ps[i].ri, S1.ps[i].ci);
		cout << " ";		
	}
	for (int i = 0; i < S2.size; i++)
	{
		gotoRowCol(S2.ps[i].ri, S2.ps[i].ci);
		cout << " ";
		
	}
	for (int i = 0; i < S3.size; i++)
	{	
		gotoRowCol(S3.ps[i].ri, S3.ps[i].ci);
		cout << " ";	
	}
	for (int i = 0; i < S4.size; i++)
	{
		gotoRowCol(S4.ps[i].ri, S4.ps[i].ci);
		cout << " ";
	}


}
void erasesnake2(const snake& S1, int height, int width)
{
	for (int i = 0; i < S1.size; i++)
	{
		gotoRowCol(S1.ps[i].ri, S1.ps[i].ci);
		cout << " ";
	}
	
	
}
void expandsnake(snake& S, food f)
{
	position* T = new position[S.size + 1];

	for (int i = 0; i < S.size; i++)
	{
		T[i] = S.ps[i];
	}
	T[S.size] = S.ps[S.size - 1];
	S.size++;
	delete[]S.ps;
	S.ps = T;

}
void snakedirection(snake& S1,snake& S2 , char k)
{
	
		if (k == S1.RIGHTKEY)
		{
			if (S1.D != LEFT)
			{
				S1.D = RIGHT;
			}
		}
		if (k == S1.LEFTKEY)
		{
			if (S1.D != RIGHT)
			{
				S1.D = LEFT;
			}
		}
		if (k == S1.UPKEY)
		{
			if (S1.D != DOWN)
			{
				S1.D = UP;
			}
		}
		if (k == S1.DOWNKEY)
		{
			if (S1.D != UP)
			{
				S1.D = DOWN;
			}
		}
		

	
}
void snakedirection2(snake& S2,snake& S3,snake& S4,char k)
{
	if (k == 'd')
	{
		if (S2.D != LEFT)
		{
			S2.D = RIGHT;
		}
	}
	if (k == 'a')
	{
		if (S2.D != RIGHT)
		{
			S2.D = LEFT;
		}
	}
	if (k == 'w')
	{
		if (S2.D != DOWN)
		{
			S2.D = UP;
		}
	}
	if (k == 's')
	{
		if (S2.D != UP)
		{
			S2.D = DOWN;
		}
	}
	
	if (k == 'l')
	{
		if (S3.D != LEFT)
		{
			S3.D = RIGHT;
		}
	}
	if (k == 'j')
	{
		if (S3.D != RIGHT)
		{
			S3.D = LEFT;
		}
	}
	if (k == 'i')
	{
		if (S3.D != DOWN)
		{
			S3.D = UP;
		}
	}
	if (k == 'k')
	{
		if (S3.D != UP)
		{
			S3.D = DOWN;
		}
	}
	
	if (k == 54)
	{
		if (S4.D != LEFT)
		{
			S4.D = RIGHT;
		}
	}
	if (k == 52)
	{
		if (S4.D != RIGHT)
		{
			S4.D = LEFT;
		}
	}
	if (k == 56)
	{
		if (S4.D != DOWN)
		{
			S4.D = UP;
		}
	}
	if (k == 50)
	{
		if (S4.D != UP)
		{
			S4.D = DOWN;
		}
	}

}
void displayscore(snake& S, food f)
{
	SetClr(15);
	gotoRowCol(70, 16);
	cout << S.score++;

}

void movesnake(snake& S1,snake& S2,snake& S3,snake& S4,int height,int width)
{
	if (snakeisdead(S1) || snakeisdeadmultiple(S1,S2,height,width) || snakeisdeadmultiple(S1, S3, height, width)
		|| snakeisdeadmultiple(S1, S4, height, width))
	{
		gotoRowCol(70, width / 2);
		cout << S1.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S1.size - 1; i >= 1; i--)
		{
			S1.ps[i] = S1.ps[i - 1];
		}
		switch (S1.D)
		{
		case UP:
		{
			S1.ps[0].ri--;
			if (S1.ps[0].ri == -1)
			{
				S1.ps[0].ri = height - 1;
			}

			break;
		}
		case DOWN:
		{
			S1.ps[0].ri++;
			if (S1.ps[0].ri == height)
			{
				S1.ps[0].ri = 0;
			}

			break;
		}
		case LEFT:
		{
			S1.ps[0].ci--;
			if (S1.ps[0].ci == -1)
			{
				S1.ps[0].ci = width - 1;
			}

			break;
		}
		case RIGHT:
		{
			S1.ps[0].ci++;
			if (S1.ps[0].ci == width)
			{
				S1.ps[0].ci = 0;
			}

			break;
		}
		}
	}
	if (snakeisdead(S2) || snakeisdeadmultiple(S2, S1, height, width) || snakeisdeadmultiple(S2, S3, height, width)
		|| snakeisdeadmultiple(S2, S4, height, width))
	{
		gotoRowCol(80, width / 2);
		cout << S2.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S2.size - 1; i >= 1; i--)
		{
			S2.ps[i] = S2.ps[i - 1];
		}
		switch (S2.D)
		{
		case UP:
		{
			S2.ps[0].ri--;
			if (S2.ps[0].ri == -1)
			{
				S2.ps[0].ri = height - 1;
			}
			break;
		}
		case DOWN:
		{
			S2.ps[0].ri++;
			if (S2.ps[0].ri == height)
			{
				S2.ps[0].ri = 0;
			}
			break;
		}
		case LEFT:
		{
			S2.ps[0].ci--;
			if (S2.ps[0].ci == -1)
			{
				S2.ps[0].ci = width - 1;
			}
			break;
		}
		case RIGHT:
		{
			S2.ps[0].ci++;
			if (S2.ps[0].ci == width)
			{
				S2.ps[0].ci = 0;
			}
			break;
		}
		}
	}
	if (snakeisdead(S3) || snakeisdeadmultiple(S3, S1, height, width) || snakeisdeadmultiple(S3, S2, height, width)
		|| snakeisdeadmultiple(S3, S4, height, width))
	{
		gotoRowCol(90, width / 2);
		cout << S3.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S3.size - 1; i >= 1; i--)
		{
			S3.ps[i] = S3.ps[i - 1];
		}
		switch (S3.D)
		{
		case UP:
		{
			S3.ps[0].ri--;
			if (S3.ps[0].ri == -1)
			{
				S3.ps[0].ri = height - 1;
			}
			break;
		}
		case DOWN:
		{
			S3.ps[0].ri++;
			if (S3.ps[0].ri == height)
			{
				S3.ps[0].ri = 0;
			}
			break;
		}
		case LEFT:
		{
			S3.ps[0].ci--;
			if (S3.ps[0].ci == -1)
			{
				S3.ps[0].ci = width - 1;
			}
			break;
		}
		case RIGHT:
		{
			S3.ps[0].ci++;
			if (S3.ps[0].ci == width)
			{
				S3.ps[0].ci = 0;
			}
			break;
		}
		}
	}
	if (snakeisdead(S4) || snakeisdeadmultiple(S4, S1, height, width) || snakeisdeadmultiple(S4, S2, height, width)
		|| snakeisdeadmultiple(S4, S3, height, width))
	{
		gotoRowCol(100, width / 2);
		cout << S4.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S4.size - 1; i >= 1; i--)
		{
			S4.ps[i] = S4.ps[i - 1];
		}
		switch (S4.D)
		{
		case UP:
		{
			S4.ps[0].ri--;
			if (S4.ps[0].ri == -1)
			{
				S4.ps[0].ri = height - 1;
			}
			break;
		}
		case DOWN:
		{
			S4.ps[0].ri++;
			if (S4.ps[0].ri == height)
			{
				S4.ps[0].ri = 0;
			}
			break;
		}
		case LEFT:
		{
			S4.ps[0].ci--;
			if (S4.ps[0].ci == -1)
			{
				S4.ps[0].ci = width - 1;
			}
			break;
		}
		case RIGHT:
		{
			S4.ps[0].ci++;
			if (S4.ps[0].ci == width)
			{
				S4.ps[0].ci = 0;
			}
			break;
		}
		}
	}
	

}
void movesnake2(snake& S1, snake& S2,snake& S3,snake& S4, int height, int width)
{
	if (snakeisdeadstage1(S1,height,width) || snakeisdead(S1) || snakeisdeadmultiple(S1, S2, height, width) 
		|| snakeisdeadmultiple(S1, S3, height, width)|| snakeisdeadmultiple(S1, S4, height, width))
	{
		gotoRowCol(70, width / 2);
		cout << S1.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S1.size - 1; i >= 1; i--)
		{
			S1.ps[i] = S1.ps[i - 1];
		}
		switch (S1.D)
		{
		case UP:
		{
			S1.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S1.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S1.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S1.ps[0].ci++;
			break;
		}
		}
	}
	if (snakeisdeadstage1(S2, height, width) || snakeisdead(S2) || snakeisdeadmultiple(S2, S1, height, width) 
		|| snakeisdeadmultiple(S2, S3, height, width)|| snakeisdeadmultiple(S2, S4, height, width))
	{
		gotoRowCol(80, width / 2);
		cout << S2.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S2.size - 1; i >= 1; i--)
		{
			S2.ps[i] = S2.ps[i - 1];
		}
		switch (S2.D)
		{
		case UP:
		{
			S2.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S2.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S2.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S2.ps[0].ci++;
			break;
		}
		}
	}
	if (snakeisdeadstage1(S3, height, width) || snakeisdead(S3) || snakeisdeadmultiple(S3, S1, height, width) 
		|| snakeisdeadmultiple(S3, S2, height, width)|| snakeisdeadmultiple(S3, S4, height, width))
	{
		gotoRowCol(90, width / 2);
		cout << S3.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S3.size - 1; i >= 1; i--)
		{
			S3.ps[i] = S3.ps[i - 1];
		}
		switch (S3.D)
		{
		case UP:
		{
			S3.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S3.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S3.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S3.ps[0].ci++;
			break;
		}
		}
	}
	if (snakeisdeadstage1(S4, height, width) || snakeisdead(S4) || snakeisdeadmultiple(S4, S1, height, width)
		|| snakeisdeadmultiple(S4, S2, height, width)|| snakeisdeadmultiple(S4, S3, height, width))
	{
		gotoRowCol(100, width / 2);
		cout << S3.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S4.size - 1; i >= 1; i--)
		{
			S4.ps[i] = S4.ps[i - 1];
		}
		switch (S4.D)
		{
		case UP:
		{
			S4.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S4.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S4.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S4.ps[0].ci++;
			break;
		}
		}
	}

}
void movesnake3(snake& S1, snake& S2, snake& S3, snake& S4, int height, int width)
{
	if (snakeisdeadstage1(S1, height, width)||snakeisdead(S1)||snakeisdeadstage2(S1,height,width)
		|| snakeisdeadmultiple(S1, S2, height, width) || snakeisdeadmultiple(S1, S3, height, width) || snakeisdeadmultiple(S1, S4, height, width))
	{
		gotoRowCol(70, width / 2);
		cout << S1.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S1.size - 1; i >= 1; i--)
		{
			S1.ps[i] = S1.ps[i - 1];
		}
		switch (S1.D)
		{
		case UP:
		{
			S1.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S1.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S1.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S1.ps[0].ci++;
			break;
		}
		}
	}
	if (snakeisdeadstage1(S2, height, width) || snakeisdead(S2) || snakeisdeadstage2(S2, height, width)
		|| snakeisdeadmultiple(S2, S1, height, width)|| snakeisdeadmultiple(S2, S3, height, width) || snakeisdeadmultiple(S2, S4, height, width))
	{
		gotoRowCol(80, width / 2);
		cout << S2.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S2.size - 1; i >= 1; i--)
		{
			S2.ps[i] = S2.ps[i - 1];
		}
		switch (S2.D)
		{
		case UP:
		{
			S2.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S2.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S2.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S2.ps[0].ci++;
			break;
		}
		}
	}
	if (snakeisdeadstage1(S3, height, width) || snakeisdead(S3) || snakeisdeadstage2(S3, height, width)
		|| snakeisdeadmultiple(S3, S1, height, width)|| snakeisdeadmultiple(S3, S2, height, width) || snakeisdeadmultiple(S3, S4, height, width))
	{
		gotoRowCol(90, width / 2);
		cout << S3.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S3.size - 1; i >= 1; i--)
		{
			S3.ps[i] = S3.ps[i - 1];
		}
		switch (S3.D)
		{
		case UP:
		{
			S3.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S3.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S3.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S3.ps[0].ci++;
			break;
		}
		}
	}
	if (snakeisdeadstage1(S4, height, width) || snakeisdead(S4) || snakeisdeadstage2(S4, height, width)
		|| snakeisdeadmultiple(S4, S1, height, width)|| snakeisdeadmultiple(S4, S2, height, width) || snakeisdeadmultiple(S4, S3, height, width))
	{
		gotoRowCol(100, width / 2);
		cout << S1.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S4.size - 1; i >= 1; i--)
		{
			S4.ps[i] = S4.ps[i - 1];
		}
		switch (S4.D)
		{
		case UP:
		{
			S4.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S4.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S4.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S4.ps[0].ci++;
			break;
		}
		}
	}

}
void movesnake4(snake& S1, snake& S2, snake& S3, snake& S4, int height, int width)
{
	if (snakeisdead(S1) || snakeisdeadstage2(S1, height, width) || snakeisdeadstage3(S1, height, width)
		|| snakeisdeadstage1(S1,height,width) || snakeisdeadmultiple(S1, S2, height, width)
		|| snakeisdeadmultiple(S1, S3, height, width) || snakeisdeadmultiple(S1, S4, height, width))
	{
		gotoRowCol(70, width / 2);
		cout << S1.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S1.size - 1; i >= 1; i--)
		{
			S1.ps[i] = S1.ps[i - 1];
		}
		switch (S1.D)
		{
		case UP:
		{
			S1.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S1.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S1.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S1.ps[0].ci++;
			break;
		}
		}
	}
	if (snakeisdeadstage1(S2, height, width) || snakeisdead(S2) || snakeisdeadstage2(S2, height, width)
		|| snakeisdeadstage3(S2, height, width) || snakeisdeadmultiple(S2, S1, height, width)
		|| snakeisdeadmultiple(S2, S3, height, width) || snakeisdeadmultiple(S2, S4, height, width))
	{
		gotoRowCol(80, width / 2);
		cout << S2.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S2.size - 1; i >= 1; i--)
		{
			S2.ps[i] = S2.ps[i - 1];
		}
		switch (S2.D)
		{
		case UP:
		{
			S2.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S2.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S2.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S2.ps[0].ci++;
			break;
		}
		}
	}
	if (snakeisdeadstage1(S3, height, width) || snakeisdead(S3) || snakeisdeadstage2(S3, height, width)
		|| snakeisdeadstage3(S3, height, width) || snakeisdeadmultiple(S3, S1, height, width)
		|| snakeisdeadmultiple(S3, S2, height, width) || snakeisdeadmultiple(S3, S4, height, width))
	{
		gotoRowCol(90, width / 2);
		cout << S3.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S3.size - 1; i >= 1; i--)
		{
			S3.ps[i] = S3.ps[i - 1];
		}
		switch (S3.D)
		{
		case UP:
		{
			S3.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S3.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S3.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S3.ps[0].ci++;
			break;
		}
		}
	}
	if (snakeisdeadstage1(S4, height, width) || snakeisdead(S4) || snakeisdeadstage2(S4, height, width)
		|| snakeisdeadstage3(S4, height, width) || snakeisdeadmultiple(S4, S1, height, width)
		|| snakeisdeadmultiple(S4, S2, height, width) || snakeisdeadmultiple(S4, S3, height, width))
	{
		gotoRowCol(100, width / 2);
		cout << S1.pname << "'s snake is dead...Rip!!!";
	}
	else
	{
		for (int i = S4.size - 1; i >= 1; i--)
		{
			S4.ps[i] = S4.ps[i - 1];
		}
		switch (S4.D)
		{
		case UP:
		{
			S4.ps[0].ri--;
			break;
		}
		case DOWN:
		{
			S4.ps[0].ri++;
			break;
		}
		case LEFT:
		{
			S4.ps[0].ci--;
			break;
		}
		case RIGHT:
		{
			S4.ps[0].ci++;
			break;
		}
		}
	}

}
void snakedeaderase(snake& S1, snake& S2, snake& S3, snake& S4, int height, int width)
{
	if (snakeisdeadstage1(S1, height, width))
	{
		for (int i = 0; i < S1.size; i++)
		{
			S1.ps[i].ri = 70;
			S1.ps[i].ci = 120;
		}
		erasesnake2(S1, height, width);
	}
	if (snakeisdeadstage1(S2, height, width))	
	{
		for (int i = 0; i < S2.size; i++)
		{
			S2.ps[i].ri = 80;
			S2.ps[i].ci = 120;
		}
		erasesnake2(S2, height, width);
	}
	if (snakeisdeadstage1(S3, height, width))
	{
		for (int i = 0; i < S3.size; i++)
		{
			S3.ps[i].ri = 90;
			S3.ps[i].ci = 120;
		}
		erasesnake2(S3, height, width);
	}
	if (snakeisdeadstage1(S4, height, width))
	{
		for (int i = 0; i < S3.size; i++)
		{
			S4.ps[i].ri = 100;
			S4.ps[i].ci = 120;
		}
		erasesnake2(S4, height, width);

	}
	

}
void snakedeaderase3(snake& S1, snake& S2, snake& S3, snake& S4, int height, int width)
{
	if (snakeisdeadstage2(S1, height, width))
	{
		for (int i = 0; i < S1.size; i++)
		{
			S1.ps[i].ri = 70;
			S1.ps[i].ci = 120;
		}
		erasesnake2(S1, height, width);
	}
	if (snakeisdeadstage2(S2, height, width))
	{
		for (int i = 0; i < S2.size; i++)
		{
			S2.ps[i].ri = 80;
			S2.ps[i].ci = 120;
		}
		erasesnake2(S2, height, width);
	}
	if (snakeisdeadstage2(S3, height, width))
	{
		for (int i = 0; i < S3.size; i++)
		{
			S3.ps[i].ri = 90;
			S3.ps[i].ci = 120;
		}
		erasesnake2(S3, height, width);
	}
	if (snakeisdeadstage2(S4, height, width))
	{
		for (int i = 0; i < S3.size; i++)
		{
			S4.ps[i].ri = 100;
			S4.ps[i].ci = 120;
		}
		erasesnake2(S4, height, width);

	}


}
void snakedeaderase4(snake& S1, snake& S2, snake& S3, snake& S4, int height, int width)
{
	if (snakeisdeadstage3(S1, height, width))
	{
		for (int i = 0; i < S1.size; i++)
		{
			S1.ps[i].ri = 70;
			S1.ps[i].ci = 120;
		}
		erasesnake2(S1, height, width);
	}
	if (snakeisdeadstage3(S2, height, width))
	{
		for (int i = 0; i < S2.size; i++)
		{
			S2.ps[i].ri = 80;
			S2.ps[i].ci = 120;
		}
		erasesnake2(S2, height, width);
	}
	if (snakeisdeadstage3(S3, height, width))
	{
		for (int i = 0; i < S3.size; i++)
		{
			S3.ps[i].ri = 90;
			S3.ps[i].ci = 120;
		}
		erasesnake2(S3, height, width);
	}
	if (snakeisdeadstage3(S4, height, width))
	{
		for (int i = 0; i < S3.size; i++)
		{
			S4.ps[i].ri = 100;
			S4.ps[i].ci = 120;
		}
		erasesnake2(S4, height, width);

	}


}
void snakedeaderase2(snake& S1, snake& S2, snake& S3, snake& S4, int height, int width)
{
	if (snakeisdead(S1) || snakeisdeadmultiple(S1, S2, height, width) || snakeisdeadmultiple(S1, S3, height, width)
		|| snakeisdeadmultiple(S1, S4, height, width))
	{
		for (int i = 0; i < S1.size; i++)
		{
			S1.ps[i].ri = 70;
			S1.ps[i].ci = 120;
		}
		erasesnake2(S1, height, width);
	}
	if (snakeisdead(S2)|| snakeisdeadmultiple(S2, S1, height, width) || snakeisdeadmultiple(S2, S3, height, width)
		|| snakeisdeadmultiple(S2, S4, height, width))

	{
		for (int i = 0; i < S2.size; i++)
		{
			S2.ps[i].ri = 80;
			S2.ps[i].ci = 120;
		}
		erasesnake2(S2, height, width);
	}
	if (snakeisdead(S3)|| snakeisdeadmultiple(S3, S1, height, width) || snakeisdeadmultiple(S3, S2, height, width)
		|| snakeisdeadmultiple(S3, S4, height, width))
	{
		for (int i = 0; i < S3.size; i++)
		{
			S3.ps[i].ri = 90;
			S3.ps[i].ci = 120;
		}
		erasesnake2(S3, height, width);
	}
	if (snakeisdead(S4)|| snakeisdeadmultiple(S4, S1, height, width) || snakeisdeadmultiple(S4, S2, height, width)
		|| snakeisdeadmultiple(S4, S3, height, width))
	{
		for (int i = 0; i < S3.size; i++)
		{
			S4.ps[i].ri = 100;
			S4.ps[i].ci = 120;
		}
		erasesnake2(S4, height, width);

	}

}
void changestage(snake& S1, snake& S2, snake& S3, snake& S4, int height, int width)
{
	t2b(width - 3, 1, height - 2, -37);
}
void displaysnake(const snake& S1,const snake& S2,const snake& S3,const snake& S4,int height,int width)
{
	for (int i = 0; i < S1.size; i++)
	{
		if (snakeisdead(S1)||snakeisdeadstage1(S1,height,width) || snakeisdeadstage2(S1,height,width)
			|| snakeisdeadmultiple(S1,S2,height,width) || snakeisdeadmultiple(S1, S3, height, width)
			|| snakeisdeadmultiple(S1, S4, height, width)||snakeisdeadstage3(S1,height,width))
		{
			
			break;
		}
		else
		{
			SetClr(2);
			gotoRowCol(S1.ps[i].ri, S1.ps[i].ci);
			cout << S1.sym;
		}
	}
	for (int i = 0; i < S2.size; i++)
	{
		if (snakeisdead(S2) || snakeisdeadstage1(S2, height, width) || snakeisdeadstage2(S2, height, width) 
			|| snakeisdeadmultiple(S2, S1, height, width) || snakeisdeadmultiple(S2, S3, height, width)
			|| snakeisdeadmultiple(S2, S4, height, width) || snakeisdeadstage3(S2, height, width))
		{
			break;
		}
		else
		{
			SetClr(6);
			gotoRowCol(S2.ps[i].ri, S2.ps[i].ci);
			cout << S2.sym;
		}
	}
	for (int i = 0; i < S3.size; i++)
	{
		if (snakeisdead(S3) || snakeisdeadstage1(S3, height, width) || snakeisdeadstage2(S3, height, width)
			|| snakeisdeadmultiple(S3, S1, height, width) || snakeisdeadmultiple(S3, S2, height, width)
			|| snakeisdeadmultiple(S3, S4, height, width)||snakeisdeadstage3(S3,height,width))
		{
			break;
		}
		else
		{
			SetClr(13);
			gotoRowCol(S3.ps[i].ri, S3.ps[i].ci);
			cout << S3.sym;
		}
	}
	for (int i = 0; i < S4.size; i++)
	{
		if (snakeisdead(S4) || snakeisdeadstage1(S4, height, width) || snakeisdeadstage2(S4, height, width)
			|| snakeisdeadmultiple(S4, S1, height, width) || snakeisdeadmultiple(S4, S2, height, width)
			|| snakeisdeadmultiple(S4, S3, height, width)||snakeisdeadstage3(S4,height,width))
		{
			break;
		}
		else
		{
			SetClr(4);
			gotoRowCol(S4.ps[i].ri, S4.ps[i].ci);
			cout << S4.sym;
		}
	}
	
}

int main15()
{
	int height = 60;
	int width = 180;

	char c;

	snake S1;
	snake S2, S3, S4;
	food f;

	input(S1,S2,S3,S4, height, width,f);
	makefood(f, height, width, S1);
	gotoRowCol(70, 0);
	cout << S1.pname << "'s score:";
	gotoRowCol(80, 0);
	cout << S2.pname << "'s score:";
	gotoRowCol(90, 0);
	cout << S3.pname << "'s score:";
	gotoRowCol(100, 0);
	cout << S4.pname << "'s score:";
	
	PlaySound(TEXT("bscs22064-nagin music"), NULL, SND_ASYNC);
	
	while (true)
	{
		if (_kbhit())
		{
			c = _getch();
			if (c == -32)
			{
				c = _getch();
				snakedirection(S1,S2, c);
			}
			else
			{
				snakedirection2(S2,S3,S4, c);
			}				
		}
		displaysnake(S1, S2,S3,S4,height,width);
		Sleep(40);
		erasesnake(S1, S2,S3,S4,height,width);
		movesnake(S1, S2, S3, S4, height, width);
		snakedeaderase2(S1, S2, S3, S4, height, width);
		displaysnake(S1, S2, S3, S4,height,width);
		displayfood(f, '*');
		
		SetClr(15);
		gotoRowCol(70, 16);
		cout << S1.score;
		gotoRowCol(80, 16);
		cout << S2.score;
		gotoRowCol(90, 16);
		cout << S3.score;
		gotoRowCol(100, 16);
		cout << S4.score;
		
		if (foodiseaten(f, S1))
		{		
			makefood(f, height, width, S1);
			displayfood(f, '*');
			expandsnake(S1,f);
			displayscore(S1,f);
		}		
		else if (foodiseaten(f, S2))
		{
			makefood(f, height, width, S2);
			displayfood(f, '*');
			expandsnake(S2,f);
			displayscore(S3, f);
		}
		else if (foodiseaten(f, S3))
		{
			makefood(f, height, width, S3);
			displayfood(f, '*');
			expandsnake(S3,f);
			displayscore(S3, f);
		}
		else if (foodiseaten(f, S4))
		{
			makefood(f, height, width, S4);
			displayfood(f, '*');
			expandsnake(S4,f);
			displayscore(S4, f);
		}
		if (gameover(S1, S2, S3, S4, width))
		{
			
		}
		
	}
	return _getch();
}
int main3()
{
	int height = 60;
	int width = 180;

	char c;

	snake S1;
	snake S2, S3, S4;
	food f;

	boundries3(height, width);
	input(S1, S2, S3, S4, height, width, f);
	makefood2(f, height, width, S1,S2,S3,S4);
	
	gotoRowCol(70, 0);
	cout << S1.pname << "'s score:";
	gotoRowCol(80, 0);
	cout << S2.pname << "'s score:";
	gotoRowCol(90, 0);
	cout << S3.pname << "'s score:";
	gotoRowCol(100, 0);
	cout << S4.pname << "'s score:";
	PlaySound(TEXT("bscs22064-nagin music"), NULL, SND_ASYNC);
	while (true)
	{
		if (_kbhit())
		{
			c = _getch();
			if (c == -32)
			{
				c = _getch();
				snakedirection(S1, S2, c);
			}
			else
			{
				snakedirection2(S2, S3, S4, c);
			}
		}
		displaysnake(S1, S2, S3, S4, height, width);
		Sleep(40);
		erasesnake(S1, S2, S3, S4, height, width);
		movesnake4(S1, S2, S3, S4, height, width);
		snakedeaderase4(S1, S2, S3, S4, height, width);
		snakedeaderase3(S1, S2, S3, S4, height, width);
		snakedeaderase2(S1, S2, S3, S4, height, width);
		snakedeaderase(S1, S2, S3, S4, height, width);
		displaysnake(S1, S2, S3, S4, height, width);
		displayfood(f, '*');

		SetClr(15);
		gotoRowCol(70, 16);
		cout << S1.score;
		gotoRowCol(80, 16);
		cout << S2.score;
		gotoRowCol(90, 16);
		cout << S3.score;
		gotoRowCol(100, 16);
		cout << S4.score;

		if (foodiseaten(f, S1))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S1, f);
			displayscore(S1, f);
		}
		else if (foodiseaten(f, S2))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S2, f);
			displayscore(S3, f);
		}
		else if (foodiseaten(f, S3))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S3, f);
			displayscore(S3, f);
		}
		else if (foodiseaten(f, S4))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S4, f);
			displayscore(S4, f);
		}
		if (gameover(S1, S2, S3, S4, width))
		{
		
		}

	}

	return _getch();
}
int main2()
{
	int height = 60;
	int width = 180;

	char c;

	snake S1;
	snake S2, S3, S4;
	food f;

	boundries2(height, width);
	input(S1, S2, S3, S4, height, width, f);
	makefood2(f, height, width, S1,S2,S3,S4);
	
	gotoRowCol(70, 0);
	cout << S1.pname << "'s score:";
	gotoRowCol(80, 0);
	cout << S2.pname << "'s score:";
	gotoRowCol(90, 0);
	cout << S3.pname << "'s score:";
	gotoRowCol(100, 0);
	cout << S4.pname << "'s score:";
	PlaySound(TEXT("bscs22064-nagin music"), NULL, SND_ASYNC);
	while (true)
	{
		if (_kbhit())
		{
			c = _getch();
			if (c == -32)
			{
				c = _getch();
				snakedirection(S1, S2, c);
			}
			else
			{
				snakedirection2(S2, S3, S4, c);
			}
		}
		displaysnake(S1, S2, S3, S4, height, width);
		Sleep(40);
		erasesnake(S1, S2, S3, S4, height, width);
		movesnake3(S1, S2, S3, S4, height, width);
		snakedeaderase3(S1, S2, S3, S4, height, width);
		snakedeaderase2(S1, S2, S3, S4, height, width);
		snakedeaderase(S1, S2, S3, S4, height, width);
		displaysnake(S1, S2, S3, S4, height, width);
		displayfood(f, '*');

		SetClr(15);
		gotoRowCol(70, 16);
		cout << S1.score;
		gotoRowCol(80, 16);
		cout << S2.score;
		gotoRowCol(90, 16);
		cout << S3.score;
		gotoRowCol(100, 16);
		cout << S4.score;

		if (foodiseaten(f, S1))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S1, f);
			displayscore(S1, f);
		}
		else if (foodiseaten(f, S2))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S2, f);
			displayscore(S3, f);
		}
		else if (foodiseaten(f, S3))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S3, f);
			displayscore(S3, f);
		}
		else if (foodiseaten(f, S4))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S4, f);
			displayscore(S4, f);
		}
		if (gameover(S1, S2, S3, S4, width))
		{
			SetClr(15);
			changestage(S1, S2, S3, S4, height, width);
			if (winninggame(S1))
			{
				if (S1.ps[0].ci == width - 4)
				{
					system("cls");
					main3();
				}
			}
			else if (winninggame(S2))
			{
				if (S2.ps[0].ci == width - 4)
				{
					system("cls");
					main3();
				}
			}
			else if (winninggame(S4))
			{
				if (S3.ps[0].ci == width - 4)
				{
					system("cls");
					main3();
				}
			}
			else if (winninggame(S4))
			{
				if (S4.ps[0].ci == width - 4)
				{
					system("cls");
					main3();
				}
			}
		}

	}

	return _getch();
}
int main1()
{
	int height = 60;
	int width = 180;

	char c;

	snake S1;
	snake S2, S3, S4;
	food f;

	boundries(height, width);
	input(S1, S2, S3, S4, height, width, f);
	makefood(f, height, width, S1);
	gotoRowCol(70, 0);
	cout << S1.pname << "'s score:";
	gotoRowCol(80, 0);
	cout << S2.pname << "'s score:";
	gotoRowCol(90, 0);
	cout << S3.pname << "'s score:";
	gotoRowCol(100, 0);
	cout << S4.pname << "'s score:";

	PlaySound(TEXT("bscs22064 - nagin music"), NULL, SND_ASYNC);
	while (true)
	{
		if (_kbhit())
		{
			c = _getch();
			if (c == -32)
			{
				c = _getch();
				snakedirection(S1, S2, c);
			}
			else
			{
				snakedirection2(S2, S3, S4, c);
			}
		}
		displaysnake(S1, S2, S3, S4, height, width);
		Sleep(40);
		erasesnake(S1, S2, S3, S4, height, width);
		movesnake2(S1, S2, S3, S4, height, width);
		snakedeaderase(S1, S2, S3, S4, height, width);
		snakedeaderase2(S1, S2, S3, S4, height, width);
		displaysnake(S1, S2, S3, S4, height, width);
		displayfood(f, '*');
		
		SetClr(15);
		gotoRowCol(70, 16);
		cout << S1.score;
		gotoRowCol(80, 16);
		cout << S2.score;
		gotoRowCol(90, 16);
		cout << S3.score;
		gotoRowCol(100, 16);
		cout << S4.score;

		if (foodiseaten(f, S1))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S1,f);
			displayscore(S1, f);
		}
		else if (foodiseaten(f, S2))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S2,f);
			displayscore(S3, f);
		}
		else if (foodiseaten(f, S3))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S3,f);
			displayscore(S3, f);
		}
		else if (foodiseaten(f, S4))
		{
			makefood2(f, height, width, S1, S2, S3, S4);
			displayfood(f, '*');
			expandsnake(S4,f);
			displayscore(S4, f);
		}
		if (gameover(S1,S2,S3,S4,width))
		{
			SetClr(4);
			changestage(S1, S2, S3, S4, height, width);
			if (winninggame(S1))
			{
				if (S1.ps[0].ci == width - 4)
				{
					system("cls");
					main2();
				}
			}
			else if (winninggame(S2))
			{
				if (S2.ps[0].ci == width - 4)
				{
					system("cls");
					main2();
				}
			}
			else if (winninggame(S3))
			{
				if (S3.ps[0].ci == width - 4)
				{
					system("cls");
					main2();
				}
			}
			else if (winninggame(S4))
			{
				if (S4.ps[0].ci == width - 4)
				{
					system("cls");
					main2();
				}
			}
		}		

	}

	return _getch();
}
int main()
{
	int ch;

	cout << "1.start simple game...Dunya gol h!!!" << endl;
	cout << "2.Start game with levels" << endl;
	cout << "3.apni mrzi ka level select karein:" << endl;

	cin >> ch;

	switch (ch)
	{
	case 1:
	{
		system("cls");
		PlaySound(TEXT("bscs22064-start game music"), NULL, SND_SYNC);
		main15();
	}
	case 2:
	{
		system("cls");
		PlaySound(TEXT("bscs22064-start game music"), NULL, SND_SYNC);
		main1();
	}
	case 3:
	{
		int choices;
		cout << "level 1" << endl;
		cout << "level 2" << endl;
		cout << "level 3" << endl;
		cin >> choices;
		switch (choices)
		{
		case 1:
		{
			system("cls");
			PlaySound(TEXT("bscs22064-start game music"), NULL, SND_SYNC);
			main1();
		}
		case 2:
		{
			system("cls");
			PlaySound(TEXT("bscs22064-start game music"), NULL, SND_SYNC);
			main2();
		}
		case 3:
		{
			system("cls");
			PlaySound(TEXT("bscs22064-start game music"), NULL, SND_SYNC);
			main3();
		}
		default:
			break;
		}
	}
	}
	return _getch();
}
