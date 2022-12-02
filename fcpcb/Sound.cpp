#include "Sound.h"
// Sound.cpp
// Runs on TM4C123
// Sound function definitions
// Designed for franks ECE319K PCB
// Created: 11/19/2022 
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/20/22
#include <stdint.h>



//#include "DAC.h"
#include "../flib/Timers.h"
#include "tm4c123gh6pm.h"
#include "../flib/Timers.h"
const uint8_t SinTable5[64]={16, 17, 19, 20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 30, 31, 31, 31, 31, 31, 30, 30, 29, 28, 27, 26, 25, 24, 23, 21, 20, 19, 17, 16, 14, 12, 11, 10, 8, 7, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 14
};
	
extern "C" void EnableInterrupts(void);
extern "C" void DisableInterrupts(void);
extern "C" long StartCritical(void);
extern "C" void EndCritical(long);

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
static void DAC_Init(void){volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x0000002;
  delay = 666;
	GPIO_PORTB_DEN_R |= 0x0000003F;
	GPIO_PORTB_DIR_R |= 0x0000003F;
	GPIO_PORTB_DR8R_R |= 0x0000003F;
	GPIO_PORTB_PCTL_R &= ~(0x00FFFFFF);

}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
static void DAC_Out(uint8_t data){
	GPIO_PORTB_DATA_R = data;
}

SoundData::SoundData(){

}
SoundData::SoundData(const uint8_t* t, uint32_t ts){
  table = t;tableSize = ts;
}

SongData::SongData(){

}
SongData::SongData(const uint32_t* m, uint32_t nn, uint32_t bpm){
  music = m;numNotes = nn;BPM = bpm;
}

uint32_t SoundPart = 0;
uint32_t MusicPart = 0;
uint32_t SoundIdx = 0;
uint32_t MusicIdx = 0;
SoundData_t currentSound;
SongData_t currentSong;
#define PF3 (*((volatile uint32_t *)0x40025020))
#define PF2 (*((volatile uint32_t *)0x40025010))
#define PF1 (*((volatile uint32_t *)0x40025008))
// INTERRUPT SERVICE ROUTINES
void SoundISR(void){
	PF2 |= 0x04;
  if (!currentSound.table){
    TIMER0A_Stop();
	PF2 &= ~0x04;
    return;
  }
	SoundPart = (*(currentSound.table + (SoundIdx++)))>>3;
  DAC_Out((SoundPart) + MusicPart);
  if (SoundIdx >= currentSound.tableSize){
    SoundPart = 0;
    SoundIdx = 0;
    currentSound.table = 0;
  }
	PF2 &= ~0x04;
}

void MusicISR(void){
	MusicPart = SinTable5[MusicIdx++&0x3F];
	DAC_Out(MusicPart + (SoundPart));
}

//pair of utility functions for getting proper elements of song array
static uint32_t noteAt(const uint32_t* song, uint32_t n){
	return song[3*n];
}
static uint32_t beatsAt(const uint32_t* song, uint32_t n){
	return song[(3*n)+1];
}
static uint32_t stacAt(const uint32_t* song, uint32_t n){
	return song[(3*n)+2];
}
//takes num of 8th notes and converts to num of clock cycles based on BPM constant
//assumes time signature is 4/4
static uint32_t beatsToPeriod(uint32_t beats){
	return beats * ((80000000 / (2 * (currentSong.BPM)))*60);
}
//returns given percentage of num of cycles in an 8th note
static uint32_t stacattoTime(uint8_t percent){
	return (((80000000 / (2 * (currentSong.BPM)))*60) * percent) / 100;
}
static uint32_t freqToPeriod(uint32_t f){
	return 80000000 / (f*64);
}

static void Music_Init(void){
	
}

static void Start_Tone(uint32_t f){
  TIMER1A_Init(MusicISR, freqToPeriod(f), 1);
}

static void Stop_Tone(void){
  TIMER1A_Stop();
}

static uint8_t evenodd = 1;
static uint32_t note = 0;
void Music_Control_ISR(void){
	PF3 |= 0x08;
	if (evenodd){
		uint32_t no = noteAt(currentSong.music,note);
		if (no){Start_Tone(no);}else{Stop_Tone();}
		TIMER2_TAILR_R = beatsToPeriod(beatsAt(currentSong.music,note)) - stacattoTime(stacAt(currentSong.music,note));
	} else {
		uint32_t st = stacattoTime(stacAt(currentSong.music,note++));
		note *= (note < currentSong.numNotes);
		TIMER2_TAILR_R = st;
		Stop_Tone();
	}
	evenodd ^= 0x01;
	PF3 &= ~0x08;
}

static void Music_PlaySong(const SongData_t song){	
	note = 0;
	evenodd = 1;
  currentSong = song;
  TIMER2A_Init(Music_Control_ISR, beatsToPeriod(beatsAt(currentSong.music,0)) - stacattoTime(stacAt(currentSong.music,0)),3);

}

static void Music_StopSong(void){
  TIMER2A_Stop();
	Stop_Tone();
	note = 0;
	evenodd=1;
}

DAC::DAC(void){
  DAC_Init();
}

void DAC::playSound(const SoundData_t snd){
  currentSound = snd;
  SoundIdx = 0;
  TIMER0A_Init(SoundISR, 80000000/11000,0);
}

void DAC::playSong(const SongData_t song){
  Music_PlaySong(song);
}


void DAC::stopSong(void){
  Music_StopSong();
}

void DAC::setOctaveDrop(uint32_t o){
  OCTAVE_DROP = o;
}

