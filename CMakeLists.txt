cmake_minimum_required(VERSION 3.10)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 23)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(LearnOpenGL main.cpp Application.cpp Scene.cpp Shader.cpp DrawableObject.cpp Model.cpp ShaderProgram.cpp Transform.cpp GroupNode.cpp SceneNode.cpp) 

configure_file(${CMAKE_SOURCE_DIR}/shaders/basic.vert ${CMAKE_BINARY_DIR}/shaders/basic.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shaders/basic.frag ${CMAKE_BINARY_DIR}/shaders/basic.frag COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shaders/transformable.vert ${CMAKE_BINARY_DIR}/shaders/transformable.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shaders/transformable_9float.vert ${CMAKE_BINARY_DIR}/shaders/transformable_9float.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shaders/transformable_9float.frag ${CMAKE_BINARY_DIR}/shaders/transformable_9float.frag COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shaders/rotation_Zaxis.vert ${CMAKE_BINARY_DIR}/shaders/rotation_Zaxis.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shaders/rotation_Yaxis.vert ${CMAKE_BINARY_DIR}/shaders/rotation_Yaxis.vert COPYONLY)

target_include_directories(LearnOpenGL PRIVATE . ${CMAKE_SOURCE_DIR}/../external_headers/)
target_link_libraries(LearnOpenGL
    PRIVATE
    glfw
    OpenGL::GL
    ${CMAKE_DL_LIBS}
)

