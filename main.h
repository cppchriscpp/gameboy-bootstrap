// This file contains configuration options generated by the game builder. 
// The C code refers to the values defined here to load data and run the game 

#include <gb/gb.h>

// Our graphics, as defined in our map & tile files.
extern UBYTE tiles[];
extern UBYTE map[];
extern UBYTE sprites[];
#define TILES tiles
#define SPRITES sprites
#define MAP map
#define FIRST_SOLID_TILE 1U
#define FIRST_DAMAGE_TILE 64U

// These are the options for types of sprites to use for the main character
#define MAIN_CHARACTER_SPRITE_SINGLE 0
#define MAIN_CHARACTER_SPRITE_DIRECTIONAL 1
#define MAIN_CHARACTER_SPRITE_ANIMATED_DIRECTIONAL 2
// This is the type actually in use in your game
#define MAIN_CHARACTER_SPRITE_TYPE MAIN_CHARACTER_SPRITE_DIRECTIONAL

// This handles how frequently we animate the main character's sprite.
#define PLAYER_ANIM_INTERVAL 0x08U 	// In binary, this is %00000100 - we will animate every 8 tiles as a result
#define PLAYER_ANIM_SHIFT 3U		// This works together with the variable above. We do 3 bit shifts to make %00000100 into %00000001