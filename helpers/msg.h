#pragma once

namespace Yoka
{
	namespace Helpers
	{
		namespace Message
		{
			void CacheMessage(dpp::cluster* yoka, const dpp::message_create_t& event);
			void UpdateCachedMessage(dpp::cluster* yoka, const dpp::message_update_t& event);
			std::string GetRandomMessage(const std::string messageKey);
		}
	}
}