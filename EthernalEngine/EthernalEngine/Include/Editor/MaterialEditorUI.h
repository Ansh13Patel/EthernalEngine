#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Rendering/Material.h"

namespace EthernalEngine
{
	class MaterialEditorUI 
	{
	public:
		MaterialEditorUI() = default;
		~MaterialEditorUI() = default;

		void ShowMaterialParameters(Material* material);
	};
}