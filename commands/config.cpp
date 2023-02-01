#include "../includes.h"

void Yoka::Commands::Config::Helpers::ClearMessage(dpp::message& message)
{
	message.set_content("");
	message.components.clear();
	message.embeds.clear();
}

void Yoka::Commands::Config::Helpers::SetMainComponents(dpp::message& message, const std::string guildId)
{
	dpp::embed messageEmbed;

	ClearMessage(message);

	messageEmbed.set_color(dpp::colors::lime);
	messageEmbed.set_description(Yoka::Helpers::Message::GetRandomMessage("configBegin"));

	message.add_component(
		dpp::component().
		add_component(
			dpp::component().set_label("Save & Exit").
			set_style(dpp::cos_secondary).
			set_id("config_save_" + guildId)
		).
		add_component(
			dpp::component().set_label("Logging").
			set_style(dpp::cos_primary).
			set_id("config_logging_" + guildId)
		)
	);

	message.add_embed(messageEmbed);
}

void Yoka::Commands::Config::Helpers::SetLoggingComponents(dpp::message& message, const std::string guildId)
{
	dpp::embed messageEmbed;
	std::string channel;

	bool messagesSettings = Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"].contains("messages");

	if (Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"].contains("channel"))
	{
		channel = Yoka::Helpers::Json::ValueToString(Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"], "channel");
		channel.erase(0, 1); // erase the first character
		channel.erase(channel.size() - 1); // erase the last character
		channel = "<#" + channel + ">";
	}
	else
		channel = "None";

	ClearMessage(message);

	messageEmbed.set_color(dpp::colors::lime);
	messageEmbed.set_description(Yoka::Helpers::Message::GetRandomMessage("configLogging"));

	messageEmbed.add_field(
		"Channel",
		channel,
		true
	);

	messageEmbed.add_field(
		"Log edited/deleted messages",
		messagesSettings ? "Yes" : "No",
		true
	);

	message.add_component(
		dpp::component().
		add_component(
			dpp::component().set_label("Back").
			set_style(dpp::cos_secondary).
			set_id("config_main_" + guildId)
		).
		add_component(
			dpp::component().set_label("Set channel").
			set_style(dpp::cos_primary).
			set_id("config_setchannel_" + guildId)
		)
	);

	dpp::component_style buttonStyle = messagesSettings ? dpp::component_style::cos_success : dpp::component_style::cos_danger;

	message.add_component(
		dpp::component().
		add_component(
			dpp::component().set_label("Messages").
			set_style(buttonStyle).
			set_id("config_logging_messages_" + guildId)
		)
	);

	message.add_embed(messageEmbed);
}

void Yoka::Commands::Config::ButtonClick(dpp::cluster* yoka, const dpp::button_click_t& event)
{
	const std::string guildId = std::to_string(event.command.guild_id);

	if (Yoka::Json::Config["guild_configs"][guildId].contains("configurationStart"))
	{
		if (Yoka::Json::Config["guild_configs"][guildId]["authorId"] != event.command.usr.id)
		{
			dpp::message configMessage(Yoka::Helpers::Message::GetRandomMessage("configCant"));
			configMessage.set_flags(dpp::message_flags::m_ephemeral);

			event.reply(configMessage);
			return;
		}

		if (event.command.message_id != Yoka::Json::Config["guild_configs"][guildId]["messageId"])
			return;

		if (event.custom_id == "config_main_" + guildId)
		{
			dpp::message editedMsg = event.command.msg;
			
			Yoka::Commands::Config::Helpers::SetMainComponents(editedMsg, guildId);

			yoka->message_edit(editedMsg);
		}

		if (event.custom_id == "config_logging_messages_" + guildId)
		{
			dpp::message editedMsg = event.command.msg;

			bool messagesSettings = Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"].contains("messages");
			messagesSettings = !messagesSettings;

			if (messagesSettings)
				Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"]["messages"] = true;
			else
				Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"].erase(Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"].find("messages"));

			Yoka::Commands::Config::Helpers::SetLoggingComponents(editedMsg, guildId);

			yoka->message_edit(editedMsg);
		}

		if (event.custom_id == "config_logging_" + guildId)
		{
			dpp::message editedMsg = event.command.msg;

			Yoka::Commands::Config::Helpers::SetLoggingComponents(editedMsg, guildId);

			yoka->message_edit(editedMsg);
		}

		if (event.custom_id == "config_setchannel_" + guildId)
		{
			dpp::message editedMsg = event.command.msg;
			dpp::embed embedMsg;

			Yoka::Json::Config["guild_configs"][guildId]["tempChannel"] = true;

			embedMsg.set_description("You need to mention a channel now and send a message, i'll handle the rest.");
			embedMsg.set_color(dpp::colors::lime);

			Yoka::Commands::Config::Helpers::ClearMessage(editedMsg);

			editedMsg.add_embed(embedMsg);

			yoka->message_edit(editedMsg);
		}

		if (event.custom_id == "config_save_" + guildId)
		{
			dpp::message editedMsg = event.command.msg;
			dpp::embed embedMsg;

			embedMsg.set_color(dpp::colors::lime);
			embedMsg.set_description(Yoka::Helpers::Message::GetRandomMessage("configSaved"));

			Yoka::Commands::Config::Helpers::ClearMessage(editedMsg);

			editedMsg.add_embed(embedMsg);

			yoka->message_edit(editedMsg);

			Yoka::Json::Config["guild_configs"][guildId].erase(Yoka::Json::Config["guild_configs"][guildId].find("messageId"));
			Yoka::Json::Config["guild_configs"][guildId].erase(Yoka::Json::Config["guild_configs"][guildId].find("authorId"));
			Yoka::Json::Config["guild_configs"][guildId].erase(Yoka::Json::Config["guild_configs"][guildId].find("channelId"));
			Yoka::Json::Config["guild_configs"][guildId].erase(Yoka::Json::Config["guild_configs"][guildId].find("configurationStart"));
			Yoka::Helpers::Json::Save(yoka, "dbs", "config", Yoka::Json::Config);
		}

		event.reply();
	}
}

void Yoka::Commands::Config::MessageCreate(dpp::cluster* yoka, const dpp::message_create_t& event)
{
	const std::string guildId = std::to_string(event.msg.guild_id);

	if (Yoka::Json::Config["guild_configs"][guildId].contains("configurationStart"))
	{
		if (Yoka::Json::Config["guild_configs"][guildId].contains("tempChannel"))
		{
			if (Yoka::Json::Config["guild_configs"][guildId]["authorId"] != event.msg.author.id)
				return;

			if (Yoka::Json::Config["guild_configs"][guildId]["channelId"] != event.msg.channel_id)
				return;

			for (const auto& channels : yoka->channels_get_sync(guildId)) {
				if (channels.second.is_category()) continue;
				if (channels.second.is_voice_channel()) continue;

				if (event.msg.content.find("<#" + std::to_string(channels.second.id) + ">") not_eq -1) {
					yoka->message_delete_sync(event.msg.id, Yoka::Json::Config["guild_configs"][guildId]["channelId"]);

					Yoka::Json::Config["guild_configs"][guildId]["settings"]["logging"]["channel"] = channels.second.id;

					dpp::message editedMsg = yoka->message_get_sync(Yoka::Json::Config["guild_configs"][guildId]["messageId"], Yoka::Json::Config["guild_configs"][guildId]["channelId"]);

					Yoka::Commands::Config::Helpers::SetLoggingComponents(editedMsg, guildId);

					yoka->message_edit(editedMsg);

					Yoka::Json::Config["guild_configs"][guildId].erase(Yoka::Json::Config["guild_configs"][guildId].find("tempChannel"));

					break;
				}
			}
		}

		if (!Yoka::Json::Config["guild_configs"][guildId].contains("messageId"))
			Yoka::Json::Config["guild_configs"][guildId]["messageId"] = event.msg.id;
	}
}

void Yoka::Commands::Config::Handler(dpp::cluster* yoka, const dpp::slashcommand_t& event)
{
	const std::string guildId = std::to_string(event.command.guild_id);

	if (Yoka::Json::Config["guild_configs"][guildId].contains("configurationStart"))
	{
		dpp::message configMessage(Yoka::Helpers::Message::GetRandomMessage("configRunning"));

		configMessage.set_flags(dpp::message_flags::m_ephemeral);

		event.reply(configMessage);
		return;
	}

	Yoka::Json::Config["guild_configs"][guildId]["configurationStart"] = true;
	Yoka::Json::Config["guild_configs"][guildId]["authorId"] = event.command.usr.id;
	Yoka::Json::Config["guild_configs"][guildId]["channelId"] = event.command.channel_id;

	dpp::message configMsg;

	Yoka::Commands::Config::Helpers::SetMainComponents(configMsg, guildId);

	event.reply(configMsg);
}