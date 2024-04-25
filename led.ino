 void REFRESH_LEDS(){
  
  boolean refresh=false;

  if (refreshMODES){
    refreshMODES=false;
    refresh=true;
    // Borrar modos
    for (byte f=16;f<24;f++){
      strip.setPixelColor(real_led[f],strip.Color(0, 0, 0));  
    }
    switch (modeZ){
      case tPad:
        strip.setPixelColor(real_led[16],strip.Color(0, 200, 0));
        break;  
      case tSel:
        strip.setPixelColor(real_led[16],strip.Color(0, 0, 200));
        break;
      case tWrite:
        strip.setPixelColor(real_led[16],strip.Color(200, 0, 0));
        break; 
      case tMute:
        strip.setPixelColor(real_led[20],strip.Color(0, 0, 200));
        break;
      case tSolo:
        strip.setPixelColor(real_led[20],strip.Color(200, 0, 0));
        break; 
      case tMelod:
        strip.setPixelColor(real_led[20],strip.Color(200, 200, 0));
        break;                
      case tClear:
        strip.setPixelColor(real_led[20],strip.Color(200, 80, 0));
        break; 
      case tLoadP:
        strip.setPixelColor(real_led[18],strip.Color(0, 0, 200));
        break;
      case tLoadS:
        strip.setPixelColor(real_led[18],strip.Color(0, 200, 200));
        break;  
      case tLoaPS:
        strip.setPixelColor(real_led[18],strip.Color(100, 200, 200));
        break;                
      case tSaveP:
        strip.setPixelColor(real_led[19],strip.Color(200, 0, 0));
        break; 
      case tSaveS:
        strip.setPixelColor(real_led[19],strip.Color(200, 200, 0));
        break;
      case tSavPS:
        strip.setPixelColor(real_led[19],strip.Color(200, 200, 100));
        break;
     
      case tRndS:
        strip.setPixelColor(real_led[17],strip.Color(100, 100, 0));
        break;
      case tRndP:
        strip.setPixelColor(real_led[17],strip.Color(100, 0, 100));
        break; 
      case tRndNo:
        strip.setPixelColor(real_led[17],strip.Color(0, 100, 100));
        break; 
      case tPiano:
        strip.setPixelColor(real_led[21],strip.Color(100, 20, 60));
        break; 
      case tSong:
        strip.setPixelColor(real_led[23],strip.Color(50, 50, 200));
        break;
    }                
    // play/stop
    if (playing) {
      strip.setPixelColor(real_led[22],strip.Color(30, 200, 30));
    }
    if (recording) {
      strip.setPixelColor(real_led[22],strip.Color(200, 0, 0));
    }    
    // song
    if (songing) {
      strip.setPixelColor(real_led[23],strip.Color(100, 0, 100));
    }
  }

  if (refreshPATTERN){
    refreshPATTERN=false;
    refresh=true;
    switch (modeZ){
      case tWrite:

        for (byte f=0;f<16;f++){
          if bitRead(pattern[selected_sound],f){
            strip.setPixelColor(real_led[f],strip.Color(250, 50, 50));
          } else {
            strip.setPixelColor(real_led[f],strip.Color(0, 0, 0));     
          }
        }
        break;
      case tMute:
        //refresh=true;
        for (byte f=0;f<16;f++){
          if bitRead(mutes,f){
            strip.setPixelColor(real_led[f],strip.Color(220, 50, 0));
          } else {
            strip.setPixelColor(real_led[f],strip.Color(0, 0, 0));     
          }
        }
        break;
      case tSolo:
        //refresh=true;
        for (byte f=0;f<16;f++){
          if bitRead(solos,f){
            strip.setPixelColor(real_led[f],strip.Color(50, 220, 0));
          } else {
            strip.setPixelColor(real_led[f],strip.Color(0, 0, 0));     
          }
        }
        break;
      case tMelod:
        //refresh=true;
        for (byte f=0;f<16;f++){
          if bitRead(isMelodic,f){
            strip.setPixelColor(real_led[f],strip.Color(100, 50, 220));
          } else {
            strip.setPixelColor(real_led[f],strip.Color(0, 0, 0));     
          }
        }
        break;        
      case tFirst:
        strip.setPixelColor(real_led[firstStep],strip.Color(0, 220, 0));
        break; 
      case tLast:
        strip.setPixelColor(real_led[newLastStep],strip.Color(220, 0, 0));
        break; 
    }    
  }
  
  if (refreshSTEP){
    refreshSTEP=false;
    // Si SEQ activo muestro los pads que suenan
    if (playing ){  
       for (byte f=0;f<16;f++){
          if bitRead(pattern[f],sstep){
            strip.setPixelColor(real_led[f],strip.Color(0, 50, 200));
          }
       }

    }    
    // Mostrar step actual
    strip.setPixelColor(real_led[sstep],strip.Color(200, 200, 0));   
  }

  if (clearPATTERN){
    clearPATTERN=false;
      if (modeZ!=2){
        refresh=true;
        for (byte f=0;f<16;f++){
          strip.setPixelColor(real_led[f],strip.Color(0, 0, 0));
        }
      }

      // refresco lo que hay en el step en el siguiente bucle
      refreshPATTERN=true;
  }
  
  if (refresh) strip.show();
  
}

void REFRESH_VUMETER(){
//  if (vmeter>255) vmeter=255;
//  int xleds=map(vmeter,0,255,0,7);
//  for (byte f=24;f<32;f++){
//    strip.setPixelColor(real_led[f],strip.Color(0, 0, 0));
//  }
//  for (byte f=24;f<24+xleds;f++){
//    strip.setPixelColor(real_led[f],strip.Color(100, 255, 100));
//  } 
//  strip.show(); 
}
