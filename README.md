# DRUM_2004_V1
ESP32 DRUM SYNTH MACHINE

This is my DRUM SYNTH LOFI MACHINE.

Hardware:
- Lolin S2 Mini (ESP32 S2)
- PCM5102A I2s dac
- 24 push buttons (8x3)
- Rotary encoder
- OLED display I2c
- 32 LED WS2812B
  
Synth engine:
- Wavetable synthesizer based on DZL Arduino library "The Synth" (github.com/dzlonline/the_synth)
- 16 sound polyphony
- Sound parameters: Table, Length, Envelope, Pitch, Modulation, + Volume, Pan and Filter.
* Filter (LowPassFilter) comes from Mozzi library (github.com/sensorium/Mozzi)

Sequencer:
16 step/pattern editor and random generators (pattern, sound parameters and notes)

IDE:
Arduino 1.8.19

Boards:
Expressif Systems 2.0.14
