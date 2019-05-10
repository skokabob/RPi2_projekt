/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "UltraSonic.h"

void setup() {
    wiringPiSetup();

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    digitalWrite(TRIG, LOW);
    delay(30);
}

int getCM() {

    while (1) {
        long duration, distance;
        digitalWrite(TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        while (digitalRead(ECHO) == LOW);
        long startTime = micros();
        while (digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;

        distance = (travelTime / 2) / 29.1;

        delay(200);
        return distance;
    }
}