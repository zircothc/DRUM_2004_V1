
void synthESP32_begin(){

  lpfL.setResonance(reso);
  lpfR.setResonance(reso);
 
  lpfL.setCutoffFreq(cutoff);
  lpfR.setCutoffFreq(cutoff); 
  
  lpf0.setResonance(reso);
  lpf1.setResonance(reso);
  lpf2.setResonance(reso);
  lpf3.setResonance(reso);
  lpf4.setResonance(reso);
  lpf5.setResonance(reso);
  lpf6.setResonance(reso);
  lpf7.setResonance(reso);
  lpf8.setResonance(reso);
  lpf9.setResonance(reso);
  lpf10.setResonance(reso);
  lpf11.setResonance(reso);
  lpf12.setResonance(reso);
  lpf13.setResonance(reso);
  lpf14.setResonance(reso);
  lpf15.setResonance(reso);
  lpf0.setCutoffFreq(cutoff);
  lpf1.setCutoffFreq(cutoff);
  lpf2.setCutoffFreq(cutoff);
  lpf3.setCutoffFreq(cutoff);
  lpf4.setCutoffFreq(cutoff);
  lpf5.setCutoffFreq(cutoff);
  lpf6.setCutoffFreq(cutoff);
  lpf7.setCutoffFreq(cutoff);
  lpf8.setCutoffFreq(cutoff);
  lpf9.setCutoffFreq(cutoff);
  lpf10.setCutoffFreq(cutoff);
  lpf11.setCutoffFreq(cutoff);
  lpf12.setCutoffFreq(cutoff);
  lpf13.setCutoffFreq(cutoff);
  lpf14.setCutoffFreq(cutoff);
  lpf15.setCutoffFreq(cutoff);
      
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX ),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    //.communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2,
    .dma_buf_count = DMA_NUM_BUF,
    .dma_buf_len = DMA_BUF_LEN,
    .use_apll = true,
  };

  i2s_pin_config_t i2s_pin_config = {
    .bck_io_num = I2S_BCK_PIN,
    .ws_io_num =  I2S_WS_PIN,
    .data_out_num = I2S_DATA_OUT_PIN
  };


  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

  i2s_set_pin(I2S_NUM_0, &i2s_pin_config);
  //i2s_zero_dma_buffer(I2S_NUM_0);


    // Highest possible priority for realtime audio task
    xTaskCreate(audio_task, "audio", 8000, NULL, configMAX_PRIORITIES - 1, NULL); 
}
static void write_buffer() {
	
	size_t bytes_written;

  for (int i=0; i < DMA_BUF_LEN; i++) {
	
	
  //-------------------------------
  // Time division
  //-------------------------------
  divider++;
  if (divider>=16) divider=0;
  
  //-------------------------------
  // Volume envelope generator
  //-------------------------------

   if (!(((unsigned char*)&EPCW[divider])[1]&0x80)) {
   AMP[divider] = wenvs[envs[divider]][ (((unsigned char*)&(EPCW[divider]+=EFTW[divider]))[1])];

	
  } else {
    AMP[divider] = 0;
  }
  
	
  //-------------------------------
  //  Synthesizer/audio mixer
  //-------------------------------

	// Generate sounds
	
	int32_t sound_A[16];
 
	sound_A[0]=(((signed char)(wtables[wavs[0] ][((unsigned char *)&(PCW[0]  += FTW[0] ))[1]]) * AMP[0]   ) >> 8);
	sound_A[1]=(((signed char)(wtables[wavs[1] ][((unsigned char *)&(PCW[1]  += FTW[1] ))[1]]) * AMP[1]   ) >> 8);
	sound_A[2]=(((signed char)(wtables[wavs[2] ][((unsigned char *)&(PCW[2]  += FTW[2] ))[1]]) * AMP[2]   ) >> 8);
	sound_A[3]=(((signed char)(wtables[wavs[3] ][((unsigned char *)&(PCW[3]  += FTW[3] ))[1]]) * AMP[3]   ) >> 8);
	sound_A[4]=(((signed char)(wtables[wavs[4] ][((unsigned char *)&(PCW[4]  += FTW[4] ))[1]]) * AMP[4]   ) >> 8);
	sound_A[5]=(((signed char)(wtables[wavs[5] ][((unsigned char *)&(PCW[5]  += FTW[5] ))[1]]) * AMP[5]   ) >> 8);
	sound_A[6]=(((signed char)(wtables[wavs[6] ][((unsigned char *)&(PCW[6]  += FTW[6] ))[1]]) * AMP[6]   ) >> 8);	
	sound_A[7]=(((signed char)(wtables[wavs[7] ][((unsigned char *)&(PCW[7]  += FTW[7] ))[1]]) * AMP[7]   ) >> 8);	
	sound_A[8]=(((signed char)(wtables[wavs[8] ][((unsigned char *)&(PCW[8]  += FTW[8] ))[1]]) * AMP[8]   ) >> 8);	
	sound_A[9]=(((signed char)(wtables[wavs[9] ][((unsigned char *)&(PCW[9]  += FTW[9] ))[1]]) * AMP[9]   ) >> 8);	
	sound_A[10]=(((signed char)(wtables[wavs[10] ][((unsigned char *)&(PCW[10]  += FTW[10] ))[1]]) * AMP[10]   ) >> 8);
	sound_A[11]=(((signed char)(wtables[wavs[11] ][((unsigned char *)&(PCW[11]  += FTW[11] ))[1]]) * AMP[11]   ) >> 8);
	sound_A[12]=(((signed char)(wtables[wavs[12] ][((unsigned char *)&(PCW[12]  += FTW[12] ))[1]]) * AMP[12]   ) >> 8);
	sound_A[13]=(((signed char)(wtables[wavs[13] ][((unsigned char *)&(PCW[13]  += FTW[13] ))[1]]) * AMP[13]   ) >> 8);
	sound_A[14]=(((signed char)(wtables[wavs[14] ][((unsigned char *)&(PCW[14]  += FTW[14] ))[1]]) * AMP[14]   ) >> 8);
	sound_A[15]=(((signed char)(wtables[wavs[15] ][((unsigned char *)&(PCW[15]  += FTW[15] ))[1]]) * AMP[15]   ) >> 8);


  //taskYIELD(); 
  
  // tracks filter
  sound_A[0]=lpf0.next(sound_A[0])<<0;
  sound_A[1]=lpf1.next(sound_A[1])<<0;
  sound_A[2]=lpf2.next(sound_A[2])<<0;
  sound_A[3]=lpf3.next(sound_A[3])<<0;
  sound_A[4]=lpf4.next(sound_A[4])<<0;
  sound_A[5]=lpf5.next(sound_A[5])<<0;
  sound_A[6]=lpf6.next(sound_A[6])<<0;
  sound_A[7]=lpf7.next(sound_A[7])<<0;
  sound_A[8]=lpf8.next(sound_A[8])<<0;
  sound_A[9]=lpf9.next(sound_A[9])<<0;
  sound_A[10]=lpf10.next(sound_A[10])<<0;
  sound_A[11]=lpf11.next(sound_A[11])<<0;
  sound_A[12]=lpf12.next(sound_A[12])<<0;
  sound_A[13]=lpf13.next(sound_A[13])<<0;
  sound_A[14]=lpf14.next(sound_A[14])<<0;
  sound_A[15]=lpf15.next(sound_A[15])<<0;

  taskYIELD(); 
  
	// L & R outputs
	
	int32_t loutput= 127 +
	((
	((sound_A[0] * VOL_L[0]   ) >> 3 )+
	((sound_A[1] * VOL_L[1]   ) >> 3 )+
	((sound_A[2] * VOL_L[2]   ) >> 3 )+
	((sound_A[3] * VOL_L[3]   ) >> 3 )+
	((sound_A[4] * VOL_L[4]   ) >> 3 )+
	((sound_A[5] * VOL_L[5]   ) >> 3 )+
	((sound_A[6] * VOL_L[6]   ) >> 3 )+
	((sound_A[7] * VOL_L[7]   ) >> 3 )+
	((sound_A[8] * VOL_L[8]   ) >> 3 )+
	((sound_A[9] * VOL_L[9]   ) >> 3 )+
	((sound_A[10]* VOL_L[10]  ) >> 3 )+
	((sound_A[11]* VOL_L[11]  ) >> 3 )+
	((sound_A[12]* VOL_L[12]  ) >> 3 )+
	((sound_A[13]* VOL_L[13]  ) >> 3 )+
	((sound_A[14]* VOL_L[14]  ) >> 3 )+
	((sound_A[15]* VOL_L[15]  ) >> 3 ) 
    ) >> 3);

  //loutput += delay1.next(loutput)>>1;
	
	int32_t routput= 127 +
	((
	((sound_A[0] * VOL_R[0]   ) >> 3 )+
	((sound_A[1] * VOL_R[1]   ) >> 3 )+
	((sound_A[2] * VOL_R[2]   ) >> 3 )+
	((sound_A[3] * VOL_R[3]   ) >> 3 )+
	((sound_A[4] * VOL_R[4]   ) >> 3 )+
	((sound_A[5] * VOL_R[5]   ) >> 3 )+
	((sound_A[6] * VOL_R[6]   ) >> 3 )+
	((sound_A[7] * VOL_R[7]   ) >> 3 )+
	((sound_A[8] * VOL_R[8]   ) >> 3 )+
	((sound_A[9] * VOL_R[9]   ) >> 3 )+
	((sound_A[10]* VOL_R[10]  ) >> 3 )+
	((sound_A[11]* VOL_R[11]  ) >> 3 )+
	((sound_A[12]* VOL_R[12]  ) >> 3 )+
	((sound_A[13]* VOL_R[13]  ) >> 3 )+
	((sound_A[14]* VOL_R[14]  ) >> 3 )+
	((sound_A[15]* VOL_R[15]  ) >> 3 ) 
    ) >> 3); 
  
	uint8_t lrefilter=lpfL.next(loutput)<<0;
	uint8_t rrefilter=lpfR.next(routput)<<0;
	
	int16_t lsample_out=(lrefilter*mvol)>>5;
	int16_t rsample_out=(rrefilter*mvol)>>5;	
  
	if (lsample_out>250) lsample_out=250;
	if (rsample_out>250) rsample_out=250;

  out_buf[i*2]   = (uint16_t)lsample_out<<8;  
  out_buf[i*2+1] = (uint16_t)rsample_out<<8;
  
//  vmeter=lsample_out;

    //************************************************
    //  Modulation engine
    //************************************************

    FTW[divider] = PITCH[divider] + (int)   (((PITCH[divider]>>6)*(EPCW[divider]>>6))>>7)*MOD[divider];
  
	} // fin bucle del buffer de audio

  // escribir buffer audio
  
	i2s_write(I2S_NUM_0, out_buf, sizeof(out_buf), &bytes_written, portMAX_DELAY);
	
}

////////////////////////////////////////////////////////////////////////////////

static void audio_task(void *userData){
  while(1) {
      write_buffer();      
  }
} 

//*********************************************************************
//  Setup all voice parameters in MIDI range
//  voice[0-3],wave[0-6],pitch[0-127],envelope[0-4],length[0-127],mod[0-127:64=no mod],
//  vol[0-31], filter[0-255]
//*********************************************************************
// I don't use this function. I call setup voices calling setsound()
// void synthESP32_setupVoice(unsigned char voice, unsigned char wave, unsigned char pitch, unsigned char env, unsigned char length, unsigned char mod, unsigned char vol, signed char pan, unsigned char filter) {
//    synthESP32_setWave(voice, wave);
//    synthESP32_setPitch(voice, pitch);
//    synthESP32_setEnvelope(voice, env);
//    synthESP32_setLength(voice, length);
//    synthESP32_setMod(voice, mod);
//    synthESP32_setVol(voice, vol);
//    synthESP32_setPan(voice, pan);
//    synthESP32_setFilter(voice, filter);
// }
//*********************************************************************
//  Setup wave [0-13]
//*********************************************************************

void synthESP32_setWave(unsigned char voice, unsigned char wave){
  wavs[voice]=wave;
}
//*********************************************************************
//  Setup Pitch [0-127]
//*********************************************************************

void synthESP32_setPitch(unsigned char voice,unsigned char MIDInote) {
  PITCH[voice]=PITCHS[MIDInote];
}

//*********************************************************************
//  Setup Envelope [0-3]
//*********************************************************************

void synthESP32_setEnvelope(unsigned char voice, unsigned char env) {
  envs[voice]=env;
}

//*********************************************************************
//  Setup Length [0-127]
//*********************************************************************

void synthESP32_setLength(unsigned char voice,unsigned char length) {
  EFTW[voice]=EFTWS[length];
}

//*********************************************************************
//  Setup mod [0-127][64 no mod]
//*********************************************************************

void synthESP32_setMod(unsigned char voice,unsigned char mod) {
  MOD[voice]=(int)mod-64;// 64=no mod
}
//*********************************************************************
//  Setup vol [0-31]
//*********************************************************************

void synthESP32_setVol(unsigned char voice,unsigned char vol) {
	VOL_L[voice]=vol;//-(vol*PAN[voice]/100);
VOL_R[voice]=vol;  
if (PAN[voice]>0){
	VOL_L[voice]=vol-(vol*PAN[voice]/100);
}
if (PAN[voice]<0){
	VOL_R[voice]=vol+(vol*PAN[voice]/100);
}	
}
  //*********************************************************************
//  Setup master vol [0-31]
//*********************************************************************

void synthESP32_setMVol(unsigned char vol) {
  mvol=vol;	
}

//*********************************************************************
//  Setup master filter [0-255]
//*********************************************************************

void synthESP32_setMFilter(unsigned char freq)  {
  // ya que 0 es no filter hago un map y cambio el rango
  freq=map(freq,0,127,255,0);
  //freq=freq<<1; //multiplico por dos porque el valor viene de 0-127 y necesito 0-255
  lpfL.setCutoffFreq(freq);
  lpfR.setCutoffFreq(freq);
}  

  //*********************************************************************
//  Setup voice filter [0-255]
//*********************************************************************

void synthESP32_setFilter(unsigned char voice, unsigned char freq)  {
  // ya que 0 es no filter hago un map y cambio el rango
  freq=map(freq,0,127,255,0);
  
  //freq=freq<<1; //multiplico por dos porque el valor viene de 0-127 y necesito 0-255
  
  switch (voice) {
    case 0:
      lpf0.setCutoffFreq(freq);
      break;
    case 1:
      lpf1.setCutoffFreq(freq);
      break;
    case 2:
      lpf2.setCutoffFreq(freq);
      break;
    case 3:
      lpf3.setCutoffFreq(freq);
      break;
    case 4:
      lpf4.setCutoffFreq(freq);
      break;
    case 5:
      lpf5.setCutoffFreq(freq);
      break;
    case 6:
      lpf6.setCutoffFreq(freq);
      break;
    case 7:
      lpf7.setCutoffFreq(freq);
      break;
    case 8:
      lpf8.setCutoffFreq(freq);
      break;
    case 9:
      lpf9.setCutoffFreq(freq);
      break;
    case 10:
      lpf10.setCutoffFreq(freq);
      break;
    case 11:
      lpf11.setCutoffFreq(freq);
      break;
    case 12:
      lpf12.setCutoffFreq(freq);
      break;
    case 13:
      lpf13.setCutoffFreq(freq);
      break;
    case 14:
      lpf14.setCutoffFreq(freq);
      break;
    case 15:
      lpf15.setCutoffFreq(freq);
      break;
  }
} 
  
//*********************************************************************
//  Setup pan [-99% - 99%]
//*********************************************************************

void synthESP32_setPan(unsigned char voice,signed char pan) {
  PAN[voice]=pan;
  if (PAN[voice]>0){
	  VOL_L[voice]=VOL_R[voice]-(VOL_R[voice]*PAN[voice]/100);
  }
  if (PAN[voice]<0){
	  VOL_R[voice]=VOL_L[voice]+(VOL_L[voice]*PAN[voice]/100);
  }	
}
//*********************************************************************
//  Midi trigger
//*********************************************************************

void synthESP32_mTrigger(unsigned char voice,unsigned char MIDInote ) {
  PITCH[voice]=PITCHS[MIDInote];
  EPCW[voice]=0;
  FTW[divider] = (PITCH[voice] + (int)   (((PITCH[voice]>>6)*(EPCW[voice]>>6))>>7)*MOD[voice]);

}
//*********************************************************************
//  Simple trigger
//*********************************************************************

void synthESP32_trigger(unsigned char voice)  {
  EPCW[voice]=0;
  FTW[voice]=PITCH[voice];
}
//*********************************************************************
//  Set frequency direct
//*********************************************************************

void synthESP32_setFrequency(unsigned char voice,float f) {
  PITCH[voice]=f/(SAMPLE_RATE/65535.0);
}
