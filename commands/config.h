#pragma once

namespace Yoka
{
	namespace Commands
	{
		namespace Config
		{
			namespace Helpers
			{
				void ClearMessage(dpp::message& message);
				void SetMainComponents(dpp::message& message, const std::string guildId);
				void SetLoggingComponents(dpp::message& message, const std::string guildId);
			}

			void ButtonClick(dpp::cluster* yoka, const dpp::button_click_t& event);
			void MessageCreate(dpp::cluster* yoka, const dpp::message_create_t& event);

			void Handler(dpp::cluster* yoka, const dpp::slashcommand_t& event);
		}
	}
}