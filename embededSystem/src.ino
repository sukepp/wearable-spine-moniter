
#define curveSensorPin A0
#define motorPin 3
#define beepPin 4

int curveSensorBuffer = 0;
char curveSensorByte = 0;
char curveSensorByteLast = 0;
int curveSensorInit = 0;
int curveSensorSum = 0;
int vibrationLevel = 6;

void calibrate()
{
  curveSensorSum = 0;
  for (int i = 0; i < 20; i++) {
    curveSensorSum += analogRead(curveSensorPin);
    delay(50);
  }  
  curveSensorInit = curveSensorSum / 20;
}

void setup() 
{
  Serial.begin(9600);
  calibrate();
  pinMode(beepPin, OUTPUT);
}

void loop() 
{ 
  curveSensorBuffer = 0;
  for (int i = 0; i < 10; i++) {
    curveSensorBuffer += analogRead(curveSensorPin);
    delay(30); 
  }
  curveSensorBuffer = curveSensorBuffer / 10;
  curveSensorByte = constrain(curveSensorInit - curveSensorBuffer, 0, 4) + 1;
  
  curveSensorByte = (curveSensorByte + curveSensorByteLast) / 2;
  Serial.print(curveSensorByte);
  curveSensorByteLast = curveSensorByte;
  
  if (curveSensorByte >= vibrationLevel) {
    analogWrite(motorPin, 180);
    digitalWrite(beepPin, LOW);
  } else {
    analogWrite(motorPin, 0);
    digitalWrite(beepPin, HIGH);
  } 
}

void serialEvent() {
  while (Serial.available()) {
    char inCharStart = (char)Serial.read();
    char inChar = (char)Serial.read();
    char inCharEnd = (char)Serial.read();

    if (inCharStart == 'f' && inCharEnd == 'e') {
      switch (inChar) {
      case 'c':
        calibrate();
        break;
      case '0' + 1:
        vibrationLevel = 1;
        break;
      case '0' + 2:
        vibrationLevel = 2;
        break;
      case '0' + 3:
        vibrationLevel = 3;
        break;
      case '0' + 4:
        vibrationLevel = 4;
        break;
      case '0' + 5:
        vibrationLevel = 5;
        break;
      default:
        break;
      }
    }
  }
}
