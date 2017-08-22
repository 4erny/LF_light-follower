#include <Servo.h>

Servo pan;
Servo tilt;
float LDR1, LDR2, LDR3, errorpan, errortilt, preverrorpan, preverrortilt;
float PanPosition = 90;
float TiltPosition = 90;

float PanPropOut;
float TiltPropOut;
float Kpp = 0.02;
float PanDerOut;
float TiltDerOut;
float Kpd = 0.05;
float PanIntOut;
float TiltIntOut;
float Kpi = 0.00001;

void setup() 
{
 Serial.begin(9600);  
 pinMode(13, OUTPUT); 
 pan.attach(7);
 pan.write(90);
 tilt.attach(8);
 tilt.write(90);
 delay(1000);
}

void loop() 
{
LDR_read();
LDR_callibration();
LightDirection_to_PanServo();
/*LightDirection_to_TiltServo();*/
}
//Functions
void LDR_read()
{
 LDR1 = analogRead(A0);
 LDR2 = analogRead(A1);
 LDR3 = analogRead(A2);
 
 Serial.print(LDR1);
 Serial.print(",");
 Serial.print(LDR2);
 Serial.print(",");
 Serial.println(LDR3);
/*
 Serial.print(errorpan);
 Serial.print(",");
 Serial.println(errortilt);*/
 
}
void LDR_callibration()
{
 LDR1 = analogRead(A0);
 LDR1 = LDR1+0.0000000551*pow(LDR1, 3)-0.0001367619*pow(LDR1, 2)+0.0188846089*LDR1+55.5380799;
 
 LDR2 = analogRead(A1);
 LDR2 = LDR2-0.0000000014*pow(LDR2, 4)+0.0000031686*pow(LDR2, 3)-0.0026678229*pow(LDR2, 2)+1.0068194756*LDR2-122.998784;
 
 LDR3 = analogRead(A2)*0.91;
 LDR3 = LDR3+0.0000002885*pow(LDR3, 3)-0.0004019424*pow(LDR3, 2)+0.2100826929*LDR3-103.310999105;
}
void LightDirection_to_PanServo()
{
  float threshold = 100;
   if ((LDR1<threshold)&&(LDR2<threshold)&&(LDR3<threshold))
 {
  errorpan = 0;
  digitalWrite(13, HIGH);
  }
  else
  { 
    errorpan = LDR2-LDR1;
    digitalWrite(13, LOW);
  }
  PanPropOut = Kpp*errorpan;
  PanIntOut += Kpi*errorpan; 
  PanDerOut = Kpd*(errorpan-preverrorpan);
  preverrorpan = errorpan;
  PanPosition = PanPosition+PanPropOut+PanDerOut+PanIntOut;
  if (PanPosition>180)
  {
    PanPosition = 180;
  }
  if (PanPosition<0)
  {
    PanPosition = 0;
  }
  pan.write(PanPosition);
}
void LightDirection_to_TiltServo()
{
  float threshold = 100;
   if ((LDR1<threshold)&&(LDR2<threshold)&&(LDR3<threshold))
 {
  errortilt = 0;
  digitalWrite(13, HIGH);
  }
  else
  { 
    errortilt = LDR3-((LDR2+LDR1)/2);
    digitalWrite(13, LOW);
  }
  TiltPropOut = Kpp*errortilt;
  TiltIntOut += Kpi*errortilt;
  TiltDerOut = Kpd*(errortilt-preverrortilt);
  preverrortilt = errortilt;
  TiltPosition = TiltPosition-(TiltPropOut+TiltDerOut+TiltIntOut);
  if (TiltPosition>180)
  {
    TiltPosition = 180;
  }
  if (TiltPosition<0)
  {
    TiltPosition = 0;
  }
  tilt.write(TiltPosition);
}

  

