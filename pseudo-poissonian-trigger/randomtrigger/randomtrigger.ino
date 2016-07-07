/* Pseudo-poissonian trigger for FACT
 * 
 * If you want have a smaller mean rate than 200 Events/s, give tau in milliseconds
 * and use delay(delta_ts[i]) instead of delayMicroseconds(delta_ts[i]).
 * The arduino delayMicroseconds function only works correctly with delays < 16383.
 */

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
}

void loop() {
  for(int i = 0; i < buffer_size; i++){
    delta_ts[i] = round(exponential_random_number(tau));
  }
  
  for(int i = 0; i < buffer_size; i++){
    delayMicroseconds(delta_ts[i]);
    digitalWrite(output, LOW);
    digitalWrite(output, HIGH);  
  }
}
