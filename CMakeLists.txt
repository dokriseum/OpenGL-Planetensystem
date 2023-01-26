cmake_minimum_required(VERSION 3.10)
project(CGTutorial)
add_executable(${CMAKE_PROJECT_NAME} 
    src/cpp/CGTutorial.cpp
    src/cpp/objects.cpp
    src/cpp/objloader.cpp
    src/cpp/shader.cpp
    src/cpp/texture.cpp
	src/cpp/Applikation.cpp
	src/cpp/Applikation.h
	src/cpp/Start.cpp
	src/cpp/Universumskoerper.h
	src/cpp/Univerumskoerper.cpp
		)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL GLX)
if(NOT OpenGL_FOUND)
	message("OpenGL libraries not found")
endif(NOT OpenGL_FOUND)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)

target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE ${OpenGL_LIBRARIES}
  PRIVATE glfw
  PRIVATE libglew_static
  PRIVATE glm
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/asset.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/asset.hpp
)

target_include_directories(${CMAKE_PROJECT_NAME}
  PRIVATE ${OpenGL_INCLUDE_DIRS}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include
)
