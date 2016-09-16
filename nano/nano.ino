#include <SoftwareSerial.h>

#define bufferSize 80
#define sensorCount 10

byte inputBuffer[bufferSize];

byte byteBuffer[sensorCount][8];
int numByteBuffer = 0;
int result = 0;

SoftwareSerial receiver(5, 4);

void setup() {
  Serial.begin(115200);
  receiver.begin(19200);
  arrayClear();
}

void loop() {

  checkReciever();
  checkSerial();

}

void checkReciever(){
  while(receiver.available() > 8){
    
    byte TEMP[8];

    for(int i = 0; i < 8; i++){
      TEMP[i] = receiver.read();
    }

    //TEMP valide
    if(TEMP[0] == 0x02 && TEMP[7] == 0x03){ 

      //Schon Sensorenworte vorhanden?
      if(numByteBuffer > 0){

        int result = -1;

        //SensorID schon vorhanden?
        for(int i = 0; i < numByteBuffer; i++){

          
          if(byteBuffer[i][1] == convertTEMPOne(TEMP[1])){
              result = i;
              break;
          }
          
        }

        //Sensor schon vorhanden => ersetzen
        if(result >= 0){
          writeTEMP(result, TEMP);
          
        }else{
          if(numByteBuffer < sensorCount){
            writeTEMP(numByteBuffer, TEMP);
            numByteBuffer++;
          }
        }
        
      }else{
        //wenn noch keine Sensorenwerte existieren
        writeTEMP(0, TEMP);
        numByteBuffer++;
      }
    
    }else{
      //Wenn etwas schief ging: Weg damit
      
      while(receiver.available() > 0)
          receiver.read();

      break;    
    }
  }  
}

void checkSerial(){
  if(Serial.available()){
    
    //Clears the Serial buffer
    while(Serial.available()){
      Serial.read();
    }

    String temp = "";

    for(size_t i = 0; i < numByteBuffer; i++){
      for(size_t j = 0; j < 8; j++){
        temp += String(byteBuffer[i][j], HEX);
      }
        
    }
    Serial.print(temp);
    arrayClear();

  }
}

byte convertTEMPOne(byte temp){
  return (temp & (byte)0x7f);  
}

void writeTEMP(int num, byte TEMP[8]){
  for(int i = 0; i < 8; i++){
    if(i != 1){
      byteBuffer[num][i] = TEMP[i];
    }else{
      byteBuffer[num][1] = convertTEMPOne(TEMP[1]);
    }
  }
}

void arrayClear(){
  for(int i = 0; i < sensorCount; i++){
    for(int j = 0; j < 8; j++)
      byteBuffer[j][i] = 0x00;  
  }
  numByteBuffer = 0;
}

