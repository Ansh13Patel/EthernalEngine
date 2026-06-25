#pragma once
#include <Components/DirectionalLight.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "ImGuizmo.h"

namespace EthernalEngine
{
	class DirectionalLightEditorUI
	{
	public:
		DirectionalLightEditorUI() = default;
		~DirectionalLightEditorUI() = default;

		void ShowDirectionalLightParameters(DirectionalLight* dirLight);
	};
}