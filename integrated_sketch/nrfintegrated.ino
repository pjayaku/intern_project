#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int LiquidSensorPin = 24;
int pw_pin=7;
int arraysize = 9;
int array[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
long inch;
int exact_cm_value;
RF24 radio(7, 8); // CNS, CE
const byte address[][6] = {"00001", "00002"};

void setup() {
  Serial.begin(9600);      
  pinMode(pw_pin, INPUT);
  pinMode(LiquidSensorPin, INPUT);
  radio.begin();
  radio.openWritingPipe(address[1]); // 00001
  radio.openReadingPipe(1, address[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);
}

int getLiquidLevel(){
    return !digitalRead(LiquidSensorPin);
}

void nrfTransmit(){
  radio.write(&text, sizeof(text));
  radio.stopListening();
  String threatMessage[]= "Sensor X Detected threat";
  radio.write(&threatMessage, sizeof(threatMessage));
}

void nrfInput(){
  char command;
  radio.startListening();
  while (!radio.available());
  radio.read(&command, sizeof(command));
  switch(command){
    case '1': //Provide info about threat break;
    case '2': //Remove threat;
    case '3': //Remove all threats;
    default: breal;
  }
}

void sensorRead(){
for(int i = 0; i < arraysize; i++)
{ 
inch = pulseIn(pw_pin, HIGH);
array[i] = inch/58;
delay(10);
}
}
void array_arrangment(int *a,int n){
// Author: Bill Gentles, Nov. 12, 2010)
for (int i = 1; i < n; ++i)
{
int j = a[i];
int k;
for (k = i - 1; (k >= 0) && (j < a[k]); k--)
{
a[k + 1] = a[k];
}
a[k + 1] = j;
}
}

int filter(int *a,int n){
int i = 0;
int count = 0;
int maxCount = 0;
int filter = 0;
int median;
int prevCount = 0;
while(i<(n-1)){
prevCount=count;
count=0;
while(a[i]==a[i+1]){
count++;
i++;
}
if(count>prevCount && count>maxCount){
filter=a[i];
maxCount=count;
median=0;
}
if(count==0){
i++;
}
if(count==maxCount){//If the dataset has 2 or more modes.
median=1;
}
if(filter==0||median==1){//Return the median if there is no mode.
filter=a[(n/2)];
}

return filter;

} 
}

void loop() {
  sensorRead();
  array_arrangment(array,arraysize);
  exact_cm_value= filter(array,arraysize);
  boolean underwater = getLiquidLevel();
  String liquidStatus = "Liquid level is not underwater";
  if(getLiquidLevel()){ // if threat detected
    liquidStatus = "Threat Detected: Liquid level is underwater";
    nrfTransmit();
    nrfInput();
  }
  
  Serial.print(liquidStatus + " Distance: " + (String) exact_cm_value);
  Serial.print("\n");
  delay(1000);
}


