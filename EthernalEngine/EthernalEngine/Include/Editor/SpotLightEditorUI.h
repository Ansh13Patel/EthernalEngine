#pragma once

#include "Components/SpotLight.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "ImGuizmo.h"

namespace EthernalEngine
{
	class SpotLightEditorUI
	{
	public:
		SpotLightEditorUI() = default;
		~SpotLightEditorUI() = default;

		void ShowSpotLightParameters(SpotLight* spotLight);
	};
}