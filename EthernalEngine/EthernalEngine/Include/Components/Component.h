#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace EthernalEngine
{
	class Component
	{
	public:
		virtual ~Component() = default;
		virtual json SerializeComponent() const = 0;
		virtual void DeserializeComponent(const json& componentJson) = 0;
	};
}