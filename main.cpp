#include "CShader.h"
#include "CShaderProgram.h"
#include "glad\include\glad\glad.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include <SDL.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <ctime>
#include "World.h"
#define GLM_FORCE_RADIANS
#define BUFFER_OFFSET(i) ((void *)(i))
int w = 800;
int h = 600;
int frames = 0;
float x = -1;

std::clock_t start; //return the amount of ticks -> get deltaTicks and devide by ticks/s
double dur;
bool Initialize();
void Close();
void render();
void initOpenGL();

GLuint bufferID, indicesID, colorID, vertexArrayID;

SDL_Window *window = nullptr;
SDL_Surface *screen = nullptr;
CShader shVertex, shFragment;
CShaderProgram spMain;

SDL_GLContext mainContext = nullptr;
GLuint indices[] = {0, 1, 2,999,3,4,5,999,6,7,8};
void resize();
void resize() {
    int width, height;
    std::cout << "resized" << std::endl;
    SDL_GetWindowSize(window, &width, &height);
    w = width;
    h = height;

	int iProjLoc = glGetUniformLocation(spMain.getProgramID(), "proj");
	float fov = 0.45f;
	glm::mat4 mProj =
		glm::perspective(fov, (float)w / (float)h, 0.001f, 1000.0f);

	glUniformMatrix4fv(iProjLoc, 1, GL_FALSE, glm::value_ptr(mProj));
    glViewport(0, 0, w, h);
}

World w1;
int main(int argc, char *args[]) {
	w1.readLevel("test.da");
	start = std::clock();
    if (!Initialize()) {
        return -1;
    }
    bool exit = false;

    SDL_Event e;
    while (!exit) {
        // v[3] = v[3] - 0.001f;
        render();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT ||
                e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                exit = true;
            }
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                resize();
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_F) {
                if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
                    SDL_SetWindowFullscreen(window, 0);
                    resize();
                } else {
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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        printf("something went wrong");
        return false;
    }

    window = SDL_CreateWindow("OPENGL test", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, w, h,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    if (window == nullptr) {
        printf("window failed");
        return false;
    }


	mainContext = SDL_GL_CreateContext(window);
    screen = SDL_GetWindowSurface(window);
	
	
    initOpenGL();
    return true;
}
GLuint vao, vbo, ibuffer;
void initOpenGL() {
    if (mainContext == nullptr) {
        printf("Failed to get context \n");
    }


	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	

	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(999);
    glClearColor(1, 1,1, 1);
    // gen vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // gen vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // copy to vbo
	glBufferData(GL_ARRAY_BUFFER, w1.vboArray.size()*sizeof(float), &w1.vboArray[0] , GL_STATIC_DRAW);
    // setup vertex attrib
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6,
                          (const void *)(sizeof(float) * 3));

    glGenBuffers(1, &ibuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

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
    glm::mat4 mProj =
        glm::perspective(fov, (float)w / (float)h, 0.001f, 1000.0f);
    glm::mat4 mcam = glm::lookAt(glm::vec3(1, 2, 30), glm::vec3(0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 t(0, 0, 0);
    glm::mat4 identity = glm::mat4(1);
    glm::mat4 trns = glm::translate(identity, t);

    // mProj = trns;
    // mcam = trns;

    glUniformMatrix4fv(iProjLoc, 1, GL_FALSE, glm::value_ptr(mProj));
    glUniformMatrix4fv(iCamLoc, 1, GL_FALSE, glm::value_ptr(mcam));
    glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(trns));

    printf("initialised openGL\n");
}
void Close() {
    spMain.deleteProgram();
    shVertex.deleteShader();
    shFragment.deleteShader();
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
float angle = 0;
std::clock_t startT = std::clock();
void render() {

	glClear(GL_COLOR_BUFFER_BIT);
	double deltaT = (std::clock() - startT) / ((double)CLOCKS_PER_SEC);
	angle += (float)deltaT;
	startT = std::clock();
	glm::mat4 rot = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0));
	GLint transLoc = glGetUniformLocation(spMain.getProgramID(), "trans");
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(rot));
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, w1.vboArray.size(), GL_UNSIGNED_INT,
		(void *)0);//3 vertices per triangle
	frames++;

	dur = (std::clock() - start) / ((double)CLOCKS_PER_SEC);
	if (dur > 1) {
		std::cout << "FPS: " << frames << std::endl;
		frames = 0;
		start = std::clock();
	}
    SDL_GL_SwapWindow(window);
}