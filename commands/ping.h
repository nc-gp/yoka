#pragma once

namespace Yoka
{
	namespace Commands
	{
		namespace Ping
		{
			void Handler(dpp::cluster* yoka, const dpp::slashcommand_t& event);
		}
	}
}