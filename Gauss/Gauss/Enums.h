#pragma once

namespace GameEnums
{
	enum control_index { C_UP = 0, C_DOWN, C_LEFT, C_RIGHT, C_FIRE };
	enum texture_index { T_SHIP = 0, T_LASER01, T_MISSILE01 };
	enum main_gun_type {MGT_MAIN_GUN, MGT_MAIN_GUN02, MGT_MAIN_GUN03 };
	enum gun_index { G_LASER = 0, G_MISSILE01, G_MISSILE02 };
	enum laser_level { DEFAULT_LASER = 0, LEVEL_2_LASER, LEVEL_3_LASER };
	enum enemy_type { E_MOVE_LEFT = 0, E_FOLLOW, E_MOVE_LEFT_SHOOT, E_FOLLOW_FAST, E_FOLLOW_SHOOT, E_FOLLOW_FAST_SHOOT};
	enum enemy_bullet_type { EB_DEFAULT = 0, EB_MISSILE };
	enum upgrade_type { UT_DOUBLE_RAY = 0, UT_TRIPLE_RAY, UT_PIERCING_SHOT, UT_SHEILD, UT_HEALTH_TANK };
}
