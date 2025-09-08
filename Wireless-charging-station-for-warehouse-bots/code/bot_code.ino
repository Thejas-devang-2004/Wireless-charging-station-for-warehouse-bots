#include <Arduino.h> 
bool leftdone=false; 
bool rightdone=false; 
bool stopped=true; 
bool juncdetected=false; 
bool enddetected=false; 
 
int leftState; 
int middleState; 
int rightState; 
 
int leftIR = 9; 
int middleIR = 10; 
int rightIR = 11; 
int M1F = 4; 
int M1R = 5; 
int M1Speed = 3; 
int M2F = 8; 
int M2R = 7; 
int M2Speed = 6; 
 
String lastCommand = "stop"; 
String recent = "start"; 
int juncs=0; 
 
#include "BluetoothSerial.h" 
#include <Arduino.h> 
BluetoothSerial serialBT; 
 
char btSignal; 
 
#define R 0 
#define L 1 
//initial Speed 
int Speed = 90; 
 
//declare channel for pwm Output 
#define R 0 
#define L 1 
 
//PWM Pin for Controlling the speed 
int enA = 26; 
int enB = 25; 
 

 
20 
//motor controlling pin 
 
int IN4 = 13; 
int IN3 = 12; 
int IN2 = 14; 
int IN1 = 27; 
*/ 
void setup() { 
   
  serialBT.begin("Cool Car"); 
 
  pinMode(enA, OUTPUT); 
  pinMode(enB, OUTPUT); 
   
  ledcSetup(R, 5000, 8);  // Channel 0 for Motor A, 5 kHz frequency, 8-bit resolution 
  ledcAttachPin(enA, R); 
  ledcSetup(L, 5000, 8);  // Channel 0 for Motor A, 5 kHz frequency, 8-bit resolution 
  ledcAttachPin(enB, L); 
 
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
 
  stop(); 
 
  pinMode(M1F, OUTPUT); 
  pinMode(M1R, OUTPUT); 
  pinMode(M2F, OUTPUT); 
  pinMode(M2R, OUTPUT); 
 
  pinMode(leftIR, INPUT); 
  pinMode(middleIR, INPUT); 
  pinMode(rightIR, INPUT); 
 
  pinMode(M1Speed, OUTPUT); 
  pinMode(M2Speed, OUTPUT); 
  analogWrite(M1Speed, 90); 
  analogWrite(M2Speed, 90); 
} 
 
void loop() { 
   
  while (serialBT.available()) { 
    btSignal = tolower((char)serialBT.read()); 
    if (btSignal=="a"){ 
      if (recent!="start"){ 
        navigate(recent,"start"); 
      } 
    } 
    else if (btSignal=="b"){ 

 
21 
      if (recent!="L1"){ 
        navigate(recent,"L1"); 
      } 
    } 
    else if (btSignal=="c"){ 
      if (recent!="R1"){ 
        navigate(recent,"R1"); 
      } 
    } 
    else if (btSignal=="d"){ 
      if (recent!="L2"){ 
        navigate(recent,"L2"); 
      } 
    } 
    else if (btSignal=="e"){ 
      if (recent!="R1"){ 
        navigate(recent,"R1"); 
      } 
    } 
    else if (btSignal=="f"){ 
      if (recent!="end"){ 
        navigate(recent,"end"); 
      } 
    } 
  } 
   
} 
 
void IRstates(){ 
  leftState = digitalRead(leftIR); 
  middleState = digitalRead(middleIR); 
  rightState = digitalRead(rightIR);  
} 
 
void lineFollow(){ 
  IRstates(); 
  if (middleState == HIGH) { 
    forward(); 
    lastCommand = "forward"; 
  } 
  else if (leftState == HIGH) { 
    left(); 
    lastCommand = "left"; 
  } 
  else if (rightState == HIGH) { 
    right(); 
    lastCommand = "right"; 
  } 
  else { 
    if (lastCommand == "forward") { 
      forward(); 

 
22 
    } 
    else if (lastCommand == "left") { 
      left(); 
    } 
    else if (lastCommand == "right") { 
      right(); 
    } 
    else { 
      stop(); 
    } 
  } 
} 
 
void leftjunc(){ 
  while (juncdetected == true){ 
    left(); 
    IRstates(); 
    if (leftState==HIGH){ 
      while (juncdetected == true){ 
        left(); 
        IRstates(); 
        if (middleState==HIGH){ 
          juncdetected = false; 
        } 
      }  
    }    
  } 
  lastCommand = "left"; 
} 
 
void rightjunc(){ 
  while (juncdetected == true){ 
    right(); 
    IRstates(); 
    if (rightState==HIGH){ 
      while (juncdetected == true){ 
        right(); 
        IRstates(); 
        if (middleState==HIGH){ 
          juncdetected = false; 
        } 
      }  
    }    
  } 
  lastCommand = "right"; 
} 
 
void juncturner(String direction){ 
  juncs=0; 
  while(juncs==0){ 
    IRstates(); 

 
23 
    if (leftState == HIGH && middleState == HIGH && rightState == HIGH){ 
      juncdetected=true; 
      stop(); 
      juncs++; 
      delay(300); 
    } 
    if (juncdetected==true){ 
      if (direction=="right"){ 
        rightjunc();   
      } 
      else if (direction=="left"){ 
        leftjunc(); 
      } 
      else if (direction=="straight"){ 
        stop(); 
        lastCommand = "forward"; 
      } 
    } 
    else{ 
      lineFollow(); 
    } 
  } 
} 
 
void uturn(){ 
  while(juncdetected==true){ 
    backward(); 
    IRstates(); 
    if ( 
      (leftState == LOW && middleState == LOW && rightState == LOW)  || 
       
      (leftState == HIGH && middleState == LOW && rightState == LOW) || 
      (leftState == LOW && middleState == LOW && rightState == HIGH) || 
       
 
      (leftState == HIGH && middleState == LOW && rightState == HIGH) 
    ) 
    { 
      juncdetected = false; 
      delay(500); 
      stop(); 
    } 
  } 
  while(juncdetected==false){ 
    right(); 
    IRstates(); 
    if (rightState==HIGH){ 
      while (juncdetected == false){ 
        right(); 
        IRstates(); 
        if (middleState==HIGH){ 

 
24 
          juncdetected = true; 
          stop(); 
        } 
      } 
    } 
  } 
  juncdetected = false; 
  lastCommand = "right"; 
} 
 
void uturner(){ 
  juncs=0; 
  while(juncs==0){ 
    IRstates(); 
    if (leftState == HIGH && middleState == HIGH && rightState == HIGH){ 
      juncdetected=true; 
      stop(); 
      juncs++; 
      delay(300); 
    } 
    if(juncdetected==true){ 
      uturn();    
    } 
    else{ 
      lineFollow(); 
    } 
  } 
} 
// --- Motor control functions --- 
void left() { 
  digitalWrite(M1F, LOW); 
  digitalWrite(M1R, HIGH); 
  digitalWrite(M2F, HIGH); 
  digitalWrite(M2R, LOW); 
} 
 
void right() { 
  digitalWrite(M1F, HIGH); 
  digitalWrite(M1R, LOW); 
  digitalWrite(M2F, LOW); 
  digitalWrite(M2R, HIGH); 
} 
 
void forward() { 
  digitalWrite(M1F, HIGH); 
  digitalWrite(M1R, LOW); 
  digitalWrite(M2F, HIGH); 
  digitalWrite(M2R, LOW); 
} 
 
void backward() { 

 
25 
  digitalWrite(M1F, LOW); 
  digitalWrite(M1R, HIGH); 
  digitalWrite(M2F, LOW); 
  digitalWrite(M2R, HIGH); 
} 
 
void stop() { 
  digitalWrite(M1F, LOW); 
  digitalWrite(M1R, LOW); 
  digitalWrite(M2F, LOW); 
  digitalWrite(M2R, LOW); 
} 
 
void navigate(String current, String destination){ 
 
  if (current=="start"){ 
    if(destination=="R1"){ 
      juncturner("right"); 
      uturner(); 
      recent="R1"; 
    } 
    else if(destination=="L1"){ 
      juncturner("left"); 
      uturner(); 
      recent="L1"; 
    } 
    else if(destination=="R2"){ 
      juncturner("straight"); 
      juncturner("right"); 
      uturner(); 
      recent="R2"; 
    } 
    else if(destination=="L2"){ 
      juncturner("straight"); 
      juncturner("left"); 
      uturner(); 
      recent="L2"; 
    } 
    else if(destination=="end"){ 
      juncturner("straight"); 
      juncturner("straight"); 
      uturner(); 
      recent="end"; 
    } 
  } 
  else if (current=="L1"){ 
    if(destination=="start"){ 
      juncturner("right"); 
      uturner(); 
      recent="start"; 
    } 

 
26 
    else if(destination=="R1"){ 
      juncturner("straight"); 
      uturner(); 
      recent="R1"; 
    } 
    else if(destination=="L2"){ 
      juncturner("left"); 
      juncturner("left"); 
      uturner(); 
      recent="L2"; 
    } 
    else if(destination=="R2"){ 
      juncturner("left"); 
      juncturner("right"); 
      uturner(); 
      recent="R2"; 
    } 
    else if(destination=="end"){ 
      juncturner("left"); 
      juncturner("straight"); 
      uturner(); 
      recent="end"; 
    } 
  } 
 
  else if (current=="R1"){ 
    if(destination=="start"){ 
      juncturner("left"); 
      uturner(); 
      recent="start"; 
    } 
    else if(destination=="L1"){ 
      juncturner("straight"); 
      uturner(); 
      recent="L1"; 
    } 
    else if(destination=="L2"){ 
      juncturner("right"); 
      juncturner("left"); 
      uturner(); 
      recent="L2"; 
    } 
    else if(destination=="R2"){ 
      juncturner("right"); 
      juncturner("right"); 
      uturner(); 
      recent="R2"; 
    } 
    else if(destination=="end"){ 
      juncturner("right"); 
      juncturner("straight"); 

 
27 
      uturner(); 
      recent="end"; 
    } 
  } 
 
  else if (current=="L2"){ 
    if(destination=="start"){ 
      juncturner("right"); 
      juncturner("straight"); 
      uturner(); 
      recent="L2"; 
    } 
    else if(destination=="R1"){ 
      juncturner("right"); 
      juncturner("left"); 
      uturner(); 
      recent="R1"; 
    } 
    else if(destination=="R2"){ 
      juncturner("straight"); 
      uturner(); 
      recent="R2"; 
    } 
    else if(destination=="L1"){ 
      juncturner("right"); 
      juncturner("right"); 
      uturner(); 
      recent="L1"; 
    } 
    else if(destination=="end"){ 
      juncturner("left"); 
      uturner(); 
      recent="end"; 
    } 
  } 
 
  else if (current=="R2"){ 
    if(destination=="start"){ 
      juncturner("left"); 
      juncturner("straight"); 
      uturner(); 
      recent="start"; 
    } 
    else if(destination=="R1"){ 
      juncturner("left"); 
      juncturner("left"); 
      uturner(); 
      recent="R1"; 
    } 
    else if(destination=="L2"){ 
      juncturner("straight"); 

 
28 
      uturner(); 
      recent="L2"; 
    } 
    else if(destination=="L1"){ 
      juncturner("left"); 
      juncturner("right"); 
      uturner(); 
      recent="L1"; 
    } 
    else if(destination=="end"){ 
      juncturner("right"); 
      uturner(); 
      recent="end"; 
    } 
  } 
  else if (current=="end"){ 
    if(destination=="R1"){ 
      juncturner("straight"); 
      juncturner("left"); 
      uturner(); 
      recent="R1"; 
    } 
    else if(destination=="L1"){ 
      juncturner("straight"); 
      juncturner("right"); 
      uturner(); 
      recent="L1"; 
    } 
    else if(destination=="R2"){ 
      juncturner("left"); 
      uturner(); 
      recent="R2"; 
    } 
    else if(destination=="L2"){ 
      juncturner("right"); 
      uturner(); 
      recent="L2"; 
    } 
    else if(destination=="start"){ 
      juncturner("straight"); 
      juncturner("straight"); 
      uturner(); 
      recent="start"; 
    } 
  } 
}