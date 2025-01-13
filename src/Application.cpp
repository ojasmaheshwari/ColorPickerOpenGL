#include <Application.h>
#include <string>

Application::Application()
: m_Logger("Application.cpp")
{
	initialize();
}

Application::~Application() {}

void Application::run() {
	renderLoop();
}

void Application::initialize() {
	glfwInitialize();
	imguiInitialize();
}

void GLAPIENTRY Application::glfwErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	Logging logger("glDebugMessageCallback");
	std::string typeDesc;

	switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			typeDesc = "OPENGL_ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			typeDesc = "OPENGL_DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			typeDesc = "OPENGL_UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			typeDesc = "OPENGL_PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			typeDesc = "OPENGL_PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_OTHER:
			typeDesc = "OPENGL_OTHER";
			break;
		default:
			typeDesc = "OPENGL_UNKNOWN";
	}

	switch (severity){
		case GL_DEBUG_SEVERITY_LOW:
			logger.info(typeDesc + ": " + std::string(message));
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			logger.warn(typeDesc + ": " + std::string(message));
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			logger.error(typeDesc + ": " + std::string(message));
			break;
		default:
			logger.info(typeDesc + ": " + std::string(message));
	}
}

void Application::glfwInitialize() {

	if (!glfwInit()) {
		m_Logger.error("glfwInit() failed!");
		return;
	}

	// Window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_Window = glfwCreateWindow(640, 480, "Color Picker OpenGL", NULL, NULL);
	if (!m_Window) {
		m_Logger.error("Failed to create glfw window!");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);
	if (glewInit() != GLEW_OK) {
		m_Logger.error("glewInit() failed!");
	} else {
		m_Logger.info("OpenGL and GLFW successfully initialized!");
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(glfwErrorCallback, nullptr);
}

void Application::imguiInitialize() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	m_Logger.info("ImGui successfully initialized");
}

void Application::renderLoop() {
	while (!glfwWindowShouldClose(m_Window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		imguiRender();

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

// To be ran in renderLoop()
void Application::imguiRender() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Sample ImGui Window");
	ImGui::Text("Hello there!");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
