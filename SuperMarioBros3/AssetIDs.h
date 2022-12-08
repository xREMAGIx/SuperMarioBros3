#pragma once

//
// This file contains all IDs of game assets 
// FORMAT regionId(1) miscId(2) ani(2) sprite(2)
//

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20

// CHARACTER
#define OBJECT_TYPE_MARIO	101
#define OBJECT_TYPE_BRICK	1

// MISCS
#define OBJECT_TYPE_COIN 201
#define OBJECT_TYPE_MUSHROOM 203
#define OBJECT_TYPE_GREEN_MUSHROOM 204
#define OBJECT_TYPE_QUESTION_BLOCK 205

// ENEMIES
#define OBJECT_TYPE_GOOMBA	301
#define OBJECT_TYPE_RED_GOOMBA 302
#define OBJECT_TYPE_KOOPA	303
#define OBJECT_TYPE_RED_KOOPA	304

// HUD
#define OBJECT_TYPE_POINT 401
#define OBJECT_TYPE_ITEM_BOX 402
#define OBJECT_TYPE_FONT 403
#define OBJECT_TYPE_GAMEBOARD 404
#define OBJECT_TYPE_PLAYER_FONT	405
#define OBJECT_TYPE_THIRD_FONT 406

//INVISIBLES
#define OBJECT_TYPE_PLATFORM 5
#define OBJECT_TYPE_PORTAL	50

//ID: 0
#pragma region MARIO 

//ANIMATION
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1010100
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1010200

#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1010300
#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1010400

#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1010500
#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1010600

#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1010700
#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1010800

#define ID_ANI_MARIO_SMALL_BRACE_WALK_LEFT 1010900
#define ID_ANI_MARIO_SMALL_BRACE_WALK_RIGHT 1011000

#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1011200
#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1011300

#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1011400
#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1011500

#define ID_ANI_MARIO_SMALL_KICK_LEFT 1011600
#define ID_ANI_MARIO_SMALL_KICK_RIGHT 1011700

#define ID_ANI_MARIO_DIE 1011800

#define ID_ANI_MARIO_SMALL_HOLD_LEFT 1011900
#define ID_ANI_MARIO_SMALL_HOLD_RIGHT 1012000

#define ID_ANI_MARIO_SMALL_CLIMB 1012100

//BIG MARIO
#define ID_ANI_MARIO_IDLE_LEFT 1020100
#define ID_ANI_MARIO_IDLE_RIGHT 1020200

#define ID_ANI_MARIO_WALKING_LEFT 1020300
#define ID_ANI_MARIO_WALKING_RIGHT 1020400

#define ID_ANI_MARIO_RUNNING_LEFT 1020500
#define ID_ANI_MARIO_RUNNING_RIGHT 1020600

#define ID_ANI_MARIO_JUMP_WALK_LEFT 1020700
#define ID_ANI_MARIO_JUMP_WALK_RIGHT 1020800

#define ID_ANI_MARIO_JUMP_RUN_LEFT 1020900
#define ID_ANI_MARIO_JUMP_RUN_RIGHT 1021000

#define ID_ANI_MARIO_BRACE_LEFT 1021100
#define ID_ANI_MARIO_BRACE_RIGHT 1021200

#define ID_ANI_MARIO_SIT_LEFT 1021300
#define ID_ANI_MARIO_SIT_RIGHT 1021400

#define ID_ANI_MARIO_KICK_LEFT 1021500
#define ID_ANI_MARIO_KICK_RIGHT 1021600

#define ID_ANI_MARIO_HOLD_LEFT 1021700
#define ID_ANI_MARIO_HOLD_RIGHT 1021800

#define ID_ANI_MARIO_CLIMB 1021900

//RACCOON
#define ID_ANI_RACCOON_MARIO_IDLE_LEFT 1030100
#define ID_ANI_RACCOON_MARIO_IDLE_RIGHT 1030200

#define ID_ANI_RACCOON_MARIO_WALKING_LEFT 1030300
#define ID_ANI_RACCOON_MARIO_WALKING_RIGHT 1030400

#define ID_ANI_RACCOON_MARIO_RUNNING_LEFT 1030500
#define ID_ANI_RACCOON_MARIO_RUNNING_RIGHT 1030600

#define ID_ANI_RACCOON_MARIO_JUMP_WALK_LEFT 1030700
#define ID_ANI_RACCOON_MARIO_JUMP_WALK_RIGHT 1030800

#define ID_ANI_RACCOON_MARIO_JUMP_SLIDE_LEFT 1030900
#define ID_ANI_RACCOON_MARIO_JUMP_SLIDE_RIGHT 1031000

#define ID_ANI_RACCOON_MARIO_JUMP_RUN_LEFT 1031100
#define ID_ANI_RACCOON_MARIO_JUMP_RUN_RIGHT 1031200

#define ID_ANI_RACCOON_MARIO_JUMP_FLY_LEFT 1031300
#define ID_ANI_RACCOON_MARIO_JUMP_FLY_RIGHT 1031400

#define ID_ANI_RACCOON_MARIO_BRACE_LEFT 1031500
#define ID_ANI_RACCOON_MARIO_BRACE_RIGHT 1031600

#define ID_ANI_RACCOON_MARIO_SIT_LEFT 1031700
#define ID_ANI_RACCOON_MARIO_SIT_RIGHT 1031800

#define ID_ANI_RACCOON_MARIO_KICK_LEFT 1031900
#define ID_ANI_RACCOON_MARIO_KICK_RIGHT 1032000

#define ID_ANI_RACCOON_MARIO_HOLD_LEFT 1032100
#define ID_ANI_RACCOON_MARIO_HOLD_RIGHT 1032200

#define ID_ANI_RACCOON_MARIO_CLIMB 1032300

#define ID_ANI_RACCOON_MARIO_ATTACK_LEFT 1032400
#define ID_ANI_RACCOON_MARIO_ATTACK_RIGHT 1032500

#pragma endregion 

#define ID_SPRITE_BRICK 20000

#define ID_SPRITE_CLOUD 50000
#define ID_SPRITE_CLOUD_BEGIN (ID_SPRITE_CLOUD+1000)
#define ID_SPRITE_CLOUD_MIDDLE (ID_SPRITE_CLOUD+2000)
#define ID_SPRITE_CLOUD_END (ID_SPRITE_CLOUD+3000)

//ID: 2
#pragma region MISC 
//COIN
#define ID_ANI_COIN 2010101

//MUSHROOM
#define ID_ANI_MUSHROOM 2030101

//GREEN MUSHROOM
#define ID_ANI_GREEN_MUSHROOM 2040101

//QUESTION BLOCK
#define ID_ANI_QUESTION_BLOCK_IDLE 2050100
#define ID_ANI_QUESTION_BLOCK_OPENED 2050200

#pragma endregion 



//ID: 3
#pragma region ENEMIES 

//GOOMBA
#define ID_ANI_GOOMBA_WALKING 3010100
#define ID_ANI_GOOMBA_DIE 3010200

//RED GOOMBA
#define ID_ANI_RED_GOOMBA_WALKING 3020100
#define ID_ANI_RED_GOOMBA_DIE 3020200

//GREEN KOOPA
#define ID_ANI_KOOPA_WALKING_LEFT 3030100
#define ID_ANI_KOOPA_WALKING_RIGHT	3030500

#define ID_ANI_KOOPA_SHELL 3030200
#define ID_ANI_KOOPA_RESPAWN 3030300

#define ID_ANI_KOOPA_SHELL_ROLL 3030400

//RED KOOPA
#define ID_ANI_RED_KOOPA_WALKING_LEFT 3040100
#define ID_ANI_RED_KOOPA_WALKING_RIGHT	3040500

#define ID_ANI_RED_KOOPA_SHELL 3040200
#define ID_ANI_RED_KOOPA_RESPAWN 3040300

#define ID_ANI_RED_KOOPA_SHELL_ROLL 3040400

#pragma endregion 

//ID: 4
#pragma region HUD 

//POINT
#define ID_SPRITE_POINT_100 4010001
#define ID_SPRITE_POINT_200 4010002
#define ID_SPRITE_POINT_400 4010003
#define ID_SPRITE_POINT_800 4010004
#define ID_SPRITE_POINT_1000 4010005
#define ID_SPRITE_POINT_2000 4010006
#define ID_SPRITE_POINT_4000 4010007
#define ID_SPRITE_POINT_8000 4010008
#define ID_SPRITE_POINT_UP 4010009

//ITEM BOX
#define ID_SPRITE_ITEM_BOX_EMPTY 4020001
#define ID_SPRITE_ITEM_BOX_MUSHROOM 4020002
#define ID_SPRITE_ITEM_BOX_FLOWER 4020003
#define ID_SPRITE_ITEM_BOX_STAR 4020004

//FONT
#define ID_SPRITE_FONT_0 4030001
#define ID_SPRITE_FONT_1 4030002
#define ID_SPRITE_FONT_2 4030003
#define ID_SPRITE_FONT_3 4030004
#define ID_SPRITE_FONT_4 4030005
#define ID_SPRITE_FONT_5 4030006
#define ID_SPRITE_FONT_6 4030007
#define ID_SPRITE_FONT_7 4030008
#define ID_SPRITE_FONT_8 4030009
#define ID_SPRITE_FONT_9 4030010

#define ID_SPRITE_FONT_PLAY_CHARACTER 4030011
#define ID_SPRITE_FONT_ARROW_WHITE 4030012
#define ID_SPRITE_FONT_P_WHITE 4030013
#define ID_SPRITE_FONT_ARROW_BLACK 4030014
#define ID_SPRITE_FONT_P_BLACK 4030015

//GAME BOARD
#define ID_SPRITE_BOARD_BACKGROUND 4040001
#define ID_SPRITE_BOARD_UI 4040002

//PLAYER FONT
#define ID_SPRITE_PLAYER_FONT_1 4050001
#define ID_SPRITE_PLAYER_FONT_2 4050002

//THIRD FONT
#define ID_ANI_THIRD_FONT 4060100

#pragma endregion 