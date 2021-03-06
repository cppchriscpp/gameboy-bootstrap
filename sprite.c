// Sprite functions are broken out here.
// Important note: This runs in a non-primary bank! 

#include <gb/gb.h>
#include <rand.h>
#include "variables.h"
#include "sprite.h"
#include "main.h"
#include "game_config.h"
#include "constants.h"

void clear_sprites_from_temp2() {
	while (temp2 != MAX_SPRITES) {
		// Fill in the rest -- both in actual sprites and in our structs.
		for (i = 0U; i < 4U; i++)
			move_sprite(WORLD_SPRITE_START + (temp2 << 2U) + i, SPRITE_OFFSCREEN, SPRITE_OFFSCREEN);
		
		mapSprites[temp2].type = SPRITE_TYPE_NONE;
		mapSprites[temp2].x = mapSprites[temp2].y = SPRITE_OFFSCREEN;
		mapSprites[temp2].size = 0U;
		temp2++;
	}
}

void test_sprite_collision() {
	UBYTE spriteWidth, spriteHeight;
	for (i = 0U; i < MAX_SPRITES; i++) {
		spriteWidth = mapSprites[i].size;
		spriteHeight = mapSprites[i].size;
		// Offset from center, used for mini sprites.
		temp1 = 0;

		if (playerX < mapSprites[i].x + spriteWidth && playerX + SPRITE_WIDTH > mapSprites[i].x && 
				playerY < mapSprites[i].y + spriteHeight && playerY + SPRITE_HEIGHT > mapSprites[i].y) {
			
			if (mapSprites[i].type <= LAST_ANIMATED_DIRECTIONAL_ENEMY_SPRITE) {
				if (playerHealth < 2) {
					gameState = GAME_STATE_GAME_OVER;
					return;
				}

				if (!playerInvulnTime) {
					playerHealth--;
					update_health();
					playerVelocityLock = DAMAGE_COLLISION_LOCK_TIME;
					playerInvulnTime = DAMAGE_COLLISION_INVULN_TIME;
					if (playerXVel == 0 && playerYVel == 0) {
						playerYVel = PLAYER_MOVE_DISTANCE;
					} else {
						playerYVel = 0U-playerYVel;
						playerXVel = 0U-playerXVel;
					}
				}
				return;
			} else if (mapSprites[i].type <= LAST_ENDGAME_SPRITE) {
				gameState = GAME_STATE_WINNER;
			} else if (mapSprites[i].type <= LAST_DOOR_SPRITE) {
				if (playerMoney >= DOOR_COST) {
					playerMoney -= DOOR_COST;
					mapSprites[i].type = SPRITE_TYPE_NONE;
					for(j = 0; j != 4; j++) {
						move_sprite(WORLD_SPRITE_START + (i << 2U) + j, SPRITE_OFFSCREEN, SPRITE_OFFSCREEN);
					}
					worldState[playerWorldPos] |= 1U << i;
					update_money();
				} else {
					playerVelocityLock = 1;
					if (playerXVel == 0 && playerYVel == 0) {
						// Should never happen!
						playerYVel = PLAYER_MOVE_DISTANCE;
					} else {
						playerYVel = 0U-playerYVel;
						playerXVel = 0U-playerXVel;
					}
				}
			} else if (mapSprites[i].type <= LAST_HEALTH_SPRITE) {
				if (playerHealth < MAX_HEALTH)
					playerHealth++;
				
				mapSprites[i].type = SPRITE_TYPE_NONE;
				move_sprite(WORLD_SPRITE_START + (i << 2U), SPRITE_OFFSCREEN, SPRITE_OFFSCREEN);
				worldState[playerWorldPos] |= 1U << i;

				update_health();
			} else if (mapSprites[i].type <= LAST_MONEY_SPRITE) {
				if (playerMoney < MAX_MONEY) 
					playerMoney++;
				mapSprites[i].type = SPRITE_TYPE_NONE;
				move_sprite(WORLD_SPRITE_START + (i << 2U), SPRITE_OFFSCREEN, SPRITE_OFFSCREEN);
				worldState[playerWorldPos] |= 1U << i;

				update_money();
			}
			
		}
	}
}

void move_sprites() {
	temp1 = cycleCounter % MAX_SPRITES;
	if (mapSprites[temp1].type == SPRITE_TYPE_NONE || mapSprites[temp1].type > LAST_ANIMATED_DIRECTIONAL_ENEMY_SPRITE)
		return;

	directionalize_sprites();
		
	// Now, we test collision with our temp4 and temp5
	if (mapSprites[temp1].direction == SPRITE_DIRECTION_STOP)
		return;

	// mapSprites[temp1].size is our sprite width.
	if (mapSprites[temp1].direction == SPRITE_DIRECTION_LEFT || mapSprites[temp1].direction == SPRITE_DIRECTION_RIGHT) {
		if (temp4+mapSprites[temp1].size >= SCREEN_WIDTH || temp4 <= 4U) {
			temp4 = mapSprites[temp1].x;
		} else {
			if (mapSprites[temp1].direction == SPRITE_DIRECTION_RIGHT) {
				if (test_collision(temp4+mapSprites[temp1].size, temp5) || test_collision(temp4 + mapSprites[temp1].size, temp5+mapSprites[temp1].size)) {
					temp4 = mapSprites[temp1].x;
				}
			} else {
				if (test_collision(temp4-1U, temp5) || test_collision(temp4-1U, temp5+mapSprites[temp1].size)) {
					temp4 = mapSprites[temp1].x;
				}
			}
		}
	}

	if (mapSprites[temp1].direction == SPRITE_DIRECTION_UP || mapSprites[temp1].direction == SPRITE_DIRECTION_DOWN) {
		if (temp5+mapSprites[temp1].size >= SCREEN_HEIGHT || temp5 <= 4U) {
			temp5 = mapSprites[temp1].y;
		} else {
			if (mapSprites[temp1].direction == SPRITE_DIRECTION_DOWN) {
				if (test_collision(temp4, temp5+mapSprites[temp1].size) || test_collision(temp4+mapSprites[temp1].size, temp5 + mapSprites[temp1].size)) {
					temp5 = mapSprites[temp1].y;
				}
			} else {
				if (test_collision(temp4, temp5) || test_collision(temp4 + mapSprites[temp1].size, temp5)) {
					temp5 = mapSprites[temp1].y;
				}
			}
		}
	}

	// Okay, you can move.
	move_enemy_sprite();

}

void move_enemy_sprite() {
	mapSprites[temp1].x = temp4;
	mapSprites[temp1].y = temp5;
	
	if (NUMBER_OF_ENEMY_SPRITES != 0 && mapSprites[temp1].type <= LAST_ENEMY_SPRITE) {
		temp6 = ENEMY_SPRITE_START + (mapSprites[temp1].type << 2U);
	} else if (NUMBER_OF_ANIMATED_ENEMY_SPRITES != 0 && mapSprites[temp1].type <= LAST_ANIMATED_ENEMY_SPRITE) {
		temp6 = mapSprites[temp1].type - LAST_ENEMY_SPRITE - 1;
		// Special case: If there were no enemy sprites, this will become -1 == 255. 
		// The "correct" way to do this would be to multiply each NUMBER_OF_SPRITES by the space it takes up, however this is much more efficient.
		if (temp6 == 255U)
			temp6 = 0;
		temp6 = ANIMATED_ENEMY_SPRITE_START + (temp6 << 3U);
		temp6 += ((cycleCounter >> 4U) % 2) << 2U;
	} else if (NUMBER_OF_DIRECTIONAL_ENEMY_SPRITES != 0 && mapSprites[temp1].type <= LAST_DIRECTIONAL_ENEMY_SPRITE) {
		temp6 = mapSprites[temp1].type - LAST_ANIMATED_ENEMY_SPRITE - 1;
		// Special case: If there were no enemy sprites before this, this would become -1 == 255. 
		// The "correct" way to do this would be to multiply each NUMBER_OF_SPRITES by the space it takes up, however this is much more efficient.
		if (temp6 == 255U)
			temp6 = 0;
		temp6 = DIRECTIONAL_ENEMY_SPRITE_START + (temp6 << 4U);
		if (mapSprites[temp1].direction != SPRITE_DIRECTION_STOP)
			temp6 += (mapSprites[temp1].direction - 1) << 3U;
	} else if (NUMBER_OF_ANIMATED_DIRECTIONAL_ENEMY_SPRITES != 0 && mapSprites[temp1].type <= LAST_ANIMATED_DIRECTIONAL_ENEMY_SPRITE) { // directional and animated.
		
		temp6 = mapSprites[temp1].type - LAST_DIRECTIONAL_ENEMY_SPRITE - 1U;
		// Special case: If there were no enemy sprites before this, this would become -1 == 255. 
		// The "correct" way to do this would be to multiply each NUMBER_OF_SPRITES by the space it takes up, however this is much more efficient.
		if (temp6 == 255U)
			temp6 = 0;

		
		// You might be asking yourself, why on EARTH would you EVER bit shift by 4, then immediately by 1 instead of shifting by 5.
		// The answer: our C compiler is, let's say buggy, and a variable = to 1 << 5U == 64, whereas var << 4U << 1U == 32U. 
		// It makes my head hurt too...
		temp6 = ANIMATED_DIRECTIONAL_ENEMY_SPRITE_START + (temp6 << 4U << 1U);
		temp6 += ((cycleCounter >> 4U) % 2) << 2U;
		if (mapSprites[temp1].direction != SPRITE_DIRECTION_STOP)
			temp6 += (mapSprites[temp1].direction - 1) << 3U;
	}
	for (i = 0; i != 4U; i++) {
		set_sprite_tile(WORLD_SPRITE_START + (temp1 << 2U) + i, temp6 + i);
		move_sprite(WORLD_SPRITE_START + (temp1 << 2U) + i, temp4 + ((i%2) << 3), temp5 + ((i/2) << 3));
	}

}

void directionalize_sprites() {
	// Kind of bizarre, but it gives us a good variation.
	if (cycleCounter % 60U < MAX_SPRITES) {
		temp3 = rand() & 0x31;
		if (temp3 > SPRITE_DIRECTION_DOWN) {
			if (temp3 < 9) {
				temp3 = SPRITE_DIRECTION_STOP;
			} else if (temp3 < 21) {
				temp4 = playerX + (mapSprites[temp1].size/2U);
				temp5 = mapSprites[temp1].x + (mapSprites[temp1].size/2U);
				if (temp3 % 2 == 0)
					temp3 = SPRITE_DIRECTION_LEFT;
				else
					temp3 = SPRITE_DIRECTION_RIGHT;
			} else {
				temp4 = playerY + (mapSprites[temp1].size/2U);
				temp5 = mapSprites[temp1].y + (mapSprites[temp1].size/2U);
				if (temp3 % 2 == 0)
					temp3 = SPRITE_DIRECTION_UP;
				else
					temp3 = SPRITE_DIRECTION_DOWN;

			}
		}
		mapSprites[temp1].direction = temp3;
	}
	
	temp4 = mapSprites[temp1].x;
	temp5 = mapSprites[temp1].y;
	switch (mapSprites[temp1].direction) {
		case SPRITE_DIRECTION_LEFT: 
			temp4 -= SPRITE_SPEED;
			break;
		case SPRITE_DIRECTION_RIGHT:
			temp4 += SPRITE_SPEED;
			break;
		case SPRITE_DIRECTION_UP:
			temp5 -= SPRITE_SPEED;
			break;
		case SPRITE_DIRECTION_DOWN:
			temp5 += SPRITE_SPEED;
			break;
	}
	
}

void load_sprite() {
	// Apply it to the 2x2 big sprites (encompasses both enemy sprites and the endgame sprites)
	mapSprites[temp2].size = 16U;
	
	// Temp1 is our position.. convert to x/y
	mapSprites[temp2].x = ((temp1 % MAP_TILES_ACROSS) << 4U) + 8U; // add 8 to deal with offset by 1.
	mapSprites[temp2].y = ((temp1 / MAP_TILES_ACROSS) << 4U) + 16U; // Add 16 so the first tile = 16

	if (mapSprites[temp2].type <= LAST_ANIMATED_DIRECTIONAL_ENEMY_SPRITE) {
		temp3 = mapSprites[temp2].type - LAST_DIRECTIONAL_ENEMY_SPRITE - 1;
		
		// Lots of funky variable reassignment, because...
		temp4 = mapSprites[temp2].x;
		temp5 = mapSprites[temp2].y;
		temp1 = temp2;
		// Rather than do it ourselves... let's just draw the sprite as if we'd moved it.
		move_enemy_sprite();

	} else if (mapSprites[temp2].type <= LAST_DOOR_SPRITE) {
		temp3 = mapSprites[temp2].type - LAST_ANIMATED_DIRECTIONAL_ENEMY_SPRITE - 1;

		for (i = 0; i != 4U; i++) {
			set_sprite_tile(WORLD_SPRITE_START + (temp2 << 2U) + i, ENDGAME_SPRITE_START + (temp3 << 2U) + i);
			move_sprite(WORLD_SPRITE_START + (temp2 << 2U) + i, mapSprites[temp2].x + ((i%2U) << 3U), mapSprites[temp2].y + ((i/2U) << 3U));
		}
	} else if (mapSprites[temp2].type <= LAST_MONEY_SPRITE) { // And also apply it to the rest of our smaller sprites - health, and money.
		mapSprites[temp2].size = 8U;
		
		// Temp1 is our position.. convert to x/y
		mapSprites[temp2].x += 4U; // Center based on 8x8 sprite rather than 16x16
		mapSprites[temp2].y += 4U;
		
		// Put our sprite on the map!
		set_sprite_tile(WORLD_SPRITE_START + (temp2 << 2U), HEALTH_SPRITE_START + (mapSprites[temp2].type - FIRST_8PX_SPRITE));
		move_sprite(WORLD_SPRITE_START + (temp2 << 2U), mapSprites[temp2].x, mapSprites[temp2].y);
		for (i = 1; i != 4U; i++) {
			move_sprite(WORLD_SPRITE_START + (temp2 << 2U) + i, SPRITE_OFFSCREEN, SPRITE_OFFSCREEN);
		}
	}

}