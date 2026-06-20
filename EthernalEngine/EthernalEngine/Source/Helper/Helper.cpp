#include "Helper/Helper.h"
#include <windows.h>
#include <commdlg.h>
#include <filesystem>


namespace EthernalEngine
{
	std::string Helper::OpenFileDialog(const char* filter)
	{
		char filename[MAX_PATH] = "";

		OPENFILENAMEA ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn))
		{
			return filename;
		}

		return "";
	}

	std::string Helper::GetFileName(std::string& path)
	{
		std::filesystem::path filepath{ path };
		
		return filepath.stem().string();
	}

	std::string Helper::GetFileExtension(std::string& path)
	{
		std::filesystem::path filepath{ path };

		return filepath.extension().string();
	}
}