#include "../includes.h"

void Yoka::Helpers::Message::CacheMessage(dpp::cluster* yoka, const dpp::message_create_t& event)
{
	if (event.msg.author.id == yoka->me.id || event.msg.author.is_bot())
		return;

	dpp::message* msg = new dpp::message();

	*msg = event.msg;

	Yoka::Handlers::Message::CachedMessages.store(msg);
}

void Yoka::Helpers::Message::UpdateCachedMessage(dpp::cluster* yoka, const dpp::message_update_t& event)
{
	if (event.msg.author.id == yoka->me.id || event.msg.author.is_bot())
		return;

	dpp::message* msg = Yoka::Handlers::Message::CachedMessages.find(event.msg.id);

	if (msg != nullptr)
	{
		*msg = event.msg;
		Yoka::Handlers::Message::CachedMessages.store(msg);
	}
}

std::string Yoka::Helpers::Message::GetRandomMessage(const std::string messageKey)
{
	return Yoka::Json::Messages[messageKey][rand() % Yoka::Json::Messages[messageKey].size()];
}