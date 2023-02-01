#include "../includes.h"

std::map<std::string, cmd> commands = {
    { "ping",   { "Responses with Pong!",                                    Yoka::Commands::Ping::Handler  } },
    { "stats",  { "Shows current Yoka stats.",                               Yoka::Commands::Stats::Handler } },
    { "coffe",  { "Gift a one-off Yoka coffee",                              Yoka::Commands::Coffe::Handler } },
    { "voice",  { "Yoka tries to join a voice channel on action specified.", Yoka::Commands::Voice::Handler, {
        { 
            dpp::command_option(dpp::co_string, "action", "Yoka should join or leave?", true).
            add_choice(dpp::command_option_choice("join", std::string("voice_join"))).
            add_choice(dpp::command_option_choice("leave", std::string("voice_leave")))
        }
    } } },
    { "play",   { "Plays specified youtube video.",                          Yoka::Commands::Play::Handler, {
        {
            dpp::command_option(dpp::co_string, "title", "Youtube link", true)
        }
    } } },
    { "config", { "Configures a settings for current guild of bot.",         Yoka::Commands::Config::Handler } }
};

void Yoka::Handlers::Command::InitCommands(dpp::cluster* yoka)
{
    if (dpp::run_once<struct bulk_register>()) {
        std::vector<dpp::slashcommand> slashCommands;

        for (auto& cmd : commands) {
            dpp::slashcommand command;

            command.set_name(cmd.first).
                set_description(cmd.second.description).
                set_application_id(yoka->me.id);

            command.options = cmd.second.parameters;

            slashCommands.push_back(command);
        }

        yoka->global_bulk_command_create(slashCommands);
    }
}

void Yoka::Handlers::Command::InitHandlers(dpp::cluster* yoka, const dpp::slashcommand_t& event)
{
    dpp::command_interaction cmd_data = event.command.get_command_interaction();

    auto command_iter = commands.find(cmd_data.name);
    if (command_iter != commands.end()) {
        command_iter->second.handler(yoka, event);
    }
}

std::string Yoka::Handlers::Command::GetParameterString(const dpp::slashcommand_t& event, const std::string parameterName)
{
    return std::get<std::string>(event.get_parameter(parameterName));
}