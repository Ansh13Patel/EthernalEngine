#pragma once

#include <string>

namespace EthernalEngine
{
	class Helper
	{
	public:
		Helper() = default;
		~Helper() = default;
		static std::string OpenFileDialog(const char* filter);
		static std::string GetFileName(std::string& path);
		static std::string GetFileExtension(std::string& path);
	};
}