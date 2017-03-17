/* Pseudo-poissonian trigger for FACT
 *
 * If you want have a smaller mean rate than 200 Events/s, give tau in milliseconds
 * and use delay(delta_ts[i]) instead of delayMicroseconds(delta_ts[i]).
 * The arduino delayMicroseconds function only works correctly with delays < 16383.
 */

#include "tinymt64.h"

tinymt64_t tinymt;
const unsigned short trigger_pin = 2;
const unsigned short buffer_size = 500;
unsigned short delta_ts[buffer_size];
double tau = 5000;  // mean delta_t in micro seconds

void setup() {
  pinMode(trigger_pin, OUTPUT);
  digitalWrite(trigger_pin, HIGH);
  tinymt64_init(&tinymt, analogRead(A0));
  Serial.begin(9600);
  while(!Serial){
    delay(1);
  };
  Serial.println("ready");
}

void loop() {
    bool on = receive_user_command();
    if (on) generate_trigger();
}

bool receive_user_command() {
    static bool on = false;
    if (Serial.available() > 0)
    {
        int b = Serial.parseInt();
        print("received: ");
        if (b == 0){
            on = false;
            Serial.println("stop");
        } else if (b == 1) {
            on = true;
            Serial.println("start");
        } else {
            Serial.println("unknown");
        }
    }
    return on;
}

void generate_trigger() {
    for(int i = 0; i < buffer_size; i++)
    {
        delta_ts[i] = round(exponential_random_number(tau));
    }

    for(int i = 0; i < buffer_size; i++)
    {
        delayMicroseconds(delta_ts[i]);
        digitalWrite(trigger_pin, LOW);
        digitalWrite(trigger_pin, HIGH);
    }
}

double exponential_random_number(double tau){
  double u = tinymt64_generate_double(&tinymt);
  return - tau * log(u);
}
