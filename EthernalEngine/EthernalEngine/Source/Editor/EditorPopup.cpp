#include "IconsFontAwesome7.h"

#include <Editor/EditorPopup.h>
#include <algorithm>

namespace EthernalEngine
{
	std::vector<Popup> EditorPopup::popupToShow;

	void EditorPopup::ShowPopup(Popup popup)
	{
		popupToShow.push_back(popup);
		ImGui::OpenPopup(popup.title.c_str());
	}

	bool EditorPopup::ShowConfirmation(Popup& popup)
	{
		bool isClosed = false;
		if (ImGui::BeginPopupModal(popup.title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "%s Confirmation", ICON_FA_CIRCLE_CHECK);

			ImGui::Text(popup.msg.c_str());
			ImGui::Separator();

			if (ImGui::Button("Yes",ImVec2(120,0)))
			{
				popup.callback();
				ImGui::CloseCurrentPopup();
				isClosed = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				isClosed = true;
			}
			ImGui::EndPopup();
		}

		return isClosed;
	}

	bool EditorPopup::ShowWarning(Popup& popup)
	{
		bool isClosed = false;
		if (ImGui::BeginPopupModal(popup.title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%s Warning", ICON_FA_TRIANGLE_EXCLAMATION);
			ImGui::Text(popup.msg.c_str());
			ImGui::Separator();

			if (ImGui::Button("Ok", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				isClosed = true;
			}
			
			ImGui::EndPopup();
		}

		return isClosed;
	}

	bool EditorPopup::ShowError(Popup& popup)
	{
		bool isClosed = false;
		if (ImGui::BeginPopupModal(popup.title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s Error", ICON_FA_CIRCLE_XMARK);

			ImGui::Text(popup.msg.c_str());
			ImGui::Separator();

			if (ImGui::Button("Close", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				isClosed = true;
			}

			ImGui::EndPopup();
		}
		return isClosed;
	}

	bool EditorPopup::ShowInfo(Popup& popup)
	{
		bool isClosed = false;
		if (ImGui::BeginPopupModal(popup.title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::TextColored(ImVec4(0.2f, 0.7f, 1.0f, 1.0f), "%s Information", ICON_FA_CIRCLE_INFO);

			ImGui::Text(popup.msg.c_str());
			ImGui::Separator();

			if (ImGui::Button("Ok", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				isClosed = true;
			}

			ImGui::EndPopup();
		}
		return isClosed;
	}

	void EditorPopup::Render()
	{
        if (popupToShow.empty()) return;

		auto& popup = popupToShow.front();

		ImGui::OpenPopup(popup.title.c_str());
		bool closed = false;

		switch (popup.type)
		{
		case PopupType::Confirmation: closed = ShowConfirmation(popup); break;
		case PopupType::Error:        closed = ShowError(popup);        break;
		case PopupType::Warning:      closed = ShowWarning(popup);      break;
		case PopupType::Information:  closed = ShowInfo(popup);         break;
		}

		if (closed)
		{
			popupToShow.erase(popupToShow.begin());

			if (!popupToShow.empty())
			{
				ImGui::OpenPopup(popupToShow.front().title.c_str());
			}
		}
	}
}