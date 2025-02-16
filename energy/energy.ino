// AC Frequency with LCD By Solarduino 

// Note Summary
// Note :  Safety is very important when dealing with electricity. We take no responsibilities while you do it at your own risk.
// Note :  This AC Frequency Code is useing AC current module or AC Voltage Module for reference value to determine the frequency value.
// Note :  The value shown in Serial Monitor is refreshed every second (for 50Hz).
// Note :  The frequency is measured by counting time and average it for every 50 samples taken (1 sample is 1 cycle).
// Note :  No calibration is needed.  
// Note :  The unit provides reasonable accuracy and may not be comparable with other expensive branded and commercial product.
// Note :  All credit shall be given to Solarduino.

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/////////////*/


        /* 0- General */

        int decimalPrecision = 2;                 // decimal places for all values shown in LED Display & Serial Monitor


        /* 1- frequency measurement */
        
        unsigned long startMicros;                /* start counting time for frequency (in micro seconds)*/
        unsigned long currentMicros;              /* current counting time for frequency (in micro seconds) */
        int expectedFrequency = 50;               // Key in your grid frequency. No issue if key wrong. This is to collect number of samples.
        int frequencyAnalogPin = A0;              // The signal for frequency sensed. Can be by AC Current sensor or can be by AC Votlage sensor.
        float frequencySampleCount = 0;           /* count the number of sample, 1 sample equivalent to 1 cycle */
        float frequency =0 ;                      /* shows the value of frequency*/
        float a;                                  /* use for calculation purpose*/                            
        float switch01 = 0;                       /* use for switching function */
        float vAnalogRead = 0;                    /* read analog value, can be use for AC current sensor or AC voltage sensor*/

 
        /* 2 - LCD Display  */

        #include<LiquidCrystal.h>                   /* Load the liquid Crystal Library (by default already built-it with arduino solftware)*/
        LiquidCrystal LCD(8,9,4,5,6,7);             /* Creating the LiquidCrystal object named LCD. The pin may be varies based on LCD module that you use*/
        unsigned long startMicrosLCD;               /* start counting time for frequency (in micro seconds)*/
        unsigned long currentMicrosLCD;             /* current counting time for frequency (in micro seconds) */
        
void setup() 
{
  
        /* 0- General */
 
        Serial.begin(9600);                         /* to display readings in Serial Monitor at 9600 baud rates */


        /* 1- frequency measurement */

        startMicros = micros();                     /* Start counting time for frequency measurement */


        /* 2 - LCD Display  */

        LCD.begin(16,2);                            /* Tell Arduino that our LCD has 16 columns and 2 rows*/
        LCD.setCursor(0,0);                         /* Set LCD to start with upper left corner of display*/  
        startMicrosLCD = micros();                  /* start countint time for LCD refesh time*/
}

void loop() 
{

        /* 1- frequency measurement */

        currentMicros = micros();                                           /* record current time for frequency calculation*/
        vAnalogRead = analogRead(frequencyAnalogPin) - 512;                 /* read the analog value from sensor */ 
        
        if(vAnalogRead >=0 && switch01 == 0)                                /* if analog value higher than 0, initiate the code*/
          {
              frequencySampleCount = frequencySampleCount +1 ;              /* count the sample*/
              switch01 = 1;                                                 /* straight away go to standby mode by switching to other function*/
          }

        if(vAnalogRead < 0 && switch01 == 1)                                /* if analog value lower than 0, initiate the code*/
          {
              switch01 = 0;                                                 /* do nothing but to switch back the function for the previous function to be active again*/
                                                                            /* this purpose is to make sure whole wave form is complete and counting quantity of sample with no mistake */ 
          }

        if(frequencySampleCount == expectedFrequency)                       /* if couting sample reach at 50 (default) which is eqivalent to 1 second*/
          {
              a = currentMicros-startMicros ;                               /* use for calculation purpose*/
              frequency = 1/((a/1000000)/frequencySampleCount);             /* formula for frequency value*/
              Serial.print(frequency,decimalPrecision);                     
              Serial.println(" Hz");
              startMicros = currentMicros;                                  /* reset the counting time for the next cycle */
              frequencySampleCount = 0;                                     /* reset the total sample taken become 0 for next cycle */


              
              /* 2 - LCD Display  */
      
              currentMicrosLCD = micros();                                  /* record current time for LCD display*/
              if(currentMicrosLCD - startMicrosLCD >=1000000)               /* if time difference more than 1 second, initiate the code */
              {
                      LCD.setCursor(0,0);                                   /* Set cursor to first colum 0 and second row 1  */
                      LCD.print("f=");
                      LCD.print(frequency,decimalPrecision);                /* display frequency value in LCD in first row  */
                      LCD.print("Hz                  ");
                      LCD.setCursor(0,1); 
                      LCD.print("                   ");                     /* display nothing in LCD in second row */
                      startMicrosLCD = currentMicrosLCD ;                   /* reset the time again for the next counting set*/
          
              }
          }



      
 
}