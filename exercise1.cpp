/*
* Name & Email: Valerie Wong
* Discussion Section: 021
* Assignment: Lab 6 Exercise 1
* Exercise Description: Automatic Ice Maker

* I acknowledge all content contained herein, excluding template 
  or example code, is my own original work.

* Demo Link: https://youtu.be/i3nY3y5vkJs
*/

/*
NOTE:
* DO NOT USE ANALOGWRITE OR YOU WILL LOSE POINTS
*/

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];

// New variables
int waterLevelSensor;    // A0
int iceLevelSensor;     // A1
bool isOFF = true;      // Determines if system is ON/OFF
bool wait = false;      // Determines if in the wait state
bool waterLOW = false;    // Water is low -- Red LED ON
bool iceFULL = false;   // Ice is full -- Yellow LED ON
unsigned char cycle = 0;  // Determines which part of the system to go back to after pausing
// Pins
int iceTray = 2;
int waterPump = 3;
int waterLevel = 4;
int nitroPump = 5;
int iceDispenser = 6;
// Counters
unsigned char i, j, k = 0;

// Helper Functions
void readPins() {
  waterLevelSensor = analogRead(A0);
  iceLevelSensor = analogRead(A1);
}

// Turns all LEDs off
void ledsOFF() {
  digitalWrite(iceTray, LOW);
  digitalWrite(waterPump, LOW);
  digitalWrite(waterLevel, LOW);
  digitalWrite(nitroPump, LOW);
  digitalWrite(iceDispenser, LOW);
}

// Turns Red LED on
void waterLevelON() {
  digitalWrite(iceTray, LOW);
  digitalWrite(waterPump, LOW);
  digitalWrite(waterLevel, HIGH);
  digitalWrite(nitroPump, LOW);
  digitalWrite(iceDispenser, LOW);
}

// Turns Yellow LED on
void iceTrayFULL() {
  digitalWrite(iceTray, HIGH);
  digitalWrite(waterPump, LOW);
  digitalWrite(waterLevel, LOW);
  digitalWrite(nitroPump, LOW);
  digitalWrite(iceDispenser, LOW);
}

// Turns Blue LED on
void waterPumpON() {
  digitalWrite(iceTray, LOW);
  digitalWrite(waterPump, HIGH);
  digitalWrite(waterLevel, LOW);
  digitalWrite(nitroPump, LOW);
  digitalWrite(iceDispenser, LOW);
}

// Turns White LED on
void liquidNitroON() {
  digitalWrite(iceTray, LOW);
  digitalWrite(waterPump, LOW);
  digitalWrite(waterLevel, LOW);
  digitalWrite(nitroPump, HIGH);
  digitalWrite(iceDispenser, LOW);
}

// Turns Green LED on
void dispenserON() {
  digitalWrite(iceTray, LOW);
  digitalWrite(waterPump, LOW);
  digitalWrite(waterLevel, LOW);
  digitalWrite(nitroPump, LOW);
  digitalWrite(iceDispenser, HIGH);
}

// ONLY blue, white, & green LEDs off
void systemOFF() {
  digitalWrite(waterPump, LOW);
  digitalWrite(nitroPump, LOW);
  digitalWrite(iceDispenser, LOW);
}

// Checks if water level is low
bool checkWaterLevel() {
  bool isLow = false;
  
  if (waterLevelSensor < 30 && iceLevelSensor > 30) {
    isLow = true;
  }
  
  return isLow;
}

// Checks if ice is full
bool checkIceLevel() {
  bool isFull = false;
  if (waterLevelSensor > 30 && iceLevelSensor < 50) {
    isFull = true;
  }
  
  return isFull;
}


// Sensors
enum SM1_States { START_1, ON, OFF, WAIT, WATER_LOW, ICE_FULL};
int SM1_Tick(int state1) {
  switch (state1) { // State transitions
    case START_1:
      state1 = OFF;
      break;
    
    case OFF:
      readPins();
      if (waterLevelSensor < 30 && iceLevelSensor < 30) {
        wait = true;
        state1 = WAIT;
      }
      break;
    
    case WAIT:
      readPins();
      if (waterLevelSensor > 40 && iceLevelSensor > 40 && isOFF) {
        isOFF = false;
        wait = false;
        state1 = ON;
      }
      else if (waterLevelSensor > 70 && iceLevelSensor > 70 && !isOFF) {
        isOFF = true;
        wait = false;
        state1 = OFF;
      }
      break;
    
    case ON:
      readPins();
      waterLOW = checkWaterLevel();
    
      if (waterLOW) {
        state1 = WATER_LOW;
      }
    
      if (i > 4) {
        iceFULL = checkIceLevel();
        i = 0;
        
        if (iceFULL) {
          state1 = ICE_FULL; 
        }
      }
    
      // !A0!A1 -- Turning OFF
      if (waterLevelSensor < 40 && iceLevelSensor < 40) {
        wait = true;
        state1 = WAIT;
      }
      break;

    case WATER_LOW:
      readPins();
    
      if (waterLevelSensor > 50 && iceLevelSensor > 50) {
        ledsOFF();
        waterLOW = false;
        state1 = ON;
      }
      break;

    case ICE_FULL:
      readPins();
    
      if (waterLevelSensor > 70 && iceLevelSensor > 70) {
        ledsOFF();
        iceFULL = false;
        state1 = ON;
      }
      break;
  }
  
  switch (state1) { // State Action
    case START_1:
      break;
    
    case OFF: 
      break;
    
    case WAIT:
      break;
    
    case ON:
      ++i;
      break;

    case WATER_LOW:
      waterLevelON();
      break;

    case ICE_FULL:
      iceTrayFULL();
      break;
  }
  
  return state1;
}

// System Cycle
enum SM2_States { START_2, OFF_2, WAIT_2, WATERPUMP_H, WATERPUMP_L, NITRO_H, NITRO_L, DISPENSER_H, DISPENSER_L };
int SM2_Tick(int state2) {
  switch (state2) { // State transitions
    case START_2:
      state2 = OFF_2;
      break;
    
    case OFF_2:
      if (!wait) {
        state2 = OFF_2;
      }
      else {
        state2 = WAIT_2;
      }
      break;
    
    case WAIT_2:
      if (wait || waterLOW || iceFULL) {
        state2 = WAIT_2;
      }
      else if (!wait && !waterLOW && !iceFULL && cycle == 0) {
        j = 0;
        k = 0;
        state2 = WATERPUMP_H;
      }
      else if (!wait && !waterLOW && !iceFULL && cycle == 1) {
        cycle = 0;
        state2 = WATERPUMP_H;
      }
      else if (!wait && !waterLOW && !iceFULL && cycle == 2) {
        cycle = 0;
        state2 = NITRO_H;
      }
      else if (!wait && !waterLOW && !iceFULL && cycle == 3) {
        cycle = 0;
        state2 = DISPENSER_H;
      }
      break;
    
    case WATERPUMP_H:
      if (isOFF) {
        cycle = 0;
        state2 = OFF_2;
      }
      else if (wait || waterLOW || iceFULL) {
        cycle = 1;
        state2 = WAIT_2;
      }
      else if (k > 10) {
        j = 0;
        k = 0;
      state2 = NITRO_H;
      }
      else if (j <= 6 && k <= 10) {
        state2 = WATERPUMP_H;
      }
      else if (j > 6 && k <= 10) {
        j = 0;
        state2 = WATERPUMP_L; 
      }
      break;

    case WATERPUMP_L:
      if (k > 10) {
        j = 0;
        k = 0;
      state2 = NITRO_H;
      }
      else if (j <= 24 && k <= 10) {
        state2 = WATERPUMP_L;
      }
      else if (j > 24 && k <= 10) {
        j = 0;
        ++k;
        state2 = WATERPUMP_H;
      }      
      break;
    
    case NITRO_H:
      if (isOFF) {
        cycle = 0;
        state2 = OFF_2;
      }
      else if (wait || waterLOW || iceFULL) {
        cycle = 2;
        state2 = WAIT_2;
      }
      else if (k > 10) {
        j = 0;
        k = 0;
      state2 = DISPENSER_H;
      }
      else if (j <= 35 && k <= 10) {
        state2 = NITRO_H;
      }
      else if (j > 35 && k <= 10) {
        j = 0;
        state2 = NITRO_L; 
      }
      break;
    
    case NITRO_L:
      if (k > 10) {
        j = 0;
        k = 0;
      state2 = DISPENSER_H;
      }
      else if (j <= 15 && k <= 10) {
        state2 = NITRO_L;
      }
      else if (j > 15 && k <= 10) {
        j = 0;
        ++k;
        state2 = NITRO_H;
      }     
      break;
    
    case DISPENSER_H:
      if (isOFF) {
        cycle = 0;
        state2 = OFF_2;
      }
      else if (wait || waterLOW || iceFULL) {
        cycle = 3;
        state2 = WAIT_2;
      }
      else if (k > 20) {
        j = 0;
        k = 0;
      state2 = WATERPUMP_H;
      }
      else if (j <= 1 && k <= 20) {
        state2 = DISPENSER_H;
      }
      else if (j > 1 && k <= 20) {
        j = 0;
        state2 = DISPENSER_L; 
      }
      break;
    
    case DISPENSER_L:
      if (k > 20) {
        j = 0;
        k = 0;
      state2 = WATERPUMP_H;
      }
      else if (j <= 9 && k <= 20) {
        state2 = DISPENSER_L;
      }
      else if (j > 9 && k <= 20) {
        j = 0;
        ++k;
        state2 = DISPENSER_H;
      }     
      break;
  }
  
  switch (state2) { // State Action
    case START_2:
      ledsOFF();
      break;
    
    case OFF_2:
      ledsOFF();
      break;
    
    case WAIT_2:
      systemOFF();
      break;
    
    case WATERPUMP_H:
      ++j;
      waterPumpON();
      break;

    case WATERPUMP_L:
      ++j;
      ledsOFF();
      break;
    
    case NITRO_H:
      ++j;
      liquidNitroON();
      break;
    
    case NITRO_L:
      ++j;
      ledsOFF();
      break;
    
    case DISPENSER_H:
      ++j;
      dispenserON();
      break;
    
    case DISPENSER_L:
      ++j;
      ledsOFF();
      break;
  }

  return state2;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(iceTray, OUTPUT);
  pinMode(waterPump, OUTPUT);
  pinMode(waterLevel, OUTPUT);
  pinMode(nitroPump, OUTPUT);
  pinMode(iceDispenser, OUTPUT);
  
  unsigned char i = 0;
  tasks[i].state = START_1;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = START_2;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

  delay_gcd = 1000; // GCD
  Serial.begin(9600);
}

void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
}
