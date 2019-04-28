#include "pulse.h"
#include "serial.h"

volatile uint16_t BPM;  
volatile uint16_t IBI = 600;             
volatile uint8_t Pulse = 0; 
volatile uint8_t QS = 0;                     
volatile uint32_t sampleCounter = 0;   
volatile uint32_t lastBeatTime = 0;      
volatile uint16_t P = ADC_THRESH;                
volatile uint16_t T = ADC_THRESH;               
volatile uint16_t thresh = ADC_THRESH;       
volatile uint16_t amp = 100;          
volatile uint8_t firstBeat = 1;       
volatile uint8_t secondBeat = 0;
volatile uint16_t rate[10];     


uint16_t adc_read(uint8_t adcx){
	/*Select which adc pin to target*/
	ADMUX	&=	0xf0;
	ADMUX	|=	adcx;

	/* This starts the conversion. */
	ADCSRA |= _BV(ADSC);

	/* This is an idle loop that just wait around until the conversion
	 * is finished.  It constantly checks ADCSRA's ADSC bit, which we just
	 * set above, to see if it is still set.  This bit is automatically
	 * reset (zeroed) when the conversion is ready so if we do this in
	 * a loop the loop will just go until the conversion is ready. */
	while ( (ADCSRA & _BV(ADSC)) );

	/* Finally, we return the converted value to the calling function. */
	return ADC;
}


void pulse_init(void){
	/* Enable the ADC */
	ADCSRA |= _BV(ADEN);
	// Set the Timer Mode to CTC
    //TCCR0A |= (1 << WGM01);

    // Set the value that you want to count to
    //OCR0A = 0x13; // OCRn = ((clock/prescalar) * time in sec) - 1 => ((9830400/1024) * 0.002) - 1 = 19

 	//Set the ISR COMPA vect
    //TIMSK0 |= (1 << OCIE0A);   


    //set prescaler to 1024 and start the timer
    //TCCR0B |= (1 << CS02);
   	//TCCR0B |= (1 << CS00);
   	
}

uint16_t calc_BPM(void){
    ADCSRA |= _BV(ADEN);
  uint16_t counter = 0;
  //uint8_t no_data = 1;
  while(counter < 2000){

    uint16_t Signal = adc_read(ADC_PIN1);
  sampleCounter += 2;               
  uint16_t N = sampleCounter - lastBeatTime; 
  //if(Signal > 390){
    //o_data = 0;
  //}
    //updates Trough when the period since last beat is larger than (5/3)IBI
    if(Signal < thresh && N > (IBI/5)*3){

        if (Signal < T){

            T = Signal;

        }    

    }
    //always updates Peak
    if(Signal > thresh && Signal > P){

        P = Signal;

    }

    /**Do following after a buffer period from last beat**/
    if (N > 250){
      //output_string("signal:");
      //output_number(Signal);
      //output_string("   thresh:");
      //output_number(thresh);
      //Detects a pulse when a peak is found and time since last beat is larger than (5/3)IBI
      if ( (Signal > thresh) && (Pulse == 0) && (N > ((IBI/5)*3) ) ){  
        //peak is detected
        //output_string(" PULSE ");
        //output_number(Signal);
        Pulse = 1;                      
        //update beat time with current time            
        IBI = sampleCounter - lastBeatTime;
        lastBeatTime = sampleCounter;

        //Reset second beat flag
        if(secondBeat){              
          secondBeat = 0;     
          //why?
          uint8_t j;
          for(j=0; j<=9; j++){   
            rate[j] = IBI;                      
          }

        }                       

      //update firstBeat & secondBeat flags  
      if(firstBeat == 1){                     
        firstBeat = 0; 
        secondBeat = 1;
        //sei();           
        //break;
        } 
      uint32_t  runningTotal = 0;
      uint8_t i;
      for(i = 0; i<=8; i++){
        rate[i] = rate[i+1]; 
        runningTotal += rate[i];      
        }

      rate[9] = IBI;                   
      runningTotal += rate[9];             
      runningTotal /= 10;             
      BPM = 30240/runningTotal;
      //QS = 1;                        
    }             

  }

  //Detects drop in signal after the pulse peak
  if (Signal < thresh && Pulse == 1){  
      //update flag to say we reached the end of a pulse
      Pulse = 0;  
      //update threshold so its exactly the middle value of the signal                    
      amp = P - T;                        
      thresh = amp/2 + T;            
      P = thresh;                         
      T = thresh;
    }

  //reset variables after a long time has passed since last beat
    
  if (N > 2500){    
       thresh = ADC_THRESH; 
       P = 512; 
        T = 512; 
        firstBeat = 1;                 
       secondBeat = 0;
       lastBeatTime = sampleCounter;
    }
    counter = counter + 1;
    _delay_ms(2);
    output_number(Signal);
  }

	 ADCSRA &= ~_BV(ADEN);

  //if(no_data == 1){
    //return 10;
  //} else {
    return BPM;
  //}
}