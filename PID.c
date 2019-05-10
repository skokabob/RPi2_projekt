/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include "PID.h"

//Gloabal varriables
float dt = 150.0; // vzorkovaci perioda
float kp = 0; // konstanta regulatoru - P slozka (pouzit jen P regulator)
float ki = 0; // konstanta regulatoru - I slozka
float kd = 0; // konstanta regulatoru D slozka

/**
 * Gloabal varriables for PID
 * @param vKP konstanta regulatoru - P slozka (pouzit jen P regulator)
 * @param vKI konstanta regulatoru - I slozka
 * @param vKD konstanta regulatoru - D slozka
 */
void varPID(float vKP, float vKI, float vKD) {
    kp = vKP; // konstanta regulatoru - P slozka (pouzit jen P regulator)
    ki = vKI; // konstanta regulatoru - I slozka
    kd = vKD; // konstanta regulatoru - D slozka
}

/**
 * PID regulator
 * @param desiredValue zadana hodnota - pozice vuci care = 0
 * @param measuredValue merena velicina y(t) - zmerena pozice
 * @return output vystupni omega
 */
float PIDController(float desiredValue, float measuredValue) {
    
    float error, integral, derivative, output;
    static float previous_error;
    error = desiredValue - measuredValue;
    integral = integral + error*dt;
    //printf("\nintegral=%f\n",integral);
    derivative = (error - previous_error) / dt;
    output = kp * error + ki * integral + kd*derivative;
    printf("epsilon: %4.4f ommega: %4.4f\n", error, output);
    previous_error = error;
    return output;
}
