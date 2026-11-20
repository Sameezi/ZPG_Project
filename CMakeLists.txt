cmake_minimum_required(VERSION 3.10)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(LearnOpenGL 
src/main.cpp 
src/core/Application.cpp 
src/core/Scene.cpp 
src/core/Camera.cpp
src/core/Window.cpp
src/graphics/DrawableObject.cpp
src/graphics/Model.cpp
src/graphics/Shader.cpp
src/graphics/ShaderProgram.cpp
src/graphics/InstancedModel.cpp
src/graphics/Texture.cpp
src/graphics/Cubemap.cpp
src/graphics/Skybox.cpp
src/graphics/lights/LightObject.cpp
src/graphics/lights/PointLight.cpp
src/graphics/lights/SpotLight.cpp
src/graphics/lights/DirLight.cpp
src/transformation/Rotation.cpp
src/transformation/Scaling.cpp
src/transformation/Translation.cpp
src/transformation/TransformComposite.cpp
)

file(GLOB SHADER_FILES ${CMAKE_SOURCE_DIR}/shaders/*.vert ${CMAKE_SOURCE_DIR}/shaders/*.frag)
foreach(SHADER ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    configure_file(${SHADER} ${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME} COPYONLY)
endforeach()

target_include_directories(LearnOpenGL PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/../external_headers/
)

target_link_libraries(LearnOpenGL
    PRIVATE
    glfw
    OpenGL::GL
    ${CMAKE_DL_LIBS}
)

