#pragma once
#include <SDL.h>
/**
* TODO
* - Linux
* - add Audio
* - render a counter for each player
* - rewrite collision check
* - optimize structure
*/


/*
* The directions something goes on the X
* */
enum class XDirections
{
	RIGHT,
	LEFT
};

/*
* The directions something goes on the Y
* */
enum class YDirections
{
	UP,
	DOWN
};

/*
* The player class with an rectangle to display in render and the players win count
* */
class Player
{
public:
	SDL_Rect rect;
	int count;
};

/*
* The ball class with an rectangle to display in render and the directions moving directions of x and y
* */
class Ball
{
public:
	SDL_Rect rect;
	XDirections xDirection;
	YDirections yDirection;
};

class BingBong
{
private:
	const char* TITLE = "BingBong";
	const int WIDTH = 800;
	const int HEIGHT = 600;
	const int PLAYERWIDTH = 25;
	const int PLAYERHEIGHT = 200;
	const int BALLSIZE = 20;
	const int WIN = 5;

	bool quit = false;

	SDL_Window* window;
	SDL_Renderer* renderer;

	Player players[2];
	Player* player1 = &players[0];
	Player* player2 = &players[1];

	Ball ball;

	void Handle();
	void HandleEvents();
	void HandleInputOutput();


	void Render();
	void RenderLine();
	void RenderPlayers();
	void RenderBall();
	void RenderShow();
	void RenderClear();

public:
	/**
	* The initializer of some stuff
	* */
	void Init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
	}

	/**
	* The game loop
	* */
	void Loop()
	{
		while (quit != true)
		{
			Handle();
			Render();
		}
	}

	/**
	* Constructor
	* */
	BingBong()
	{
		// Create the window and the renderer
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		// Set player count and rectangle size 
		for (auto& player : players)
		{
			player.count = 0;

			player.rect.w = PLAYERWIDTH;
			player.rect.h = PLAYERHEIGHT;
		}

		// Set players coordinates
		player1->rect.x = 50;
		player1->rect.y = (HEIGHT / 2) - (PLAYERHEIGHT / 2);

		player2->rect.x = WIDTH - (50 + PLAYERWIDTH);
		player2->rect.y = (HEIGHT / 2) - (PLAYERHEIGHT / 2);

		// Set ball size
		ball.rect.w = BALLSIZE;
		ball.rect.h = BALLSIZE;

		// Set ball coordinates
		ball.rect.x = (WIDTH / 2) - (BALLSIZE / 2);
		ball.rect.y = (HEIGHT / 2) - (BALLSIZE / 2);

		// Set ball moving directions
		ball.xDirection = XDirections::LEFT;
		ball.yDirection = YDirections::DOWN;
	}

	/**
	* Deconstructor
	* */
	~BingBong()
	{
		SDL_Quit();
	}
};

void BingBong::Handle()
{
	HandleEvents();
	HandleInputOutput();
}

void BingBong::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			quit = true;
		}
	}
}

void BingBong::HandleInputOutput()
{
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);

	// Player 1 movement
	if (keyStates[SDL_SCANCODE_W])
	{
		if (player1->rect.y >= 0)
			player1->rect.y -= 2;
	}
	if (keyStates[SDL_SCANCODE_S])
	{
		if (player1->rect.y < (HEIGHT - PLAYERHEIGHT))
			player1->rect.y += 2;
	}

	// Player 2 movement
	if (keyStates[SDL_SCANCODE_UP])
	{
		if (player2->rect.y >= 0)
			player2->rect.y -= 2;
	}
	if (keyStates[SDL_SCANCODE_DOWN])
	{
		if (player2->rect.y < (HEIGHT - PLAYERHEIGHT))
			player2->rect.y += 2;
	}

	// Ball directions
	switch (ball.xDirection)
	{
	case XDirections::LEFT:
		ball.rect.x--;
		// X collision Check
		if (ball.rect.x == player1->rect.x + PLAYERWIDTH)
		{
			// Y collision check
			if ((ball.rect.y + BALLSIZE >= player1->rect.y) && (ball.rect.y <= player1->rect.y + PLAYERHEIGHT))
			{
				// change Direction
				ball.xDirection = XDirections::RIGHT;
			}
		}
		break;

	case XDirections::RIGHT:
		ball.rect.x++;
		
		if (ball.rect.x >= player2->rect.x - BALLSIZE)
		{
			if ((ball.rect.y >= player2->rect.y) && (ball.rect.y <= player2->rect.y + PLAYERHEIGHT))
			{
				ball.xDirection = XDirections::LEFT;
			}
		}
		break;
	}

	switch (ball.yDirection)
	{
	case YDirections::UP:
		ball.rect.y--;
		if (ball.rect.y <= 0)
			ball.yDirection = YDirections::DOWN;
		break;

	case YDirections::DOWN:
		ball.rect.y++;
		if (ball.rect.y >= HEIGHT - BALLSIZE)
			ball.yDirection = YDirections::UP;
		break;
	}

	// Out of bound check player1
	if (ball.rect.x >= WIDTH)
	{
		player1->count++;
		// Set ball coordinates
		ball.rect.x = (WIDTH / 2) - (BALLSIZE / 2);
		ball.rect.y = (HEIGHT / 2) - (BALLSIZE / 2);
	}

	// Out of bound check player2
	if (ball.rect.x <= 0)
	{
		player2->count++;
		// Set ball coordinates
		ball.rect.x = (WIDTH / 2) - (BALLSIZE / 2);
		ball.rect.y = (HEIGHT / 2) - (BALLSIZE / 2);
	}

	// Check for Winner
	if (player1->count == WIN)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Winner", "Player1 has won!", window);
		quit = true;
	}
	if (player2->count == WIN)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Winner", "Player2 has won!", window);
		quit = true;
	}
}

void BingBong::Render()
{
	RenderClear();
	RenderLine();
	RenderBall();
	RenderPlayers();
	RenderShow();
}

void BingBong::RenderLine()
{
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	for (int current_h = 0; current_h <= HEIGHT; current_h += 32)
	{
		SDL_RenderDrawLine(renderer, WIDTH / 2, current_h, WIDTH / 2, current_h + 16);
	}
}

void BingBong::RenderPlayers()
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for (const auto& player : players)
	{
		SDL_RenderFillRect(renderer, &player.rect);
	}
}

void BingBong::RenderBall()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &ball.rect);
}

void BingBong::RenderShow()
{
	SDL_RenderPresent(renderer);
}

void BingBong::RenderClear()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}