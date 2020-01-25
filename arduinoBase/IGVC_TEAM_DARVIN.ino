int motor1pin1=5;
int motor1pin2=6;
int motor1pwmpin=9;
int pwm=0;
int delayloop=1000;
int masterSwitch=0;

void setup()
{
  Serial.begin(9600);
  pinMode(motor1pin1,OUTPUT);
  pinMode(motor1pin2,OUTPUT);
  pinMode(motor1pwmpin,OUTPUT);
}

void loop()
{
  if(Serial.available()>0)
  {
    String s = Serial.readStringUntil('\n');
    char ch=s.charAt(0);
    switch(ch)
    {
      case '1':
    }
  }
  if(masterSwitch==1)
  {
    analogWrite(motor1pwmpin,pwm);
  }
  else if(masterSwitch==0)
  {
    analogWrite(motor1pwmpin,0);
  }
  
  delay(delayloop);
}

void changeDelay(int ndelay)
{
  delayloop=ndelay;
}

void motor1Clk()
{
  digitalWrite(motor1pin1,HIGH);
  digitalWrite(motor1pin2,LOW);
}

void motor1AClk()
{
  digitalWrite(motor1pin2,HIGH);
  digitalWrite(motor1pin1,LOW);
}

void motor1Pwm(int npwm)
{
  pwm=npwm;
}

void getEncoderRotation()
{
  
}

void setMasterSwitch(int nms)
{
  masterSwitch=nms;
}
