#pragma once
#include <Components/PointLight.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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