#pragma once

namespace Yoka
{
	namespace Features
	{
		namespace Logging
		{
			void DeletedMessage(dpp::cluster* yoka, const dpp::message_delete_t& event);
			void EditedMessage(dpp::cluster* yoka, const dpp::message_update_t & event);
		} 
	}
}