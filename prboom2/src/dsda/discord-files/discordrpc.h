#ifndef __DISCORDRPC__
#define __DISCORDRPC__

#include "discord_game_sdk.h"
#include "dsda/utility.h"

void INIT_DISCORDRPC();
void UpdateGameLevelDiscord(void);
void updateMapName(dsda_string_t* map_title);
struct DiscordApplication {
    struct IDiscordCore* core;
    // struct IDiscordUserManager* users;
    // struct IDiscordAchievementManager* achievements;
    struct IDiscordActivityManager* activities;
    // struct IDiscordRelationshipManager* relationships;
    struct IDiscordApplicationManager* application;
    // struct IDiscordLobbyManager* lobbies;
    DiscordUserId user_id;
};
extern struct DiscordApplication discordapp;

#endif
