#include <SoftwareSerial.h>

byte Buffer[64];
size_t bufLength;

SoftwareSerial receiver(4, 5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  receiver.begin(19200);

  pinMode(6, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(receiver.available() == 8){
      digitalWrite(13, HIGH);

      byte tempBuffer[8];
      
      for(size_t i = 0; i < 8; i++){
        tempBuffer[i] = receiver.read();
      }

      for(size_t i = 0; i < 8; i++){
        Buffer[bufLength*8+i] = tempBuffer[i];
      }

      bufLength++;
            
      digitalWrite(13, LOW);
  }

  if(Serial.available()){

    //Clears the Serial buffer
    while(Serial.available()){
      Serial.read();
    }

    for(size_t i = 0; i < bufLength; i++){
      for(size_t j = 0; j < 8; j++){
        Serial.write(Buffer[i*8+j]);
      }
      Serial.println();
    }

    bufLength = 0;
  }


}
