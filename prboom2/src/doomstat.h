/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2006 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *  Copyright 2005, 2006 by
 *  Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *   All the global variables that store the internal state.
 *   Theoretically speaking, the internal state of the engine
 *    should be found by looking at the variables collected
 *    here, and every relevant module will have to include
 *    this header file.
 *   In practice, things are a bit messy.
 *
 *-----------------------------------------------------------------------------*/

#ifndef __D_STATE__
#define __D_STATE__

// We need the playr data structure as well.
#include "d_player.h"
#include "umapinfo.h"

#ifdef __GNUG__
#pragma interface
#endif

// ------------------------
// Command line parameters.
//

extern  dboolean nomonsters; // checkparm of -nomonsters
extern  dboolean respawnparm;  // checkparm of -respawn
extern  dboolean fastparm; // checkparm of -fast
extern  dboolean devparm;  // DEBUG: launched with -devparm

// -----------------------------------------------------
// Game Mode - identify IWAD as shareware, retail etc.
//

extern GameMode_t gamemode;
extern GameMission_t  gamemission;
extern const char *doomverstr;

extern char *MAPNAME(int e, int m);

// Set if homebrew PWAD stuff has been added.
extern  dboolean modifiedgame;

// CPhipps - new compatibility handling
extern complevel_t compatibility_level, default_compatibility_level;

// CPhipps - old compatibility testing flags aliased to new handling
#define compatibility (compatibility_level<=boom_compatibility_compatibility)
#define demo_compatibility (compatibility_level < boom_compatibility_compatibility)
#define mbf_features (compatibility_level>=mbf_compatibility)
#define mbf21 (compatibility_level == mbf21_compatibility)

// v1.1-like pitched sounds
extern int pitched_sounds;        // killough

extern int demo_insurance;      // killough 4/5/98

// -------------------------------------------
// killough 10/98: compatibility vector

enum {
  comp_telefrag,
  comp_dropoff,
  comp_vile,
  comp_pain,
  comp_skull,
  comp_blazing,
  comp_doorlight,
  comp_model,
  comp_god,
  comp_falloff,
  comp_floors,
  comp_skymap,
  comp_pursuit,
  comp_doorstuck,
  comp_staylift,
  comp_zombie,
  comp_stairs,
  comp_infcheat,
  comp_zerotags,
  comp_moveblock,
  comp_respawn,  /* cph - alias of comp_respawnfix from eternity */
  comp_sound,
  comp_666,
  comp_soul,
  comp_maskedanim,

  //e6y
  comp_ouchface,
  comp_maxhealth,
  comp_translucency,

  // mbf21
  comp_ledgeblock,
  comp_friendlyspawn,
  comp_voodooscroller,
  comp_reservedlineflag,

  MBF_COMP_TOTAL = 32  // limit in MBF format
};

enum {
  comperr_zerotag,
  comperr_passuse,
  comperr_hangsolid,
  comperr_blockmap,
  comperr_freeaim,

  COMPERR_NUM
};

extern int comp[MBF_COMP_TOTAL];
extern int /*comperr[COMPERR_NUM], */default_comperr[COMPERR_NUM];

// -------------------------------------------
// Language.
extern  Language_t   language;

// -------------------------------------------
// Selected skill type, map etc.
//

// Defaults for menu, methinks.
extern  skill_t   startskill;
extern  int             startepisode;
extern  int   startmap;

extern  dboolean   autostart;

// Selected by user.
extern  skill_t         gameskill;
extern  int   gameepisode;
extern  int   gamemap;

// Nightmare mode flag, single player.
extern  dboolean         respawnmonsters;

// Netgame? Only true if >1 player.
extern  dboolean netgame;

// Flag: true only if started as net deathmatch.
// An enum might handle altdeath/cooperative better.
extern  dboolean deathmatch;

extern int solo_net;
extern dboolean coop_spawns;

extern dboolean randomclass;

// ------------------------------------------
// Internal parameters for sound rendering.
// These have been taken from the DOS version,
//  but are not (yet) supported with Linux
//  (e.g. no sound volume adjustment with menu.

// These are not used, but should be (menu).
// From m_menu.c:
//  Sound FX volume has default, 0 - 15
//  Music volume has default, 0 - 15
// These are multiplied by 8.
extern int snd_SfxVolume;      // maximum volume for sound
extern int snd_MusicVolume;    // maximum volume for music

// CPhipps - screen parameters
extern int desired_screenwidth, desired_screenheight;

// -------------------------
// Status flags for refresh.
//

enum automapmode_e {
  am_active = 1,  // currently shown
  am_overlay= 2,  // covers the screen, i.e. not overlay mode
  am_rotate = 4,  // rotates to the player facing direction
  am_follow = 8,  // keep the player centred
  am_grid   =16,  // show grid
};
extern enum automapmode_e automapmode; // Mode that the automap is in

enum menuactive_e {
  mnact_inactive, // no menu
  mnact_float, // doom-style large font menu, doesn't overlap anything
  mnact_full, // boom-style small font menu, may overlap status bar
};
extern enum menuactive_e menuactive; // Type of menu overlaid, if any

extern  dboolean paused;        // Game Pause?
extern  dboolean nodrawers;
extern  dboolean noblit;

#define PAUSE_COMMAND  1
#define PAUSE_PLAYBACK 2

#define interpolate_view (!paused && movement_smooth)
#define paused_via_menu (!demoplayback && menuactive && !netgame)
#define paused_during_playback (paused & PAUSE_PLAYBACK)
#define paused_outside_demo (paused_during_playback || paused_via_menu)
#define paused_camera (paused && !walkcamera.type)

// This one is related to the 3-screen display mode.
// ANG90 = left side, ANG270 = right
extern  int viewangleoffset;
extern  int viewpitchoffset;

// Player taking events, and displaying.
extern  int consoleplayer;
extern  int displayplayer;

// -------------------------------------
// Scores, rating.
// Statistics on a given map, for intermission.
//
extern  int totalkills, totallive;
extern  int totalitems;
extern  int totalsecret;
extern  int show_alive;

// Timer, for scores.
extern  int basetic;    /* killough 9/29/98: levelstarttic, adjusted */
extern  int leveltime;  // tics in game play for par

// --------------------------------------
// DEMO playback/recording related stuff.

extern  dboolean usergame;
extern  dboolean demoplayback;
extern  dboolean demorecording;
extern  int demover;

// Quit after playing a demo from cmdline.
extern  dboolean   singledemo;
// Print timing information after quitting.  killough
extern  dboolean   timingdemo;
// Run tick clock at fastest speed possible while playing demo.  killough
extern  dboolean   fastdemo;

extern  gamestate_t  gamestate;

//-----------------------------
// Internal parameters, fixed.
// These are set by the engine, and not changed
//  according to user inputs. Partly load from
//  WAD, partly set at startup time.

extern  int   gametic;

//e6y
extern  dboolean realframe;

// Bookkeeping on players - state.
extern  player_t  players[MAX_MAXPLAYERS];
extern  int       upmove;

// Alive? Disconnected?
extern  dboolean   playeringame[MAX_MAXPLAYERS];

extern pclass_t PlayerClass[MAX_MAXPLAYERS];

extern  mapthing_t *deathmatchstarts;     // killough
extern  size_t     num_deathmatchstarts; // killough

extern  mapthing_t *deathmatch_p;

// Player spawn spots.
#define MAX_PLAYER_STARTS 8
extern  mapthing_t playerstarts[MAX_PLAYER_STARTS][MAX_MAXPLAYERS];

// Intermission stats.
// Parameters for world map / intermission.
extern wbstartstruct_t wminfo;

//-----------------------------------------
// Internal parameters, used for engine.
//

// File handling stuff.
extern  FILE   *debugfile;

// if true, load all graphics at level load
extern  dboolean precache;

// wipegamestate can be set to -1
//  to force a wipe on the next draw
extern  gamestate_t     wipegamestate;

extern  int             mouseSensitivity_horiz; // killough
extern  int             mouseSensitivity_vert;

// debug flag to cancel adaptiveness
extern  dboolean         singletics;

extern  int             bodyqueslot;

// Needed to store the number of the dummy sky flat.
// Used for rendering, as well as tracking projectiles etc.

extern int    skyflatnum;

extern  int        maketic;

// Networking and tick handling related.
#define BACKUPTICS              12

extern  ticcmd_t   netcmds[][BACKUPTICS];
extern  int        ticdup;

//-----------------------------------------------------------------------------

extern int allow_pushers;         // MT_PUSH Things    // phares 3/10/98

extern int variable_friction;  // ice & mud            // phares 3/10/98

extern int monsters_remember;                          // killough 3/1/98

extern int weapon_recoil;          // weapon recoil    // phares

extern int player_bobbing;  // whether player bobs or not   // phares 2/25/98

extern int dogs;     // killough 7/19/98: Marine's best friend :)
extern int dog_jumping;   // killough 10/98

/* killough 8/8/98: distance friendly monsters tend to stay from player */
extern int distfriend;

/* killough 9/8/98: whether monsters are allowed to strafe or retreat */
extern int monster_backing;

/* killough 9/9/98: whether monsters intelligently avoid hazards */
extern int monster_avoid_hazards;

/* killough 10/98: whether monsters are affected by friction */
extern int monster_friction;

/* killough 9/9/98: whether monsters help friends */
extern int help_friends;

extern int flashing_hom; // killough 10/98

/* killough 7/19/98: whether monsters should fight against each other */
extern int monster_infighting;

extern int monkeys;

extern int HelperThing;          // type of thing to use for helper

extern dboolean forceOldBsp;

#endif
