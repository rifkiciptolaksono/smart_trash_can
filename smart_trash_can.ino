/* This example shows how to get single-shot range
 measurements from the VL53L0X. The sensor can optionally be
 configured with different ranging profiles, as described in
 the VL53L0X API user manual, to get better performance for
 a certain application. This code is based on the four
 "SingleRanging" examples in the VL53L0X API.

 The range readings are in units of mm. */

#include <Wire.h>
// deklarasi library sensor jarak
#include <VL53L0X.h>
// deklarasi library led
#include <FastLED.h>

// deklarasi variable untuk menyimpan nilai jarak satuan default mm
VL53L0X sensor_jarak;
// deklarasi variable untuk menyimpan nilai jarak satuan cm
int hasil;
// deklarasi variable untuk led
#define pin_led 3
#define num_led 8
#define CLOCK_PIN 13
#define FASTLED_ALLOW_INTERRUPTS 0
#define BRIGHTNESS  200

CRGB color_lampu[num_led];


// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

#define LONG_RANGE
//#define HIGH_SPEED
#define HIGH_ACCURACY
#define tgs2602 A0
//Awal Tgs2602
const long interval = 1000;
int dataAdc = 0;
float vTgs2602 = 0.00;

void preHeating(int det){
unsigned long previousMillis = 0;
int detSkg = 0;
Serial.print("Warming up sensors..");
Serial.print("Time: ");
do{
unsigned long currentMillis = millis();
if(currentMillis - previousMillis >= interval){
previousMillis = currentMillis;
detSkg++;
Serial.print(" ");
Serial.print(detSkg);
 }
 }
while(detSkg <= det);
detSkg = 0;
}//End Tgs2602


void setup(){
  /* sensor tgs2602*/
  Serial.print("V TGS2602: Volt");
  /* end */
  
  /* setup fungsi lednya */
   FastLED.addLeds<NEOPIXEL, pin_led>(color_lampu, num_led);  // GRB ordering is assumed
   FastLED.setBrightness(BRIGHTNESS);
  /* END */

  /* setup sensor jaraknya */
  Serial.begin(9600);
  Wire.begin();

  sensor_jarak.setTimeout(500);
  if (!sensor_jarak.init())
  {
//    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor_jarak.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor_jarak.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 14);
  sensor_jarak.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 10);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor_jarak.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor_jarak.setMeasurementTimingBudget(200000);
#endif
/* END */
}

void loop()
{


  /* logic program khusus bagian sensor jarak */
//  Serial.print(sensor_jarak.readRangeSingleMillimeters());
  // rumus untuk mengubah mm ke cm
  hasil = sensor_jarak.readRangeSingleMillimeters()/10;
//  Serial.print("jarak = ");
Serial.print(hasil);
Serial.print(" cm");
  /* END */

  /* sensor tgs 2602 */
  dataAdc = analogRead(tgs2602);
  vTgs2602 = dataAdc * 0.00488;
//  Serial.print(" ");
 Serial.println(vTgs2602);
  delay(50); 
  /* END */

// logic penting (mengukur ketinggian isi sampah melalui level/jarak )
 
 indikator();
 Sensor_TGS_2602();     
     
  if (sensor_jarak.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

//  Serial.println();
  /* END */
}

/** function indikator untuk menentukan warna dari
setiap level/ketinggian sampahnya**/
void indikator(){

     /* LOGIC LEVEL 100% (penuh) */
     if(hasil >= 4 && hasil <= 8) {
      LEDOK3();
      LEDOK4();
      LEDOK5();
      LEDOK6();
      LEDOK7();
      LEDOK8();
     }else{
       color_lampu[2] = CRGB::Black;
       color_lampu[3] = CRGB::Black;
       color_lampu[4] = CRGB::Black;
       color_lampu[5] = CRGB::Black;
       color_lampu[6] = CRGB::Black;
       color_lampu[7] = CRGB::Black;
      }
      /* 60% - 90% */
      if(hasil >= 8 && hasil <= 15) {
      LEDOK3();
      LEDOK4();
      LEDOK5();
      LEDOK6();
      LEDOK7();
     }else{
       color_lampu[2] = CRGB::Black;
       color_lampu[3] = CRGB::Black;
       color_lampu[4] = CRGB::Black;
       color_lampu[5] = CRGB::Black;
       color_lampu[6] = CRGB::Black;
      }
      
      /* LOGIC LEVEL 50% (setengah)*/
      if  ( hasil >= 15 && hasil <= 20) {
      LEDOK3();
      LEDOK4();
      LEDOK5();
      LEDOK6();
      }else{
       color_lampu[2] = CRGB::Black;
       color_lampu[3] = CRGB::Black;
       color_lampu[4] = CRGB::Black;
        color_lampu[5] = CRGB::Black;

        }
      /* 40% */
        if  ( hasil >= 20 && hasil <= 28) {
      LEDOK3();
      LEDOK4();
      LEDOK5();
      }else{
       color_lampu[2] = CRGB::Black;
       color_lampu[3] = CRGB::Black;
       color_lampu[4] = CRGB::Black;
        }
        /* END */

      /* LOGIC LEVEL KOSONG 20% */
      if ( hasil >= 30 && hasil <= 32) {
      LEDOK3();
      LEDOK4();

     }else{
        color_lampu[2] = CRGB::Black;
       color_lampu[3] = CRGB::Black;
      }
      
      /* kosong sekali */
      if ( hasil >= 32 && hasil <= 38) {
      LEDOK3();

     }else{
       color_lampu[2] = CRGB::Black;
      }

      /* END */
  }

  
/* LOGIC PROGRAM (LED STRIP)*/
void LEDOK3(){
  color_lampu[2] = CRGB::Green;
  
  FastLED.show();
  }

void LEDOK4(){
  color_lampu[3] = CRGB::Green;
 
  FastLED.show();
  }

void LEDOK5(){
  color_lampu[4] = CRGB::Yellow;
  
  FastLED.show();
  } 

void LEDOK6(){
  color_lampu[5] = CRGB::Yellow;
  
  FastLED.show();
  }

void LEDOK7(){
  color_lampu[6] = CRGB::Red;
  
  FastLED.show();
  }

void LEDOK8(){
  color_lampu[7] = CRGB::Red;
  FastLED.show();
  }

void Black(){
//  leds[0] = CRGB::Black;
//  leds[1] = CRGB::Black;
  color_lampu[2] = CRGB::Black;
  color_lampu[3] = CRGB::Black;
  color_lampu[4] = CRGB::Black;
  color_lampu[5] = CRGB::Black;
  color_lampu[6] = CRGB::Black;
  color_lampu[7] = CRGB::Black;
  FastLED.show();
  }
/* END */

void Sensor_TGS_2602(){

   if (vTgs2602 >= 4.90){
     color_lampu [0] = CRGB::Red;
   }
   else if (vTgs2602 >= 3.90 && vTgs2602 <= 4.90 ){
    color_lampu [0] = CRGB::Yellow;
   }
   else if (vTgs2602 >= 0.00 && vTgs2602 <= 3.90 ){
    color_lampu [0] = CRGB::Green;
    }
 } 
