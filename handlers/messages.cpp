#include "../includes.h"

dpp::cache<dpp::message> Yoka::Handlers::Message::CachedMessages;

void Yoka::Handlers::Message::InitCreateMessages(dpp::cluster* yoka, const dpp::message_create_t& event)
{
    Yoka::Helpers::Message::CacheMessage(yoka, event);
    Yoka::Commands::Config::MessageCreate(yoka, event);
}

void Yoka::Handlers::Message::InitDeleteMessages(dpp::cluster* yoka, const dpp::message_delete_t& event)
{
    Yoka::Features::Logging::DeletedMessage(yoka, event);
}

void Yoka::Handlers::Message::InitEditMessages(dpp::cluster* yoka, const dpp::message_update_t& event)
{
    Yoka::Features::Logging::EditedMessage(yoka, event);
    Yoka::Helpers::Message::UpdateCachedMessage(yoka, event); // we need to update cache messsage after logging it, because it will be the same as pervious one
}