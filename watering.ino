#define NUM 4 //Number of pots
#define READ_INTERVAL 300000 //Milis between read
#define READ_DELAY 25 //Milis before read sensor
#define WATER_DELAY 10000 //Milis the relay is turned on
#define WATER_MUTE 4
#define SENSOR_LOW 400 //If watering and sensor goes below this, watering is done.
#define SENSOR_HIGH 600 //If sensor goes up this, watering is need
#define NO_SENSOR 899 //If sensors goes up this, sensor is off the soil. Prevent infinite watering.
#define NO_WATER_NEED -1
#define LOGGING
//#undef LOGGING

struct Pot {
  int PowerSensor;
  int DataSensor;
  int WaterMute;
  boolean Watering;
  int PowerRelay;
  int Value; } pots[NUM];

int i, n;

void UpdateValues(){
  for(i = 0; i < NUM; i++){
    digitalWrite(pots[i].PowerSensor, HIGH);
    delay(READ_DELAY);
    pots[i].Value = analogRead(pots[i].DataSensor);
    digitalWrite(pots[i].PowerSensor, LOW);

    #ifdef LOGGING
    Serial.print(i);
    Serial.print(",UPDATE,");
    Serial.println(pots[i].Value);
    #endif
  }
}

void NeedWater(){
  for(i = 0; i < NUM; i++){
    if(pots[i].Watering){
      if(pots[i].WaterMute == 0){
        pots[i].Watering = pots[i].Value > SENSOR_LOW; //if Watering and WaterMute == 0, pot is watered
        if(pots[i].Watering == false){
          Serial.print(i);
          Serial.print(",DONEWATER,");
          Serial.println(pots[i].Value);
        }
      }else{
        pots[i].WaterMute--;
      }
    }else{
      if(NO_SENSOR < pots[i].Value){
        #ifdef LOGGING
        Serial.print(i);
        Serial.print(",NOSENSOR,");
        Serial.println(pots[i].Value);
        #endif
      }else if(SENSOR_HIGH < pots[i].Value){
        #ifdef LOGGING
        Serial.print(i);
        Serial.print(",NEEDWATER,");
        Serial.println(pots[i].Value);
        #endif
        pots[i].Watering = true;
      }
    }
  }
}

void WaterPots(){
  n = 0;
  for(i = 0; i < NUM; i++){
    if(pots[i].Watering && pots[i].WaterMute == 0){
      #ifdef LOGGING
      Serial.print(i);
      Serial.print(",WATERING,");
      Serial.println(pots[i].Value);
      #endif

      digitalWrite(pots[i].PowerRelay, LOW);
      delay(WATER_DELAY);
      digitalWrite(pots[i].PowerRelay, HIGH);
      
      pots[i].WaterMute = WATER_MUTE; //Mute the watering
      
      n++;
    }
  }
}

void DelayRead(){
  n *= WATER_DELAY;
  n *= -1;
  n += READ_INTERVAL;
  delay(n);
}

void setup(){
  #ifdef LOGGING
  Serial.begin(9600);
  #endif
  
  pots[0].PowerSensor = 2;
  pots[0].DataSensor = A0;
  pots[0].PowerRelay = 6;

  if(NUM > 0){
    pots[1].PowerSensor = 3;
    pots[1].DataSensor = A1;
    pots[1].PowerRelay = 7;
  }

  if(NUM > 1){
    pots[2].PowerSensor = 4;
    pots[2].DataSensor = A2;
    pots[2].PowerRelay = 8;
  }

  if(NUM > 2){
    pots[3].PowerSensor = 5;
    pots[3].DataSensor = A3;
    pots[3].PowerRelay = 9;
  }

  for(i = 0; i < NUM; i++){
    pinMode(pots[i].PowerSensor, OUTPUT);
    pinMode(pots[i].DataSensor, INPUT);
    pinMode(pots[i].PowerRelay, OUTPUT);
    
    digitalWrite(pots[i].PowerRelay, HIGH);

    pots[i].Value = 0;
    pots[i].WaterMute = 0;
    pots[i].Watering = false;
  }
}

void loop(){
  UpdateValues();
  NeedWater();
  WaterPots();
  DelayRead();
  
}
