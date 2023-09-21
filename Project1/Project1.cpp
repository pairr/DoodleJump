#include "frame.h"
#include "Project1.h"
#include "Framework.h"
#include "Doodle.h"
#include "Bullet.h"
#include "Grid.h"
#include <chrono>
#include <windows.h>
#include <string>
#include <shellapi.h>
#include <fstream>

class MyFramework : public Framework {
private:

	Doodle* hero; // doodle - hero of the game

	int Width, Height; // width and height of window

	bool move_left = false; // true iff <- pressed
	bool move_right = false; // true iff -> pressed
	bool game_over = false; // true iff doodle is dead 
	bool mouse_click = false; // true iff left mouse button is clicked
	int mousex = 0, mousey = 0; // coordinates of mouse at a given frame
	unsigned int  last_shot_time; // time of the last shot
								 // use it to limit number of bullets by 10 in a second
public:

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = Width;
		height = Height;
		fullscreen = false;
	}
	void set(int w, int h) // set window size and initialize some variables
	{
		Width = w;
		Height = h;
		move_left = false;
		move_right = false;
		mouse_click = false;
		mousex = 0;
		mousey = 0;
		game_over = false;
		last_shot_time = getTickCount();
	}
	virtual bool Init() {
		
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {

		if (hero == NULL)
		{
			game_over = false;  hero = new Doodle(Width, Height);
		}
		if (game_over)
		{
			delete hero;
			hero = NULL;
		}
		else
		{
			bool shooting = false;
			if(mouse_click == true) // if mouse_click == true then user tries to shoot
			{
				unsigned int time_now = getTickCount(); // time now

				long long elapsed_milliseconds = time_now - last_shot_time; // time elapsed since last shot
				if (elapsed_milliseconds >= 100){ shooting = true; last_shot_time = time_now;} // if enough time passed then let it shoot
																							  // and last shot time
				else shooting = false;
			}

			game_over = hero->update(move_left, move_right, shooting, mousex, mousey);
			hero->draw(mouse_click, move_left, move_right); // if mouse is clicked draw as if shooting
														   // more pleasent for an eye
		}
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		// store position of mouse
		mousex = x;
		mousey = y;
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		if (button == FRMouseButton::LEFT)
		{
			//store mouse clicks
			mouse_click = !isReleased;
		}
	}

	virtual void onKeyPressed(FRKey k) {
		// tracking -> and <- presses
		// while key is pressed, user tries to move
		switch (k)
		{
		case FRKey::RIGHT:
			move_right = true;
			break;
		case FRKey::LEFT:
			move_left = true;
			break;
		}

	}

	virtual void onKeyReleased(FRKey k) {
		// tracking -> and <- releases
		// if key is released, user don't want to move in that direction
		switch (k)
		{
		case FRKey::RIGHT:
			move_right = false;
			break;
		case FRKey::LEFT:
			move_left = false;
			break;
		}
	}

	virtual const char* GetTitle() override
	{
		return "DoodleJump";
	}
};;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int w = 800, h = 600; // default width and height of window

	//using command line arguments
	LPWSTR* argv;//number of arguments
	int argc; // arguments
	argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	if (argc > 2)
	{
		// setting width and height
		w = _wtoi(argv[1]);
		h = _wtoi(argv[2]);
	}

	// Free the memory allocated by CommandLineToArgvW
	LocalFree(argv);

	MyFramework* game = new MyFramework();
	game->set(w, h); // set window size and initialize some variables
	game->Init();
	return run(game);
}