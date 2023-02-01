#pragma once

namespace Yoka
{
	namespace Handlers
	{
		namespace Message
		{
			extern dpp::cache<dpp::message> CachedMessages;

			void InitCreateMessages(dpp::cluster* yoka, const dpp::message_create_t& event);
			void InitDeleteMessages(dpp::cluster* yoka, const dpp::message_delete_t& event);
			void InitEditMessages(dpp::cluster* yoka, const dpp::message_update_t& event);
		}
	}
}