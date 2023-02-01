#include "../includes.h"

void Yoka::Commands::Ping::Handler(dpp::cluster* yoka, const dpp::slashcommand_t& event)
{
	float websocketPing = yoka->get_shard(0)->websocket_ping;
	float ping = (yoka->rest_ping + websocketPing) * 1000;

	dpp::message msg("Pong! `" + std::to_string(ping) + "ms`");

	msg.set_flags(dpp::message_flags::m_ephemeral);

	event.reply(msg);
}