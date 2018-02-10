#define NUM 4 //Number of pots
#define READ_INTERVAL 300000 //Milis between read
#define READ_DELAY 25 //Milis before read sensor
#define WATER_DELAY 10000 //Milis the relay is turned on
#define WATER_INTERVAL 290000 //Milis between watering and checking sensor
#define SENSOR_LOW 300 //If watering and sensor goes below this, watering is done.
#define SENSOR_HIGH 600 //If sensor goes up this, watering is need
#define NO_SENSOR 899 //If sensors goes up this, sensor is off the soil. Prevent infinite watering.
#define NO_WATER_NEED -1
#define LOGGING
//#undef LOGGING

struct Pot {
  int PowerSensor;
  int DataSensor;
  int PowerRelay;
  int Value; } pots[NUM];

int i;

void UpdateValues(){
  for(i = 0; i < NUM; i++){
    UpdateValue();
  }
}

void UpdateValue(){
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

void NeedWater(){
  for(i = 0; i < NUM; i++){
    if(NO_SENSOR < pots[i].Value){
      #ifdef LOGGING
      Serial.print(i);
      Serial.print(",NOSENSOR,");
      Serial.println(pots[i].Value);
      #endif
      continue;
    }else if(SENSOR_HIGH < pots[i].Value){
      #ifdef LOGGING
      Serial.print(i);
      Serial.print(",NEEDWATER,");
      Serial.println(pots[i].Value);
      #endif
      return;
    }
  }
  i = NO_WATER_NEED;
}

void WaterPot(){
  do{
    #ifdef LOGGING
    Serial.print(i);
    Serial.print(",WATERING,");
    Serial.println(pots[i].Value);
    #endif
    
    digitalWrite(pots[i].PowerRelay, LOW);
    delay(WATER_DELAY);
    digitalWrite(pots[i].PowerRelay, HIGH);
    delay(WATER_INTERVAL);

    UpdateValue();
  } while(SENSOR_LOW < pots[i].Value);
  Serial.print(i);
  Serial.print(",WATERDONE,");
  Serial.println(pots[i].Value);
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

    pots[i].Value = 0;
  }
}

void loop(){
  UpdateValues();
  NeedWater();
  if(i == NO_WATER_NEED){
    delay(READ_INTERVAL);
  }else{
    WaterPot();
  }
}
