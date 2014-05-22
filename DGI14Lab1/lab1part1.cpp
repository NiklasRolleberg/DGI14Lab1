// Introduction lab that covers:
// * C++
// * SDL
// * 2D graphics
// * Plotting pixels
// * Video memory
// * Color representation
// * Linear interpolation
// * glm::vec3 and std::vector

#include "SDL.h"
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "SDLauxiliary.h"

using namespace std;
using glm::vec3;

// --------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Surface* screen;

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
void Interpolate(vec3 a, vec3 b, vector<vec3>& result);

// --------------------------------------------------------
// FUNCTION DEFINITIONS

int main(int argc, char* argv[])
{
	//cout << "Start\n" << endl;
	/*
	//egen funktion
	vector<vec3> result(4);
	vec3 a(1, 4, 9.2);
	vec3 b(4, 1, 9.8);

	//cout << "HEJSAN"<<endl;
	//cout << result.size()<<endl; //funkar!
	
	Interpolate(a, b, result);
	for (int i = 0; i<result.size(); ++i)
	{
		cout << "( "
			<< result[i].x << ", "
			<< result[i].y << ", "
			<< result[i].z << " )\n ";
	}
	*/

	//ritar
	
	screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
	while (NoQuitMessageSDL())
	{
		Draw();
		SDL_Delay(200);
	}
	SDL_SaveBMP(screen, "part1.bmp");
	
	return 0;
}

void Draw()
{
	vec3 topLeft(1, 0, 0); // red
	vec3 topRight(0, 0, 1); // blue
	vec3 bottomLeft(0, 1, 0); // green
	vec3 bottomRight(1, 1, 0); // yellow

	vector<vec3> leftSide(SCREEN_HEIGHT);
	vector<vec3> rightSide(SCREEN_HEIGHT);
	Interpolate(topLeft, bottomLeft, leftSide);
	Interpolate(topRight, bottomRight, rightSide);

	for (int y = 0; y<SCREEN_HEIGHT; ++y)
	{

		vector<vec3> colors(SCREEN_WIDTH);
		Interpolate(leftSide[y], rightSide[y], colors);

		for (int x = 0; x<SCREEN_WIDTH; ++x)
		{
			PutPixelSDL(screen, x, y, colors[x]);
		}
	}

	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void Interpolate(vec3 a, vec3 b, vector<vec3>& result) {
	int steps = result.size()-1; //4 spaces --> 2 interpolation points
	//cout << steps << endl;
	if (steps <= 0)
		return;

	//X
	float startx = (a.x);
	float stopx = b.x;
	float dx = (stopx - startx) / steps;

	//Y
	float starty = (a.y);
	float stopy = b.y;
	float dy = (stopy - starty) / steps;

	//Z
	float startz = (a.z);
	float stopz = b.z;
	float dz = (stopz - startz) / steps;

	for (int i = 0; i <= steps; ++i) {
		result[i].x = startx + i*dx;
		result[i].y = starty + i*dy;
		result[i].z = startz + i*dz;
	}
}