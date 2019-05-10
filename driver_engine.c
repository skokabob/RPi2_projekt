/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "driver_engine.h"
#include <linux/i2c-dev.h>
#include <stdio.h>
#include "i2c.h"
#include "km2.h"
#include "PID.h"

// kinematika podvozku
// rychlost, kterou chceme jet, pokud chceme jet nejakym smerem,
// rychlost kazdeho kola je dopocitana dle potreby.
float speed_front = 0.1; // 5cm/s
const float track_width = 0.15; // 150 mm
const float konst = 1264.28828; // konstanta krokoveho motoru
float prirustek_rychlosti = 1.0;

/**
 * Rozpohybuje robota zadanym smerem zvolenou rychlosti.
 * Rychlost kazdeho kola je dopocitana dle chteneho smeru jizdy.
 * @param smer_jizdy je uhel zatacky omega, ziskany regulatorem
 */
void run_direction(float smer_jizdy) {
    if (smer_jizdy < 0.2 && smer_jizdy > -0.2) {
        DEBUG_MSG_PID(printf("strednik: %.4f pro 0.2\n", smer_jizdy));
        varPID(3.0, 0, 1.0);
        if (prirustek_rychlosti < 4.0) {
            DEBUG_MSG_PID(printf("strednik: %.4f pro 8\n", smer_jizdy));

            prirustek_rychlosti += 0.02;
        }
    } else if (smer_jizdy > 1.0 || smer_jizdy < -1.0) {
        DEBUG_MSG_PID(printf("strednik: %.4f pro 1\n", smer_jizdy));

        varPID(6.0, 0, 1.0);
        speed_front = 0.08;
        smer_jizdy = smer_jizdy * 2;
    } else if (smer_jizdy > 1.2 || smer_jizdy < -1.2) {
        DEBUG_MSG_PID(printf("strednik: %.4f pro 1.2\n", smer_jizdy));
        //varPID(10.0, 0, 1.75);
        speed_front = 0.04;
        smer_jizdy = smer_jizdy * 3;
    } else {
        if (prirustek_rychlosti > 1.0) {
            varPID(4, 0, 1.0);
            prirustek_rychlosti -= 0.05 * prirustek_rychlosti;
        }
    }

    float speed_left = speed_front * prirustek_rychlosti + 0.5f * track_width * smer_jizdy;
    float speed_right = speed_front * prirustek_rychlosti - 0.5f * track_width * smer_jizdy;

    // Prevodni charakteristika akcniho clenu
    float speed_left_reg = speed_left * konst;
    float speed_right_reg = speed_right * konst;
    printf("Rychlost L: %.4f, P: %.4f\n", speed_left_reg, speed_right_reg);

    if ((speed_left_reg < -1000) || (speed_left_reg > 1000))
        speed_left_reg = speed_left_reg / 10;
    if ((speed_right_reg < -1000) || (speed_right_reg > 1000))
        speed_right_reg = speed_right_reg / 10;

    i2cDriverMotor(speed_left_reg, speed_right_reg);
}

/**
 * @param rychlostL rychlost leveho kola
 * @param rychlostP rychlost praveho kola
 */
void i2cDriverMotor(int16_t rychlostL, int16_t rychlostP) {

    // Vytvoříme si spojení s I2C driverem uvnitř Linuxu
    int fd = open("/dev/i2c-1", O_RDWR);

    // Dále odešleme data do modulu jedinou transakcí (opět záporná hodnota vede na chybu)
    km2_drive(fd, ADDR_KM2_DEFAULT, -rychlostL, -rychlostP);

    // Soubor uzavřeme
    close(fd);
}