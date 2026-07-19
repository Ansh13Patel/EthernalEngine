#pragma once

#include <string>

namespace EthernalEngine
{
	class FileHelper
	{
	public:
		FileHelper() = default;
		~FileHelper() = default;
		static std::string OpenFilePick(const char* filter);
		static std::string OpenFileSave(const char* filter, std::string sceneData);
		static std::string GetFileName(std::string& path);
		static std::string GetFileExtension(std::string& path);
	};
}