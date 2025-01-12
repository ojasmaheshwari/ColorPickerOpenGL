cmake_minimum_required(VERSION 3.16)
project(ColorPicker VERSION 0.0.1)

set(SOURCES src/main.cpp src/imgui/imgui.cpp src/imgui/imgui_impl_opengl3.cpp src/imgui/imgui_impl_glfw.cpp src/imgui/imgui_draw.cpp src/imgui/imgui_tables.cpp src/imgui/imgui_widgets.cpp src/imgui/imgui_demo.cpp)
set(INCLUDES src/includes src/imgui)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
add_executable(ColorPicker ${SOURCES})
include_directories(${GLEW_INCLUDE_DIRS})
target_include_directories(ColorPicker PRIVATE ${INCLUDES})
target_link_libraries(ColorPicker glfw)
target_link_libraries(ColorPicker OpenGL::GL)
target_link_libraries(ColorPicker ${GLEW_LIBRARIES})

