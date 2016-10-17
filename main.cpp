#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include "glad\include\glad\glad.h"
#include <vector>
#include "CShader.h"
#include "CShaderProgram.h"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/glm.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#define GLM_FORCE_RADIANS
#define BUFFER_OFFSET(i) ((void*)(i))

int w = 800;
int h = 600;

float x = -1;
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
int indices[] = { 0,1,2 };
void resize();
void resize() {
	int width, height;
	std::cout << "resized" << std::endl;
	SDL_GetWindowSize(window, &width, &height);
	w = width;
	h = height;
	glViewport(0, 0, w, h);
}
float v[] = { -1,0,1,
0,1,1,
1,-1,1
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
		//v[3] = v[3] - 0.001f;
		render();

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || e.type == SDLK_ESCAPE) {
				exit = true;
			}
			if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
				resize();
			}	
			if (e.window.event == SDLK_f) {
				if (SDL_GetWindowFlags(window)& SDL_WINDOW_FULLSCREEN) {
					SDL_SetWindowFullscreen(window, 0);
					resize();
				}
				else {
					SDL_SetWindowFullscreen(window, 1);
					resize();
				}
			}

		}
	}
	Close();
	return 0;
}

bool Initialize() {

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); 
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetSwapInterval(1);
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) < 0) {
		printf("something went wrong");
		return false;
	}

	window = SDL_CreateWindow("my sdl game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w, h, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
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

	glEnable(GL_MULTISAMPLE);
	glViewport(0, 0, width, height);
	glClearColor(0,0,0, 1);

	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &bufferID);
	glGenBuffers(1, &colorID);


	glBindVertexArray(vertexArrayID);

	//color info
	glBindBuffer(GL_ARRAY_BUFFER, colorID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(c), c, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);//stride is zero because in different arrays, ie tightly packed


	shVertex.loadShader("shader.vert", GL_VERTEX_SHADER);
	shFragment.loadShader("shader.frag", GL_FRAGMENT_SHADER);

	spMain.createProgram();
	spMain.addShaderToProgram(&shVertex);
	spMain.addShaderToProgram(&shFragment);

	spMain.linkProgram();
	spMain.useProgram();


	int iCamLoc = glGetUniformLocation(spMain.getProgramID(), "camera");
	int iProjLoc = glGetUniformLocation(spMain.getProgramID(), "proj");
	int trans = glGetUniformLocation(spMain.getProgramID(), "trans");

	float fov = 0.45f;
	glm::mat4 mProj = glm::perspective(fov, (float)w/(float)h, 0.001f, 1000.0f);
	glm::mat4 mcam = glm::lookAt(glm::vec3(0, 1, 10), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 t(0, 0, 0);
	glm::mat4 identity = glm::mat4(1);
	glm::mat4 trns = glm::translate(identity, t);

	//mProj = trns;
	//mcam = trns;

	glUniformMatrix4fv(iProjLoc, 1, GL_FALSE, glm::value_ptr(mProj));
	glUniformMatrix4fv(iCamLoc, 1, GL_FALSE, glm::value_ptr(mcam));
	glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(trns));


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
float time;
void render() {


	GLuint uniformLoc = glGetUniformLocation(spMain.getProgramID(), "time");
	glUniform1f(uniformLoc, time);

	time = (time+0.01f);

	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	SDL_GL_SwapWindow(window);
	//glDeleteBuffers(1, &bufferID);
	//glDeleteBuffers(1, &colorID);
}