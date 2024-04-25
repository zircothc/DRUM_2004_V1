void IRAM_ATTR i_ENCODERS() {
  bitWrite(ENC_PORT1,0,!digitalRead(pinR1A));
  bitWrite(ENC_PORT1,1,!digitalRead(pinR1B));
  readencoders=true;
  READ_ENCODERS();
}
void READ_ENCODERS(){
  
  if (!readencoders) return;
  
  readencoders=false; 
  
  if (selected_rot==11){
    counter1=master_filter; 
  } else if (selected_rot==12){
    counter1=octave;      
  } else if (selected_rot==8){
    counter1=bpm;
  } else if (selected_rot==9){
    counter1=master_vol;
  } else if (selected_rot==10){
    transpose=0;
    counter1=transpose;
   } else if (selected_rot==13){
    counter1=pattern_song_counter;               
  } else {
    counter1=ROTvalue[selected_sound][selected_rot];
  }
  old_counter1=counter1;

  counter1=counter1+read_encoder1();
  if (counter1>max_values[selected_rot]) counter1=max_values[selected_rot];
  if (counter1<min_values[selected_rot]) counter1=min_values[selected_rot];
  if (counter1!=old_counter1){
    
    if (selected_rot==11){
      master_filter=counter1;
      synthESP32_setMFilter(master_filter); //  filter is 0-255 and i got 0-127. inside function i 'll multiply by 2
    } else if (selected_rot==12){
      octave=counter1;
    } else if (selected_rot==8){
      bpm=counter1;
      uClock.setTempo(bpm);
    } else if (selected_rot==9){
      master_vol=counter1;
      synthESP32_setMVol(master_vol);
    } else if (selected_rot==10){
      transpose=counter1;
      // cambiar pitch en melodic
      for (byte f=0; f<16;f++){
        melodic[selected_sound][f]=melodic[selected_sound][f]+transpose;
      }
    } else if (selected_rot==13){
      pattern_song_counter=counter1;                 
    } else {
      ROTvalue[selected_sound][selected_rot]=counter1;
      setSound(selected_sound);
      int nvalue=ROTvalue[selected_sound][selected_rot];
      // arreglar pan
      if (selected_rot==6) {
        nvalue=map(nvalue,-99,99,0,127);
      } 
      // play sound if
      if (selected_rot<5){
        synthESP32_trigger(selected_sound);
      }
    }
    
    refreshOLED=true;
  }
}
int16_t read_encoder1() {
  
  // Select rotary reading mode
// static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Si dejo esto cuenta 4 pasos cada click
  static int8_t enc_states[] =  {0,-1,0,0,1,0,0, 0, 0,0,0,0,0,0, 0,0};  // Si dejo esto cuenta 1 paso  cada click

  static uint8_t old_AB1;
  old_AB1 <<= 2; //remember previous state
  old_AB1 |= ( ENC_PORT1 & 0x03 ); //add current state
  int8_t result=enc_states[( old_AB1 & 0x0f )];
  if (result>0) return (result +(9*shiftR1) ); 
  if (result<0) return (result -(9*shiftR1) ); 
  return 0;
}
