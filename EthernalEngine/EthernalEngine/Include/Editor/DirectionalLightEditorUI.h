#pragma once
#include <Components/DirectionalLight.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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