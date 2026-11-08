#pragma once
#include "raylib.h"
#include "global.hpp"

enum SFX {
	trans_up_s,
	trans_down_s,
	button_select_s,
	summoning_s,
	deleting_s,
	correct_s,
	fail_s,
	charge_s,
	red_spawn_s,
	button_click_s,
};


void restart_music(Music music);

void play_music(Music music);
void load_music_and_sound();

void unload_music_and_sound();

void play_music_based_on_screen(Screen old_screen);

void play_sound(SFX sound_fx);

void play_charge_in_pitch(float pitch);