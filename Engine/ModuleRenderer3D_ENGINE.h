#pragma once
#include "Engine_Module.h"
#include "Globals_ENGINE.h"
#include "SDL2/SDL.h"
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"

class ModuleRenderer3D_ENGINE : public Engine_Module
{
public:
	ModuleRenderer3D_ENGINE(GameEngine* gEngine, bool start_enabled = true);
	~ModuleRenderer3D_ENGINE();

	bool Init();
	engine_status PreUpdate();
	engine_status Update();
	engine_status PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);

	void DrawGrid(int size, int step, bool xzAxis = true, bool xyAxis = false, bool zyAxis = false);

	void DrawAxis(float lineWidth = 1.0f);

	void SetTargetWindow(SDL_Window* target) { 
		targetWindow = target; 
	}
	void SetScreenSize(int width, int height) { 
		screen_width = width; 
		screen_height = height;
	}
	void SetVsync(bool active) { 
		vsync = active; 
	}

public:

	SDL_GLContext context;
	glm::mat3 NormalMatrix;
	glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

private:

	SDL_Window* targetWindow;
	bool vsync;
	int screen_width;
	int screen_height;

	void drawQuadFaceTriangles(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
		glVertex3fv(&a.x);
		glVertex3fv(&b.x);
		glVertex3fv(&c.x);

		glVertex3fv(&c.x);
		glVertex3fv(&d.x);
		glVertex3fv(&a.x);
	}
	void drawCubeTest() {

		glm::vec3 a = glm::vec3(-1, -1, 1);
		glm::vec3 b = glm::vec3(1, -1, 1);
		glm::vec3 c = glm::vec3(1, 1, 1);
		glm::vec3 d = glm::vec3(-1, 1, 1);
		glm::vec3 e = glm::vec3(-1, -1, -1);
		glm::vec3 f = glm::vec3(1, -1, -1);
		glm::vec3 g = glm::vec3(1, 1, -1);
		glm::vec3 h = glm::vec3(-1, 1, -1);

		glBegin(GL_TRIANGLES);
		//front
		glColor3f(255, 0, 0);
		drawQuadFaceTriangles(a, b, c, d);
		//back
		glColor3f(0, 255, 0);
		drawQuadFaceTriangles(h, g, f, e);
		//left
		glColor3f(0, 0, 255);
		drawQuadFaceTriangles(e, a, d, h);
		//right
		glColor3f(255, 255, 0);
		drawQuadFaceTriangles(b, f, g, c);
		//top
		glColor3f(255, 255, 255);
		drawQuadFaceTriangles(d, c, g, h);
		//bottom
		glColor3f(0, 0, 0);
		drawQuadFaceTriangles(b, a, e, f);
		glEnd();
	}
};