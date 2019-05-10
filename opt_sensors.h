/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   opt_sensors.h
 * Author: Bobo
 *
 * Created on 7. ledna 2017, 18:51
 */

#ifndef OPT_SENSORS_H
#define OPT_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_MSG_SENSORS(a) //a

    float cteni_napeti_cidel_AD799X(unsigned cPIN);
    void read_AD799X(unsigned *vstup, float** vystup, unsigned rozmer_vstup);
    float ReadSensors(unsigned *sensors, unsigned pocet_snimacu);
    float avgSensors(unsigned *sensors, unsigned pocet_snimacu);
    float CalibrateSensors(unsigned *sensors, unsigned pocet_snimacu);


#ifdef __cplusplus
}
#endif

#endif /* OPT_SENSORS_H */

