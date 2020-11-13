#include "music_driver.h"
#include "music_metadata.h"
#include "pwm.h"
#include "timer.h"

#define F_CPU 84000000


#define MII_THEME_TEMPO 114
#define MARIO_TEMPO     200


static void music_set_tone(int freq){
    pwm_set_frequency(freq, CHANNEL_PIN44);
    pwm_set_duty_cycle(0.5, CHANNEL_PIN44);
}


static void music_buzz(int freq, int duration){
    music_set_tone(freq);
    _delay_ms(duration);
}


static void music_play_song(int* song, int song_size, int tempo, float pause_fraction) {
    // duration of a whole note in milliseconds
    int wholenote = 240000 / tempo;

    int size = 0;
    int divider = 0;
    int note_duration = 0;
    int note_pause = 0;
    
    for(int current_note = 0; current_note < song_size; current_note += 2){
        divider = song[current_note + 1];
        if (divider < 0) { 
            // quarter note
            note_duration = -1.5 * wholenote / divider;
        }
        else {
            // regular note
            note_duration = wholenote / divider;
        }
        music_buzz(song[current_note], (1 - pause_fraction)*note_duration);

        note_pause = pause_fraction*note_duration;
        music_buzz(0, note_pause);
    }
}


void music_play(SONG song){
    switch(song){
        case MII_THEME:
        {
            int size = sizeof(mii_theme_notes)/sizeof(int);
            music_play_song(mii_theme_notes, size, MII_THEME_TEMPO, 0.1);
            break;
        }

        case MARIO:
        {
            int size = sizeof(super_mario_notes)/sizeof(int);
            music_play_song(super_mario_notes, size, MARIO_TEMPO, 0.1);
            break;
        }

        case SIMPSON:
        {
            break;
        }

        default:
        {
            break;
        }
    }
    music_buzz(0,0);
}


