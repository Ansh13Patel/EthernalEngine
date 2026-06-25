#pragma once
#include <Components/PointLight.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "ImGuizmo.h"

namespace EthernalEngine
{
	class PointLightEditorUI
	{
	public:
		PointLightEditorUI() = default;
		~PointLightEditorUI() = default;

		void ShowPointLightParameters(PointLight* pointLight);
	};
}