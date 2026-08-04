cmake_minimum_required(VERSION 3.20)
include(FetchContent)

project(EthernalEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.5
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(
    nlohmann_json
    imgui
    glfw
    assimp
)

add_executable(EthernalEngine 
Source/main.cpp

Source/Core/Input.cpp
Source/Core/Window.cpp
Source/Core/EngineCamera.cpp

Source/Editor/EditorUI.cpp
Source/Editor/DirectionalLightEditorUI.cpp
Source/Editor/PointLightEditorUI.cpp
Source/Editor/MeshEditorUI.cpp
Source/Editor/SpotLightEditorUI.cpp
Source/Editor/MaterialEditorUI.cpp
Source/Editor/EditorPopup.cpp

Source/Helper/FileHelper.cpp
Source/Helper/DebugDraw.cpp

Source/Rendering/CubeMesh.cpp
Source/Rendering/Mesh.cpp
Source/Rendering/Model.cpp
Source/Rendering/Renderer.cpp
Source/Rendering/Shader.cpp
Source/Rendering/stb_image.cpp
Source/Rendering/Texture.cpp
Source/Rendering/Material.cpp
Source/Rendering/Skybox.cpp
Source/Rendering/CubeMap.cpp
Source/Rendering/FrameBuffer.cpp

Source/Components/DirectionalLight.cpp
Source/Components/PointLight.cpp
Source/Components/SpotLight.cpp
Source/Components/Camera.cpp

Source/Scene/GameObject.cpp
Source/Scene/Scene.cpp
Source/Scene/Transform.cpp

ThirdParty/glad.c
ThirdParty/ImGuizmo.cpp

${imgui_SOURCE_DIR}/imgui.cpp
${imgui_SOURCE_DIR}/imgui_demo.cpp
${imgui_SOURCE_DIR}/imgui_draw.cpp
${imgui_SOURCE_DIR}/imgui_tables.cpp
${imgui_SOURCE_DIR}/imgui_widgets.cpp
${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
 "Source/Editor/MainCameraEditorUI.cpp" "Include/Editor/MainCameraEditorUI.h")

target_include_directories(
    EthernalEngine
    PRIVATE
    Include
    ../includes
    ../Dependencies/GLAD/include
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(
    EthernalEngine
    PRIVATE
    glfw
    OpenGL::GL
    assimp
    nlohmann_json::nlohmann_json
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT EthernalEngine)

add_custom_command(
   TARGET EthernalEngine POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   ${CMAKE_CURRENT_SOURCE_DIR}/Assets
   $<TARGET_FILE_DIR:EthernalEngine>
)