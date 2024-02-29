const int ledPin = 5;  
const int buttonPin = 14; 

int buttonState;
int lastButtonState;
int ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  buttonState=digitalRead(buttonPin);
}

void loop() {
  lastButtonState=buttonState;
  buttonState=digitalRead(buttonPin);
  if (lastButtonState== HIGH && buttonState==LOW) {
    Serial.println("button pressed");
    if (ledState== LOW) {
      Serial.println("turning led on");
      ledState = HIGH;
    }
    else{
      ledState=LOW;
      Serial.println("Turning led off");
    }
      digitalWrite(ledPin, ledState); 
  }
}
