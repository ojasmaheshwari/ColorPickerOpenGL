#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <Logging.h>

class Application {
public: // public methods
	Application();
	~Application();

	void run();

private: // private methods
	void initialize();
	void renderLoop();
	void imguiRender();
	void glfwErrorCallback(int error, const char* description);
	void glfwInitialize();
	void imguiInitialize();

private: // private members
	GLFWwindow* m_Window;
	Logging m_Logger;
};
