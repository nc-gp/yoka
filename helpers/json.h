#pragma once

namespace Yoka
{
	namespace Json
	{
		extern nlohmann::json Statistics;
		extern nlohmann::json Messages;
		extern nlohmann::json Coffes;

		extern nlohmann::json Config;
	}

	namespace Helpers
	{
		namespace Json
		{
			void Load(const dpp::cluster* yoka, const std::string directory, const std::string name, nlohmann::json& json);
			void Save(const dpp::cluster* yoka, const std::string directory, const std::string name, const nlohmann::json json);
			std::string ValueToString(const nlohmann::json json, const std::string keyName);
		}
	}
}