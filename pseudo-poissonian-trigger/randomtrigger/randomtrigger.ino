#include "tinymt64.h"

tinymt64_t tinymt;
unsigned short output = 2;

const unsigned short buffer_size = 500;
unsigned short delta_ts[buffer_size];
double tau = 5000;  // mean delta_t in micro seconds


double exponential_random_number(double tau){
  double u = tinymt64_generate_double(&tinymt);
  return - tau * log(u);
}

void setup() {
  pinMode(output, OUTPUT);
  digitalWrite(output, HIGH);
  tinymt64_init(&tinymt, analogRead(A0)); 
  Serial.begin(9600);
  while(!Serial){
    delay(1);
  };
  Serial.println("ready");
}

bool on = false;

void loop() {
  if (Serial.available() > 0){
    int b = Serial.parseInt();
    if (b == 0){
      on = false;
      Serial.println("Rec stop");
    } else if (b == 1) {
      on = true;
      Serial.println("Rec start");
    } else {
      Serial.println("Rec unknown");
    }
  }
  if (on){
    for(int i = 0; i < buffer_size; i++){
      delta_ts[i] = round(exponential_random_number(tau));
    }

    for(int i = 0; i < buffer_size; i++){
      if (delta_ts[i] > 15000){
        delay(delta_ts[i] / 1000);
      } else {
        delayMicroseconds(delta_ts[i]);
      }
      digitalWrite(output, LOW);
      digitalWrite(output, HIGH);
    }
  }
}
