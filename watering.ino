#define NUM 4 //Number of pots
#define READ_INTERVAL 300000
#define READ_DELAY 25
#define WATER_DELAY 10000
#define SENSOR_LOW 300
#define SENSOR_HIGH 600
#define NO_SENSOR 899
#define NO_WATER_NEED -1

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
}

void NeedWater(){
  for(i = 0; i < NUM; i++){
    if(NO_SENSOR < pots[i].Value){
      continue;
    }else if(SENSOR_HIGH < pots[i].Value){
      return;
    }
  }
  i = NO_WATER_NEED;
}

void WaterPot(){
  do{
    digitalWrite(pots[i].PowerRelay, LOW);
    delay(WATER_DELAY);
    digitalWrite(pots[i].PowerRelay, HIGH);
    delay(READ_INTERVAL);

    UpdateValue();
  } while(SENSOR_LOW < pots[i].Value);
}

void setup(){
  pots[0].PowerSensor = 2;
  pots[0].DataSensor = A0;
  pots[0].PowerRelay = 6;

  pots[1].PowerSensor = 3;
  pots[1].DataSensor = A1;
  pots[1].PowerRelay = 7;

  pots[2].PowerSensor = 4;
  pots[2].DataSensor = A2;
  pots[2].PowerRelay = 8;

  pots[3].PowerSensor = 5;
  pots[3].DataSensor = A3;
  pots[3].PowerRelay = 9;

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
