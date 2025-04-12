#include <Application.h>
#include <string>

Application::Application()
: m_Logger("Application.cpp")
{
	initialize();
}

Application::~Application() {}

void Application::run() {
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_TrianglePositions), m_TrianglePositions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Position2D), 0);

	std::string vertexShaderSource = loadShaderSourceFromPath("src/shaders/vertex.glsl");
	std::string fragmentShaderSource = loadShaderSourceFromPath("src/shaders/fragment.glsl");

	m_shaderProgram = CreateShader(vertexShaderSource, fragmentShaderSource);
	glUseProgram(m_shaderProgram);

	m_ColorLocation = glGetUniformLocation(m_shaderProgram, "u_Color");

	renderLoop();
}

void Application::initialize() {
	glfwInitialize();
	imguiInitialize();
}

unsigned int Application::CompileShader(GLenum type, const std::string& shaderSource) {
	unsigned int id = glCreateShader(type);
	const char* src = shaderSource.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	return id;
}

unsigned int Application::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

std::string Application::loadShaderSourceFromPath(const std::string& path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		m_Logger.error("Unable to open shader source file");
		return "INVALID_SHADER_DUE_TO_ERROR";
	}

	std::string line;
	std::string source;
	while (std::getline(file, line)) {
		source += line + "\n";
	}

	return source;
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
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
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

	std::string versionStr = "OpenGL Version: " + std::string(glfwGetVersionString());
	m_Logger.info(versionStr);
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

		glUniform4f(m_ColorLocation, m_Color.red, m_Color.green, m_Color.blue, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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

	ImGui::Begin("Color Dialog");
	ImGui::Text("Color Picker");
	ImGui::SliderFloat("Red", &m_Color.red, 0.0f, 1.0f);
	ImGui::SliderFloat("Green", &m_Color.green, 0.0f, 1.0f);
	ImGui::SliderFloat("Blue", &m_Color.blue, 0.0f, 1.0f);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
