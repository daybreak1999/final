#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
Timer t;
BBCar car(pin5, pin6, servo_ticker);
DigitalInOut ping1(D11);
BufferedSerial uart(D1,D0); //tx,rx
//BufferedSerial xbee(D10, D9);

int main(){
  // char buf[256], outbuf[256];
  // FILE *devin = fdopen(&xbee, "r");
  // FILE *devout = fdopen(&xbee, "w");

 //  parallax_ping  ping1(pin10);
   bool flag = 1;
   bool fin = 0;
   uart.set_baud(9600);
   car.goStraight(-20);
   while(1){
      if(uart.readable()){
            char recv[1];
            int x1 = -1, x2 = -1, y1 = -1, y2 = -1;
            
            uart.read(recv, sizeof(recv));;
            if(recv[0] == 'l' && flag) {
               printf("1");
               uart.read(recv, sizeof(recv));
               char recv2[1], recv3[1], recv4[1];

               uart.read(recv2, sizeof(recv2));
               uart.read(recv3, sizeof(recv3));
               uart.read(recv4, sizeof(recv4));
               x1 = (recv2[0] - '0') * 100 + (recv3[0] - '0') * 10 +(recv4[0] - '0');
               uart.read(recv, sizeof(recv));

               uart.read(recv2, sizeof(recv2));
               uart.read(recv3, sizeof(recv3));
               uart.read(recv4, sizeof(recv4));
               y1 = (recv2[0] - '0') * 100 + (recv3[0] - '0') * 10 +(recv4[0] - '0');
               uart.read(recv, sizeof(recv));

               uart.read(recv2, sizeof(recv2));
               uart.read(recv3, sizeof(recv3));
               uart.read(recv4, sizeof(recv4));
               x2 = (recv2[0] - '0') * 100 + (recv3[0] - '0') * 10 +(recv4[0] - '0');
               uart.read(recv, sizeof(recv));

               uart.read(recv2, sizeof(recv2));
               uart.read(recv3, sizeof(recv3));
               uart.read(recv4, sizeof(recv4));
               y2 = (recv2[0] - '0') * 100 + (recv3[0] - '0') * 10 +(recv4[0] - '0');
               uart.read(recv, sizeof(recv));

               float tan = ((float)x2-x1) / ((float)y2 - y1);
               
               if (tan < -0.05) {
                  // left
                  car.turn(-20, -0.3);
                  ThisThread::sleep_for(std::chrono::milliseconds(100));
               }
               if (tan > 0.05) {
                  // right
                  car.turn(-20, 0.3);
                  ThisThread::sleep_for(std::chrono::milliseconds(100));
               }
                 
            }
            else if(recv[0] == 'a') {
 
               float x = 0;
               if(flag) {
                    flag = 0;
                    car.stop();
               }
               char recv2[1], recv3[1];
               uart.read(recv, sizeof(recv));
               uart.read(recv2, sizeof(recv2));
               if (recv2[0]== '-') {
                  uart.read(recv2, sizeof(recv2));
                  uart.read(recv, sizeof(recv));
                  uart.read(recv3, sizeof(recv3));
                  x =  -(1.0 * (recv2[0] - '0') + 0.1 * (recv3[0] - '0'));
                  if (x < -0.5) {
                  // right

                     car.turn(-40, 0.3);
                     ThisThread::sleep_for(std::chrono::milliseconds(100));
                  }
               }
               else {
                  uart.read(recv2, sizeof(recv2));
                  uart.read(recv, sizeof(recv));
                  uart.read(recv3, sizeof(recv3));
                  x =  1.0 * (recv2[0] - '0') + 0.1 * (recv3[0] - '0');
                  if (x > 0.5) {
                  // left

                     car.turn(40, 0.3);
                     ThisThread::sleep_for(std::chrono::milliseconds(100));
                  }
               }
                car.stop();
               if(-0.5<x<0.5)fin = 1;
               if(fin == 1) break;
            }

      }

   }  
   float val;
   car.goStraight(-30);

   while(1) {

      ping1.output();
      ping1 = 0; wait_us(200);
      ping1 = 1; wait_us(5);
      ping1 = 0; wait_us(5);

      ping1.input();
      while(ping1.read() == 0);
      t.start();
      while(ping1.read() == 1);
      val = t.read();
      if(val*17700.4f < 10) break;
      t.stop();
      t.reset();
      ThisThread::sleep_for(1s);
   }
   car.stop();
}
   
