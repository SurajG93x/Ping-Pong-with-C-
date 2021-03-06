// PongGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <conio.h>
#include <Windows.h>

using namespace std;

enum eDir
{
	STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3,
	RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6,
};

class Ball {
public:
	inline int getX() {
		return x;
	}
	inline int getY() {
		return y;
	}
	inline eDir getDirection() {
		return direction;
	}

	Ball(int posX, int posY) {
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
		direction = STOP;
	}

	void Reset() {
		x = originalX;
		y = originalY;
		direction = STOP;
	}

	void changeDirection(eDir d) {
		direction = d;
	}

	void randomDirection() {
		direction = (eDir)((rand() % 6) + 1 );
	}

	void move() {
		switch (direction)
		{
		case STOP:
			break;
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UPLEFT:
			x--; y--;
			break;
		case DOWNLEFT:
			x--; y++;
			break;
		case UPRIGHT:
			x++; y--;
			break;
		case DOWNRIGHT:
			x++; y++;
			break;
		default:
			break;
		}
	}

	friend ostream & operator <<(ostream & o, Ball c) {
		o << "Ball [" << c.x << "," << c.y << "][" << c.direction << "]" << endl;
		return o;
	}
	
private:
	int x, y;
	int originalX, originalY;
	eDir direction;
};

class Paddle
{
public:
	Paddle() {
		x = y = 0;
	}
	Paddle(int posX, int posY) :Paddle() {
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
	}

	inline void Reset() {
		x = originalX;
		y = originalY;
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline void moveUp(){ y--; }
	inline void moveDown() { y++; }

	friend ostream & operator <<(ostream & o, Paddle c) {
		o << "Ball [" << c.x << "," << c.y << "]" << endl;
		return o;
	}
	
private:
	int x, y;
	int originalX, originalY;

};

class GameManager {

public:
	GameManager(int w, int h) {
		srand(time(NULL));
		quit = false;
		up1 = 'w';
		up2 = 'i';
		down1 = 's';
		down2 = 'k';
		score1 = score2 = 0;
		width = w;
		height = h;
		ball = new Ball(w / 2, h / 2);
		player1 = new Paddle(1, h / 2 - 3);
		player2 = new Paddle(w - 2, h / 2 - 3);
	}

	~GameManager(){
		delete player1, player2, ball;
	}

	void ScoreUp(Paddle *player) {
		if (player == player1)
			score1++;
		else if (player == player2)
			score2++;
		
		ball->Reset();
		player1->Reset();
		player2->Reset();
	}

	void Draw() {
		system("cls");
		for (int i = 0; i < width + 2; i++) cout << "#";
		cout << endl;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				int ballx = ball->getX();
				int bally = ball->getY();
				int player1x = player1->getX();
				int player2x = player2->getX();
				int player1y = player1->getY();
				int player2y = player2->getY();

				if (j == 0)cout << "#";

				if (ballx == j&&bally == i)cout << "O";
				else if (player1x == j&&player1y == i)cout << "#";
				else if (player2x == j&&player2y == i)cout << "#";
				
				else if (player1x == j&&player1y +1 == i)cout << "#";
				else if (player1x == j&&player1y +2 == i)cout << "#";
				else if (player1x == j&&player1y +3 == i)cout << "#";

				else if (player2x == j&&player2y + 1 == i)cout << "#";
				else if (player2x == j&&player2y + 2 == i)cout << "#";
				else if (player2x == j&&player2y + 3 == i)cout << "#";

				else cout << " ";

				if (j == width - 1)cout << "#";
			}
			cout << endl;
		}
		for (int i = 0; i < width + 2; i++)cout << "#";
		cout << endl;

		cout << "Score1: " << score1 << endl << "Score2 :" << score2 << endl;
	}

	void Input() {
		ball->move();

		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();

		if (_kbhit()) {
			char current = _getch();
			if (current == up1)
				if (player1y > 0)
					player1->moveUp();
			if (current == up2)
				if (player2y > 0)
					player2->moveUp();
			if (current == down1)
				if (player1y + 4 < height)
					player1->moveDown();
			if (current == down2)
				if (player2y + 4 < height)
					player2->moveDown();

			if (ball->getDirection() == STOP)
				ball->randomDirection();

			if (current == 'q')
				quit = true;
		}
	}

	void Logic() {
		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();

		for (int i = 0; i < 4; i++)
			if (ballx == player1x + 1)
				if (bally == player1y + i)
					ball->changeDirection((eDir)((rand() % 3) + 4));

		for (int i = 0; i < 4; i++)
			if (ballx == player2x - 1)
				if (bally == player2y + i)
					ball->changeDirection((eDir)((rand() % 3) + 1));

		if (bally == height - 1)
			ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);

		if (bally == 0)
			ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);

		if (ballx == width - 1)
			ScoreUp(player1);
		if (ballx == 0)
			ScoreUp(player2);
	}

	void Run() {
		while (!quit) {
			Draw();
			Input();
			Logic();
		}
	}

private:
	int width, height;
	int score1, score2;
	char up1, down1, up2, down2;
	bool quit;
	Ball * ball;
	Paddle *player1;
	Paddle *player2;

};

int main()
{
	GameManager g(40, 20);
	g.Run();
    return 0;
}
