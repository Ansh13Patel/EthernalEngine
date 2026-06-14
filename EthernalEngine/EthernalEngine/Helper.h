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
	};
}