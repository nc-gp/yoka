#include "../includes.h"

void Yoka::Commands::Voice::Handler(dpp::cluster* yoka, const dpp::slashcommand_t& event)
{
    const std::string actionParameter = Yoka::Handlers::Command::GetParameterString(event, "action");
    dpp::guild guild = event.command.get_guild();

    const dpp::voiceconn* currentVoiceChat = event.from->get_voice(guild.id);

    if (actionParameter == "voice_leave") 
    {
        if (currentVoiceChat)
        {
            event.from->disconnect_voice(guild.id);

            dpp::message disconnectMessage(Yoka::Helpers::Message::GetRandomMessage("voiceLeave"));
            disconnectMessage.set_flags(dpp::message_flags::m_ephemeral);

            event.reply(disconnectMessage);
            return;
        }

        dpp::message noVoiceMessage(Yoka::Helpers::Message::GetRandomMessage("noVoice"));
        noVoiceMessage.set_flags(dpp::message_flags::m_ephemeral);

        event.reply(noVoiceMessage);
    }
    else if (actionParameter == "voice_join") 
    {
        if (currentVoiceChat)
        {
            const auto usersOnVoiceChat = guild.voice_members.find(event.command.usr.id);

            if (usersOnVoiceChat != guild.voice_members.end() && currentVoiceChat->channel_id == usersOnVoiceChat->second.channel_id)
            {
                dpp::message voiceAlreadyMessage(Yoka::Helpers::Message::GetRandomMessage("voiceAlready"));
                voiceAlreadyMessage.set_flags(dpp::message_flags::m_ephemeral);

                event.reply(voiceAlreadyMessage);
                return;
            }

            event.from->disconnect_voice(guild.id);
            return;
        }

        if (!guild.connect_member_voice(event.command.usr.id))
        {
            dpp::message notInMessage(Yoka::Helpers::Message::GetRandomMessage("voiceNotIn"));
            notInMessage.set_flags(dpp::message_flags::m_ephemeral);

            event.reply(notInMessage);
            return;
        }

        dpp::message voiceJoinMessage(Yoka::Helpers::Message::GetRandomMessage("voiceJoin"));
        voiceJoinMessage.set_flags(dpp::message_flags::m_ephemeral);

        event.reply(voiceJoinMessage);
    }
}