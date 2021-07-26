#include <algorithm>
#include "debug.h"

#include "Game.h"
#include "MarioWorld.h"


void CMarioWorld::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	if (state == MARIO_WORLD_STATE_MOVING) {
		if (x == moving_x && y == moving_y) {
			SetState(MARIO_WORLD_STATE_IDLE);
		}
		else {
			x += dx;
			y += dy;

			if (vx > 0) {
				if (x > moving_x) {
					SetPosition(moving_x, y);
				}
			}
			else if (x < moving_x) {
				SetPosition(moving_x, y);
			}

			if (vy > 0) {
				if (y > moving_y) {
					SetPosition(x, moving_y);
				}
			}
			else if (y < moving_y) {
				SetPosition(x, moving_y);
			}
		}
	}
}

void CMarioWorld::Render()
{
	animation_set->at(0)->Render(x, y, nx, 255);
}

void CMarioWorld::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MARIO_SMALL_BBOX_WIDTH;
	bottom = y + MARIO_SMALL_BBOX_HEIGHT;
}

void CMarioWorld::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_WORLD_STATE_MOVING: {
		nx = static_cast<int>((moving_x - x) / abs(moving_x - x));
		ny = static_cast<int>((moving_y - y) / abs(moving_y - y));

		if (moving_x - x != 0) {
			vx = nx*MARIO_WORLD_WALKING_SPEED;
		}
		else {
			vx = 0;
		}
		if (moving_y - y != 0) {
			vy = ny*MARIO_WORLD_WALKING_SPEED;
		}
		else {
			vy = 0;
		}
		break;
	}
	default: {
			vx = 0;
			vy = 0;
			break;
		}
	}
}

void CMarioWorld::MovingPosition(float x, float y)
{
	moving_x = x;
	moving_y = y;
	SetState(MARIO_WORLD_STATE_MOVING);
}