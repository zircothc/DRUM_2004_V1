////////////////////////////
// DRUM 2024              //
////////////////////////////
// ZCARLOS 2024
// Based on DZL's library "the_synth"
// Ported to ESP32 internal DACs but...
// DAC outs sounds very noisy
// Instead of modify the library I cut it into functions to try to port to pcm5102a i2s DAC out, DONE :)
// Extended tables up to 14 and voices up to 16
// Mixer section (L&R) per voice
// Filter per voice and master filter (from Mozzi library)


// includes
#include <Arduino.h>
////////////////////////////// SPIFFS
#include <FS.h>
#include <SPIFFS.h>

////////////////////////////// OLED
#include <U8g2lib.h>
#include <Wire.h>
////////////////////////////// SYNTH
#define SAMPLE_RATE 22050
#include "driver/i2s.h"
#include "synthESP32LowPassFilter_E.h" // filter
#include "tablesESP32_E.h"


////////////////////////////// LEDS
#include <Adafruit_NeoPixel.h>
////////////////////////////// TIMER SEQ 
#include <uClock.h>
////////////////////////////// KEYPAD
#include <Keypad.h>
////////////////////////////// MIDI USB
//#include <Adafruit_TinyUSB.h>
//#include <MIDI.h>


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////// OLED
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R3, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 5, /* data=*/ 3);   // pin remapping with ESP8266 HW I2C

const String trot[18] = { "WAV", "ENV", "LEN", "PIT", "MOD", "VOL", "PAN", "FIL", "BPM","MVO","TRP","MFI","OCT","PSN","GP1","GD1","GP2","GD2"};
const String tmodeZ[20] = { "Pad", "Sel", "Write", "Mute", "Solo", "Clear","LoadP","LoadS","LoaPS","SaveP","SaveS","SavPS","RndS","RndP","First","Last","Melod","RndNo","Piano","Song"};

// modeZ types

//  0 - play pad sound
//  1 - select sound
//  2 - edit track, write
//  3 - mute
//  4 - solo
//  5 - clear

//  6 - load pattern
//  7 - load pattern
//  8 - load pattern and sound

//  9 - save pattern
// 10 - save sound
// 11 - save sound and pattern
 
// 12 - random sound
// 13 - random pattern
// 14 - first step
// 15 - last step
// 16 - set melodic (read pitch from pattern melodic instead of rotary value)
// 17 - random note
// 18 - piano
// 10 - song (edit song)

#define tPad    0
#define tSel    1
#define tWrite  2
#define tMute   3
#define tSolo   4
#define tClear  5
#define tLoadP  6
#define tLoadS  7
#define tLoaPS  8
#define tSaveP  9
#define tSaveS 10
#define tSavPS 11
#define tRndS  12
#define tRndP  13
#define tFirst 14
#define tLast  15
#define tMelod 16
#define tRndNo 17
#define tPiano 18
#define tSong  19

////////////////////////////// SYNTH

// SINE     0
// TRIANGLE 1
// SQUARE   2
// SAW      3
// RAMP     4
// NOISE    5
// PHASOR   6
// WN1      7
// WN2      8
// WN3      9
// WN4      10
// WN5      11
// WN6      12
// WN7      13
//
// ENVELOPE0 0
// ENVELOPE1 1
// ENVELOPE2 2
// ENVELOPE3 3


#define SAMPLE_RATE 22050

// i2s

#define I2S_BCK_PIN 33
#define I2S_WS_PIN 37
#define I2S_DATA_OUT_PIN 35

#define DMA_BUF_LEN     32          
#define DMA_NUM_BUF     2

static uint16_t out_buf[DMA_BUF_LEN * 2];

// Filters master (L&R) and 16 sounds
LowPassFilter lpfR;
LowPassFilter lpfL;

LowPassFilter lpf0;
LowPassFilter lpf1;
LowPassFilter lpf2;
LowPassFilter lpf3;
LowPassFilter lpf4;
LowPassFilter lpf5;
LowPassFilter lpf6;
LowPassFilter lpf7;
LowPassFilter lpf8;
LowPassFilter lpf9;
LowPassFilter lpf10;
LowPassFilter lpf11;
LowPassFilter lpf12;
LowPassFilter lpf13;
LowPassFilter lpf14;
LowPassFilter lpf15;

const int cutoff=255;
const int reso=511;

int vmeter=0;

static unsigned int PCW[16] = {0, 0, 0, 0,0 ,0 ,0 ,0,0, 0, 0, 0,0 ,0 ,0 ,0};      //-Wave phase accumolators
static unsigned int FTW[16] = {1000, 200, 300, 400, 1000, 200, 300, 400, 1000, 200, 300, 400, 1000, 200, 300, 400};           //-Wave frequency tuning words
static unsigned char AMP[16] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};           //-Wave amplitudes [0-255]
static unsigned int PITCH[16] = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};          //-Voice pitch
static int MOD[16] = {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};                      //-Voice envelope modulation [0-1023 512=no mod. <64 pitch down >64 pitch up]

static int VOL_R[16] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}; 
static int VOL_L[16] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};   
static int PAN[16] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}; 
static int FILTER[16]= {127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};                  //-mozzi FILTERS  0-255

static unsigned int wavs[16];                                  //-Wave table selector [address of wave in memory]
static unsigned int envs[16];                                  //-Envelope selector [address of envelope in memory]
static unsigned int EPCW[16] = {0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000}; //-Envelope phase accumulator
static unsigned int EFTW[16] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};               //-Envelope speed tuning word
static unsigned int divider = 16;                             //-Sample rate decimator for envelope

static int mvol=10;
int master_vol=17;
int master_filter=0;
int octave=5;

////////////////////////////// LEDS
#define LED_PIN    39
#define LED_COUNT 32
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Set more friendly led numbers
// 0 to 15 as 16 step, 16 sounds,... and 16 to 23 as selected modes
const byte real_led[32]={ 8, 9,10,11,24,25,26,27,15,14,13,12,31,30,29,28,
                          0, 1, 2, 3,16,17,18,19, 7, 6, 5, 4,23,22,21,20};
int counter_refresh=0;
const byte limit_counter_refresh=7;
boolean start_counter_refresh=false;

////////////////////////////// MIDI USB
//Adafruit_USBD_MIDI usb_midi;
//MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);


////////////////////////////// TIMER SEQ 
int bpm=120;
int stepInterval;
int microStep=0;
byte selected_pattern=0;
byte sstep=0;
byte selected_len=16;
boolean updateBPM=false;
uint16_t mutes=0;
uint16_t solos=0;
uint16_t pattern[16];
byte song[255];
int pattern_song_counter=-1;
byte last_pattern_song=255;
uint8_t  melodic[16][16];
//uint16_t isMelodic=B11111111<<8 | B00000000; // last 8 sounds are set to melodic
uint16_t isMelodic=B00000000<<8 | B00000000; // none set to melodic
byte firstStep=0;
byte lastStep=15;
byte newLastStep=15; 
byte selected_sound=0;
byte oldselected_sound=0;
byte selected_sndSet=0;
int transpose=0;
boolean load_flag=false;

////////////////////////////// ROTARY
void IRAM_ATTR i_ENCODERS();
const byte pinBR1=9;
const byte pinR1A=11;
const byte pinR1B=12;
byte ENC_PORT1=0;
int counter1=0;
int old_counter1=0;
boolean readencoders=false;
byte shiftR1=false;
byte RV;

// 8 sound parameters + bpm + master vol + transpose + master filter + octave + Pattern song selector +  4 Audino parameters
const int max_values[18]={13,3,127,127,127,31, 99,127,400,31, 1,127,10,255,511,127,511,255}; 
const int min_values[18]={ 0,0,  0,  0,  0, 0,-99,  0,  0, 0,-1,  0, 0,  0,0,0,0,0};

int ROTvalue[16][8]={ // init sound values
  { 0,2, 32, 35,29,7,-31,0}, // THIS IS VOICE #0
  { 5,1, 50,127,31,7,-31,0}, // THIS IS VOICE #1
  { 9,2, 25, 41,95,7,  0,0}, // ...
  { 0,0, 60, 64,30,7,  0,0},
  {13,2, 47, 44,82,7,  0,0},
  { 8,2, 42, 29,31,7,  0,0},
  { 0,0, 30, 54,64,7,  0,0},
  { 3,0, 25, 44,64,7,  0,0},
  
  { 0,2, 32, 25,29,7, 66,0},
  { 5,2, 50,107,31,7, 66,0},
  { 9,2, 25, 31,95,7,  0,0},
  { 0,0, 60, 54,30,7,  0,0},
  {13,2, 47, 54,82,7,  0,0},
  { 8,2, 42, 19,31,7,  0,0},
  { 0,0, 40, 44,64,7, 30,0},
  { 3,0, 80, 34,64,7,-30,0}
};

byte selected_rot=0;

////////////////////////////// KEYPAD

const byte ROWS = 8; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
  {'a','s','d'},
  {'z','x','c'},
  {'f','g','h'},
  {'v','b','n'}  
};
// need to confirm if this is the right order in the PCB version
byte rowPins[ROWS] = {14, 13, 10, 8,6,4,2,7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {40,38,36}; //connect to the column pinouts of the keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
byte trigger_on[24];
byte nkey;

// Set more friendly key numbers
// 0 to 15 as 16 step, 16 sounds,... and 16 to 23 as selected modes
const byte real_key[24]={ 
 0, 8,16,
 1, 9,17,
 2,10,18,
 3,11,19,
 4,12,20,
 5,13,21,
 6,14,22,
 7,15,23
};

byte pad_touched=0;

////////////////////////////// MISC
byte modeZ=0;
byte last_modeZ=0;
byte last_sound=0;

boolean playing   = false;
boolean songing   = false; // switch to make load auto patterns++
boolean recording = false;
//boolean shifting  = false;

boolean clearPATTERN=false;
boolean clearSTEP=false;
boolean refreshPATTERN=true;
boolean refreshSTEP=false;
boolean refreshSTATUS=false;
boolean refreshMODES=true;
boolean refreshOLED=true;
boolean refreshPADTOUCHED=false;
boolean stepzero = false; // tool to reset the first step and last step the seq searchs for start/end point of the loop
#define MIDI_CLOCK 0xF8
#define MIDI_START 0xFA
#define MIDI_STOP  0xFC


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void setSound(byte f){
  synthESP32_setWave(f,ROTvalue[f][0]);  
  synthESP32_setEnvelope(f,ROTvalue[f][1]);
  synthESP32_setLength(f,ROTvalue[f][2]);
  synthESP32_setPitch(f,ROTvalue[f][3]);
  synthESP32_setMod(f,ROTvalue[f][4]); 
  synthESP32_setVol(f,ROTvalue[f][5]);  
  synthESP32_setPan(f,ROTvalue[f][6]);  
  synthESP32_setFilter(f,ROTvalue[f][7]);  
}
void setRandomVoice(byte f){
  ROTvalue[f][0]=random(0, 14);
  ROTvalue[f][1]=random(0, 4);
  ROTvalue[f][2]=random(1, 60);
  ROTvalue[f][3]=random(0, 127);
  ROTvalue[f][4]=random(10,100);
  ROTvalue[f][5]=random(10,15);
  ROTvalue[f][6]=random(-99,99);
  ROTvalue[f][7]=random(0,60);
}
void setRandomPattern(byte f){
  byte veces=random(0,2);
  for (byte b = 0; b < 16; b++) {
    byte mybit=random(0,2);
    if (veces) {
      if (mybit) mybit=random(0,2); // Si es 1 hago otro random para que haya menos unos
    }
    bitWrite(pattern[f],b,mybit);
  } 
  setRandomPitch(f);
}

void setRandomPitch(byte f){
  // Tomo como referencia para el rango el valor del pot pitch actual
  uint8_t actual=ROTvalue[selected_sound][3];
  uint8_t limite;
  uint8_t prelimite=24;
  if (shiftR1) prelimite = 24;
  for (byte b = 0; b < 16; b++) {
     limite=actual+random(0,prelimite);
     melodic[f][b]=random(actual-(prelimite>>1),limite);
  }  
}

void setRandomNotes(byte f){
  setRandomPitch(f);
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void onSync24Callback(uint32_t tick){

  
  // Lanzar cambio de step
  if (!(tick % (6))) tic(tick);
  // Limpiar patrón
  if ((tick % (6))==4) clearPATTERN=true;

  
}

void tic(uint32_t tick){
  
//  if (playing){
    //if (microStep==0){
      clearSTEP=true;
      for (int f = 0; f < 16; f++) { 
        if (!bitRead(mutes, f)) {
          if (solos == 0 || (solos > 0 && bitRead(solos, f))) {
            if (bitRead(pattern[f], sstep)) { // note on
              if (bitRead(isMelodic,f)){
                //synthESP32_setPitch(f,melodic[f][sstep]);

                  synthESP32_mTrigger(f,melodic[f][sstep]);                  

              } else {
                // reestablecer el pitch
                synthESP32_setPitch(f,ROTvalue[f][3]);                
                synthESP32_trigger(f);
              }
            } 
          }
        }
      }
  
      sstep++;
      // Comprobar step final
      if (sstep==(lastStep+1) || sstep==(newLastStep+1) || (stepzero) || sstep==16) {
        lastStep=newLastStep;
        stepzero=false;
        sstep=firstStep;
        //Serial.println("Toc");
        if (songing){
          load_flag=true; // inside loop I will load next pattern
        }
      }
      refreshSTEP=true;
      refreshPATTERN=true; 
//    }


    

}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void load_pattern(byte pat){
  
  // Leer pattren
  String nombreArchivoP = "/PAT" + String(pat);
  File archivoP = SPIFFS.open(nombreArchivoP, FILE_READ);   
  if (!archivoP) {
    Serial.println("Error al abrir el archivo para leer");
    return;
  }
  int fila = 0;
  while (archivoP.available()) {
    pattern[fila] = (archivoP.readStringUntil('\n')).toInt();
    //Serial.println(pattern[fila]);
    fila++;
  }
  archivoP.close();
  //Serial.println("-");


  // Leer melodic
  String nombreArchivoM = "/MED" + String(pat);
  File archivoM = SPIFFS.open(nombreArchivoM, FILE_READ);
  if (!archivoM) {
    Serial.println("Error al abrir el archivo para leer");
    return;
  }
  fila = 0;
  int columna = 0;  
  while (archivoM.available()) {
    melodic[fila][columna] = (archivoM.readStringUntil('\n')).toInt();
    columna++;
    if (columna == 16) {
      fila++;
      columna = 0;
    }
  }
  archivoM.close(); 

  
}

void load_sound(byte pat){
  
  // Leer sound
  String nombreArchivoS = "/SND" + String(pat);
  File archivoS = SPIFFS.open(nombreArchivoS, FILE_READ);
  if (!archivoS) {
    Serial.println("Error al abrir el archivo para leer");
    return;
  }
  int fila = 0;
  int columna = 0;  
  while (archivoS.available()) {
    ROTvalue[fila][columna] = (archivoS.readStringUntil('\n')).toInt();
    columna++;
    if (columna == 8) {
      // Set voices
      setSound(fila);
      fila++;
      columna = 0;
    }
  }
  archivoS.close(); 

}

void save_pattern(byte pat){
  
  // Guardar pattern
  String nombreArchivoP = "/PAT" + String(pat);
  File archivoP = SPIFFS.open(nombreArchivoP, FILE_WRITE);   
  if (!archivoP) {
    Serial.println("Error al abrir el archivo para escribir");
    return;
  }
  for (int i = 0; i < 16; i++) {
    archivoP.println(pattern[i]);
    //Serial.println(pattern[i]);
  }
  archivoP.close();
  //Serial.println("-");  

  // Guardar melodic
  String nombreArchivoM = "/MED" + String(pat);
  File archivoM = SPIFFS.open(nombreArchivoM, FILE_WRITE); 
  if (!archivoM) {
    Serial.println("Error al abrir el archivo para escribir");
    return;
  }
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      archivoM.println(melodic[i][j]);
    }
  }
  archivoM.close();

  
}
void save_sound(byte pat){
  
  // Guardar sound
  String nombreArchivoS = "/SND" + String(pat);
  File archivoS = SPIFFS.open(nombreArchivoS, FILE_WRITE); 
  if (!archivoS) {
    Serial.println("Error al abrir el archivo para escribir");
    return;
  }
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 8; j++) {
      archivoS.println(ROTvalue[i][j]);
    }
  }
  archivoS.close();

}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////  S E T U P  //////////////////////////////
void setup() {
  btStop();
  // Serial
  Serial.begin(115200);
  // MISI USB
//  MIDI.begin(MIDI_CHANNEL_OMNI);
//  MIDI.setHandleNoteOn(handleNoteOn);  
//  MIDI.setHandleControlChange(handleCC);
//  // check device mounted
//  if ( !TinyUSBDevice.mounted() ){
//    Serial.println("Error USB");
//  }
  
  // Synth
  synthESP32_begin();
  // Set 16 voices
  for (byte f=0;f<16;f++){
    setSound(f);
  }
  // Set master vol
  synthESP32_setMVol(master_vol);
  // Set master filter
  synthESP32_setMFilter(master_filter);  


  
  // SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar el sistema de archivos SPIFFS");
    return;
  } 
  
  // Leds
  strip.begin();
  strip.clear();           
  strip.show();            
  strip.setBrightness(50);
  // Seq


   // fill melodic with 60
  for  (byte a=0;a<16;a++){
    for  (byte b=0;b<16;b++){
     melodic[a][b]=60;
    }    
  }
  // set 8-15 as melodic;
  isMelodic=B11111111<<8 | B00000000;
  // Setup our clock system
  uClock.init();
  //uClock.setOnStep(onStepCallback);
  uClock.setOnSync24(onSync24Callback);
  uClock.setTempo(bpm);

//    // demo pattern
//    pattern[0]=B00010001<<8 | B10010001;
//    pattern[2]=B11000101<<8 | B10101011;
//    pattern[6]=B10110101<<8 | B10001111;
//    pattern[3]=B00000110<<8 | B00000000;
//    pattern[7]=B00000000<<8 | B01001000;
//    // start playing demo pattern
//    uClock.start();
//    sstep=firstStep;
//    refreshSTEP=true; 
//    playing=true;

  // Oled
  u8g2.begin();
  // Rotary
  pinMode(pinR1A,INPUT_PULLUP);
  pinMode(pinR1B,INPUT_PULLUP);
  pinMode(pinBR1,INPUT_PULLUP);
  attachInterrupt(pinR1A, i_ENCODERS, CHANGE);  
  attachInterrupt(pinR1B, i_ENCODERS, CHANGE);  
}

//////////////////////////////  L O O P  //////////////////////////////

void loop() {

  // flag to do things outside sequencer timer isr
  if (load_flag){
    load_flag=false;

    pattern_song_counter++;
    if (pattern_song_counter==255) pattern_song_counter=0;
    if (pattern_song_counter>last_pattern_song) pattern_song_counter=0; 
        
    load_pattern(song[pattern_song_counter]);
    selected_pattern=song[pattern_song_counter];
    load_sound(song[pattern_song_counter]);
    selected_sndSet=song[pattern_song_counter];
           
    refreshOLED=true;
  }

  // Read MIDI ports
  //MIDI.read();

  shiftR1=!digitalRead(pinBR1);
  READ_ENCODERS(); 
  READ_KEYPAD(); 
  DO_KEYPAD();
  REFRESH_LEDS();
  REFRESH_OLED();
  REFRESH_VUMETER();


}
