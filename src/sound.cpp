#include "sound.hpp"
#include "cstdlib"
Music menu_music;
Music gameplay_music;

Sound trans_up;
Sound trans_down;
Sound button_select;
Sound summoning_sfx;
Sound deleting_sfx;

Sound fail;
Sound correct_sfx;

Sound charge;
Sound red_spawn_sfx;

Sound button_click;

void load_music_and_sound(){
	// Para portabilidade, talvez seja necessario converter para wav
	menu_music = LoadMusicStream("assets/music/MenuSong.mp3");
	SetMusicVolume(menu_music, 0.5);
	gameplay_music = LoadMusicStream("assets/music/GameplayMusic.mp3");
	SetMusicVolume(gameplay_music, 0.5);


	trans_up = LoadSound("assets/sounds/trans_up.mp3");				
	SetSoundVolume(trans_up, 0.6);
	trans_down = LoadSound("assets/sounds/trans_down.mp3");			
	SetSoundVolume(trans_down, 0.6);
	button_select = LoadSound("assets/sounds/creatorshome-turn-a-page-336933.mp3");	
	SetSoundVolume(button_select, 0.05);
	summoning_sfx = LoadSound("assets/sounds/fire_spell_fireball_burst_bright_03.wav");
	SetSoundVolume(summoning_sfx, 0.2);
	deleting_sfx = LoadSound("assets/sounds/dark_spell_cast_dark_fire_03.wav");
	SetSoundVolume(deleting_sfx, 0.5);

	fail = LoadSound("assets/sounds/fail.mp3");
	SetSoundVolume(fail, 0.7);
	correct_sfx = LoadSound("assets/sounds/correct.mp3");
	SetSoundVolume(correct_sfx, 0.6);

	charge = LoadSound("assets/sounds/charge.mp3");
	SetSoundVolume(charge, 0.7);

	red_spawn_sfx = LoadSound("assets/sounds/fire_spell_fireball_burst_bright_09.wav");
	SetSoundVolume(red_spawn_sfx, 0.5);

	button_click = LoadSound("assets/sounds/Abstract2.mp3");
	SetSoundVolume(button_click, 0.8);

}

void unload_music_and_sound(){
	UnloadMusicStream(menu_music);
	UnloadMusicStream(gameplay_music);

	UnloadSound(trans_up);
	UnloadSound(trans_down);
	UnloadSound(button_select);
	UnloadSound(summoning_sfx);
	UnloadSound(deleting_sfx);

	UnloadSound(fail);
	UnloadSound(correct_sfx);

	UnloadSound(charge);
	UnloadSound(red_spawn_sfx);

	UnloadSound(button_click);
}

void play_sound(SFX sound_fx){
	float random_pitch = 1 + 0.3 * (rand() % 7) - 3;
	switch(sound_fx){
		case trans_up_s: PlaySound(trans_up); break;
		case trans_down_s: PlaySound(trans_down); break;

		case button_select_s: 
		SetSoundPitch(button_select, random_pitch); 
		PlaySound(button_select); break;
		
		case summoning_s:
		SetSoundPitch(summoning_sfx, random_pitch); 
		PlaySound(summoning_sfx); break;

		case deleting_s:
		SetSoundPitch(deleting_sfx, random_pitch); 
		PlaySound(deleting_sfx); break;

		case correct_s: PlaySound(correct_sfx); break; 
		case fail_s: PlaySound(fail); break;

		case red_spawn_s:
		SetSoundPitch(red_spawn_sfx, random_pitch); 
		PlaySound(red_spawn_sfx); break;

		case button_click_s:
		SetSoundPitch(red_spawn_sfx, random_pitch); 
		PlaySound(button_click); break;
		
		default: break;
	}
}

void play_charge_in_pitch(float pitch){
	if(IsSoundPlaying(charge)) return;
	SetSoundPitch(charge, pitch); 
	PlaySound(charge);
}

void play_music(Music music){
    if(!IsMusicStreamPlaying(music)) {
        PlayMusicStream(music);
    }
    UpdateMusicStream(music);
}

void restart_music(Music music){
	StopMusicStream(music);
    PlayMusicStream(music);
}


void play_music_based_on_screen(Screen old_screen){
	if(old_screen == current_screen){
		switch(current_screen){
		case Screen::GAMEPLAY:
			play_music(gameplay_music);
			break;
		case Screen::START_MENU:
		case Screen::DEFEAT: // TODO: defeat music?
		case Screen::SCORE:
			play_music(menu_music);
			break;
		}
		return;
	}
	// mudou de tela!
	if(old_screen == Screen::GAMEPLAY){
		restart_music(menu_music);
	}
	if(old_screen == Screen::START_MENU){
		restart_music(gameplay_music);
		
	}
}