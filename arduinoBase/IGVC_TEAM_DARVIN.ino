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
      setMasterSwitch((s.substring(1)).toInt());
      break;
      case '2':
      changeDelay((s.substring(1)).toInt());
      break;
      case '3':
      setMotor1Pwm((s.substring(1)).toInt());
      break;
      case '4':
      setMotor1Clk();
      break;
      case '5':
      setMotor1AClk();
      break;
      case '6':
      getEncoderRotation();
      break;
      case '7':
      getDelay();
      break;
      case '8':
      getPwm();
      break;
      case '9':
      getMasterSwitch();
      break;
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


//SETTERS
void changeDelay(int ndelay)
{
  delayloop=ndelay;
}
void setMasterSwitch(int nms)
{
  masterSwitch=nms;
}
void setMotor1Pwm(int npwm)
{
  pwm=npwm;
}
void setMotor1Clk()
{
  digitalWrite(motor1pin1,HIGH);
  digitalWrite(motor1pin2,LOW);
}
void setMotor1AClk()
{
  digitalWrite(motor1pin2,HIGH);
  digitalWrite(motor1pin1,LOW);
}
//SETTERS



//GETTERS
void getEncoderRotation()
{
  
}
void getDelay()
{
  Serial.println(delayloop);
}
void getPwm()
{
  Serial.println(pwm);
}
void getMasterSwitch()
{
  Serial.println(masterSwitch);
}
//GETTERS
