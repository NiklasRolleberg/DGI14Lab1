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
#include <string>
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
vector<vec3> stars;
int  t;
float v = 0.0002;
int writefps;
// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
void Interpolate(vec3 a, vec3 b, vector<vec3>& result);
void Update();

// --------------------------------------------------------
// FUNCTION DEFINITIONS

int main(int argc, char* argv[])
{
	cout << "number of stars" << endl;
	string input = "";
	getline(cin, input);

	screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
	//int nstars = 1000;
	int nstars = atoi(input.c_str());
	cout << nstars << " stars";

	vector<vec3> stars2(nstars);
	stars = stars2;
	for (int i = 0; i < nstars; i++) {
		stars[i].x = 1 - 2 * (float(rand()) / float(RAND_MAX));
		stars[i].y = 1 - 2 * (float(rand()) / float(RAND_MAX));
		stars[i].z = (float(rand()) / float(RAND_MAX));
	}

	t = SDL_GetTicks();
	while (NoQuitMessageSDL())
	{
		Update();
		Draw();
	}
	SDL_SaveBMP(screen, "screenshot2.bmp");

	return 0;
}

void Draw()
{

	int f = SCREEN_HEIGHT / 2; //focus length

	SDL_FillRect(screen, 0, 0);
	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);
	//vec3 color(1, 1, 1);
	for (size_t s = 0; s<stars.size(); s++)
	{
		//Projection
		float x = stars[s].x;
		float y = stars[s].y;
		float z = stars[s].z;
		int u = f*(x / z) + (SCREEN_WIDTH / 2);
		int v = f*(y / z) + (SCREEN_HEIGHT / 2);
		//cout << z << endl;
		//plot pixel
		vec3 color = 0.1f * vec3(1, 1, 1) / (stars[s].z*stars[s].z);
		PutPixelSDL(screen, u, v, color);
	}
	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void Update() {

	int t2 = SDL_GetTicks();
	float dt = float(t2 - t);
	t = t2;

	if (dt < 16) {
		SDL_Delay(16 - dt);
	}

	if ((writefps = (writefps++) % 10) == 0) {
		system("CLS");
		if (dt < 16) {
			cout << "FPS " << 1000 / 16 << endl;
		}
		else {
			cout << "FPS " << 1000 / dt << endl;
		}
	}

	int n = stars.size();
	for (int i = 0; i < n; i++) {

		stars[i].z = stars[i].z - dt*v;

		if (stars[i].z <= 0)
			stars[i].z += 1;
		if (stars[i].z > 1)
			stars[i].z -= 1;
	}
}

void Interpolate(vec3 a, vec3 b, vector<vec3>& result) {
	int steps = result.size() - 1; //4 spaces --> 2 interpolation points
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