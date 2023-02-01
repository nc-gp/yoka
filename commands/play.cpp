#include "../includes.h"

void Yoka::Commands::Play::Handler(dpp::cluster* yoka, const dpp::slashcommand_t& event)
{
	std::string link = Yoka::Handlers::Command::GetParameterString(event, "title");

	dpp::message playMessage("This command is not avaialble yet");
	playMessage.set_flags(dpp::message_flags::m_ephemeral);

	event.reply(playMessage);
}