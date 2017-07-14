cmake_minimum_required(VERSION 3.0)

project(ebib_demo)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -DPROG_DEBUG")

add_subdirectory ("${CMAKE_SOURCE_DIR}/external")

include_directories(
        "${CMAKE_SOURCE_DIR}/inc"
		"${CMAKE_SOURCE_DIR}/external/glfw-3.1.2/include/"
		"${CMAKE_SOURCE_DIR}/external/glm-0.9.7.1/"
		"${CMAKE_SOURCE_DIR}/external/glew-1.13.0/include/"
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
