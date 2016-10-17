#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include "glad\include\glad\glad.h"
#include <vector>
#include "CShader.h"
#include "CShaderProgram.h"
#define BUFFER_OFFSET(i) ((void*)(i))


const int w = 800;
const int h = 600;

bool Initialize();
void Close();
void render();
void initOpenGL();

GLuint bufferID, indicesID, colorID, vertexArrayID;

SDL_Window* window = nullptr;
SDL_Surface* screen = nullptr;
CShader shVertex, shFragment;
CShaderProgram spMain;

SDL_GLContext mainContext = nullptr;
int indices[] = { 0,1,3 };

float v[] = { 1,0,0
-1,0,0,
0,1,0
};
float c[] = {
	1,0,0,
	0,1,0,
	0,0,1
};

int main(int argc, char* args[]) {
	if (!Initialize()) {
		return -1;
	}
	bool exit = false;

	SDL_Event e;
	while (!exit) {

		render();
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				exit = true;
			}
		}
	}
	Close();
	return 0;
}

bool Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("something went wrong");
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetSwapInterval(1);
	window = SDL_CreateWindow("my sdl game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w, h, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		printf("window failed");
		return false;
	}
	screen = SDL_GetWindowSurface(window);
	initOpenGL();
	return true;
}

void initOpenGL() {
	mainContext = SDL_GL_CreateContext(window);
	if (mainContext == nullptr) {
		printf("Failed to get context \n");
	}
	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	glViewport(0, 0, width, height);
	glClearColor(1, 0, 0, 1);

	glGenVertexArrays(1,&vertexArrayID);
	glGenBuffers(1, &bufferID);
	glGenBuffers(1, &colorID);


	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), v, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER,colorID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), c, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);//stride is zero because in different arrays, ie tightly packed

	shVertex.loadShader("shader.vert", GL_VERTEX_SHADER);
	shFragment.loadShader("shader.frag", GL_FRAGMENT_SHADER);

	spMain.createProgram();
	spMain.addShaderToProgram(&shVertex);
	spMain.addShaderToProgram(&shFragment);

	spMain.linkProgram();
	spMain.useProgram();

	printf("initialised openGL");
}
void Close() {
	spMain.deleteProgram();
	shVertex.deleteShader();
	shFragment.deleteShader();
	SDL_GL_DeleteContext(mainContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	SDL_GL_SwapWindow(window);
}