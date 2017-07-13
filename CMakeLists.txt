cmake_minimum_required(VERSION 3.0)

project(ebib_demo)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -DPROG_DEBUG")

include_directories(
        "${CMAKE_SOURCE_DIR}/inc"
		"${CMAKE_SOURCE_DIR}/external/glfw/include/"
		"${CMAKE_SOURCE_DIR}/external/glm/"
		"${CMAKE_SOURCE_DIR}/external/glew/include/"
		)
		
set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
)

add_subdirectory("${CMAKE_SOURCE_DIR}/inc")
add_subdirectory("${CMAKE_SOURCE_DIR}/src")
add_subdirectory("${CMAKE_SOURCE_DIR}/resources")


add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_INCLUDE_LIST} ${PROJECT_SOURCE_LIST} ${PROJECT_SHADER_LIST})
target_link_libraries(${CMAKE_PROJECT_NAME} ${ALL_LIBS})
