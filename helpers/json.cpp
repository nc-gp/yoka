#include "../includes.h"

nlohmann::json Yoka::Json::Statistics;
nlohmann::json Yoka::Json::Messages;
nlohmann::json Yoka::Json::Coffes;

nlohmann::json Yoka::Json::Config;

void Yoka::Helpers::Json::Load(const dpp::cluster* yoka, const std::string directory, const std::string name, nlohmann::json& json)
{
	std::string fullDirectory = directory + "/" + name + ".json";
	std::ifstream temp(fullDirectory);

	yoka->log(dpp::loglevel::ll_info, "Loading \"" + fullDirectory + "\"...");
	json = nlohmann::json::parse(temp);
}

void Yoka::Helpers::Json::Save(const dpp::cluster* yoka, const std::string directory, const std::string name, const nlohmann::json json)
{
	std::string fullDirectory = directory + "/" + name + ".json";
	std::ofstream temp(directory + "/" + name + ".json");

	yoka->log(dpp::loglevel::ll_info, "Saving \"" + fullDirectory + "\"...");
	temp << std::setw(4) << json << std::endl;
}

std::string Yoka::Helpers::Json::ValueToString(const nlohmann::json json, const std::string keyName)
{
	return nlohmann::to_string(json[keyName]);
}