void reset_(){ // tourne jusqu'a la fin de course ATTENTION AU SENS DE ROTATION
  //A ECRIRE SI BESOIN le jour ou il  y aura des fin de courses
} // end reset

void set_zero(){ // set curent position a 0
    for (int i = 0; i < 2; i++) {stepper[i].setCurrentPosition(0);}
} // end set_zero

void set_resolution(int res){ // set resolution [0-4] FULLRANGE1/1 to 1/16
  if((res>=0)&&(res<=4)){
    MS_state = res;
    digitalWrite(MS1, MS[MS_state][0]); // LOW-HIGH
    digitalWrite(MS2, MS[MS_state][1]);
    digitalWrite(MS3, MS[MS_state][2]);
    Serial.print("resolution = 1/"); Serial.println(pow(2,MS_state));
  }
  else{
    Serial.print("resolutionFalseValue");
  }
}

void set_MaxSpeed(int MaxSpeed, int motor){
  if(((MaxSpeed>=0)&&(MaxSpeed<=30000))&&((motor>=0)&&(motor<=1))){
  stepper[motor].setMaxSpeed(MaxSpeed); 
  Serial.print("Stepper ");Serial.print(motor); Serial.print(" MaxSpeed = "); Serial.println(MaxSpeed);
  }
  else{
    Serial.print("MaxSpeedFalseValue");
  }
}

void set_Acceleration(int Acceleration, int motor){
  if(((Acceleration>0)&&(Acceleration<=30000))&&((motor>=0)&&(motor<=1))){
  stepper[motor].setAcceleration(Acceleration); 
  Serial.print("Stepper ");Serial.print(motor); Serial.print(" Acceleration = "); Serial.println(Acceleration);
  }
  else{
    Serial.print("AccelerationFalseValue");
  }
}

void set_moveTo(int MoveTo, int motor){
  if(((MoveTo>-2147483648)&&(MoveTo<=2147483647))&&((motor>=0)&&(motor<=1))){
  stepper[motor].moveTo(MoveTo); 
  Serial.print("Stepper ");Serial.print(motor); Serial.print(" MoveTo = "); Serial.println(MoveTo);
  }
  else{
    Serial.print("AccelerationFalseValue");
  }
}

void osc_speed(OSCMessage &msg) {
  if (msg.isInt(0)){
    int Speed = msg.getInt(0);
    if ((msg.size() == 2)&&(msg.isInt(1))){  // si 2 arguments 1 moteur
      int motor = msg.getInt(1);
      set_MaxSpeed(Speed,motor);
    }
    else{                                   // sinon les deux moteurs
      for (int i = 0; i <= 2; i++) {
        set_MaxSpeed(Speed,i);
      }
    }
  }
}

void osc_acc(OSCMessage &msg) {
  if (msg.isInt(0)){
    int Acc = msg.getInt(0);
    if ((msg.size() == 2)&&(msg.isInt(1))){  // si 2 arguments 1 moteur
      int motor = msg.getInt(1);
      set_Acceleration(Acc,motor);
    }
    else{                                   // sinon les deux moteurs
      for (int i = 0; i <= 2; i++) {
        set_Acceleration(Acc,i);
      }
    }
  }
}
void osc_speedacc(OSCMessage &msg) {
  if ((msg.isInt(0)) && msg.isInt(1)){
    int Speed = msg.getInt(0);
    int Acc = msg.getInt(1);
    if ((msg.size() == 3)&&(msg.isInt(2))){  // si 3 arguments 1 moteur
      int motor = msg.getInt(2);
      set_MaxSpeed(Speed,motor);
      set_Acceleration(Acc,motor);
    }
    else{                                   // sinon les deux moteurs
      for (int i = 0; i <= 2; i++) {
        set_MaxSpeed(Speed,i);
        set_Acceleration(Acc,i);
      }
    }
  }
}

void osc_setzero(OSCMessage &msg) {
  set_zero();
}

void osc_resolution(OSCMessage &msg) {
  if (msg.isInt(0)){
    int Res = msg.getInt(0);
    set_resolution(Res);
  }
}

void osc_moveto(OSCMessage &msg) {
  if (msg.isInt(0)){
    int Move = msg.getInt(0);
    if ((msg.size() == 2)&&(msg.isInt(1))){  // si 2 arguments 1 moteur
      int motor = msg.getInt(1);
      set_moveTo(Move,motor);
    }
    else{                                   // sinon les deux moteurs
      for (int i = 0; i <= 2; i++) {
        set_moveTo(Move,i);
      }
    }
  }
}
