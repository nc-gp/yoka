#include "includes.h"

std::string BOT_TOKEN = "xyz";
const uint64_t intents = dpp::i_default_intents | dpp::i_message_content;

int main()
{
    srand((unsigned int)time(NULL));

    dpp::cluster yoka(BOT_TOKEN, intents);

    yoka.on_log(dpp::utility::cout_logger());

    Yoka::Helpers::Json::Load(&yoka, "dbs", "statistics", Yoka::Json::Statistics);
    Yoka::Helpers::Json::Load(&yoka, "dbs", "messages", Yoka::Json::Messages);
    Yoka::Helpers::Json::Load(&yoka, "dbs", "coffes", Yoka::Json::Coffes);

    Yoka::Helpers::Json::Load(&yoka, "dbs", "config", Yoka::Json::Config);

    yoka.on_slashcommand([&yoka](const dpp::slashcommand_t& event) {
        std::string commandName = event.command.get_command_name();
        dpp::user user = event.command.usr;
        dpp::guild guild = event.command.get_guild();
        dpp::channel channel = event.command.get_channel();

        Yoka::Handlers::Command::InitHandlers(&yoka, event);

        // command log
        // debug purposes only
        yoka.log(dpp::loglevel::ll_info, "user '" + user.username + "' executed command '" + commandName + "' on guild '" + guild.name + "' in channel '" + channel.name + "'");
    });

    yoka.on_message_reaction_add([&yoka](const dpp::message_reaction_add_t& event) {
        // reaction roles
    });

    yoka.on_button_click([&yoka](const dpp::button_click_t& event) {
        Yoka::Commands::Config::ButtonClick(&yoka, event);
    });

    yoka.on_message_create([&yoka](const dpp::message_create_t& event) {
        
        Yoka::Handlers::Message::InitCreateMessages(&yoka, event);

        for (const auto& user_guild_member : event.msg.mentions) {
            const dpp::user& user = user_guild_member.first;
            const dpp::guild_member& guild_member = user_guild_member.second;
            
            if (user.id == yoka.me.id)
            {
                event.reply("?");
                break;
            }
        }
    });

    yoka.on_message_delete([&yoka](const dpp::message_delete_t& event) {
        Yoka::Handlers::Message::InitDeleteMessages(&yoka, event);
    });

    yoka.on_message_update([&yoka](const dpp::message_update_t& event) {
        Yoka::Handlers::Message::InitEditMessages(&yoka, event);
    });

    yoka.on_guild_create([&yoka](const dpp::guild_create_t& event) {
        const std::string guildId = std::to_string(event.created->id);

        if (!Yoka::Json::Config["guild_configs"].contains(guildId)) {
            yoka.log(dpp::loglevel::ll_info, "joining to new guild with id: " + guildId);

            Yoka::Json::Config["guild_configs"][guildId].is_null();
            Yoka::Json::Config["guild_configs"][guildId]["settings"].is_null();
            Yoka::Json::Config["guild_configs"][guildId]["settings"] = Yoka::Json::Config["guild_config_default"];
            Yoka::Json::Config["guild_configs"][guildId]["id"] = guildId;

            Yoka::Helpers::Json::Save(&yoka, "dbs", "config", Yoka::Json::Config);
        }
    });

    yoka.on_guild_delete([&yoka](const dpp::guild_delete_t& event) {
        const std::string guildId = std::to_string(event.deleted->id);

        if (Yoka::Json::Config["guild_configs"].contains(guildId)) {
            yoka.log(dpp::loglevel::ll_info, "leaving the guild with id: " + guildId);

            Yoka::Json::Config["guild_configs"].erase(Yoka::Json::Config["guild_configs"].find(guildId));
            Yoka::Helpers::Json::Save(&yoka, "dbs", "config", Yoka::Json::Config);
        }
    });

    yoka.on_ready([&yoka](const dpp::ready_t& event) {
        Yoka::Json::Statistics["runs"] = (int)Yoka::Json::Statistics["runs"] + 1;

        Yoka::Helpers::Json::Save(&yoka, "dbs", "statistics", Yoka::Json::Statistics);

        yoka.log(dpp::loglevel::ll_info, Yoka::Helpers::Message::GetRandomMessage("onReady"));

        yoka.set_presence({
            dpp::presence_status::ps_online,
            dpp::activity_type::at_watching,
            "Latte coffee"
        });

        Yoka::Handlers::Command::InitCommands(&yoka);
    });

    yoka.start(dpp::st_wait);
}
