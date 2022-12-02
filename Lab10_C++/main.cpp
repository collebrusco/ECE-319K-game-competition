// main.cpp
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10 in C++
// Frank Collebrusco Fall 2022
// All of the provided starter project has been replaced by Frank C
// But I'll leave these disclaimers just in case

// Last Modified: 1/17/2020 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2017

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2017

 Copyright 2018 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../fcpcb/ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "Images.h"
#include "../flib/Timers.h"
#include "../fcpcb/FranksPCB.h"
#include "../flib/SysTick.h"
#include "Animation.h"
#include "../flib/fmath.h"
#include "../inc/StringConversionCpp.h"
#include "TExaS.h"

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" long StartCritical(void);
extern "C" void EndCritical(long);
//TEMP

#define PF3 (*((volatile uint32_t *)0x40025020))
#define PF2 (*((volatile uint32_t *)0x40025010))
#define PF1 (*((volatile uint32_t *)0x40025008))

#define UT_ORANGE 0x037F
#define FIFTY_GREY 0x8410

#define AREA_YMAX 140
#define AREA_YMIN 0
#define AREA_XMAX 128
#define AREA_XMIN 0
int32_t debug;
bool flag = false;
uint32_t clock;

// ********************************ANIMATIONS*******************************
// define arrays of ptrs to images that will be cycled through
// sprites w/ no animation still use an animation object, 
// just with a single element.
const unsigned short* const cowboyFrontRunAni[2] = {cowboy_right, cowboy_left};
const unsigned short* const cowboyRearRunAni[2] = {cowboy_left_rear, cowboy_right_rear};
const unsigned short* const cowboyFrontAni[1] = {cowboy_base};
const unsigned short* const cowboyRearAni[1] = {cowboy_base_rear};
const unsigned short* const coinani[1] = {coin};
const unsigned short* const cowboyRightRollAni[4] = {cowboy_Rroll1, cowboy_Rroll2, cowboy_Rroll3, cowboy_Lroll1};
const unsigned short* const cowboyLeftRollAni[4] = {cowboy_Lroll1, cowboy_Lroll2, cowboy_Lroll3, cowboy_Rroll1};
const unsigned short* const keyAni[1] = {key};
const unsigned short* const doorAni[2] = {door1, door2};
const unsigned short* const aggieFrontAni[1] = {aggie};
const unsigned short* const aggieBossAni[1] = {aggie_boss};
const unsigned short* const hAggieFrontAni[1] = {helmet_aggie};
const unsigned short* const bevoAni[2] = {bevo_cage, bevo_pixel};

// the objects hold a ptr to the array and num elements
Animation CowboyFrontRunAnimation(cowboyFrontRunAni,2);
Animation CowboyRearRunAnimation(cowboyRearRunAni,2);
Animation CowboyFrontAnimation(cowboyFrontAni,1);
Animation CowboyRearAnimation(cowboyRearAni,1);
Animation CowboyRightRollAni(cowboyRightRollAni,4);
Animation CowboyLeftRollAni(cowboyLeftRollAni,4);
Animation CoinAni(coinani,1);
Animation KeyAni(keyAni,1);
Animation DoorAni(doorAni, 2);
Animation AggieFrontAnimation(aggieFrontAni,1);
Animation HAggieFrontAnimation(hAggieFrontAni,1);
Animation BevoAnimation(bevoAni, 2);
Animation AggieBossAnimation(aggieBossAni,1);

//finally arrays of animations for different sprites are defined.
//currently only player has more than one animation
Animation PlayerAnimations[6] = {CowboyFrontAnimation, CowboyRearAnimation,
                                 CowboyFrontRunAnimation, CowboyRearRunAnimation,
                                 CowboyRightRollAni, CowboyLeftRollAni};

Animation AggieAnimations[1] = {AggieFrontAnimation};
Animation HAggieAnimations[1] = {HAggieFrontAnimation};


void cDec2String(uint32_t n, char* buf){ //this funciton sucks, i apologize for its existence
  if (n == 0){
    buf[0] = '0';
    buf[1] = 0;
    return;
  }
  uint32_t numDig = 0;
  uint32_t a = n;
  while (a){
    a /= 10;
    numDig++;
  }
  buf[numDig--] = 0;
  while (n){
    char curr = '0' + (n % 10);
    n /= 10;
    buf[numDig--] = curr;
  }
}

// ********************************PROJECTILE OBJECT********************************
// data struct for info on different tiers of projectiles
struct ProjectileType {
  uint16_t color;
  uint8_t dmg;
  uint8_t speed;
  //size?
  ProjectileType(uint16_t c, uint8_t d, uint8_t s){
    color = c;dmg = d; speed = s;
  }
};
// BALANCE
// here are the different types, and an array the player progresses through
const ProjectileType pebble(0xFFFF,                         1, 12);
const ProjectileType stone(0xFFFF,                          2, 12);
const ProjectileType rock(0xFFFF,                           3, 16);
const ProjectileType boulder(0xFFFF,                        4, 16);
const ProjectileType aggieRock(0x001F,                      1, 10);
const ProjectileType bossRock(0x001F,                       2, 8);

const ProjectileType * const RockTypes[4] = {&pebble, &stone, &rock, &boulder};

// BALANCE
// cost of different upgrades @ each level and in some cases values of upgrades
const int32_t rockUpdrades[3] =       {24, 48, 96};            //cost of each rock level
const int32_t maxHealthUpgrades[4] =  {16, 64, 64, 80};   //cost of each maxhealth level, each adds 2
const int32_t fireRateUpgrades[4] =   {24, 64, 128, 256};   //cost of each firerate increase, each decreases cooldown by ?
const uint8_t fireRateCooldowns[5] =  {24, 16, 12, 8, 4};  //VALUE of fire rate cooldowns in 30Hz units
const int32_t rollDodgeCost =          16;                         //cost to unlock dodge
const int32_t heartCost =              8;                             //cost to buy one heart (2 health units)

// ********************************GLOBAL PLAYER STATE STRUCT********************************
// PlayerData holds player state info such as health, progression and score
struct PlayerData {
  bool cheater;
  int8_t maxHealth;
  int8_t health;
  int16_t gold;
  int16_t totalGold;
  uint8_t rockIdx;
  uint8_t fireCooldownI;
  bool hasKey;
  bool canRoll;
  PlayerData(){
    cheater = false;
    canRoll = false;
    hasKey = false;
    maxHealth = 2;
    health = maxHealth;
    gold = totalGold = 0;
    rockIdx = 0;
    fireCooldownI = 0;
  }
  void addGold(int16_t g){
    gold += g;
    totalGold += g;
    //dac.playSound(GoldSound);
  }
};
PlayerData PData;

// ********************************HEADS UP DISPLAY********************************
// references global player state to display health and gold on the screen
// takes up bottom 16 px of screen, just enough for 2 rows of chars
class HUD {
  public:
  void drawBG(){
    uint16_t bg = 0;
    ST7735_DrawFastHLine(0, 159-17, 128, 0xFFFF);
    ST7735_DrawFastHLine(0, 159-18, 128, 0xFFFF);
    ST7735_DrawFastVLine(64, 159-18, 18, 0xFFFF);
    ST7735_DrawFastVLine(63, 159-18, 18, 0xFFFF);
    ST7735_DrawChar(67, 159-15, '$', ST7735_Color565(242,201,0), bg, 2);
  }
  void updateGold(){
    char gld[5] = {0, 0, 0, 0, 0};
    cDec2String(PData.gold, gld);
    uint32_t idx = 0;
    uint16_t gcolor = ST7735_Color565(242,201,0);
    while(gld[idx]){
      ST7735_DrawChar(79 + (12*idx), 159-15, gld[idx], gcolor, 0, 2);
      idx++;
    }
  }
  void updateHealth(){
    uint32_t hearts = PData.health / 2;
    uint32_t half = PData.health % 2;
    hearts += half;
    uint32_t i;
    for (i = 0; i < hearts; i++){
      ST7735_DrawChar(2 + (12*i), 159-16, 3, 0x001F, 0, 2);
    }
    i--;
    if (half){
      ST7735_FillRect(7 + (12*i), 159-14, 5, 12, 0);
    }
    if (PData.health < 9){
      ST7735_FillRect(2 + (6*PData.health), 159-14, 62 - (2 + (6*PData.health)), 12, 0);
    }
  }
};
HUD hud;

// ********************************LANGUAGE SYSTEM********************************
// Global var points to one of two parallel arrays w/ all in game messages necessary
const char **GameMessages;
#define GAME_OVER 0
#define BEVOS_DEAD 1
#define TOTAL_SCORE 2
#define STORE 3
#define UP_WEAPON 4
#define UP_FIRERATE 5
#define UP_MAXHEALTH 6
#define UP_DODGE 7
#define NEXT_LEVEL 8
#define BUY_HEART 9
#define YOU_WIN 10
#define BEVOS_SAFE 11

const char* English[] = {"Game Over!", "   Aggies ate Bevo!", "Score: ", "Store", "Damage++", "Fire Rate++", "Max Health++", "Roll Dodge", "Press Y to exit", "Health++", "You Win!!", "Bevo is safe!"};
const char* Spanish[] = {"Juego\nTerminado!", "\nAggies Comieron Bevo!", "Puntos: ", "Tienda", "Da\xA4" "o++", "Velocidad\nde fuego++", "M\xA0" "xima salud++", "Esquivar", "Y para salir", "Salud++", "T\xA3 Ganas!!", "Bevo es seguro!!"};
const char* CreatorA = "Frank Collebrusco";
const char* CreatorB = "Jasmine Pok";
const char* GithubLink = "github.com/\ncollebrusco";

// I should just add this to the display driver, ST7735_OutString with more options
void OutString(int16_t x, int16_t y, const char* c, uint16_t txclr, uint16_t bgcolor, uint8_t size){
  uint32_t i = 0;
  while (*c){
    if ((*c) == 0x0A){
      y += (8 * size);
      i = 0;
    } else {
      ST7735_DrawChar(x + (6*size*i), y, *c, txclr, bgcolor, size);
      i++;
    }
    c++;
  }
}

// ********************************FULL SCREEN UI'S********************************
// start screen, shows art and selects language
static const char** StartScreen(){
  ST7735_DrawBitmap(0, 160, StartupArt, 128, 160);
  OutString(7, 60, "Press X for English", UT_ORANGE, 0, 1);
  OutString(16, 70, "o B para Espa\xA4ol", UT_ORANGE, 0, 1);
  OutString(8, 151, "Frank C & Jasmine P", UT_ORANGE, 0, 1);
  while (1){
    if (flag){
      if (dpad.x()){
        dac.playSound(SelectSound);
        return English;
      }
      if (dpad.b()){
        dac.playSound(SelectSound);
        return Spanish;
      }
    }
  }
}

// ****************STORE****************
// opens store screen & enters loop, which is also controlled by SysTick,
// that handles input, display & selection of upgrades and items.
static void OpenStore(){
  uint16_t bgc = UT_ORANGE;
  uint16_t fontc = 0xFFFF;
  uint16_t sfontc = 0;
  ST7735_FillScreen(bgc);
  ST7735_DrawFastHLine(0, 0, 128, fontc);
  ST7735_DrawFastHLine(0, 1, 128, fontc);
  ST7735_DrawFastHLine(0, 158, 128, fontc);
  ST7735_DrawFastHLine(0, 159, 128, fontc);
  ST7735_DrawFastVLine(0, 0, 160, fontc);
  ST7735_DrawFastVLine(1, 0, 160, fontc);
  ST7735_DrawFastVLine(126, 0, 160, fontc);
  ST7735_DrawFastVLine(127, 0, 160, fontc);

  OutString(36, 4, GameMessages[STORE], fontc, bgc, 2);
  ST7735_DrawFastHLine(16, 20, 128-32, fontc);
  ST7735_DrawFastHLine(16, 21, 128-32, fontc);//24
  uint32_t selectPos = 0;
  bool up = true;
  bool jPrev = false;
  bool maxRock = (PData.rockIdx > 2);
  bool maxFR = (PData.fireCooldownI > 3);
  bool maxH = (PData.maxHealth > 9);
  bool hfull = (PData.health==PData.maxHealth);
  char buf[5] = {0, 0, 0, 0, 0};
  char* costBuf;
  buf[0] = '$';
  char mbuf[] = "MAX";
  while (!(dpad.y())){
    if (flag){
      if ((joy.y() > 0)){
        if (!jPrev){
          if (selectPos){
            selectPos--;
          } else {
            selectPos = 4;
          }
          up = true;
        }
        jPrev = true;
      } else if ((joy.y() < 0)){
        if (!jPrev) {
          selectPos = (selectPos + 1) % 5;
          up = true;
        }
        jPrev = true;
      } else {jPrev = false;}
      //check a_ris...
      if (dpad.a_ris()){
        switch (selectPos){
          case 0: // damage++
          if ((!maxRock) && ((((int32_t)PData.gold) - rockUpdrades[PData.rockIdx]) >= 0)){
            PData.gold -= rockUpdrades[PData.rockIdx];
            PData.rockIdx++;
            dac.playSound(SelectSound);
          }
          break;

          case 1: // fire rate ++
          if ((!maxFR)&& ((((int32_t)PData.gold) - fireRateUpgrades[PData.fireCooldownI]) >= 0)){
            PData.gold -= fireRateUpgrades[PData.fireCooldownI];
            dac.playSound(SelectSound);
            PData.fireCooldownI++;
          }
          break;

          case 2: // maxhealth++
          if ((PData.maxHealth < 10) && ((((int32_t)PData.gold) - maxHealthUpgrades[((PData.maxHealth)/2)-1 - (maxH)]) >= 0)){
            PData.gold -= maxHealthUpgrades[((PData.maxHealth)/2)-1 - (maxH)];
            dac.playSound(SelectSound);
            PData.maxHealth += 2;
          }
          break;

          case 3: //dodge unlock
          if ((!PData.canRoll) && ((((int32_t)PData.gold) - rollDodgeCost) >= 0)){
            PData.gold -= rollDodgeCost;
            dac.playSound(SelectSound);
            PData.canRoll = true;
          }
          break;

          case 4: //buy heart
          if ((!hfull) && ((((int32_t)PData.gold) - heartCost) >= 0)){
            PData.gold -= heartCost;
            dac.playSound(SelectSound);
            PData.health = min(PData.health + 2, PData.maxHealth);
          }
          break;
          default:
          break;
        }
        up = true;
      }
      if (up) {
        maxRock = (PData.rockIdx > 2);
        maxFR = (PData.fireCooldownI > 3);
        maxH = (PData.maxHealth > 9);
        hfull = (PData.health==PData.maxHealth);
        ST7735_FillRect(4, 30, 120, 110, bgc);
        ST7735_FillRect(2, 141, 124, 20, 0);
        ST7735_FillRect(4, 30 + (selectPos * 20), 120, 20, fontc);
        hud.drawBG();
        hud.updateHealth();
        hud.updateGold();

        OutString(6, 32,  GameMessages[UP_WEAPON],    (selectPos==0) ? sfontc : fontc, (selectPos==0) ? fontc : bgc, 1);
        maxRock ? (costBuf = &mbuf[0]) : (costBuf = &buf[0]);
        cDec2String(rockUpdrades[PData.rockIdx],buf+1);
        OutString(97, 32,  costBuf,                   (selectPos==0) ? sfontc : fontc, (selectPos==0) ? fontc : bgc, 1);
        OutString(6, 52,  GameMessages[UP_FIRERATE],  (selectPos==1) ? sfontc : fontc, (selectPos==1) ? fontc : bgc, 1);
        maxFR ? (costBuf = &mbuf[0]) : (costBuf = &buf[0]);
        cDec2String(fireRateUpgrades[PData.fireCooldownI], buf+1);
        OutString(97, 52,  costBuf,                   (selectPos==1) ? sfontc : fontc, (selectPos==1) ? fontc : bgc, 1);

        OutString(6, 72,  GameMessages[UP_MAXHEALTH], (selectPos==2) ? sfontc : fontc, (selectPos==2) ? fontc : bgc, 1);
        maxH ? (costBuf = &mbuf[0]) : (costBuf = &buf[0]);
        cDec2String(maxHealthUpgrades[((PData.maxHealth)/2)-1 - (maxH)], buf+1);
        OutString(97, 72,  costBuf,                   (selectPos==2) ? sfontc : fontc, (selectPos==2) ? fontc : bgc, 1);

        OutString(6, 92,  GameMessages[UP_DODGE],     (selectPos==3) ? sfontc : fontc, (selectPos==3) ? fontc : bgc, 1);
        PData.canRoll ? (costBuf = &mbuf[0]) : (costBuf = &buf[0]);
        cDec2String(rollDodgeCost,buf+1);
        OutString(97, 92,  costBuf,                  (selectPos==3) ? sfontc : fontc, (selectPos==3) ? fontc : bgc, 1);

        OutString(6, 112, GameMessages[BUY_HEART],   (selectPos==4) ? sfontc : fontc, (selectPos==4) ? fontc : bgc, 1);
        hfull ? (costBuf = &mbuf[0]) : (costBuf = &buf[0]);
        cDec2String(heartCost, buf+1);
        OutString(97, 112, costBuf,                  (selectPos==4) ? sfontc : fontc, (selectPos==4) ? fontc : bgc, 1);

        OutString(6, 132, GameMessages[NEXT_LEVEL],   fontc, bgc, 1);
      }
      up = false;
    }
  }
  dac.playSound(SelectSound);
}

//**************** :( ****************
static void GameOver(){
  dac.playSound(LoseSound);
  ST7735_FillScreen(0);
  char gld[5];
  if (!PData.cheater){
    Dec2String(PData.totalGold, gld);
  } 
  OutString(0, 0, GameMessages[GAME_OVER], 0xFFFF, 0, 2);
  OutString(0, 24, GameMessages[BEVOS_DEAD], 0xFFFF, 0, 1);
  OutString(0, 48, GameMessages[TOTAL_SCORE], 0xFFFF, 0, 2);
  OutString(12*6, 48+16, PData.cheater ? "-999" : gld, ST7735_Color565(242,201,0), 0, 2);
  OutString(0, 124, CreatorA, 0xFFFF, 0, 1);
  OutString(0, 132, CreatorB, 0xFFFF, 0, 1);  
  OutString(0, 140, GithubLink, 0xFFFF, 0, 1);  
  while (1);
}

//**************** :) ****************
static void YouWin(){
  dac.playSong(TexasSong);
  ST7735_FillScreen(0);
  char gld[5];
  if (!PData.cheater){
    Dec2String(PData.totalGold, gld);
  } 
  OutString(0, 0, GameMessages[YOU_WIN], 0xFFFF, 0, 2);
  OutString(0, 24, GameMessages[BEVOS_SAFE], 0xFFFF, 0, 1);
  OutString(0, 48, GameMessages[TOTAL_SCORE], 0xFFFF, 0, 2);
  OutString(12*6, 48+16, PData.cheater ? "-999" : gld, ST7735_Color565(242,201,0), 0, 2);
  OutString(0, 124, CreatorA, 0xFFFF, 0, 1);
  OutString(0, 132, CreatorB, 0xFFFF, 0, 1);  
  OutString(0, 140, GithubLink, 0xFFFF, 0, 1);  
  ST7735_DrawBitmap(4,110,bevo_pixel,64,32);
  while (1);
}

// ********************************ACTION INTERFACE********************************
// most in game actions are carried out by one of these. e.g. move, shoot, drop loot
// stores ID of sprite who created it and contains function that performs it
class Action {
  protected:
  uint32_t ID;
  public:
  virtual void perform(uint32_t clock) = 0;
};

// *******************************ACTION FIFO********************************
// When each sprite takes their turn, they push one or more actions to this queue
// they push pointers as all actions are heap allocated
// the main thread clears the queue, performs and deletes all actions each frame

#define MAX_FIFO_SIZE 64
uint8_t size = 0;
uint8_t putI = 0;
uint8_t getI = 0;
Action* FIFO[MAX_FIFO_SIZE];

static uint32_t Fifo_isEmpty(void){
	return size == 0;
}
static uint32_t Fifo_isFull(void){
	return size == MAX_FIFO_SIZE;
}

// *********** FiFo_Put**********
// Adds an element to the FIFO

static uint32_t Fifo_Put(Action* data) {
	if (Fifo_isFull()){return 0;}
	long sr = StartCritical();
	FIFO[putI] = data;
	putI = (putI + 1) % MAX_FIFO_SIZE;
	size++;
	EndCritical(sr);
  return(1);

}

// *********** Fifo_Get**********
// Gets an element from the FIFO

static uint32_t Fifo_Get(Action** datapt){
  if (Fifo_isEmpty()){return 0;}
	long sr = StartCritical();
	*datapt = FIFO[getI];
	getI = (getI + 1) % MAX_FIFO_SIZE;
	size--;
	EndCritical(sr);
	return(1);
}
// *****************************END ACTION FIFO*****************************

// ********************************SPRITE INTERFACE********************************
// interface for each renderable object that is more than a single pixel & exists in game world (isnt UI)
// contains textures, position, and an ID which is its index in the global array of sprites
// each sprite implements a taketurn function which is essentially their AI
// that function pushes new actions to the action FIFO based on input or AI or whatever's needed
class Sprite {
  public:
  bool collisions;
  uint32_t ID;
	int16_t x,y;
  uint8_t w,h;
  Animation* animArray;
  uint8_t animIdx;
  virtual void takeTurn(void) = 0;
  virtual int8_t* healthPtr(void){
    return 0;
  }
  virtual void kill(void){
  }
  virtual uint32_t stClk(void){
    return 0;
  }
  virtual void wasHitBy(uint32_t OID){
  }
  virtual void draw(void){
    //TODO: health bar here maybe
    ST7735_DrawBitmap(x, y, animArray[animIdx].getFrame(), w, h);
  }
};

// *****************************SPRITE DATA STRUCTURE*****************************
// the following could be an object too but I went with a C style implementation
// global array of sprite handles currently in game
// loop through them with (i = 0; i < GSIdx; i++) 
// THE PLAYER IS ALWAYS STORED AT INDEX ZERO so the player sprite is ALWAYS accessible
volatile int8_t GSIdx = 0;
Sprite* gameSprites[48];
// adds a new heap allocated sprite handle to end of array
static void pushSprite(Sprite* sp){
  gameSprites[GSIdx] = sp;
  gameSprites[GSIdx]->ID = GSIdx;
  GSIdx++;
  if (GSIdx > 47){
    PF3 |= 0x08;
  }
}
// removes sprite with given ID, and crucially shuffles handles around so all are adjacent in array
// calls kill function on each if they need to do anything on death
static void popSprite(uint32_t id){
  gameSprites[id]->kill();
  if (id == (GSIdx - 1)) {
    ST7735_FillRect(gameSprites[id]->x + 2, gameSprites[id]->y - 15, 14, 14, 0);
    delete gameSprites[id];
    GSIdx--;
  } else {
    ST7735_FillRect(gameSprites[id]->x + 2, gameSprites[id]->y - 15, 14, 14, 0);
    delete gameSprites[id];
    GSIdx--;
    gameSprites[id] = gameSprites[GSIdx];
    gameSprites[id]->ID = id;
  }
}
// kills all sprites but player
static void killAllButPlayer(void){
  for (int i = 1; i < GSIdx; i++){
    delete gameSprites[i];
    gameSprites[i] = 0;
  }
  GSIdx = 1;
}
// simply calls taketurn on all sprites
static void takeAllTurns(void){
  for (int i = 0; i < GSIdx; i++){
    gameSprites[i]->takeTurn();
  }
}
// checks for any sprites with health <= 0 and pops them
static void removeDeadSprites(void){
  for (int i = 0; i < GSIdx; i++){
    int8_t* hpt = gameSprites[i]->healthPtr();
    if (hpt){
      if ((*hpt) < 1){
        popSprite(i);
        removeDeadSprites();
        break;
      }
    }
  }
  return;
}
// ***************************END SPRITE STRUCTURE***************************

// ********************************PROJECTILE SYSTEM********************************
// Projectile object holds info about projectile 
// calculates data necessary to move in given direction on construct
// call erase, update, draw in that order, and maximize time between draw & erase calls
class Projectile {
  public:
  int16_t x,y;
  uint8_t ID, firerID, mx, my;
  int8_t dx, dy;
  const ProjectileType * type;
  Projectile(const ProjectileType* typ, uint8_t fid, int8_t ddx, int8_t ddy){ // ddy & ddx indicate dir, not speed
    firerID = fid;type = typ;
    x = gameSprites[firerID]->x + (gameSprites[firerID]->w / 2);
    // oldx = x;
    y = gameSprites[firerID]->y - (gameSprites[firerID]->h / 2);
    // oldy = y;
    dx = (ddx * type->speed)>>7;
    dy = (ddy * type->speed)>>7;
    if (dx){
      bool dxNeg = (dx < 0);
      uint8_t adx = abs(dx);
      bool lowmag = (adx < 8); // if jx 1 to 7
      mx = (lowmag ? (8-adx) : 1);
      dx = (lowmag ? 1 : adx - 7); // if mag under 8, dx is 1, otherwise its 1,2,
      if (dxNeg){
        dx *= -1;
      }
    }
    if (dy){
      bool dyNeg = (dy < 0);
      uint8_t ady = abs(dy);
      bool lowmag = (ady < 8); // if jx 1 to 7
      my = (lowmag ? (8-ady) : 1);
      dy = (lowmag ? 1 : ady - 7); // if mag under 8, dx is 1, otherwise its 1,2,
      if (dyNeg){
        dy *= -1;
      }
    }
  }
  void erase(){
    ST7735_DrawPixel(x, y, 0);
  }
  void update(uint32_t clock){
    if (!(clock % mx)){
      x += dx;
    }
    if (!(clock % my)){
      y += dy;
    }
  }
  void draw(){
    ST7735_DrawPixel(x, y, type->color);
  }
};

//****************PROJECTILE DATA STRUCTURE****************
// almost identical to sprite structure
uint8_t GPIdx = 0;
Projectile* gameProjectiles[32];
// same as sprite structure
static void pushProjectile(Projectile* sp){
  gameProjectiles[GPIdx] = sp;
  gameProjectiles[GPIdx]->ID = GPIdx;
  GPIdx++;
  if (GPIdx > 31){
    PF3 |= 0x08;
  }
}
// same as sprite structure
static void popProjectile(uint32_t id){
  delete gameProjectiles[id];
  GPIdx--;
  gameProjectiles[id] = gameProjectiles[GPIdx];
  gameProjectiles[id]->ID = id;
}
// erases all projectiles
static void eraseProjectiles(){
  for(int i = 0; i < GPIdx; i++){
    gameProjectiles[i]->erase();
  }
}
// updates all projectiles
static void updateProjectiles(uint32_t clock){
  for(int i = 0; i < GPIdx; i++){
    gameProjectiles[i]->update(clock);
  }
}
// draws all projectiles
static void drawProjectiles(){
  for(int i = 0; i < GPIdx; i++){
    gameProjectiles[i]->draw();
  }
}
// checks collisions for all projectiles. this is handled differently than sprites for a couple reasons
// 1. theyre only a pixel so there's less calculation
// 2. projectiles move on their own, not with move actions, which is where normal collision checking happens
// 3. they have some special considerations, like sprites cant shoot themselves and enemies cant shoot each other
static void checkProjectiles(){
  for (int p = 0; p < GPIdx; p++){
    int16_t x = gameProjectiles[p]->x;
    int16_t y = gameProjectiles[p]->y;
    if ((x > AREA_XMAX) ||
        (x < AREA_XMIN) ||
        (y > AREA_YMAX) ||
        (y < AREA_YMIN)){ //bullet outta bounds
      popProjectile(p);
    } else if (gameProjectiles[p]->firerID != 0){ //enemy fired it, only check player so they cant shoot each other
      if (gameSprites[0]->collisions){
        if ((y < (gameSprites[0]->y - 2)) &&
            (y > gameSprites[0]->y -(gameSprites[0]->h - 2)) &&
            (x > (gameSprites[0]->x + 2)) &&
            (x < (gameSprites[0]->x + (gameSprites[0]->w - 2)))){
          int8_t* hptr = (gameSprites[0]->healthPtr());
          if (hptr){
            dac.playSound(DamageSound);
            *hptr -= (gameProjectiles[p]->type->dmg);//TODO: replace w dmg action>??????????
          }
          popProjectile(p);
        }
      }
    } else { //player fired bullet, check everything
      for (int s = 0; s < GSIdx; s++){
        if (gameSprites[s]->collisions && (gameSprites[s]->ID != gameProjectiles[p]->firerID)){
          if ((y < (gameSprites[s]->y - 2)) &&
              (y > gameSprites[s]->y -(gameSprites[s]->h - 2)) &&
              (x > (gameSprites[s]->x + 2)) &&
              (x < (gameSprites[s]->x + (gameSprites[s]->w - 2)))){
            int8_t* hptr = (gameSprites[s]->healthPtr());
            if (hptr){
              dac.playSound(DamageSound);
              *hptr -= (gameProjectiles[p]->type->dmg);//TODO: replace w dmg action>??????????
            }
            popProjectile(p);
          }
        }
      }
    }
  }
}

// ********************************COLLISION CHECKERS********************************
// called by move actions, these check whether the given move is valid or a collision
// X
bool checkMoveXAction(uint32_t ID, int8_t dx){
  //check if hit screen edges
  if ((((gameSprites[ID]->x + dx) > (AREA_XMAX - (gameSprites[ID]->w-2))) && (dx > 0)) || (((gameSprites[ID]->x + dx) < (AREA_XMIN-2)) && (dx < 0))){
    return false;
  }
  if (!(gameSprites[ID]->collisions)){return true;}
  //check for coll w every other sprite (O(n))
  int16_t newX = gameSprites[ID]->x + dx;
  bool dxNeg = (dx < 0);
  for (int i = 0; i < GSIdx; i++){
    if ((gameSprites[i]->collisions) && (i != ID)){ // dont check against urself
      if ((gameSprites[ID]->y >= (gameSprites[i]->y - (gameSprites[i]->h - 2) )) && ((gameSprites[ID]->y - (gameSprites[ID]->h - 2)) <= (gameSprites[i]->y))){ // y's overlap
        if (dxNeg){ // moving | <-- |
          if (newX >= (gameSprites[i]->x + (gameSprites[i]->w / 2))){ //we're on the right side of obj
            if (newX <= (gameSprites[i]->x + (gameSprites[i]->w - 4))){
              //COLLISION
              gameSprites[i]->wasHitBy(ID);
              return false;
            }
          }
        } else {    // moving | --> |
          if ((newX + (gameSprites[ID]->w/2)) <= gameSprites[i]->x){ // we're on left side of obj
            if ((newX + gameSprites[ID]->w - 4) >= gameSprites[i]->x){
              //COLLISION
              gameSprites[i]->wasHitBy(ID);
              return false;
            }
          }
        }
      }
    }
  }
  return true;
}
// Y
bool checkMoveYAction(uint32_t ID, int8_t dy){
  //check if hit screen edges
  if ((ID != 0) || (!PData.hasKey) || (dy > 0)) {
    if ((((gameSprites[ID]->y + dy) > (AREA_YMAX)) && (dy > 0)) || (((gameSprites[ID]->y + dy) < (AREA_YMIN + (gameSprites[ID]->h-2))) && (dy < 0))){
      return false;
    }
  } else if ((PData.hasKey)&& (ID==0)){
    if ((((gameSprites[ID]->y + dy) > (AREA_YMAX)) && (dy > 0)) || (((gameSprites[ID]->y + dy) < (AREA_YMIN + (gameSprites[ID]->h-2))) && (dy < 0))){
      if ((gameSprites[ID]->x > 94) || (gameSprites[ID]->x < 48)) {
        return false;
      }
    }
  }
  if (!(gameSprites[ID]->collisions)){return true;}
  //check for coll w every other sprite (O(n))
  int16_t newY = gameSprites[ID]->y + dy;
  bool dyNeg = (dy < 0);
  for (int i = 0; i < GSIdx; i++){
    if ((gameSprites[i]->collisions) && (i != ID)){ // dont check against urself
      if ((gameSprites[ID]->x <= (gameSprites[i]->x + (gameSprites[i]->w - 2) )) && ((gameSprites[ID]->x + (gameSprites[ID]->w - 2)) >= (gameSprites[i]->x))){ // x's overlap
        if (!dyNeg){ // moving \/ DOWN
          if (newY <= (gameSprites[i]->y - (gameSprites[i]->h / 2))){ //we're on the top side of obj
            if (newY >= (gameSprites[i]->y - (gameSprites[i]->h - 4))){
              //COLLISION
              gameSprites[i]->wasHitBy(ID);
              return false;
            }
          }
        } else {    // moving /\ UP
          if ((newY - (gameSprites[ID]->h/2)) >= gameSprites[i]->y){ // we're on bot side of obj
            if ((newY - (gameSprites[ID]->h - 4)) <= gameSprites[i]->y){
              //COLLISION
              gameSprites[i]->wasHitBy(ID);
              return false;
            }
          }
        }
      }
    }
  }
  return true;
}

// ********************************ACTION SUBTYPES********************************
// see Action interface. these are the different types of actions, each with execution routines and any data necessary

// this is "depricated" if you can call it that. use moveX and moveY instead
class Move : public Action {
  private:
  int8_t x,y,mod;
  public:
  Move(uint32_t id, int8_t xx, int8_t yy, int8_t m){
    ID = id;x = xx;y = yy;mod = m;
  }
  virtual void perform(uint32_t clock)  {
    if (clock % mod){return;}
    if (((gameSprites[ID]->x + x) <= (AREA_XMAX - (gameSprites[ID]->w-2))) && ((gameSprites[ID]->x + x) >= (AREA_XMIN-2))){
      gameSprites[ID]->x += x;
    }
    if (((gameSprites[ID]->y + y) <= (AREA_YMAX)) && ((gameSprites[ID]->y + y) >= (AREA_YMIN + (gameSprites[ID]->h-2)))){
      gameSprites[ID]->y += y;
    }
  }
};

// ****************MOVE X & Y ACIONS****************
// inputs: ID, DX/Y, Mod
// moves sprite at ID DX/Y pixels every Mod frames
class MoveXAction : public Action {
  private:
  int8_t x,mod;
  public:
  MoveXAction(uint32_t id, int8_t xx, int8_t m){
    ID = id;x = xx;mod = m;
  }
  virtual void perform(uint32_t clock)  {
    if (clock % mod){return;}
    if (!checkMoveXAction(ID, x)){return;}
    gameSprites[ID]->x += x;
  }
};
class MoveYAction : public Action {
  private:
  int8_t y,mod;
  public:
  MoveYAction(uint32_t id, int8_t yy, int8_t m){
    ID = id;y = yy;mod = m;
  }
  virtual void perform(uint32_t clock)  {
    if (clock % mod){return;}
    if (!checkMoveYAction(ID, y)){return;}
    gameSprites[ID]->y += y;
  }
};

// ****************SHOOT ACTION****************
// proj type, firer id, dx and dy
// spawns specified projectile
// the code for normalizing the magnatude of the direction vector is GARBAGE b/c I dont have a sqrt
// TODO: implement a sqrt func in fmath.cpp
class ShootAction : public Action {
  private:
    const ProjectileType* type;
    uint8_t firerID;
    int8_t dx, dy;
  public:
  ShootAction(const ProjectileType* typ, uint8_t fid, int8_t ddx, int8_t ddy){
    type=typ; firerID = fid; dx = ddx; dy = ddy;
  }
  virtual void perform(uint32_t clock){
    if (dx || dy){ // only shoot if dir given
      int8_t maxd = max(abs(dx),abs(dy));
      uint8_t scale = 127 / maxd;
      if (scale > 1){
        dx *= scale;
        dy *= scale;
      }
      pushProjectile(new Projectile(type, firerID, dx, dy));
      dac.playSound(Shoot);
    }
  }
};

// ****************ANIMATE ACTION****************
// steps a frame in id's animation every m frames
class AnimateAction : public Action {
  private:
  uint32_t mod;
  public:
  AnimateAction(uint32_t id, uint32_t m){
    ID = id;mod = m;
  }
  virtual void perform(uint32_t clock){
    if (clock % mod){return;}
    gameSprites[ID]->animArray[gameSprites[ID]->animIdx].nextframe();
  }
};
// ****************DAMAGE ACTION****************
// damages sprite id with d damage every m frames
class DamageAction : public Action {
  private:
  int8_t dmg;
  uint8_t mod;
  public:
  DamageAction(uint32_t id, int8_t d,uint8_t m){
    ID = id;dmg = d;mod = m;
  }
  virtual void perform(uint32_t clock){
    if (clock % mod){return;}
    if (ID==0){dac.playSound(DamageSound);}
    *(gameSprites[ID]->healthPtr()) -= dmg;
  }
};

// ********************************SPRITE SUBTYPES********************************
// ****************COIN SPRITE****************
// simple sprite that dies and adds an amnt of gold to the player when overlap
// uses its own single pixel collision algorithm,
// TODO: abstract that because projectiles do it too so this is repetitive
class Coin : public Sprite {
  private:
  int8_t health;
  uint8_t value;
  public:
  Coin(int16_t xx, int16_t yy){
    animArray = &CoinAni;
    x = xx;y = yy;
    w = h = 2;
    animIdx = 0;
    health = 100;
    collisions = false;
    value = 1;
  }
  Coin(int16_t xx, int16_t yy, uint8_t v){
    animArray = &CoinAni;
    x = xx;y = yy;
    w = h = 2;
    animIdx = 0;
    health = 100;
    collisions = false;
    value = v;
  }
  virtual int8_t* healthPtr(){
    return &health;
  }
  virtual void takeTurn(){
    if ((y < (gameSprites[0]->y - 2)) &&
        (y > gameSprites[0]->y -(gameSprites[0]->h - 2)) &&
        (x > (gameSprites[0]->x + 2)) &&
        (x < (gameSprites[0]->x + (gameSprites[0]->w - 2))))
    {
      PData.addGold(value);
      Fifo_Put(new DamageAction(ID, 127, 1));
    }
  }
};

// this action is only defined among the sprites so it can know about the coin sprite
// ****************DROP COINS ACTION****************
// drops $n worth of coins at given location but only spawns 8 max
// TODO: improve this because it only drops coins worth approximately n
// because every coin it drops will have the same value
class DropCoinsAction : public Action {
  private:
  int16_t x, y;
  uint16_t num;
  public:
  DropCoinsAction(int16_t xx, int16_t yy, uint16_t n){
    x = xx;y = yy; num = n;
  }
  virtual void perform(uint32_t clock){
    uint32_t valEach = 4;
    num /= valEach;
    if (num > 8){
      valEach = ((num-1) / 8) + 1;
      num /= valEach;
    }
    for (int i = 0; i < num; i++){
      pushSprite(new Coin((x + (Random() % 10)), y - (Random()%10),valEach));
    }
  }
};

// ****************KEY SPRITE****************
// player's ticket to the next level
class Key : public Sprite {
  private:
  int8_t health;
  public:
  Key(int16_t xx, int16_t yy){
    animArray = &KeyAni;
    x = xx;y = yy;
    w = 15;h = 6;
    animIdx = 0;
    health = 100;
    collisions = false;
  }
  virtual int8_t* healthPtr(){
    return &health;
  }
  virtual void takeTurn(){
    if ((y < (gameSprites[0]->y - 2)) &&
        (y > gameSprites[0]->y -(gameSprites[0]->h - 2)) &&
        (x > (gameSprites[0]->x + 2)) &&
        (x < (gameSprites[0]->x + (gameSprites[0]->w - 2))))
    {
      PData.hasKey = true;
      Fifo_Put(new DamageAction(ID, 127, 1));
    }
  }
};

// ****************PLAYER SPRITE****************
// the player! taketurn converts inputs into actions
// roll dodge is implemented here, not as an action
uint32_t rollStartTime;
int8_t rollx, rolly;
uint32_t cooldownTimer;
class Player : public Sprite {
private:
public:
  // Player(uint32_t id){
  //   ID = id;
  Player(int16_t xx, int16_t yy){
    x = xx;y = yy;
    animArray = PlayerAnimations;
    animIdx = 0;
    w = h = 18;
    collisions = true;
  }
  virtual void kill(){
    GameOver();
  }
  virtual int8_t* healthPtr(){
    return &PData.health;
  }
  virtual void takeTurn(void)  {
    if (collisions){
      int32_t speed = slide.uScaleByData(9);
      int32_t jx = joy.scaleByX(speed);
      if (jx){ // jx -9 to 9 no 0
        uint32_t ajx = abs(jx); // jx 1 to 9
        bool lowmag = (ajx < 8); // if jx 1 to 7
        int32_t mx = (lowmag ? (8-ajx) : 1);
        int32_t dx = (lowmag ? 1 : ajx - 7); // if mag under 8, dx is 1, otherwise its 1,2,
        if (jx < 0){
          dx *= -1;
        }
        Fifo_Put(new MoveXAction(ID, dx, mx));
      }
      int32_t jy = joy.scaleByY(speed);
      if (jy){ // jy -9 to 9 no 0
        uint32_t ajy = abs(jy); // jy 1 to 9
        bool lowmag = (ajy < 8); // if jy 1 to 7
        int32_t my = (lowmag ? (8-ajy) : 1);
        int32_t dy = (lowmag ? 1 : ajy - 7); // if mag under 8, dx is 1, otherwise its 1,2,
        if (jy > 0){
          dy *= -1;
        }
        Fifo_Put(new MoveYAction(ID, dy, my));
      }
      if (jx || jy){
        Fifo_Put(new AnimateAction(ID, 2));
        if (jy > 0){
          animIdx = 3;
        } else {
          animIdx = 2;
        }
      } else {
        if (jy < 0){
          animIdx = 1;
        } else {
          animIdx = 0;
        }
      }
      //SHOOT
      if (dpad.a_ris()){
        if ((clock - cooldownTimer) > fireRateCooldowns[PData.fireCooldownI]) {
          Fifo_Put( new ShootAction(RockTypes[PData.rockIdx], ID, joy.scaleByX(127), -joy.scaleByY(127)) ); 
          cooldownTimer = clock;
        }
      }
      //ROLL
      if (dpad.b_ris() && PData.canRoll){
        rollx = joy.scaleByX(2);
        rolly = -joy.scaleByY(2);
        if (rollx || rolly){
          rollStartTime = clock;
          collisions = false;
          animIdx = (rollx > 0) ? 4 : 5;
        }
      }
    } else {
      //rolling...
      if ((clock - rollStartTime) <= 17){
        if (!((clock - rollStartTime) % 4)){
          Fifo_Put(new AnimateAction(ID, 1));
        }
        collisions = false;
        Fifo_Put(new MoveXAction(ID, rollx, 1));
        Fifo_Put(new MoveYAction(ID, rolly, 1));
      } else {
        collisions = true;
      }
    }
  }
};

// BALANCE
// ****************ENEMY TYPE OBJECT****************
// interface for type object classes, each type has their own stats and AI
// this should probably not be an interface and just have a funciton pointer field? who cares
class AggieType {
  public:
  int8_t maxHealth;
  uint8_t value;
  Animation* animarr;
  virtual void turnHandler(Sprite*) const = 0;
};

// lvl 1 enemy
class BasicAggie : public AggieType {
  public:
  BasicAggie(){
    value = 8;
    maxHealth = 3;
    animarr = AggieAnimations;
  }
  virtual void turnHandler(Sprite* caller) const {
    //MOVEMENT AI
    int32_t dx = gameSprites[0]->x - caller->x;
    int32_t dy = gameSprites[0]->y - caller->y;
    int32_t mx = 0;
    int32_t my = 0;
    if (dx < 0){
      mx = -1;
    } else if (dx) {
      mx = 1;
    }
    if (dy < 0){
      my = -1;
    } else if (dy){
      my = 1;
    }
    Fifo_Put(new MoveXAction(caller->ID,mx,2));
    Fifo_Put(new MoveYAction(caller->ID,my,2));
    // ATTACK AI (this guy has a basic 1 dmg meelee)
    if ((abs(dx) < 16) && (abs(dy) < 16) && gameSprites[0]->collisions){
      Fifo_Put(new DamageAction(0, 1, 30));
    }
  }
};
// lvl 2 enemy
class ToughAggie : public AggieType {
  public:
  ToughAggie(){
    value = 16;
    maxHealth = 8;
    animarr = HAggieAnimations;
  }
  virtual void turnHandler(Sprite* caller) const {
    //MOVEMENT AI
    int32_t dx = gameSprites[0]->x - caller->x;
    int32_t dy = gameSprites[0]->y - caller->y;
    int32_t mx = 0;
    int32_t my = 0;
    if (dx < 0){
      mx = -1;
    } else if (dx) {
      mx = 1;
    }
    if (dy < 0){
      my = -1;
    } else if (dy){
      my = 1;
    }
    Fifo_Put(new MoveXAction(caller->ID,mx,3));
    Fifo_Put(new MoveYAction(caller->ID,my,3));
    // ATTACK AI (this guy has a stonger but slower 2 dmg meelee)
    if ((abs(dx) < 16) && (abs(dy) < 16) && gameSprites[0]->collisions){
      Fifo_Put(new DamageAction(0, 1, 30));
    }

  }
};
// lvl 3 & first ranged enemy
class RangeAggie : public AggieType {
  private:
  uint32_t stClk;
  public:
  RangeAggie(){
    value = 32;
    maxHealth = 6;
    stClk = clock;
    animarr = AggieAnimations;
  }
  virtual void turnHandler(Sprite* caller) const {
    //MOVEMENT AI
    int32_t dx = gameSprites[0]->x - caller->x;
    int32_t dy = gameSprites[0]->y - caller->y;
    if (abs(amax(dy,dx)) > 40){
      int32_t mx = 0;
      int32_t my = 0;
      if (dx < 0){
        mx = -1;
      } else if (dx) {
        mx = 1;
      }
      if (dy < 0){
        my = -1;
      } else if (dy){
        my = 1;
      }
      Fifo_Put(new MoveXAction(caller->ID,mx,2));
      Fifo_Put(new MoveYAction(caller->ID,my,2));
    }
    // ATTACK AI 
    if (!((clock-caller->stClk()) % (48))){// + randoff))){ //DEBUG: if ranged aggies are spawned at the exact same time, theyll fire exactly at the same time
      //randoff = (((int32_t)(Random() % 10))-5);
      dx = limitMagnatude(dx,127);
      dy = limitMagnatude(dy,127);
      bool yn = (dy < 0);
      bool xn = (dx < 0);
      int32_t ax = abs(dx);
      int32_t ay = abs(dy);
      if (ay > ax){
        if (ay < 127){
          int32_t diff = 127 - ay;
          ay += diff;
          ax += diff;
        }
      } else {
        if (ax < 127){
          int32_t diff = 127 - ax;
          ay += diff;
          ax += diff;
        }
      }
      if (xn){ax *= -1;} if (yn){ay *= -1;}
      if (abs(dx) < 7){ax=0;}if(abs(dy) < 7){ay=0;}
      Fifo_Put( new ShootAction(&aggieRock, caller->ID, ax, ay) ); 
    }
  }
};
// lvl 4 enemy, ranged
class AutoAggie : public AggieType {
  private:
  public:
  AutoAggie(){
    value = 64;
    maxHealth = 8;
    animarr = AggieAnimations;
  }
  virtual void turnHandler(Sprite* caller) const {
    //MOVEMENT AI
    int32_t dx = gameSprites[0]->x - caller->x;
    int32_t dy = gameSprites[0]->y - caller->y;
    if (abs(amax(dy,dx)) > 60){
      int32_t mx = 0;
      int32_t my = 0;
      if (dx < 0){
        mx = -1;
      } else if (dx) {
        mx = 1;
      }
      if (dy < 0){
        my = -1;
      } else if (dy){
        my = 1;
      }
      Fifo_Put(new MoveXAction(caller->ID,mx,2));
      Fifo_Put(new MoveYAction(caller->ID,my,2));
    } else if (abs(amax(dy,dx)) < 40){
      int32_t mx = 0;
      int32_t my = 0;
      if (dx < 0){
        mx = 1;
      } else if (dx) {
        mx = -1;
      }
      if (dy < 0){
        my = 1;
      } else if (dy){
        my = -1;
      }
      Fifo_Put(new MoveXAction(caller->ID,mx,3));
      Fifo_Put(new MoveYAction(caller->ID,my,3));
    }
    // ATTACK AI 
    if ((((clock-caller->stClk()) % 300) < 230) && (((clock-caller->stClk()) % 300) > 200)) {
      if (!(clock % (8))){            //5 times a second
        dx = limitMagnatude(dx,127);
        dy = limitMagnatude(dy,127);
        dy += ((int8_t)Random() % 15);//less accurate
        dx += ((int8_t)Random() % 15);
        bool yn = (dy < 0);
        bool xn = (dx < 0);
        int32_t ax = abs(dx);
        int32_t ay = abs(dy);
        if (ay > ax){
          if (ay < 127){
            int32_t diff = 127 - ay;
            ay += diff;
            ax += diff;
          }
        } else {
          if (ax < 127){
            int32_t diff = 127 - ax;
            ay += diff;
            ax += diff;
          }
        }
        if (xn){ax *= -1;} if (yn){ay *= -1;}
        if (abs(dx) < 7){ax=0;}if(abs(dy) < 7){ay=0;}
        Fifo_Put( new ShootAction(&aggieRock, caller->ID, ax, ay) ); 
      }
    }
  }
};

const BasicAggie basicAggie;
const ToughAggie toughAggie;
const RangeAggie rangeAggie;
const AutoAggie autoaggie;
const AggieType* aggies[4] = {&basicAggie, &toughAggie, &rangeAggie, &autoaggie};

// ****************ENEMY SPRITE****************
// has pointer to one of const type objects and calls its AI func
// also gets some stat data from it
class Aggie : public Sprite {
  private:
  bool key;
  int8_t health;
  const AggieType* type;
  uint32_t startClk;
  public:
  Aggie(const AggieType* typ, int16_t xx, int16_t yy){
    startClk = clock;
    type = typ;
    health = type->maxHealth;
    x = xx;y = yy; 
    animArray = type->animarr;
    animIdx = 0;
    w = h = 18;
    collisions = true;
    key = false;
  }
  Aggie(const AggieType* typ, int16_t xx, int16_t yy, bool k){
    startClk = clock;
    type = typ;
    health = type->maxHealth;
    x = xx;y = yy; 
    animArray = type->animarr;
    animIdx = 0;
    w = h = 18;
    collisions = true;
    key = k;
  }
  virtual uint32_t stClk(){
    return startClk;
  }
  virtual void kill(){
    Fifo_Put(new DropCoinsAction(x, y, type->value));
    if (key){
      pushSprite(new Key(x, y));
    }
  }
  virtual int8_t* healthPtr(){
    return &health;
  }
  virtual void takeTurn(void) {
    type->turnHandler(this);
  }
};

// ****************DOOR SPRITE****************
// door to the next level, only opens with key
// once its open its open forever, as a new one is meant to be created for each level
class Door : public Sprite {
  private:
  bool open;
  bool hasOpened;
  public:
  Door(){
    collisions = true;
    open = false;
    hasOpened = false;
    y = 6;
    x = 64-16;
    w = 32;
    h = 6;
    animArray = &DoorAni;
    animArray[0].reset();
    animIdx = 0;
  }
  virtual void wasHitBy(uint32_t OID){
    //open if key, set prev first...
    if((OID==0) && (PData.hasKey)){
      open = true;
    }
  }
  virtual void kill(){
    // animArray[0].nextframe();
  }
  virtual void takeTurn(){
    if (open && !hasOpened){
      Fifo_Put(new AnimateAction(ID, 1));
      hasOpened = true;
      w = 6;h = 32;y=32;x-=2;
      ST7735_FillRect(64-16,0,32,6,0);
    }
  }
};

class Bevo : public Sprite {
  private:
  public:
  Bevo(int16_t xx, int16_t yy){
    x = xx;y = yy;
    w = 64;h = 32;
    animArray = &BevoAnimation;animIdx = 0;
    collisions = true;
  }
  virtual void takeTurn(){}
};

class Boss : public Sprite {
  private:
  int8_t health;
  uint32_t startClk;
  public:
  Boss(int16_t xx, int16_t yy){
    startClk = clock;
    health = 127;
    x = xx;y = yy; 
    animArray = &AggieBossAnimation;
    animIdx = 0;
    w = 28;h = 30;
    collisions = true;
  }
  virtual uint32_t stClk(){
    return startClk;
  }
  virtual void takeTurn(){
    //MOVEMENT AI
    int32_t dx = gameSprites[0]->x - x;
    int32_t dy = gameSprites[0]->y - y;
    if (abs(amax(dy,dx)) > 60){
      int32_t mx = 0;
      int32_t my = 0;
      if (dx < 0){
        mx = -1;
      } else if (dx) {
        mx = 1;
      }
      if (dy < 0){
        my = -1;
      } else if (dy){
        my = 1;
      }
      Fifo_Put(new MoveXAction(ID,mx,2));
      Fifo_Put(new MoveYAction(ID,my,2));
    } else if (abs(amax(dy,dx)) < 40){
      int32_t mx = 0;
      int32_t my = 0;
      if (dx < 0){
        mx = 1;
      } else if (dx) {
        mx = -1;
      }
      if (dy < 0){
        my = 1;
      } else if (dy){
        my = -1;
      }
      Fifo_Put(new MoveXAction(ID,mx,3));
      Fifo_Put(new MoveYAction(ID,my,3));
    }
    // ATTACK AI 
    if ((((clock-stClk()) % 300) < 120) && (((clock-stClk()) % 300) > 50)) {
      if (!(clock % (9))){         
        dx = limitMagnatude(dx,127);
        dy = limitMagnatude(dy,127);
        dy += ((int8_t)Random() % 15);//less accurate
        dx += ((int8_t)Random() % 15);
        bool yn = (dy < 0);
        bool xn = (dx < 0);
        int32_t ax = abs(dx);
        int32_t ay = abs(dy);
        if (ay > ax){
          if (ay < 127){
            int32_t diff = 127 - ay;
            ay += diff;
            ax += diff;
          }
        } else {
          if (ax < 127){
            int32_t diff = 127 - ax;
            ay += diff;
            ax += diff;
          }
        }
        if (xn){ax *= -1;} if (yn){ay *= -1;}
        if (abs(dx) < 7){ax=0;}if(abs(dy) < 7){ay=0;}
        Fifo_Put( new ShootAction(&aggieRock, ID, ax, ay) ); 
      }
    }
    if ((((clock-stClk()) % 300) > 270)) {
      if (!(clock % (5))){         
        dx = limitMagnatude(dx,127);
        dy = limitMagnatude(dy,127);
        dy += ((int8_t)Random());//less accurate
        dx += ((int8_t)Random());
        bool yn = (dy < 0);
        bool xn = (dx < 0);
        int32_t ax = abs(dx);
        int32_t ay = abs(dy);
        if (ay > ax){
          if (ay < 127){
            int32_t diff = 127 - ay;
            ay += diff;
            ax += diff;
          }
        } else {
          if (ax < 127){
            int32_t diff = 127 - ax;
            ay += diff;
            ax += diff;
          }
        }
        if (xn){ax *= -1;} if (yn){ay *= -1;}
        if (abs(dx) < 7){ax=0;}if(abs(dy) < 7){ay=0;}
        Fifo_Put( new ShootAction(&bossRock, ID, ax, ay) ); 
      }
    }
  }
  virtual int8_t* healthPtr(){
    return &health;
  }
  virtual void kill(){
    Fifo_Put(new DropCoinsAction(x, y, 1024));
    pushSprite(new Key(x, y));
    ST7735_FillRect(x,y-h,28,30,0);
  }
};

// ********************************30HZ SYSTICK INTERRUPT SERVICE ROUTINE************************************
// sample input, increment clock, set flag
void update(void){
  PF1 |= 0x02;
  slide.sample();
  joy.sample();
  dpad.sample();
  lpad.sample();
  flag = true;
  clock++;
  PF1 &= ~0x02;
}

// general purpose func to create test situations
void initTest(void){
  hud.drawBG();
  hud.updateGold();
  hud.updateHealth();
  pushSprite(new Player(16,16));
  pushSprite(new Door());
  pushSprite(new Aggie(aggies[0],30,120,true));
  GameMessages = English;
  // for(int i = 0; i < 3; i++){
  //   pushSprite(new Aggie(aggies[0],20 + (20*i),120));
  // }
}

// starts the interactive part of the game by spawning player
// MUST call this before ANY other sprites are created
// as many many things rely on the player being at the beginning of the sprite array
void initGame(){
  pushSprite(new Player(16,16));
}

// if this is true loop will stop
static bool paused = false;

// BALANCE
// ********************************LEVEL SYSTEM********************************
// one global object controls levels, all are initialized the same but
// call different functions on tick which are stored in global array.
// this array is each level in order essentially

// lvl 1
void levelUpdate1(){
  static uint32_t levelClock = 0;
  static uint32_t numEnemies = 0;
  static const uint8_t baseWait = 240;
  uint8_t waitTime = baseWait;
  if (!(levelClock % (waitTime))){
    numEnemies++;
    waitTime = baseWait + ((int8_t)(Random() % 60)-30);
    pushSprite(new Aggie(aggies[0], -30 + (188*((Random()%2)==0)), 16 + (Random()%64), numEnemies==3));
  }
  levelClock += !paused;
}
// lvl 2
void levelUpdate2(){
  static uint32_t levelClock = 0;
  static uint32_t numEnemies = 0;
  static const uint8_t numEnemTypes = 2;
  static const uint16_t baseAggieWait[numEnemTypes] = {210, 500};
  static const uint8_t aggieWaitVariance[numEnemTypes] = {30, 30};
  static const uint8_t keydropper = 3;
  uint16_t aggieWait[numEnemTypes] = {baseAggieWait[0], baseAggieWait[1]};
  for (int i = 0; i < numEnemTypes; i++){
    if (!((levelClock+(300*i)) % (aggieWait[i]))){
      numEnemies += (i == (numEnemTypes-1));
      aggieWait[i] = baseAggieWait[i] + ((int8_t)(Random() % (aggieWaitVariance[i]<<1))-aggieWaitVariance[i]);
      bool kd = ((i==(numEnemTypes-1)) && (numEnemies==keydropper));
      pushSprite(new Aggie(aggies[i], -30 + (188*((Random()%2)==0)), 0 + (Random()%80),kd));
    }
  }
  levelClock += !paused;
}
void levelUpdate3(){
  static uint32_t levelClock = 0;
  static uint32_t numEnemies = 0;
  static const uint8_t numEnemTypes = 3;
  static const uint16_t baseAggieWait[numEnemTypes] = {300, 420, 800};
  static const uint8_t aggieWaitVariance[numEnemTypes] = {30, 30, 30};
  static const uint8_t keydropper = 3;
  uint16_t aggieWait[numEnemTypes] = {baseAggieWait[0], baseAggieWait[1], baseAggieWait[2]};
  for (int i = 1; i < numEnemTypes; i++){
    if (!((levelClock+300*i) % (aggieWait[i]))){
      numEnemies += (i == (numEnemTypes-1));
      aggieWait[i] = baseAggieWait[i] + ((int8_t)(Random() % (aggieWaitVariance[i]<<1))-aggieWaitVariance[i]);
      bool kd = ((i==(numEnemTypes-1)) && (numEnemies==keydropper));
      pushSprite(new Aggie(aggies[i], -30 + (188*((Random()%2)==0)), 0 + (Random()%80), kd));
    }
  }
  levelClock += !paused;
}
void levelUpdate4(){
  static uint32_t levelClock = 0;
  static uint32_t numEnemies = 0;
  static const uint8_t numEnemTypes = 4;
  static const uint16_t baseAggieWait[numEnemTypes] = {0, 300, 900, 900};
  static const uint8_t aggieWaitVariance[numEnemTypes] = {200, 30, 30, 30};
  static const uint8_t keydropper = 2;
  uint16_t aggieWait[numEnemTypes] = {baseAggieWait[0], baseAggieWait[1], baseAggieWait[2], baseAggieWait[3]};
  for (int i = 1; i < numEnemTypes; i+=2){//TODO change back
    if (!((levelClock+500*i) % (aggieWait[i]))){
      numEnemies += (i == (numEnemTypes-1));
      aggieWait[i] = baseAggieWait[i] + ((int8_t)(Random() % (aggieWaitVariance[i]<<1))-aggieWaitVariance[i]);
      bool kd = ((i==(numEnemTypes-1)) && (numEnemies==keydropper));
      pushSprite(new Aggie(aggies[i], -30 + (188*((Random()%2)==0)), 0 + (Random()%80), kd));
    }
  }
  levelClock += !paused;
}
void levelBossFight(){
  static uint32_t levelClock = 0;
  if (!levelClock){
    pushSprite(new Bevo(64,AREA_YMAX));
    pushSprite(new Boss(-30,100));
    gameSprites[0]->x = 100;
    gameSprites[0]->y = 140-36;
  }
  static uint32_t numEnemies = 0;
  static const uint8_t numEnemTypes = 4;
  static const uint16_t baseAggieWait[numEnemTypes] = {0, 600, 1200, 0};
  static const uint8_t aggieWaitVariance[numEnemTypes] = {200, 30, 30, 30};
  uint16_t aggieWait[numEnemTypes] = {baseAggieWait[0], baseAggieWait[1], baseAggieWait[2], baseAggieWait[3]};
  for (int i = 1; i < numEnemTypes-1; i++){//TODO change back
    if (!((levelClock+500*i) % (aggieWait[i]))){
      numEnemies += (i == (numEnemTypes-1));
      aggieWait[i] = baseAggieWait[i] + ((int8_t)(Random() % (aggieWaitVariance[i]<<1))-aggieWaitVariance[i]);
      pushSprite(new Aggie(aggies[i], -30 + (188*((Random()%2)==0)), 0 + (Random()%80)));
    }
  }
  levelClock += !paused;
}

#define NUM_LEVELS 5
// level list
void(*levelUpdateFuncs[NUM_LEVELS])(void) = {levelUpdate1, levelUpdate2, levelUpdate3, levelUpdate4, levelBossFight};

// level control object, this should be C style there's not a real reason to have a class for this
class Level {
  private:
  bool enabled;
  uint32_t levelNum;
  void(*upd)(void);
  public:
  Level(){
    levelNum = 0;
    enabled = false;
    upd = levelUpdateFuncs[levelNum];
  }
  void next(){
    if (levelNum == (NUM_LEVELS-1)){
      YouWin();
    }
    upd = levelUpdateFuncs[++levelNum];
    PData.hasKey = false;
  }
  void init(){
    gameSprites[0]->x = 64 - 8;
    gameSprites[0]->y = 135;
    ST7735_FillScreen(0);
    pushSprite(new Door());
    ST7735_FillRect(20,140-18,18,18,0);
    hud.drawBG();
    hud.updateGold();
    hud.updateHealth();
    enabled = true;
  }
  void update(){
    if (enabled){
      upd();
    }
  }
  void close(){
    killAllButPlayer();
    enabled = false;
  }
  uint32_t const& getLevel(){
    return levelNum;
  }
};
Level level;

// deallocates all sprites, this isn't needed as its running on dedicated microcontroller
void destroy(void){
  for (int i = 0; i < GSIdx; i++){
    if (gameSprites[i]){
      delete gameSprites[i];
    }
  }
}

// ****************DRAW****************
// draws all sprites, draws player on top
void Draw(void){
  for (int i = 1; i < GSIdx; i++){
    if (gameSprites[i]){
      gameSprites[i]->draw();
    }
  }
  if (GSIdx > 0){
    gameSprites[0]->draw();
  }
}

// My keil debugger is broken so this is all ive got
void DebugOut(int32_t deb){
  ST7735_SetCursor(0,10);
  ST7735_OutString((char*)"   ");
  ST7735_SetCursor(0,10);
  ST7735_OutDec(deb);
}

// ********************************MAIN********************************
int main(void){
	DisableInterrupts();
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB);
	SysTick_Init(update,(uint32_t(80000000/30)),2); // frame rate 30Hz
  // initTest();
	EnableInterrupts();
  GameMessages = StartScreen();                   // show start screen & sel lang
  initGame();                                     // spawn player FIRST
  level.init();                                   // begin level
  dac.playSong(Tetris);
  uint32_t prevGold = 1000;                       // used to detect HUB updates
  int8_t prevHealth = 0;
  Random_Init(NVIC_ST_CURRENT_R);
	while (1){
    if (flag){
      level.update();                             // level tick
      takeAllTurns();                             // every sprite adds its actions to the FIFO
      eraseProjectiles();                         // clear previous frame's projectiles
      // CLEAR FLAG
      flag = false;
      // AUX INPUT CHECKING
      if (lpad.r_ris()){                          // pause
        paused = true;
        ST7735_FillRect(96, 8, 8, 24, 0xFFFF);
        ST7735_FillRect(112,8, 8, 24, 0xFFFF);
        while (paused){
          if (flag && lpad.r_ris()){
            paused = false;
            flag = false;
            ST7735_FillRect(96, 8, 8, 24, 0);
            ST7735_FillRect(112,8, 8, 24, 0);
          }
        }
      }
      if (dpad.x()){
        if (joy.sw_ris()){
          PData.cheater = true;
          level.close();  // deallocate & clear this level
          level.next();   // advance to next level
          PData.gold = 9999;
          PData.totalGold = 0x8000;
          OpenStore();    // go shopping
          level.init(); 
        }
      }
      if (dpad.y_ris()){
        dac.stopSong();
      }
      // ACTIONS
      while (!Fifo_isEmpty()){                    // clear action FIFO
        Action* act;
        Fifo_Get(&act);
        act->perform(clock);                      // perform them all
        delete act;                               // deallocate them all
      }

      updateProjectiles(clock);                   // update projectiles & check for hits
      checkProjectiles();
      // DRAWING
      Draw();                                     // draw sprites and projectiles
      drawProjectiles();
      removeDeadSprites();                        // deallocate and remove any dead sprites
      if (PData.gold != prevGold){
        hud.updateGold();                         // update hud
      }
      if (PData.health != prevHealth){
        hud.updateHealth();
      }
      // DebugOut(debug);
      if (gameSprites[0]->y < -10){               // detect if player has gone through door
        level.close();  // deallocate & clear this level
        level.next();   // advance to next level
        OpenStore();    // go shopping
        level.init();   // begin next level
      }
    }
	}
}




