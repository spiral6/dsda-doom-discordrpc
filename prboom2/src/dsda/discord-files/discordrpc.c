#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
#include <Windows.h>
#include <sysinfoapi.h>
#include <time.h>
#else
#include <unistd.h>
#include <string.h>
#endif

#include "lprintf.h"
#include "discord_game_sdk.h"
#include "dsda/discord-files/discordrpc.h"
#include "doomstat.h"
#include "doomdef.h"
#include "dsda/mapinfo.h"
#include "dsda/skill_info.h"

#define DISCORD_REQUIRE(x) assert(x == DiscordResult_Ok)

struct DiscordApplication discordapp;
struct DiscordActivity activity;

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

void UpdateGameLevelDiscord(void) {
    
    lprintf(LO_INFO, "DiscordRPC: Playing %s\n", doomverstr);
    char asset_name[9] = "";
    switch(gamemission){
        case doom:
            strcpy(asset_name,"doom1");
            break;
        case doom2:       
            strcpy(asset_name,"doom2");
            break;
        case pack_tnt:    
            strcpy(asset_name,"tnt");
            break;
        case pack_plut:   
            strcpy(asset_name,"plutonia");
            break;
        case pack_nerve:
            strcpy(asset_name,"nerve");
            break;
    }
    lprintf(LO_INFO, "DiscordRPC: Using asset %s\n", asset_name);
    lprintf(LO_INFO, "DiscordRPC: Playing E%dM%d of %s on difficulty %s\n", gameepisode, gamemap, asset_name, skill_info.name);
    extern dsda_string_t hud_title;

    lprintf(LO_INFO, "DiscordRPC: Playing map %s\n", hud_title.string);
    char *map_name = strstr(hud_title.string, ":");
    map_name += 2;
    if (map_name != NULL){
        lprintf(LO_INFO, "DiscordRPC: Found map title: %s\n", map_name);        
    }

    // lprintf(LO_INFO, "FINISHED: %s\n", dsda_MapLumpName(gameepisode, gamemap));

    
    // struct DiscordActivity activity;
    // memset(&activity, 0, sizeof(activity));
    sprintf(activity.details, "Playing %s on %s", asset_name, skill_info.name);
    sprintf(activity.state, "Map %d: %s", gamemap, map_name);
    #ifdef _WIN32
        time_t ltime;
        time(&ltime);
        // struct tm* timeinfo = gmtime(&ltime); /* Convert to UTC */
        // ltime = mktime(timeinfo); /* Store as unix timestamp */
        activity.timestamps.start = ltime;
    #else
        activity.timestamps.start = (unsigned long) time();
    #endif
    // sprintf(activity.name, "Test", 0);
    strcpy(activity.assets.large_image, asset_name);
    discordapp.activities->update_activity(discordapp.activities, &activity, &discordapp, UpdateActivityCallback);

}