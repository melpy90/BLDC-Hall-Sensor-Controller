#include "mbed.h"


class BLDCtimer{
   
   public:
    BLDCtimer(uint32_t ticks, uint32_t begin, uint32_t end); 
    
           
        void inc_tick();
        float counter();
        float timestamp();
      
      
      private:
      Timer timer;
      float stamp;
      uint32_t ticks; uint32_t begin; uint32_t end ;
    

Ticker tick_ticker;

};