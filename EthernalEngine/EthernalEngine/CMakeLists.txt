cmake_minimum_required(VERSION 3.20)

project(EthernalEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

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

Source/Helper/FileHelper.cpp
Source/Helper/DebugDraw.cpp

Source/Rendering/CubeMesh.cpp
Source/Rendering/Mesh.cpp
Source/Rendering/Model.cpp
Source/Rendering/Renderer.cpp
Source/Rendering/Shader.cpp
Source/Rendering/stb_image.cpp
Source/Rendering/Texture.cpp

Source/Components/DirectionalLight.cpp
Source/Components/PointLight.cpp
Source/Components/SpotLight.cpp

Source/Scene/GameObject.cpp
Source/Scene/Scene.cpp
Source/Scene/Transform.cpp

ThirdParty/glad.c
ThirdParty/imgui.cpp
ThirdParty/imgui_demo.cpp
ThirdParty/imgui_draw.cpp
ThirdParty/imgui_impl_glfw.cpp
ThirdParty/imgui_impl_opengl3.cpp
ThirdParty/imgui_tables.cpp
ThirdParty/imgui_widgets.cpp
ThirdParty/ImGuizmo.cpp
)

add_subdirectory(../Dependencies/Assimp assimp-build)
add_subdirectory(../Dependencies/GLFW/glfw glfw-build)

target_include_directories(
    EthernalEngine
    PRIVATE
    Include
    ../includes
    ../Dependencies/GLAD/include
    ../Dependencies/GLFW/glfw/include
    ../Dependencies/Assimp/include
    ../includes/imgui
    ../includes/imgui/backends
)

target_link_libraries(
    EthernalEngine
    PRIVATE
    glfw
    OpenGL::GL
    assimp
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT EthernalEngine)

if(WIN32)
   add_custom_command(
      TARGET EthernalEngine POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:assimp>
      $<TARGET_FILE_DIR:EthernalEngine>
   )
    add_custom_command(
      TARGET EthernalEngine POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:glfw>
      $<TARGET_FILE_DIR:EthernalEngine>
   )
endif()

add_custom_command(
   TARGET EthernalEngine POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   ${CMAKE_CURRENT_SOURCE_DIR}/Assets
   $<TARGET_FILE_DIR:EthernalEngine>
)