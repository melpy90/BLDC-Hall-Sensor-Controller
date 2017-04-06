#include "mbed.h"
#include "BLDCtimer.h"


Timer tim;


Timer timer;
DigitalOut myled1(LED1);
DigitalIn mybutton1(USER_BUTTON);
//DigitalOut led(LED1);
//Ticker tick_ticker;
//InterruptIn mybutton(USER_BUTTON);



BLDCtimer::BLDCtimer(uint32_t ticks, uint32_t begin, uint32_t end){
    //uint32_t ticks = 0;
    
    
    }

void BLDCtimer::inc_tick() {
    ticks++;
}

float BLDCtimer::counter() {
    
    tick_ticker.attach(this, &BLDCtimer::inc_tick, 0.001);
    uint32_t begin, end;
    timer.start();
    
    int t= timer.read();
   
    
    while (1) {
        begin = ticks;
        while(myled1==1);
        
        end = ticks;
     float time = end - begin;
     
     
     //   printf("Time %f ms\n\r", time);
        return time;
    }
   
     
        }
   
   
    

