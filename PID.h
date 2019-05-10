/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PID.h
 * Author: Bobo
 *
 * Created on 7. ledna 2017, 18:43
 */

#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

    void varPID(float vKP, float vKI, float vKD);
    float PIDController(float desiredValue, float measuredValue);


#ifdef __cplusplus
}
#endif

#endif /* PID_H */
