#include "Helper.h"
#include <windows.h>
#include <commdlg.h>


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
}