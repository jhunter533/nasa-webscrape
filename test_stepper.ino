
#include <Stepper.h>
#include "ICM_20948.h"
#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;
#define SERIAL_PORT Serial
#define WIRE_PORT Wire1
#define AD0_VAL 1
ICM_20948_I2C myICM;
#include <math.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int rolePerMinute = 9;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
const float stepPerDegree=2048/360;
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);
long totalSteps=0;
bool hasRun=false;
float inputValue1=0;
float inputValue2=0;
byte headingN[8][12]={
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },

  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
byte headingNW[8][12]={
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
byte headingNE[8][12]={
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
byte headingW[8][12]={
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
byte headingE[8][12]={
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
byte headingS[8][12]={
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },

  { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 }
};
byte headingSW[8][12]={
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }
};
byte headingSE[8][12]={
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }
};
byte Left[8][12]={
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },

  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
byte Right[8][12]={
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },

  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
byte OnTarget[8][12]={
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },

  { 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 },

  { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
inline convertToDegrees(float radians){
  return radians*(180/M_PI);
}
inline convertToRadians(float degrees){
  return degrees*(M_PI/180);
}
float getBearing(float lat1,float long1,float lat2,float long2){
  lat1=convertToRadians(lat1);
  lat2=convertToRadians(lat2);
  long1=convertToRadians(long1);
  long2=convertToRadians(long2);
  bearing=atan2(sin(long2-long1)*cos(lat2),cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(long2-long1));
  bearing=convertToDegrees(bearing);
  return bearing;
}
void setup() {
  myStepper.setSpeed(rolePerMinute);
  // initialize the serial port:
  Serial.begin(9600);
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);
  myICM.begin(WIRE_PORT,AD0_VAL);
  matrix.begin();
}
float prevAngle;

void loop() {
  if (Serial.available()>0){
    String data=Serial.readStringUntil('\n');
    int space=data.indexOf(' ');
    float float1=data.substring(0,space).toFloat();
    float float2=data.substring(space+1).toFloat();
    Serial.print("recieved values: ");
    Serial.print(float1);
    Serial.print(", ");
    Serial.println(float2);
    //delay(5000);
  }
  if (myICM.dataReady()){
    myICM.getAGMT();
    //printScaledAGMT(&myICM);
    float x=myICM.magY();
    float y=myICM.magX();
    float D;
    lat1=
    long1=
    float targetAngle=getBearing(lat1,long1,lat2,long2);
    float deltaAngle;
    
    if (x==0){
      if (y<0){
        D=90;
      }
    } else {
      D=atan2(y,x)*(180/M_PI);
    }
    if (D<0){
      D+=360;
    }
    //SERIAL_PORT.print("Current Angle,");
    //SERIAL_PORT.println(D);
    /*
    if (D>=337.25||D<22.5){
      matrix.renderBitmap(headingN,8,12);
    }else if (D>=292.5&&D<337.25){
      matrix.renderBitmap(headingNW,8,12);
    }else if (D>=247.5&&D<292.5){
      matrix.renderBitmap(headingW,8,12);
    }else if (D>=202.5&&D<247.5){
      matrix.renderBitmap(headingSW,8,12);
    }else if (D>=157.5&&D<202.5){
      matrix.renderBitmap(headingS,8,12);
    }else if (D>=112.5&&D<157.5){
      matrix.renderBitmap(headingSE,8,12);
    }else if (D>=67.5&&D<112.5){
      matrix.renderBitmap(headingE,8,12);
    }else if (D>=22.5&&D<67.5){
      matrix.renderBitmap(headingNE,8,12);
    }
    delay(60);
  }*/
  
  deltaAngle=abs(targetAngle-D);
  float stepsToRotate=stepPerDegree*abs(deltaAngle);
  //SERIAL_PORT.println(stepsToRotate);
  if(abs(D-prevAngle)>3){
    //for(int i=0;i<stepsToRotate;i++){
      //SERIAL_PORT.print("ERROR, ");
      //SERIAL_PORT.println(deltaAngle);
      if(D<targetAngle-5){
        myStepper.step(-stepsToRotate);
        matrix.renderBitmap(Left,8,12);
      }else if(D>targetAngle+5){
        myStepper.step(stepsToRotate);
        matrix.renderBitmap(Right,8,12);
      }else{
        matrix.renderBitmap(OnTarget,8,12);
      }
      //SERIAL_PORT.println(i);
      //delay(60);
    //}
    }else{matrix.renderBitmap(OnTarget,8,12);}
    prevAngle=D;
    //delay(60);
  }

 
  //if(!hasRun){
  //  for(int i=0;i<=stepPerDegree*45;i++){
  //    myStepper.step(1);
  //    delayMicroseconds(800);
  //    totalSteps+=1;
  //    Serial.println(totalSteps);
  //  }
  //  hasRun=true;
  //  return;
  //}
}
