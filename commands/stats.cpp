#include "../includes.h"

void Yoka::Commands::Stats::Handler(dpp::cluster* yoka, const dpp::slashcommand_t& event)
{
    dpp::embed embed = dpp::embed().
        set_color(dpp::colors::magenta).
        set_title("Yoka").
        //set_author("Maxine \"Max\" Caulfield#3408", "", "").
        set_description(Yoka::Helpers::Message::GetRandomMessage("statsDesc")).
        add_field(
            "My executes",
            Yoka::Helpers::Json::ValueToString(Yoka::Json::Statistics, "runs"),
            true
        ).
        add_field(
            "Coffes for me",
            Yoka::Helpers::Json::ValueToString(Yoka::Json::Statistics, "coffes"),
            true
        ).
        set_thumbnail(yoka->me.get_avatar_url()).
        set_footer(dpp::embed_footer().set_text("Command executed by " + event.command.member.get_user()->username).set_icon(event.command.member.get_user()->get_avatar_url())).
        set_timestamp(time(0));

    dpp::message msg(event.command.channel_id, embed);
    msg.set_reference(event.command.message_id);
    event.reply(msg);
}