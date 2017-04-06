#include "BLDCmotorDriver.h"


BLDCmotorDriver::BLDCmotorDriver(PinName pIN1, PinName pIN2, PinName pIN3, PinName pEN1, PinName pEN2, PinName pEN3,
                    PinName pH1, PinName pH2, PinName pH3, PinName pFault, int pPoles) : 
                    IN1(pIN1), IN2(pIN2), IN3(pIN3), EN1(pEN1), EN2(pEN2), EN3(pEN3),
                    H1(pH1), H2(pH2), H3(pH3), Fault(pFault){
                     
    poles = pPoles;
    sampleTime = 1e-3;
    switchingPeriod = 1.0 / 20e3;
    dutyCycle = tempDutyCycle = 0;
    IN1.period(switchingPeriod);
    IN2.period(switchingPeriod);
    IN3.period(switchingPeriod); // applies to all PwmOut instances
    rl.setLimits(0.1, -0.1, 0, sampleTime); // initial 10 second ramp 
    //H1.mode(PullNone);
    //H2.mode(PullNone);
    //H3.mode(PullNone);
    cycles = 0;
    
    
    
    H1.rise(this, &BLDCmotorDriver::commutation);
    H2.rise(this, &BLDCmotorDriver::commutation);
    H3.rise(this, &BLDCmotorDriver::commutation);
    H1.fall(this, &BLDCmotorDriver::commutation);
    H2.fall(this, &BLDCmotorDriver::commutation);
    H3.fall(this, &BLDCmotorDriver::commutation);
}
void BLDCmotorDriver::configure(float sampleTime, float switchingFrequency, float rampUpSlope, float rampDownSlope) {
    if (sampleTime < 1e-6)
        sampleTime = 1e-3;
    if (switchingFrequency < 100)
        switchingFrequency = 20e3;
    if (rampUpSlope < 0 || rampUpSlope > 1)
        rampUpSlope = 0.1;
    if (rampDownSlope > 0 || rampDownSlope < -1)
        rampDownSlope = -0.1;
    this->sampleTime = sampleTime;
    switchingPeriod = 1.0 / switchingFrequency;
    rl.setLimits(rampUpSlope, rampDownSlope, 0, sampleTime);
}
int BLDCmotorDriver::getSector(){           // hall 120°
    
    h1 = H1.read();  
    h2 = H2.read();
    h3 = H3.read();
  
  
     
    if(h1 == 0 && h2 == 0 && h3 == 1){          
             _currentSector = 0;
             cycles++;
             
             }    
    else if(h1 == 0 && h2 == 1 && h3 == 1){
            _currentSector = 1;   
              
            }        
    else if(h1 == 0 && h2 == 1 && h3 == 0){  
            _currentSector = 2;   
              
            }  
    else if(h1 == 1 && h2 == 1 && h3 == 0){
            _currentSector = 3;  
            
           
            } 
    else if(h1 == 1 && h2 == 0 && h3 == 0){
            _currentSector = 4;   
            
            }
    else if(h1 == 1 && h2 == 0 && h3 == 1){
            _currentSector = 5; 
            
            
            }  
                    
     previousSector = _currentSector - 1;
     difference = _currentSector - previousSector;
     if (difference == 1){
       currentSector = _currentSector;
       Fault = 0;
       
       }
     else Fault = 1; 
return currentSector;                
}  
/*
void BLDCmotorDriver::incrementTicks()
{
    ticks++;
} */

void BLDCmotorDriver::commutation()  {
 
 
 dutyCycle = rl.out(tempDutyCycle);
 currentSector = getSector();
 
 
        
    
    
    if (dutyCycle > 0 ) {
          
         
         currentSector++;
         
          if(currentSector > 5)
        currentSector = 0;
  /*      
         // This part is for stimate speed with integrated mbed timer, but its still bugged, and the speed is stimated with a ticker and its call called BLDCtimer.cpp
  //
  //
        
       tmr.start();
         // cycles counter for rpm
         int cycles_per_step = 6;
        int min_revs=100;
     
        if(cycles == cycles_per_step*poles*min_revs){
        
        tmr.stop();
        
         
        uint32_t elapsed_us = tmr.read_us();
        // float  buggedspeed = tmr.read_ms();
        uint32_t  buggedspeed = ((cycles)/((cycles_per_step*poles*min_revs)*60000000))/(elapsed_us);
        
        
        }
        
        */
         
        
        
        switch(currentSector) {  
        
    /*     phase commutation
        
         for positive dutycycle
        
        1--X--0--0--X--1
        X--1--1--X--0--0
        0--0--X--1--1--X
        
        for negative dutycycle
        
        1--1--X--0--0--X
        0--X--1--1--X--0
        X--0--0--X--1--1
        
        */
         
        
          case 0:                   
               
                
                EN1 = 1; IN1 = dutyCycle;
                EN2 = 0; IN2 = 0;
                EN3 = 1; IN3 = 0; 
                              
                break;
           case 1:            
           
                
                EN1 = 0; IN1= 0;
                EN2 = 1; IN2 = dutyCycle;
                EN3 = 1; IN3 = 0;
                
                break;
           case 2:             
            
                
                EN1 = 1; IN1 = 0;
                EN2 = 1; IN2 = dutyCycle;
                EN3 = 0; IN3 = 0;
                
                break;
            case 3:             
             
                
                EN1 = 1; IN1 = 0;
                EN2 = 0; IN2 = 0;
                EN3 = 1; IN3 = dutyCycle;
              
                break;
            case 4:              
             
                
                EN1 = 0; IN1 = 0;
                EN2 = 1; IN2 = 0;
                EN3 = 1; IN3 = dutyCycle;
                 
               
                break;    
            case 5:              
             
                
                EN1 = 1; IN1 = dutyCycle;
                EN2 = 1; IN2 = 0;
                EN3 = 0; IN3 = 0;
                
                
                break;
        }
    } else if (dutyCycle < 0)
         { 
         
        
        currentSector--;
        
        if(currentSector < 0)currentSector = 5;
        switch(currentSector) {
          
           case 1:              
           
                
                EN1 = 0; IN1=0;
                EN2 = 1; IN2 = 0;
                EN3 = 1; IN3 = -dutyCycle;
                
                break;
           case 2:              
           
                
                EN1 = 1; IN1 = -dutyCycle;
                EN2 = 1; IN2 = 0;
                EN3 = 0; IN3= 0;
                
                break;
          case 3:                 
        
                
                EN1 = 1; IN1 = -dutyCycle;
                EN2 = 0; IN2 = 0;
                EN3 = 1; IN3 = 0;
                
                break;
           case 4:            
           
                EN1 = 0; IN1= 0;
                EN2 = 1; IN2 = -dutyCycle;
                EN3 = 1; IN3 = 0;
                 
                break;
           case 5:             
            
                
                EN1 = 1; IN1 = 0;
                EN2 = 1; IN2 = -dutyCycle;
                EN3 = 0; IN3=0;
                
                
                break;
            case 0:             
            
                
                EN1 = 1; IN1 = 0;
                EN2 = 0; IN2=0;
                EN3 = 1; IN3 = -dutyCycle;
                
                break;
                }                
        }else {
        stop();
    }
    
    }
     
    
 
void BLDCmotorDriver::setDutyCycle(float dc) {
    
    if (dc >= -1 && dc <= 1) {
     
        ticker.attach(this, &BLDCmotorDriver::commutation, sampleTime);
        tempDutyCycle = dc;
        
        
        
        
         } else {
        stop();
        
    }
}

    
        

/*void BLDCmotorDriver::adjustDutyCycle() {
    dutyCycle = rl.out(tempDutyCycle);
    float diff = tempDutyCycle - dutyCycle;
    if (diff < 0.01 || diff > -0.01)
        ticker.detach();
}*/
void BLDCmotorDriver::stop() {
    IN1 = 0;
    IN2 = 0;
    IN3 = 0;
    EN1 = 0;
    EN2 = 0;
    EN3 = 0;
    dutyCycle = tempDutyCycle = 0;
    cycles=0;
    rl.reset();
    ticker.detach();
    //tmr.reset();
   
}
float BLDCmotorDriver::getDutyCycle() {
    return dutyCycle;
}

/*float BLDCmotorDriver::getSpeed() {
  
     
    speedhall = ((cycles)/(cycles_per_step*poles));
    cycles=0;                                        // reset counter for the next sampling... (pressing blue button)
    return speedhall;

}
*/
uint32_t BLDCmotorDriver::buggedTimer() {
    
    return buggedspeed ;
  
    

}
float BLDCmotorDriver::getCycles(){          
    
   
                    
     
   int cycles_per_step = 6;    
      
     int revolution =  cycles/(cycles_per_step*poles);
     
return revolution;          // number of visible cycles      
}  

int BLDCmotorDriver::resetCycles(){     // RESET CYCLES COUNTER FOR SAMPLING
    
    return cycles=0;
    }
