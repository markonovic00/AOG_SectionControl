//Manual section control without AOG
//Automatic section control with AOG
//These two modes will be supported.
//First of there will be 8 Sections to start with, because we have 8 analog pins on arduino nano
//Number of sections
int sectionNum = 4; // Use from 1-8 !!! CHANGE THIS NUMBER FOR YOURSELF
//Sections pins declaration
int pinSections[] = {6,7,8,9,10,11,12,13};
//All sections off pin declaration
int pinMain = 5;
bool isMainOn = false;
//Switches pins declaration
int pinSwitches[] = {A0,A1,A2,A3,A4,A5,A6,A7}; //EDIT IF YOU NEED
//Auto or manual swtich
int pinAutoMan = 2;
bool isAuto = false;
//Pins 3-4 used for hydLift, i don't use them but somene can so I will leave it empty
uint8_t switchLo = 0; //SWITCH STATES this is where we store the switch states Sections1to8
uint8_t switchLoOff=255; // ALL TURNED OFF
uint8_t relayLo = 255; //RELAY STATES this is where we store the relay states Sections1to8 (255 because then it is 11111111 all relays off)

//SEND DATA TO AOG
uint8_t SEND_AOG[]={0x80 ,0x81 ,0x7f ,0xEA ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 , 0xCC};

void setup() {
  // put your setup code here, to run once:
  // Set pinMode for each pin
  Serial.begin(9600);
  for(int i=0;i<sectionNum;i++)
  {
    pinMode(pinSections[i],OUTPUT);
    pinMode(pinSwitches[i],INPUT);
  }
  pinMode(pinMain,INPUT);
  pinMode(pinAutoMan,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  switchLo=0;
  isMainOn=true;
  checkStates();
  setRelayStatesMan();
  setRelays();
  delay(500);
}

//Checking the states of the switches
void checkStates()
{
  for(int i=0; i<sectionNum;i++)
  {
    if(digitalRead(pinSwitches[i])==HIGH)
    {
      bitSet(switchLo,i); //SET BIT TO 1  if switch one is on 00000001  (group1on)
      bitClear(switchLoOff,i); //Opposite from switchLo (group1off)
      Serial.print(bitRead(switchLo,i));
    }
    else if (digitalRead(pinSwitches[i])==LOW)
    {
      bitClear(switchLo,i); //SET BIT TO 0
      bitSet(switchLoOff,i); // Opposite from switchLo
      Serial.print(bitRead(switchLo,i));
    }
    
  }
  Serial.println();
  //Check if auto
  if(digitalRead(pinAutoMan)==HIGH)
  {
    isAuto=true;
  }
  else if (digitalRead(pinAutoMan)==LOW)
  {
    isAuto=false;
  }
  //Check if main boom is on
  if(digitalRead(pinMain)==HIGH)
  {
    isMainOn=true;
  }
  else if (digitalRead(pinMain)==LOW)
  {
    isMainOn=false;
  }
}

//Setting the relay states for manual mode
void setRelayStatesMan()
{
  if(isMainOn){
    for(int i=0; i<sectionNum; i++)
    {
      if(bitRead(switchLo,i)==1)
      {
        bitClear(relayLo,i); //SET BIT TO 0 because then relay is on
      }
      else if(bitRead(switchLo,i)==0)
      {
        bitSet(relayLo,i); //SET BIT TO 1 because then relay is off
      }
    }
  }
  else if(isMainOn==false)
  {
    //Proveriti da li radi sa ovim 255
    relayLo=255; // ALL RELAYS OFF 
  }
}

void setRelays()
{
  for(int i=0; i<sectionNum; i++)
  {
    digitalWrite(pinSections[i],bitRead(relayLo,i));
  }
}
