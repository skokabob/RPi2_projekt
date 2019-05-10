/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UltraSonic.h
 * Author: Bobo
 *
 * Created on 8. ledna 2017, 23:40
 */

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <wiringPi.h>  

#define TRIG 25
#define ECHO 24

    void setup();
    int getCM();


#ifdef __cplusplus
}
#endif

#endif /* ULTRASONIC_H */

