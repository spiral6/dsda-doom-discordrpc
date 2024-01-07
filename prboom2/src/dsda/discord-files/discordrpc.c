#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <sqlite3.h>

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
#include "w_wad.h"
#include "dsda/mapinfo/u.h"
#include "mapnames.h"

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
    strcpy(activity.details, "Menu");
    // strcpy(activity.state, "testing app state");
    // sprintf(activity.name, "Test", 0);
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
    lprintf(LO_INFO, "DiscordRPC: Playing %s\n", doomverstr);
    lprintf(LO_INFO, "DiscordRPC: Using asset %s\n", asset_name);
    strcpy(activity.assets.large_image, asset_name);
    // strcpy(activity.assets.large_image, "https://raw.githubusercontent.com/spiral6/dsda-doom-discordrpc/discordrpc/prboom2/src/dsda/discord-files/assets/sigil2.png");
    // strcpy(activity.assets.large_image, "https://doomwiki.org/w/images/d/d1/NERF_titlepic.png");

    discordapp.activities = discordapp.core->get_activity_manager(discordapp.core);


    // lprintf(LO_INFO, discordapp.activities);
    // lprintf(LO_INFO, &discordapp);

    discordapp.activities->update_activity(discordapp.activities, &activity, &discordapp, UpdateActivityCallback);
    
    // DISCORD_REQUIRE(discordapp.core->run_callbacks(discordapp.core));
}

void UpdateGameLevelDiscord(void) {

    dsda_string_t discord_map_title;
    dsda_InitString(&discord_map_title, NULL);
    updateMapName(&discord_map_title);

    lprintf(LO_INFO, "DiscordRPC: Playing map %s\n", discord_map_title.string);
    // sqlite3_libversion();
    char *map_name = strstr(discord_map_title.string, ":");
    map_name += 2;
    if (map_name != NULL){
        lprintf(LO_INFO, "DiscordRPC: Found map title: %s\n", map_name);        
    }

    sprintf(activity.details, "Playing %s on %s", doomverstr, skill_info.name);
    if (gamemission == doom) {
        sprintf(activity.state, "%s", discord_map_title.string);
    } else {
        sprintf(activity.state, "Map %d: %s", gamemap, map_name);
    }
    #ifdef _WIN32
        time_t ltime;
        time(&ltime);
        // struct tm* timeinfo = gmtime(&ltime); /* Convert to UTC */
        // ltime = mktime(timeinfo); /* Store as unix timestamp */
        activity.timestamps.start = ltime;
    #else
        activity.timestamps.start = (unsigned long) time();
    #endif
    discordapp.activities->update_activity(discordapp.activities, &activity, &discordapp, UpdateActivityCallback);

}

void updateMapName(dsda_string_t* map_title) {

    extern dsda_string_t hud_title;

    if (modifiedgame) {
        dsda_StringCat(map_title, hud_title.string);
    } else {
        if (dsda_UHUTitle(map_title))
        return;

        switch(gamemode) {
            case shareware:
            case registered:
            case retail:
            // Chex.exe always uses the episode 1 level title
            // eg. E2M1 gives the title for E1M1
            if (gamemission == chex && gamemap < 10)
                dsda_StringCat(map_title, discordrpc_doom1_mapnames[gamemap - 1]);
            else if (gameepisode < 6 && gamemap < 10)
                dsda_StringCat(map_title, discordrpc_doom1_mapnames[(gameepisode - 1) * 9 + gamemap - 1]);
            break;

            default:  // Ty 08/27/98 - modified to check mission for TNT/Plutonia
            if (gamemission == pack_tnt && gamemap < 33)
                dsda_StringCat(map_title, discordrpc_tnt_mapnames[gamemap - 1]);
            else if (gamemission == pack_plut && gamemap < 33)
                dsda_StringCat(map_title, discordrpc_plutonia_mapnames[gamemap - 1]);
            else if (gamemap < 34)
                dsda_StringCat(map_title, discordrpc_doom2_mapnames[gamemap - 1]);
            break;
        }

        if (!map_title->string)
            dsda_StringCat(map_title, VANILLA_MAP_LUMP_NAME(gameepisode, gamemap));
    }

    

}