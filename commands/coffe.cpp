#include "../includes.h"

void Yoka::Commands::Coffe::Handler(dpp::cluster* yoka, const dpp::slashcommand_t& event)
{
	const dpp::snowflake userId = std::to_string(event.command.usr.id);

	// check if user already gived coffe to our bot
	if(std::any_of(Yoka::Json::Coffes["from"].begin(), Yoka::Json::Coffes["from"].end(), [&](const auto& item) { return item == userId; }))
	{
		event.reply(Yoka::Helpers::Message::GetRandomMessage("alreadyGivenCoffe"));
		return;
	}

	Yoka::Json::Coffes["from"] += userId;
	Yoka::Json::Statistics["coffes"] = (int)Yoka::Json::Statistics["coffes"] + 1;
    event.reply(Yoka::Helpers::Message::GetRandomMessage("coffeGift"));

	Yoka::Helpers::Json::Save(yoka, "dbs", "statistics", Yoka::Json::Statistics);
	Yoka::Helpers::Json::Save(yoka, "dbs", "coffes", Yoka::Json::Coffes);
}