#include <avr/wdt.h>

#define frstb 4
#define scndb 3
#define thrdb 2
#define frthb 1
#define engine0w0 17
#define engine0w1 16
#define engine1w0 15
#define engine1w1 14
int delaytime = 5;
bool work = false;
int step0 = 0, step1 = 0;
int steps0eng = 0;
int revs = 0;
int revsprlay = 250; 
bool chk = false;

/*
Таблица истинности шаговика
первый выход мк 0 1
второй выход мк  0 1

Большой шаговый 96 шагов
Малый шаговый 20 (6.5 шагов для прохождения 1 мм)
*/

void setup() {
  wdt_enable(WDTO_2S);
  pinMode(engine0w0, OUTPUT);  
  pinMode(engine0w1, OUTPUT);
  pinMode(engine1w0, OUTPUT);
  pinMode(engine1w1, OUTPUT);
  pinMode(frstb, INPUT);
  pinMode(scndb, INPUT);
  pinMode(thrdb, INPUT);
  pinMode(frthb, INPUT);

}

void rotatingW0(int enginew0, int enginew1) { 
  switch(step0){
    case 0:
      digitalWrite(enginew0, false);      
      break;
    case 1:
      digitalWrite(enginew1, false);      
      break;
    case 2:
      digitalWrite(enginew0, true);      
      break;
    case 3:
      digitalWrite(enginew1, true);      
      break;
  }   
  step0++;  
  if(step0 == 4)
    step0 = 0;
}

void rotatingW1(int enginew0, int enginew1) {   
  switch(step1){
    case 0:
      digitalWrite(enginew0, false);      
      break;
    case 1:
      digitalWrite(enginew1, false);      
      break;
    case 2:
      digitalWrite(enginew0, true);      
      break;
    case 3:
      digitalWrite(enginew1, true);      
      break;
  }   
  step1++;  
  if(step1 == 4)
    step1 = 0;
}

void loop() {
  delay(delaytime); 
  if(revs % 2 != 0)
    chk = false;
  wdt_reset(); 

  if(steps0eng == -1){
    steps0eng = 96;
    revs--;
  }
  if(steps0eng == 97){
    steps0eng = 0;
    revs++;
  }
  if(digitalRead(frstb) == HIGH)
    work = true;
  if(digitalRead(scndb) == HIGH)
    work = false;  
      
  if(work == true){  
    delaytime = 10;  
    rotatingW0(engine0w0, engine0w1);
  }
  
  if(digitalRead(thrdb) == true /*&& revs > 0*/){     
    steps0eng--;   
    rotatingW0(engine0w1, engine0w0);     
    if(revs % 2 == 0 && chk == false){
      rotatingW1(engine1w1, engine1w0);
      chk = true;
    }    
  }  
  if(digitalRead(frthb) == true/* && revs < revsprlay*/){    
    steps0eng++;   
    rotatingW0(engine0w0, engine0w1);    
    if(revs % 2 == 0 && chk == false){
      rotatingW1(engine1w0, engine1w1);
      chk = true;
    }
  }
}
