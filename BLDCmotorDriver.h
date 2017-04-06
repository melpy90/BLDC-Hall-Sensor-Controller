#include "mbed.h"
#include "RateLimiter.h"

class BLDCmotorDriver {
    public:
        BLDCmotorDriver(PinName pIN1, PinName pIN2, PinName pIN3, PinName pEN1, PinName pEN2, PinName pEN3,
                    PinName pH1, PinName pH2, PinName pH3, PinName pFault, int pPoles=8);    
        void configure(float sampleTime, float switchingFrequency, float rampUpSlope, float rampDownSlope);
        void setDutyCycle(float dutyCycle);
        void stop();
        uint32_t buggedTimer();
        float getDutyCycle();
        float getSpeed();
        float getCycles();
        int timerCycles();
        int timer2Cycles();
        
        int getSector();
        int resetCycles();
        void ispis();
        
        
    private:
        PwmOut IN1, IN2, IN3;
        DigitalOut EN1, EN2, EN3; 
        InterruptIn H1; // InterruptIn can be used on all pins except p19 and p20
        InterruptIn H2; 
        InterruptIn H3;
        RateLimiter rl;
        Ticker ticker;
        static Ticker timekeeper;
        float switchingPeriod, dutyCycle, tempDutyCycle, sampleTime;
        void commutation();
        int poles ;
        float speedhall;
        float buggedspeed;
        uint32_t cycles ;
        Timer timers;
        Timer timers2;
        
        int currentSector, _currentSector, previousSector, difference;
        DigitalOut Fault;
        int h1, h2, h3;
       // Timer tmr;
        void speedCalc();
        static uint32_t ticks;
        void incrementTicks();
          
        //void adjustDutyCycle();
            
};