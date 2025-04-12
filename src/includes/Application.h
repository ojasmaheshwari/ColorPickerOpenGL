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
#include <fstream>
#include <string>

struct Color {
	float red;
	float green;
	float blue;
	float alpha;

	Color() : red(0.0f), green(0.0f), blue(0.0f), alpha(0.0f) {}
	Color(const float red, const float green, const float blue, const float alpha) : red(red), green(green), blue(blue), alpha(alpha) {}
};

struct Position2D {
	float x;
	float y;

	Position2D() : x(0.0f), y(0.0f) {}
	Position2D(const float x, const float y) : x(x), y(y) {}
};


class Application {
public: // public methods
	Application();
	~Application();

	void run();

private: // private methods
	void initialize();
	void renderLoop();
	void imguiRender();
	static void GLAPIENTRY glfwErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	void glfwInitialize();
	void imguiInitialize();
	unsigned int CompileShader(GLenum type, const std::string& shaderSource);
	unsigned int CreateShader(const std::string& vertexShader, const std::string &fragmentShader);
	std::string loadShaderSourceFromPath(const std::string& path);

private: // private members
	GLFWwindow* m_Window;
	Logging m_Logger;
	Color m_Color;
	Position2D m_TrianglePositions[3] = {
		{-0.5f, -0.5f},
		{0.0f, 0.5f},
		{0.5f, -0.5f}
	};
	GLuint m_shaderProgram;
	int m_ColorLocation;
};
