#pragma once

using cmdHandler = void(*)(dpp::cluster*, const dpp::slashcommand_t&);

struct cmd {
	std::string description;
	cmdHandler handler;
	std::vector<dpp::command_option> parameters = {};
	dpp::permissions permissions;
};

namespace Yoka
{
	namespace Handlers
	{
		namespace Command
		{
			void InitCommands(dpp::cluster* yoka);
			void InitHandlers(dpp::cluster* yoka, const dpp::slashcommand_t& event);
			std::string GetParameterString(const dpp::slashcommand_t& event, const std::string parameterName);
		}
	}
}