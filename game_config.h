// This file contains configuration options generated by the game builder. 
// The C code refers to the values defined here to load data and run the game 

#include <gb/gb.h>

// Our graphics, as defined in our map & tile files.
extern UBYTE tiles[];
extern UBYTE map[];
extern UBYTE sprites[];
extern UBYTE map_sprites[];
#define TILES tiles
#define SPRITES sprites
#define MAP map
#define MAP_SPRITES map_sprites

// The game engine depends on tiles being ordered by whether or not they are solid. 
// This tells the engine which tiles are solid, and which ones should cause damage.
#define FIRST_SOLID_TILE 7
#define FIRST_DAMAGE_TILE 21
// These are the options for types of sprites to use for the main character
#define MAIN_CHARACTER_SPRITE_SINGLE 0
#define MAIN_CHARACTER_SPRITE_DIRECTIONAL 1
#define MAIN_CHARACTER_SPRITE_ANIMATED_DIRECTIONAL 2

// This is the type actually in use in your game
#define MAIN_CHARACTER_SPRITE_TYPE MAIN_CHARACTER_SPRITE_ANIMATED_DIRECTIONAL

// This handles how frequently we animate the main character's sprite.
#define PLAYER_ANIM_INTERVAL 0x08U 	// In binary, this is %00000100 - we will animate every 8 tiles as a result
#define PLAYER_ANIM_SHIFT 3U		// This works together with the variable above. We do 3 bit shifts to make %00000100 into %00000001

// Tile ids for various parts of the window (health, numbers, etc)
#define WINDOW_TILE_HEALTH_FULL 84
#define WINDOW_TILE_HEALTH_EMPTY 85
#define WINDOW_TILE_MONEY 86
#define WINDOW_TILE_NUMERIC_0 87

#define LAST_ENEMY_SPRITE 1
#define LAST_ANIMATED_ENEMY_SPRITE 2
#define LAST_DIRECTIONAL_ENEMY_SPRITE 3
#define LAST_ANIMATED_DIRECTIONAL_ENEMY_SPRITE 4
#define LAST_ENDGAME_SPRITE 5
#define LAST_DOOR_SPRITE 7
#define LAST_HEALTH_SPRITE 8
#define LAST_MONEY_SPRITE 9

// Tells us where to start looking for sprites of certain types in the list of sprite ids.
#define WORLD_SPRITE_START 16U
#define ENEMY_SPRITE_START 32

// Okay, this looks complicated, but it really isn't.
// we want to know where endgame sprites start, so we take the enemy start, then add the number of enemy sprites (LAST_ENEMY_SPRITE+1), multiplied by 4, as they all take up 4 subtiles.
#define ANIMATED_ENEMY_SPRITE_START ENEMY_SPRITE_START + (4*NUMBER_OF_ENEMY_SPRITES)
#define DIRECTIONAL_ENEMY_SPRITE_START ANIMATED_ENEMY_SPRITE_START +  (8*NUMBER_OF_ANIMATED_ENEMY_SPRITES)
#define ANIMATED_DIRECTIONAL_ENEMY_SPRITE_START DIRECTIONAL_ENEMY_SPRITE_START  + (16*NUMBER_OF_DIRECTIONAL_ENEMY_SPRITES)
#define ENDGAME_SPRITE_START ANIMATED_DIRECTIONAL_ENEMY_SPRITE_START + (32*(NUMBER_OF_ANIMATED_DIRECTIONAL_ENEMY_SPRITES))
#define DOOR_SPRITE_START ENDGAME_SPRITE_START + (4*NUMBER_OF_ENDGAME_SPRITES)
#define HEALTH_SPRITE_START DOOR_SPRITE_START + (4*NUMBER_OF_DOOR_SPRITES)// Do it again, but with endgame sprites. Same theory, but now we get the difference between two ids to tell how many we have.
#define MONEY_SPRITE_START HEALTH_SPRITE_START + (4*NUMBER_OF_HEALTH_SPRITES) // Same idea again, but health sprites are always 1x1
#define FIRST_8PX_SPRITE (LAST_DOOR_SPRITE + 1U)

#define NUMBER_OF_ENEMY_SPRITES 2
#define NUMBER_OF_ANIMATED_ENEMY_SPRITES 1
#define NUMBER_OF_DIRECTIONAL_ENEMY_SPRITES 1
#define NUMBER_OF_ANIMATED_DIRECTIONAL_ENEMY_SPRITES 1
#define NUMBER_OF_ENDGAME_SPRITES 1
#define NUMBER_OF_DOOR_SPRITES 2
#define NUMBER_OF_HEALTH_SPRITES 1
#define NUMBER_OF_MONEY_SPRITES 1

// Some gameplay-related constants
#define MAX_HEALTH 8U
#define STARTING_HEALTH 5U
#define MAX_MONEY 99U
#define STARTING_MONEY 0U
#define DOOR_COST 2U
