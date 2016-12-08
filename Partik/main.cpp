#include <SDL.h>
#include <random>
#include <time.h>
#include <stdio.h>
#include <SDL_opengl.h>

#include "particle.h"

#define SCREEN_WIDTH 1900
#define SCREEN_HEIGHT 900

void __gluMakeIdentityd(GLdouble m[16])
{
	m[0 + 4 * 0] = 1; m[0 + 4 * 1] = 0; m[0 + 4 * 2] = 0; m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0; m[1 + 4 * 1] = 1; m[1 + 4 * 2] = 0; m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0; m[2 + 4 * 1] = 0; m[2 + 4 * 2] = 1; m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0; m[3 + 4 * 1] = 0; m[3 + 4 * 2] = 0; m[3 + 4 * 3] = 1;
}

void gluPersepective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
	GLdouble m[4][4];
	double sine, cotangent, deltaZ;
	double radians = fovy / 2 * std::_Pi / 180;

	deltaZ = zFar - zNear;
	sine = sin(radians);
	if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
		return;
	}
	cotangent = std::cos(radians) / sine;

	__gluMakeIdentityd(&m[0][0]);
	m[0][0] = cotangent / aspect;
	m[1][1] = cotangent;
	m[2][2] = -(zFar + zNear) / deltaZ;
	m[2][3] = -1;
	m[3][2] = -2 * zNear * zFar / deltaZ;
	m[3][3] = 0;
	glMultMatrixd(&m[0][0]);
}

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow("Cool simulation stuff",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL
	);

	SDL_GLContext gl = SDL_GL_CreateContext(window);
	//SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPersepective(45.0, double(SCREEN_WIDTH) / double(SCREEN_HEIGHT), 10.0, 10000.0);

	glMatrixMode(GL_MODELVIEW);
	glTranslated(0, 0, -1500.0);
	
	srand((unsigned int)time(NULL));

	std::default_random_engine engine;
	std::normal_distribution<double> normdist(0, 200);

	Particle * particles = new Particle[SIZE];
	for (unsigned int i = 0; i < SIZE; ++i) {
		Particle &p = particles[i];
		new (&p) Particle();
		p.p.at_(0) = normdist(engine);
		p.p.at_(1) = normdist(engine);
		p.p.at_(2) = normdist(engine);
	}

	bool run = true;
	while (run) {
		simulate(particles);
		update(particles);

		/*SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		
		SDL_RenderDrawLine(renderer, 2 + offx, 2 + offy, -2 + offx, -2 + offy);
		SDL_RenderDrawLine(renderer, 2 + offx, -2 + offy, -2 + offx, 2 + offy);*/
		
		glClearColor(0, 0, 0, 255.0f);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glRotated(0.1, 0, 1.0, 0);

		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);

		glVertex3f(10, 0, 0);
		glVertex3f(-10, 0, 0);

		glVertex3f(0, 10, 0);
		glVertex3f(0, -10, 0);

		glVertex3f(0, 0, 10);
		glVertex3f(0, 0, -10);

		glEnd();
		
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POINTS);
		for (unsigned int i = 0; i < SIZE; ++i) {
			Particle &p = particles[i];
			glVertex3d(p.p.at_(0), p.p.at_(1), p.p.at_(2));
		}
		glEnd();

		SDL_GL_SwapWindow(window);

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) run = false;
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) run = false;
		};
	}

	SDL_GL_DeleteContext(gl);
	SDL_Quit();
	return 0;
}