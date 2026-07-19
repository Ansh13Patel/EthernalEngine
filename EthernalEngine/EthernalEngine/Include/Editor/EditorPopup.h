#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <string>
#include <vector>
#include <functional>

enum PopupType
{
	Warning,
	Error,
	Information,
	Confirmation
};
namespace EthernalEngine
{
	struct Popup
	{
		PopupType type;
		std::string title;
		std::string msg;
		std::function<void()> callback = [](){};
	};
	class EditorPopup
	{
	public:
		EditorPopup() = default;
		~EditorPopup() = default;
		static void ShowPopup(Popup popup);
		static void Render();
	
	private:
		static std::vector<Popup> popupToShow;
		static bool ShowWarning(Popup& popup);
		static bool ShowError(Popup& popup);
		static bool ShowInfo(Popup& popup);
		static bool ShowConfirmation(Popup& popup);
	};
}