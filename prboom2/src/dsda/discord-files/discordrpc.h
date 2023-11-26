#ifndef __DISCORDRPC__
#define __DISCORDRPC__

#include "discord_game_sdk.h"

void INIT_DISCORDRPC();
void UpdateGameLevelDiscord(void);
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
