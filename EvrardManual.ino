//Manual Sectino control for every sprayer
//Schematics for wiring will be provided in documentation
//Using Arduino Uno, witch means 6 sectinos, You can use Arduino nano for up to 8 Sections
//in my case I am using 4 sections.

int sectionNum = 4; // Use from 1-8 !!!
//Section pins
int pinSections[] = {6,7,8,9,10,11,12,13};
//All sections off/On
int pinMain=5;
//Switches pins
int pinSwitches[] = {A0,A1,A2,A3,A4,A5,A6,A7};
bool isMainOn=false;
//Working Logic
uint8_t switchLo=0; // Storing switch states as each bit in the number
uint8_t relayLo =255; //Turned off Relays

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i=0;i<sectionNum;i++)
  {
    pinMode(pinSections[i],OUTPUT);
    pinMode(pinSwitches[i],INPUT);
  }
  pinMode(pinMain,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkStates();
  setRelayStates();
  setRelays();
  delay(500);
}

//Checking siwtch states
void checkStates()
{
  for(int i=0;i<sectionNum;i++)
  {
    if(digitalRead(pinSwitches[i])==HIGH)
    {
      bitSet(switchLo,i); //SET BIT TO 1 if switch 1 is on 00000001
    }
    else if (digitalRead(pinSwitches[i])==LOW)
    {
      bitClear(switchLo,i); //SET BIT TO 0
    }
  } 

  //Boom ON/OFF
  if(digitalRead(pinMain)==HIGH)
  {
    isMainOn=true;
  }
  else if(digitalRead(pinMain)==LOW)
  {
    isMainOn=false; 
  }
}

//Set relay states
void setRelayStates()
{
  if(isMainOn)
  {
    //MANUALLY ADJUSTING
    /*for(int i=0; i<sectionNum;i++)
    {
      if(bitRead(switchLo,i)==1)
      {
        bitClear(relayLo,i);
        Serial.print(bitRead(relayLo,i));
        
      }
      else if(bitRead(switchLo,i)==0)
      {
        bitSet(relayLo,i);
        Serial.print(bitRead(relayLo,i));
      }
    }
    Serial.println();
    for(int i=0; i<sectionNum;i++)
    {
      Serial.print(bitRead(flipped,i));
    }
    Serial.println();*/
    //WE CAN JUST FLIP THEM
    relayLo=~switchLo;
  }
  else if(isMainOn==false)
  {
    relayLo=255; //ALL RELAYS OFF 
    //RELAYS OFF WHEN 11111111
  }
}

void setRelays()
{
  for(int i=0; i<sectionNum; i++)
  {
    digitalWrite(pinSections[i],bitRead(relayLo,i));  
  }
}
