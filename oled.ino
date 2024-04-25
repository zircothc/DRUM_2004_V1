void REFRESH_OLED(){
  refreshOLED=false;
  u8g2.clearBuffer();
  //u8g2.drawFrame(0,0,31,127);
  u8g2.setFont(u8g2_font_scrum_tr); // set fonts

  u8g2.setCursor(2,11);
  u8g2.print("S"); 
  u8g2.setCursor(14,11); 
  u8g2.print(selected_sound);
  
  u8g2.drawHLine(0,15,31);  

  u8g2.setCursor(3,26);
  u8g2.print(trot[selected_rot]);
  
  u8g2.setCursor(3,41); 
  if (selected_rot==11){
     u8g2.print(master_filter); 
  } else if (selected_rot==12){
    u8g2.print(octave);      
  } else if (selected_rot==8){
    u8g2.print(bpm);
  } else if (selected_rot==9){
     u8g2.print(master_vol);
  } else if (selected_rot==10){
     u8g2.print(transpose);
   } else if (selected_rot==13){
     u8g2.print(pattern_song_counter);
  } else {
    u8g2.print(ROTvalue[selected_sound][selected_rot]);  
  }
      


  u8g2.drawHLine(0,45,31);

  u8g2.setCursor(2,56);
  u8g2.print("P");
  u8g2.setCursor(14,56);
  u8g2.print(selected_pattern);  

  u8g2.drawHLine(0,60,31); 
  u8g2.setCursor(2,71);
  u8g2.print("s");
  u8g2.setCursor(14,71);
  u8g2.print(selected_sndSet);
    
  u8g2.setFont(u8g2_font_5x7_tf);
  if (selected_rot==13){
  u8g2.setCursor(3,91); 
  u8g2.print(song[pattern_song_counter]);    
  }
  u8g2.setCursor(3,101); 
  u8g2.print(tmodeZ[modeZ]);
  
  u8g2.drawFrame(0,0,31,76);

  u8g2.sendBuffer();  
}
