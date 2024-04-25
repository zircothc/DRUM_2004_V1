void READ_KEYPAD(){
  if (kpd.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) {
      if ( kpd.key[i].stateChanged ) {
        switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            trigger_on[kpd.key[i].kcode]=1;
            break;
          case HOLD:
            break;
          case RELEASED:
            break;
        }
      }
    }
  }
}
void DO_KEYPAD(){

  for (int f=0;f<24;f++) {
    if (trigger_on[f]==1){
      trigger_on[f]=0;
      //Serial.println(f);
      nkey=real_key[f];
      //// first 11 pad keys select rotary when shiftR1 is pressed
      if (nkey<14 && shiftR1){
        selected_rot=nkey;
        if (nkey==10) transpose=0;
        refreshOLED=true;
      //// 16 pad keys
      } else if (nkey<16){
      
        pad_touched=nkey;      
        switch (modeZ) {
          case tPiano: // 16 keys=16 notes
            synthESP32_mTrigger(selected_sound,nkey+(12*octave));
            if (recording){
              bitWrite(pattern[selected_sound],sstep,1);
              melodic[selected_sound][sstep]=nkey+(12*octave);
            }
            refreshPATTERN=true;
            break;          
          case tPad: // play pads
            if (recording){
              bitWrite(pattern[nkey],sstep,1); //!bitRead(pattern[nkey],sstep));
              melodic[selected_sound][nkey]=ROTvalue[selected_sound][3];
            } else {

                synthESP32_trigger(nkey);

              last_sound=nkey;
            }
            refreshPATTERN=true;
            break;          
          case tSel:  // Select sound
            selected_sound=nkey;
            if (selected_sound!=oldselected_sound){          
              oldselected_sound=selected_sound;
              setSound(nkey);  
            }
            last_sound=selected_sound;
            modeZ=tWrite;
            refreshPATTERN=true;
            refreshOLED=true;
            break;
          case tWrite:
            bitWrite(pattern[selected_sound],nkey,!bitRead(pattern[selected_sound],nkey));
            refreshPATTERN=true;
            melodic[selected_sound][nkey]=ROTvalue[selected_sound][3];
            break;
          case tMelod:
            bitWrite(isMelodic,nkey,!bitRead(isMelodic,nkey));
            refreshPATTERN=true;            
            synthESP32_setPitch(nkey,ROTvalue[nkey][3]);
            selected_sound=nkey;
            break;
          case tMute:
            bitWrite(mutes,nkey,!bitRead(mutes,nkey));
            refreshPATTERN=true;            
            selected_sound=nkey;
            if (selected_sound!=oldselected_sound){
              oldselected_sound=selected_sound;
            } 
            break;
          case tSolo:
            bitWrite(solos,nkey,!bitRead(solos,nkey));
            refreshPATTERN=true;            
            selected_sound=nkey;
            if (selected_sound!=oldselected_sound){
              oldselected_sound=selected_sound;
            } 
            break;
          case tClear: // clear
            pattern[nkey]=0;
            selected_sound=nkey;
            if (selected_sound!=oldselected_sound){
              oldselected_sound=selected_sound;
            } 
            break;
          case tLoadP:
            load_pattern(nkey);
            selected_pattern=nkey;
            break;
          case tSaveP:
            save_pattern(nkey);
            selected_pattern=nkey;
            modeZ=tPad; // pongo el modeZ en 0 para que no se quede en grabar
            break;
          case tLoadS:
            load_sound(nkey);
            selected_sndSet=nkey;
            break;
          case tSaveS:
            save_sound(nkey);
            selected_sndSet=nkey;
            modeZ=tPad; // pongo el modeZ en 0 para que no se quede en grabar
            break;
          case tLoaPS:
            load_pattern(nkey);
            selected_pattern=nkey;
            load_sound(nkey);
            selected_sndSet=nkey;
            break;
          case tSavPS:
            save_pattern(nkey);
            selected_pattern=nkey;
            save_sound(nkey);
            selected_sndSet=nkey;                        
            modeZ=tPad; // pongo el modeZ en 0 para que no se quede en grabar
            break;
          case tRndS:
            setRandomVoice(nkey);
            setSound(nkey);
            selected_sound=nkey;
            if (selected_sound!=oldselected_sound){
              oldselected_sound=selected_sound;
            }          
            break;
          case tRndP:
            setRandomPattern(nkey);
            setSound(nkey);
            selected_sound=nkey;
            if (selected_sound!=oldselected_sound){
              oldselected_sound=selected_sound;
            }          
            break;
          case tRndNo:
            setRandomNotes(nkey);
            setSound(nkey);
            selected_sound=nkey;
            if (selected_sound!=oldselected_sound){
              oldselected_sound=selected_sound;
            }          
            break;
          case tFirst:
            firstStep=nkey;
            break;          
          case tLast:
            newLastStep=nkey;
            break;  
          case tSong:
            song[pattern_song_counter]=nkey;
            break;       
        }
        refreshPADTOUCHED=true;
        refreshMODES=true;
      //// 8 keys
      } else {
        refreshPATTERN=true;
        clearPATTERN=true;
        refreshMODES=true;
        if (!shiftR1){
          switch (nkey) {

            // playpad/select/write
            case 16:
              // 0 - play pad sound
              // 1 - select sound
              // 2 - edit track, write
              if (modeZ>tWrite){
                modeZ=tPad;
              } else {
                modeZ++;
              }
              if (modeZ>tWrite) modeZ=tPad;
              //refreshMODES=true;  
              break;
            // random sound/pattern/Notes      
            case 17:
              if (modeZ==tRndP){
                modeZ=tRndS;
              } else if (modeZ==tRndS){
                modeZ=tRndNo;
              } else {
                modeZ=tRndP;
              }
              //refreshMODES=true;
              break;
            // load       
            case 18:
              if (modeZ>=tLoadP && modeZ<tLoaPS ) {
                modeZ=modeZ+1;
              } else {
                modeZ=tLoadP;
              }       
              break;              
            // save      
            case 19:
              if (modeZ>=tSaveP && modeZ<tSavPS ) {
                modeZ=modeZ+1;
              } else {
                modeZ=tSaveP;
              }       
              break; 
            // solos/mutes/melodic
            case 20:
              if (modeZ==tMute) {
                modeZ=tSolo;
              } else if (modeZ==tSolo){
                modeZ=tMelod;
              } else if (modeZ==tMelod){
                modeZ=tClear;
              } else if (modeZ==tClear){
                modeZ=tFirst;               
              } else if (modeZ==tFirst){
                modeZ=tLast; 
              } else {
                modeZ=tMute;
              }    
              break; 
            // piano
            case 21:
              modeZ=tPiano;
              break;
            // Play
            case 22:
              if (playing){
                //zseq.detach();
                uClock.stop();
                sstep=firstStep;
                recording=false;
                refreshSTEP=true;  
              } else {
                pattern_song_counter=-1;
                if (songing) load_flag=true;
                uClock.start();
                sstep=firstStep;
                refreshSTEP=true;  
              }
              playing=!playing; 
              break;
            // Song
            case 23:
              if (modeZ==tSong) {
                last_pattern_song=pattern_song_counter;
                pattern_song_counter=-1;
                modeZ=0;
              } else {
                modeZ=tSong;
                selected_rot=13;
                refreshOLED=true;
              }
              break;                      
          }    
        // shift 
                          
        } else {
          switch (nkey){
            // reset mutes & solos        
            case 20:
              mutes=0;
              solos=0; 
              break;
            // Play
            case 22:
              if (playing){
                recording=!recording;
              } else {
                uClock.start();
                recording=true;
                playing=true;
                sstep=firstStep;
                refreshSTEP=true;  
              }
              //refreshMODES=true;  
              break;
            case 23:
              songing=!songing;
              break;                    
          }         
        }
      }
    }
  } 
}
