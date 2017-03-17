/* Pseudo-poissonian trigger for FACT
 *
 * If you want have a smaller mean rate than 200 Events/s, give tau in milliseconds
 * and use delay(delta_ts[i]) instead of delayMicroseconds(delta_ts[i]).
 * The arduino delayMicroseconds function only works correctly with delays < 16383.
 */
#include "Arduino.h"

const unsigned short trigger_pin = 2;
const unsigned short burst_length = 5;
const unsigned short number_of_bursts = 14;

bool receive_user_command() {
    static bool on = false;
    if (Serial.available() > 0)
    {
        int b = Serial.parseInt();
        Serial.print("received: ");
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
    // 5 * 14 = 70 events -> ~1 sec.
    for(unsigned short b_id = 0; b_id < number_of_bursts; b_id++)
    {
        for(unsigned short t_id = 0; t_id < burst_length; t_id++)
        {
            delayMicroseconds(300 + b_id*50);
            digitalWrite(trigger_pin, LOW);
            digitalWrite(trigger_pin, HIGH);
        }
        delay(10); // in ms  --> so rate is always < 100Hz
    }
}

void setup() {
  pinMode(trigger_pin, OUTPUT);
  digitalWrite(trigger_pin, HIGH);
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
