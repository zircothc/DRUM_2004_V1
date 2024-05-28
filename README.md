# DRUM_2024_V1

ESP32 DRUM SYNTH MACHINE

This is my DRUM SYNTH LOFI MACHINE.

![IMG_20240406_150440](https://github.com/zircothc/DRUM_2004_V1/assets/17828930/c8327dc2-a3f7-4d81-8d82-ebfe2a7c45c3)

# Synth engine:

- Wavetable synthesizer based on DZL Arduino library ["The Synth"](https://github.com/dzlonline/the_synth)
- 16 sound polyphony
- Sound parameters: Table, Length, Envelope, Pitch, Modulation, + Volume, Pan and Filter.
- Filter (LowPassFilter) comes from [Mozzi Library](https://github.com/sensorium/Mozzi)

SEQUENCER:

- 16 step/pattern editor and random generators (pattern, sound parameters and notes)

# Hardware:

- Lolin S2 Mini (ESP32 S2)
- PCM5102A I2s dac
- 24 push buttons (8x3)
- Rotary encoder
- OLED display I2c
- 32 LED WS2812B

# Software:

IDE:
Arduino `1.8.19` 
Arduino `2.3.2`

Boards:
Expressif Systems `2.0.16` (Tested fine up to `2.0.17`)

Board: Lolin S2 Mini

Necessary Libraries:

- Sequencer Timer - [uClock](https://github.com/midilab/uClock)
- RGB Leds - [Adafruit Neopixel](https://github.com/adafruit/Adafruit_NeoPixel)
- OLED - [u8g2](https://github.com/olikraus/u8g2)
- Button input - [Keypad](https://github.com/Chris--A/Keypad)

# Notes:

Schematics uploaded.

STL 3D model uploaded.

Cheat sheet style PDF uploaded.

Join solder pads near SCK pin on PCM5102A module.

**Update**: 

- Solder pads on the back of PCM5102A module ([more info](https://github.com/pschatzmann/ESP32-A2DP/wiki/External-DAC#pcm5102-dac))
  
  - H1L: FLT - Low
  
  - H2L: DEMP - Low
  
  - H3L: XSMT - High
  
  - H4L: FMT - Low

Video demo of the prototype:

[![IMG_20240406_150231](https://img.youtube.com/vi/rXl1gpWJp-g/0.jpg)](https://www.youtube.com/watch?v=rXl1gpWJp-g)

Prototype:
![IMG_20240406_150231](https://github.com/zircothc/DRUM_2004_V1/assets/17828930/feb9b928-f76a-4b51-93ea-a7afbd6a5c28)

# PCB, PROJECT & FINAL LOOK:

https://oshwlab.com/zircothc/esp32-drum-synth-lofi-machine

https://easyeda.com/editor#project_id=08f629c08b18444ca16accbb2242c0fb

PCB
![pcb por ambas partes](https://github.com/zircothc/DRUM_2004_V1/assets/17828930/b96a9900-0efa-4e17-8604-94ad5a8391e3)

PARTS MOUNTED
![soldado](https://github.com/zircothc/DRUM_2004_V1/assets/17828930/691a3e87-74a0-4ee9-bb83-77bda648d8eb)
![soldado por detras](https://github.com/zircothc/DRUM_2004_V1/assets/17828930/2e9d8d91-a247-4251-9387-837ad2b6a9ef)

SIZE: FINAL BUILD vs PROTOTYPE
![SIZE](https://github.com/zircothc/DRUM_2004_V1/assets/17828930/9eb3fe80-272a-4285-b9d5-6d1e910351dc)

TWO MACHINES IN SYNC
![sync](https://github.com/zircothc/DRUM_2004_V1/assets/17828930/0adc1eed-482d-4931-8327-f6911d9ab73b)
