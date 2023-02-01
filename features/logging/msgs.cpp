#include "../../includes.h"

void Yoka::Features::Logging::DeletedMessage(dpp::cluster* yoka, const dpp::message_delete_t& event)
{
	const std::string guildId = std::to_string(event.deleted->guild_id);

	if (!Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"].contains("messages") ||
		!Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"].contains("channel"))
		return;

	dpp::embed deletedMessageEmbed;

	dpp::message* originalMessage = Yoka::Handlers::Message::CachedMessages.find(event.deleted->id);

	if (originalMessage == nullptr)
		return;

	if (originalMessage->author.id == yoka->me.id || originalMessage->author.is_bot())
		return;

	const std::string deletedContent = originalMessage->content;
	const std::string authorUsername = originalMessage->author.format_username();
	const std::string authorIcon = originalMessage->author.get_avatar_url();

	deletedMessageEmbed.set_title("Message deleted");
	deletedMessageEmbed.set_color(dpp::colors::scarlet_red);

	deletedMessageEmbed.add_field(
		"Content",
		"```" + deletedContent + "```",
		false
	);
	
	deletedMessageEmbed.set_footer(dpp::embed_footer().set_text("Message was created by " + authorUsername).set_icon(authorIcon));
	deletedMessageEmbed.set_timestamp(time(0));

	dpp::message deletedMessageLog(Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"]["channel"], deletedMessageEmbed);
	yoka->message_create(deletedMessageLog);

	Yoka::Handlers::Message::CachedMessages.remove(originalMessage);
}

void Yoka::Features::Logging::EditedMessage(dpp::cluster* yoka, const dpp::message_update_t& event)
{
	if (event.msg.author.id == yoka->me.id || event.msg.author.is_bot())
		return;

	const std::string guildId = std::to_string(event.msg.guild_id);

	if (!Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"].contains("messages") ||
		!Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"].contains("channel"))
		return;

	const dpp::message* originalMessage = Yoka::Handlers::Message::CachedMessages.find(event.msg.id);

	if (originalMessage == nullptr)
		return;

	if (originalMessage->author.id == yoka->me.id || originalMessage->author.is_bot())
		return;

	dpp::embed editedMessageEmbed;

	const std::string oldContent = originalMessage->content;
	const std::string newContent = event.msg.content;

	editedMessageEmbed.set_title("Message edited");
	editedMessageEmbed.set_url("https://discord.com/channels/" + guildId + "/" + std::to_string(event.msg.channel_id) + "/" + std::to_string(event.msg.id));
	editedMessageEmbed.set_color(dpp::colors::moon_yellow);

	editedMessageEmbed.add_field(
		"Original",
		"```" + oldContent + "```",
		false
	);

	editedMessageEmbed.add_field(
		"Edited",
		"```" + newContent + "```",
		false
	);

	editedMessageEmbed.set_footer(dpp::embed_footer().set_text("Message was created by " + event.msg.author.format_username()).set_icon(event.msg.author.get_avatar_url()));
	editedMessageEmbed.set_timestamp(time(0));

	dpp::message editedMessageLog(Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"]["channel"], editedMessageEmbed);
	yoka->message_create(editedMessageLog);
}