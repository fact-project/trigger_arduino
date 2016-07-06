#include "tinymt64.h"

tinymt64_t tinymt;
unsigned short output = 2;

const unsigned short buffer_size = 500;
unsigned short delta_ts[buffer_size];
double tau = 10;  // mean delta_t in ms  


double exponential_random_number(double tau){
  double u = tinymt64_generate_double(&tinymt);
  return - tau * log(1 - u);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(output, OUTPUT);
  digitalWrite(output, HIGH);
  tinymt64_init(&tinymt, analogRead(A0)); 
}

void loop() {
  for(int i = 0; i < buffer_size; i++){
    delta_ts[i] = round(exponential_random_number(tau));
  }
  
  for(int i = 0; i < buffer_size; i++){
    delay(delta_ts[i]);
    digitalWrite(output, LOW);
    digitalWrite(output, HIGH);  
  }
}
