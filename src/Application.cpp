#include <Application.h>

Application::Application()
: m_Logger("Application.cpp")
{
	initialize();
}

Application::~Application() {

}

void Application::run() {
	renderLoop();
}

void Application::initialize() {
	glfwInitialize();
	imguiInitialize();
}

void Application::glfwErrorCallback(int error, const char* description) {
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
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

	m_Window = glfwCreateWindow(640, 480, "Color Picker OpenGL", NULL, NULL);
	if (!m_Window) {
		m_Logger.error("Failed to create glfw window!");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);
	m_Logger.info("OpenGL and GLFW successfully initialized");
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
