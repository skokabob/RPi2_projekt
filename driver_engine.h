/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   km2_driver.h
 * Author: Bobo
 *
 * Created on 8. ledna 2017, 18:30
 */

#ifndef KM2_DRIVER_H
#define KM2_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "km2.h"

#define DEBUG_MSG_PID(a) a
    
    void run_direction(float smer_jizdy);
    void i2cDriverMotor(int16_t rychlostL, int16_t rychlostP);


#ifdef __cplusplus
}
#endif

#endif /* KM2_DRIVER_H */

