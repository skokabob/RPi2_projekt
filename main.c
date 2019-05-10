/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Bobo
 *
 * Created on 7. ledna 2017, 18:41
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>
#include "driver_engine.h"
#include "opt_sensors.h"
#include "rid_jedn.h"
#include "zprac_poli.h"
#include "PID.h"

#define PI (3.141592653589793)

//Map Sensor Number to ADC Channel
unsigned sensors1[] = {2, 3};
unsigned pocet_snimacu1 = (sizeof (sensors1) / sizeof (unsigned));

// Hodnoty
static float dt = 150.0;
static float omega_str = 0.0;
static float omega_kraj = 0.0;
//float zadana_str = CalibrateSensors(sensors_stredni1, pocet_snimacu_str1);
//float zadana_kraj = CalibrateSensors(sensors_krajni1, pocet_snimacu_kraj1);
//float avg = 0.0;
// VLAKNA

/**
 * Cteni hodnot ze snimacu a pocitani smeru jizdy k care
 * @param dummy
 * @return 
 *//*PI_THREAD(cteni_vypocty) {
    float suma_vaz_prumeru = 0.0;
    unsigned pocet_mereni = 10;
    unsigned i = 0;
    float delay_cteni = 10;

    do {
        suma_vaz_prumeru += ReadSensors(sensors1, pocet_snimacu1);

        delayMicroseconds(delay_cteni);
        i++;
    } while (i <= pocet_mereni);
    piLock(0);
    avg = suma_vaz_prumeru / pocet_mereni;
    piUnlock(0);
}*/

/**
 * Obsluha motoru, zrychlovani pri hodnotach omegy z urciteho intervalu
 * @param argc
 * @param argv
 * @return 
 *//*
PI_THREAD(motory) {
    float speedL = 0.0;
    float speedP = 0.0;
    while (1) {
        run_direction(omega_str);
        if (omega_str > -0.1 && omega_str < 0.1) {
            speedL += 0.01;
            speedP += 0.01;
        }
    }
}
*/

/*
 * Hlavni vstup
 */
int main(int argc, char** argv) {
    //cidla_test(); // Sergeyova funkce
    float zadana_str = CalibrateSensors(sensors1, pocet_snimacu1);
    //float zadana_kraj = CalibrateSensors(sensors_krajni1, pocet_snimacu_kraj1);
    //float uhel = 0.0;
    /*pri prudke zatacce musime pri stejne speed_front pouzit vyssi uhlovou rychlost*/

    /*int x = piThreadCreate(cteni_vypocty);
    if (x != 0) {
        printf("Thread didnt start\n");
    }*/
    while (1) {
        // dodelat kalibrace zadanych hodnot

        //static float merena_hodnota = 0.0;
        //static float zadana_hodnota = 1.5;

        // vazeny prumer hodnot snimanych cidly cary, rika nam, jakym smerem ma
        // robot jet, aby se dostal k care
        /*piLock(1);
        merena_hodnota = avg;
        piUnlock(1);*/
        // PID regulator spocita potrebnou hodnotu uhlu omega
        // (smeru jizdy), aby se robot co nejdrive dostal zpatky na caru

        float omega_zatacky = testReg_stredniCidla(zadana_str);
        //uhel = (omega_str*180.0)/PI;
        //printf("Odklon od cary/stredove osy: %.4f°\n", uhel);
        //delayMicroseconds(100000);

        run_direction(omega_zatacky);
        delayMicroseconds(dt);

        //run_direction(3.14);
        //i2cDriverMotor(700,700);
        //delay(200);
        //i2cDriverMotor(0,0);

        //omega_kraj = testReg_krajniCidla(zadana_kraj);
        //printf("Vychylka stredu robotu od cary: %.4f\n", ReadSensors(sensors1, pocet_snimacu));
    }

    return (EXIT_SUCCESS);
}