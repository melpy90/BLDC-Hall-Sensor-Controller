#include "mbed.h"
#include "RateLimiter.h"
#include "BLDCmotorDriver.h"
#include "BLDCtimer.h"
#define P_IN1 PA_8
#define P_IN2 PA_9
#define P_IN3 PA_10
#define P_EN1 PC_10
#define P_EN2 PC_11
#define P_EN3 PC_12
#define P_HALL1 PA_15
#define P_HALL2 PB_3
#define P_HALL3 PB_10
#define P_FAULT PB_2



AnalogIn vbus(PA_1);
AnalogIn temperature(PC_2);
DigitalOut myled(LED1);
DigitalOut fault_led(PB_2);
AnalogIn bemf1(PC_3);
AnalogIn bemf2(PB_0);
AnalogIn bemf3(PA_7);

InterruptIn mybutton(USER_BUTTON);
AnalogIn speedControl(PB_1);

Timer tmr ;
Ticker toggle_led_ticker;

BLDCmotorDriver M(P_IN1, P_IN2, P_IN3, P_EN1, P_EN2, P_EN3, P_HALL1, P_HALL2, P_HALL3, P_FAULT);

float dc = 0.0f;                  // SET initial DUTY CYCLE
//float dc1 = speedControl.read()*dc;

bool spinning = false;
int a; // number of rpm wanted
int b; // sense of rotation
int v; // scanf sense


void pressed()
{
    // start/stop the motor
    spinning = !spinning;
    
    
    //myled = spinning;
}
void toggle_led() {
    myled = !myled;
}
void print_vbus()
{
    float v = vbus.read();
    
    printf("vbus=%f\n", v);
}
float rpm_control()
{
  
         
           
      //     printf("cycles = %f cycles \n", M.getCycles() );
      toggle_led_ticker.attach(&toggle_led, 1.00);  // every half-seconds sample time and led1 turn on
      float time = BLDCtimer(0, 0, 0).counter()/1000; // sampling time in seconds    
   // printf("time = %f  sec \n", time );
        int cycle  = M.getCycles();
       // printf("cycles = %f cycles \n", M.getCycles() );
      
      
      
      float sampling = (60*cycle)/time;    // calculation 
       
       if(sampling == 0)
       {
          
          M.stop();
          wait(3.0); //motor is stopped for 3 seconds if its locked.
          
          }
          
        
        
       // this is for calculate rpm with mbed timer, but seems bugged, timing is fluctuating...
   /*  if ( cycle >= 10){
            tmr.stop();
            float time1 = tmr.read_ms();
             float sampling1 = (M.getCycles()/time1)*60000;
             printf("rpm timer = %f \n", sampling1 );
             tmr.reset();
             M.resetCycles();
            } */
      
      
     if(time == 0){
             printf("  \n" );
            
             }else{
        
        
         printf("Rpm with ticker  = %f \n", sampling );
        
        
        if(sampling < a && b == 1 && dc <= 1.00f){            // clockwise control
           
         M.setDutyCycle(dc);
            dc += 0.05f;
            M.setDutyCycle(dc);
           printf("dutyCycle  = %f \n", dc );
          
           }else if(sampling > a && b == 1 && dc <= 1.00f){
               dc -= 0.05f;
            M.setDutyCycle(dc);
            printf("dutyCycle  = %f \n", dc );
               }
           
           
                
          if(sampling < a && b == -1 && dc >= -1.00f){     // anticlockwise control
            
            M.setDutyCycle(-dc);
            
            dc -= 0.05f;
            M.setDutyCycle(dc);
            
           printf("dutyCycle  = %f \n", dc );
           
           }else if(sampling > a && b==-1 && dc >= -1.00f){
               dc += 0.05f;
            M.setDutyCycle(dc);
            printf("dutyCycle  = %f \n", dc );
               }  
            
        
        else   if(dc == 1.0f || dc == -1.0f){
            
                M.setDutyCycle(b*1.0f);
                printf("power limitation...\n");
                
               
                }
                
         
        
         
          M.resetCycles();
          }

}

/*void speedbybemf()
{
    
    
    float b1 = bemf1.read();
    float b2 = bemf2.read();
    float b3 = bemf3.read();
    float dc1 = speedControl.read()*dc;
    //float vbus = vbus.read();
  
  
   if(  b1 > b2  && b1 > b3 ) 
    {
        printf("bemfspeed = %f rpm \n", b1*dc1*21.9*60/(0.0367*6.28));
    }else if( b3 > b1 && b3 > b2  ){

            printf("bemfspeed = %f rpm \n", b3*dc1*21.9*60/(0.0367*6.28));
}
    else if( b2 > b1 && b2 > b3  ){

            printf("bemfspeed = %f rpm \n", b2*dc1*21.9*60/(0.0367*6.28));


            }
}

*/
void control_temperature()
{
    float t = temperature.read()* 100.0f;
    
    printf("temperature=%f\n", t);
    
   
     if (t > 80){
      printf("temperature very hot...turning off\n", t);
    fault_led = 1;
    dc = 0; 
     }
   
    
    
    }

int main() {
  
  printf("BLDC Motor Controller 1.0 \n");
  printf("Set rotation direction : 1 for clockwise, 2 for anticlockwise \n");
          
        scanf("%d", &v);
        
          if(v == 1){ b = 1;
          printf("The direction is clockwise \n", b);
          b = 1;
          
          }
          if(v== 2){ 
           b = -1;
          printf("The direction is anticlockwise \n", b);
         
          }
  
  printf("Set rpm speed\n");
           scanf("%d", &a);
           printf("You have setted %d rpm, press blue button for spinning\n", a);
   mybutton.fall(&pressed); 
    
  
      
    
    while (true) {
         
         
         
          if (spinning) {
           
           
           
           tmr.start();
          // float dc1 = speedControl.read()*dc;
           //M.setDutyCycle(b*dc);
           myled = 1;
           control_temperature();
           rpm_control();
          
           
          
       
          
          
 
        } else {
           
            M.stop();   /// turn off with blue button 
        }
    }
       
}
