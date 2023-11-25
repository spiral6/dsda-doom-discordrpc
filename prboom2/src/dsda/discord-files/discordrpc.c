#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "lprintf.h"
#include "discord_game_sdk.h"
#include "dsda/discord-files/discordrpc.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <string.h>
#endif

#define DISCORD_REQUIRE(x) assert(x == DiscordResult_Ok)

struct DiscordApplication discordapp;

void DISCORD_CALLBACK UpdateActivityCallback(void* data, enum EDiscordResult result)
{
    DISCORD_REQUIRE(result);
}

void INIT_DISCORDRPC(void)
{
    // struct DiscordApplication discordapp;
    memset(&discordapp, 0, sizeof(discordapp));

    struct DiscordCreateParams params;
    params.client_id = 1072974764467032154;
    params.flags = DiscordCreateFlags_Default;
    // params.events = &events;
    params.event_data = &discordapp;

    DISCORD_REQUIRE(DiscordCreate(DISCORD_VERSION, &params, &discordapp.core));
    lprintf(LO_INFO, "Discord RPC enabled!\n");

    struct DiscordActivity activity;
    memset(&activity, 0, sizeof(activity));
    strcpy(activity.details, "testing app details");
    strcpy(activity.state, "testing app state");
    // sprintf(activity.name, "Test", 0);
    strcpy(activity.assets.large_image, "doom2");

    discordapp.activities = discordapp.core->get_activity_manager(discordapp.core);


    // lprintf(LO_INFO, discordapp.activities);
    // lprintf(LO_INFO, &discordapp);

    discordapp.activities->update_activity(discordapp.activities, &activity, &discordapp, UpdateActivityCallback);
    
    // DISCORD_REQUIRE(discordapp.core->run_callbacks(discordapp.core));
}