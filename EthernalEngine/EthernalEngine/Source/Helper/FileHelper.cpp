#include "Helper/FileHelper.h"
#include <windows.h>
#include <commdlg.h>
#include <filesystem>
#include <fstream>

namespace EthernalEngine
{
	std::string FileHelper::OpenFilePick(const char* filter)
	{
		char filepath[MAX_PATH] = "";

		OPENFILENAMEA ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = filepath;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn))
		{
			return filepath;
		}

		return "";
	}

	std::string FileHelper::OpenFileSave(const char* filter, std::string sceneData)
	{
		char filepath[MAX_PATH] = "";
		OPENFILENAMEA ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = filepath;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
		ofn.lpstrDefExt = "ethernal";
		if (GetSaveFileNameA(&ofn))
		{
			std::ofstream newScene(filepath);
			if (newScene)
			{
				newScene << sceneData;
				newScene.close();

				return filepath;
			}
		}
		return "";
	}

	std::string FileHelper::GetFileName(std::string& path)
	{
		std::filesystem::path filepath{ path };
		
		return filepath.stem().string();
	}

	std::string FileHelper::GetFileExtension(std::string& path)
	{
		std::filesystem::path filepath{ path };

		return filepath.extension().string();
	}
}