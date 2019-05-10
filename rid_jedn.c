/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include "PID.h"
#include "rid_jedn.h"
#include "UltraSonic.h"
#include "driver_engine.h"
#include "opt_sensors.h"

//Map Sensor Number to ADC Channel
unsigned sensors[] = {2, 3};
unsigned pocet_snimacu = (sizeof (sensors) / sizeof (unsigned));
/*
unsigned sensors_krajni[] = {0, 1};
unsigned sensors_stredni[] = {2, 3};
unsigned pocet_snimacu_str = (sizeof (sensors_stredni) / sizeof (unsigned));
unsigned pocet_snimacu_kraj = (sizeof (sensors_krajni) / sizeof (unsigned));
 */

/**
 * Bobuv zdrojovy kod
 */
float testReg_stredniCidla(float zadana_hodnota) {
    // dt shodne s dt regulatoru
    //static float dt = 150.0;
    static float merena_hodnota = 0.0;
    //static float zadana_hodnota = 1.5;

    // vazeny prumer hodnot snimanych cidly cary, rika nam, jakym smerem ma
    // robot jet, aby se dostal k care
    merena_hodnota = ReadSensors(sensors, pocet_snimacu);
    // PID regulator spocita potrebnou hodnotu uhlu omega
    // (smeru jizdy), aby se robot co nejdrive dostal zpatky na caru
    float omega_zatacky = PIDController(zadana_hodnota, merena_hodnota);

    //printf("Zpracovana hodnota omegy: %f\t", omega_zatacky);
    // spocitany uhel zatacky omega predame fci pro rozjeti robota
    //run_direction(omega_zatacky);
    return (omega_zatacky);
    //delayMicroseconds(dt);
}

float testReg_krajniCidla(float zadana_hodnota) {
    // dt shodne s dt regulatoru
    //static float dt = 150.0;
    static float merena_hodnota = 0.0;
    //static float zadana_hodnota = 1.5;

    // vazeny prumer hodnot snimanych cidly cary, rika nam, jakym smerem ma
    // robot jet, aby se dostal k care
    merena_hodnota = ReadSensors(sensors, pocet_snimacu);
    // PID regulator spocita potrebnou hodnotu uhlu omega
    // (smeru jizdy), aby se robot co nejdrive dostal zpatky na caru
    float omega_zatacky = PIDController(zadana_hodnota, merena_hodnota);

    //printf("Zpracovana hodnota omegy: %f\t", omega_zatacky);
    // spocitany uhel zatacky omega predame fci pro rozjeti robota
    //run_direction(omega_zatacky);
    return (omega_zatacky);
    //delayMicroseconds(dt);
}
#if 0
/**
 * ********************************************************************************************************************
 * Sergeyuv zdrojovy kod
 * ********************************************************************************************************************
 */

/**
 * Funkce pro ultrasonic sensor
 */
void rotate() {
    while (1) {
        printf("Distance: %dcm\n", getCM());
        int a = getCM();
        if (a < 15) {
            i2cDriverMotor(100, -100);
            while (1) {
                printf("Distance: %dcm\n", getCM());
                a = getCM();
                if (a > 30) {
                    delay(600);
                    i2cDriverMotor(100, 100);
                    break;
                }
            }
            break;

        }

    }
}

/**
 * Funkce pro rychlost jizdy dopredu(calibrace cary na stred)
 * @param napeti
 * @return 
 */
float rychlostPRIMO(float napeti) {
    float vypocet = -60 * napeti + 332;
    return vypocet;
}

/**
 * Funkce pro rychlost rychlejsiho kola(odboceni doleva)
 * @param napeti
 * @return 
 */
float Lrychlostrych(float napeti) {
    float vypocet = -23 * napeti + 279.1;
    return vypocet;
}

/**
 * Funkce pro rychlost pomalejsiho kola(odboceni doleva)
 * @param napeti
 * @return 
 */
float Lrychlostpom(float napeti) {
    float vypocet = -60 * napeti + 102;
    return vypocet;
}

/**
 * Funkce pro rychlost rychlejsiho kola(odboceni doleva)
 * @param napeti
 * @return 
 */
float Prychlostrych(float napeti) {
    float vypocet = -23 * napeti + 459.1;
    return vypocet;
}

/**
 * Funkce pro rychlost pomalejsiho kola(odboceni doprava)
 * @param napeti
 * @return 
 */
float Prychlostpom(float napeti) {
    float vypocet = -60 * napeti + 172;
    return vypocet;
}

/**
 * Sergeyova funkce
 */
void cidla_test() {

    //pole pro ziskavani hodnot s cidel
    float napeti_cidlo[4];
    //ziskavame hodnoty s cidel
    napeti_cidlo[leve] = init_AD799X(0);
    napeti_cidlo[leve_str] = init_AD799X(2);
    napeti_cidlo[prave_str] = init_AD799X(3);
    napeti_cidlo[prave] = init_AD799X(1);
    printf("\n");

    //promenna pro podminky
    unsigned flag;
    //promenne pro zjisteni casu
    unsigned start_time, end_time, time;


    //jede rovne
    if (napeti_cidlo[leve_str] <= BLACK && napeti_cidlo[prave_str] <= BLACK)
        i2cDriverMotor(rychlostPRIMO(napeti_cidlo[leve_str]), (rychlostPRIMO(napeti_cidlo[prave_str])));
        //otaci se doleva
    else if (napeti_cidlo[leve_str] <= BLACK && napeti_cidlo[prave_str] >= BLACK)
        i2cDriverMotor(Lrychlostpom(napeti_cidlo[leve_str]), Lrychlostrych(napeti_cidlo[prave_str]));
        //otaci se doprava
    else if (napeti_cidlo[leve_str] >= BLACK && napeti_cidlo[prave_str] <= BLACK)
        i2cDriverMotor(Prychlostrych(napeti_cidlo[leve_str]), Prychlostpom(napeti_cidlo[prave_str]));

        //co delat jestli oba stredni cidla nejsou na care
    else if (napeti_cidlo[leve_str] >= BLACK || napeti_cidlo[prave_str] >= BLACK) {
        //nastavime podminku na 0
        flag = 0;
        //vyjit z toho cyklu muzeme jen pomoci podminky(5)
        while (1) {
            switch (flag) {
                    //nulova podminka(davame prednost odboceni doleva)
                case 0:
                    //spustime casovac
                    start_time = millis();
                    end_time = millis() - start_time;
                    time = N;
                    //budeme otacet doleva pokud nevyprsi nam casovac nebo nenajdeme cernou caru pomoci strednich cidel
                    while (end_time < time && (napeti_cidlo[leve_str] >= BLACK && napeti_cidlo[prave_str] >= BLACK)) {
                        end_time = millis() - start_time;
                        napeti_cidlo[leve] = init_AD799X(0);
                        napeti_cidlo[leve_str] = init_AD799X(2);
                        napeti_cidlo[prave_str] = init_AD799X(3);
                        //vypnime cyklus jestli zjistime ze leve cidlo se nachazi na care
                        if (napeti_cidlo[leve] <= STRED)
                            break;
                        i2cDriverMotor(-200, 200);
                    }
                    //jestli najdeme caru pomoci strenich cidel, podminka=5(pro skonceni cyklu)
                    if (napeti_cidlo[leve_str] <= BLACK || napeti_cidlo[prave_str] <= BLACK) {
                        delay(10);
                        flag = 5;
                        break;
                    }
                    //jestli najdeme caru pomoci leveho cidla, podminka=1
                    if (napeti_cidlo[leve] <= STRED) {
                        flag = 1;
                        break;
                        //v pripade ze nic nenajdeme, podminka=2
                    } else {
                        flag = 2;
                        break;
                    }
                    // podminka jestli jsme nasli caru pomoci leveho cidla
                case 1:
                    //uz vidime ze nekde blizko je cara, tak ze budeme se otecet pokud ji nenajdeme 
                    while (napeti_cidlo[leve_str] >= BLACK || napeti_cidlo[prave_str] >= BLACK) {
                        napeti_cidlo[leve_str] = init_AD799X(2);
                        napeti_cidlo[prave_str] = init_AD799X(3);
                        i2cDriverMotor(-200, 200);
                    }
                    delay(10);
                    flag = 5;
                    break;
                    // v pripade, ze jsme nenasli caru na leve strane, budeme ji hledat na prave
                case 2:
                    //spustime casovac
                    start_time = millis();
                    end_time = millis() - start_time;
                    time = N;
                    //budeme otacet doprava(zpatky) pokud nevyprsi casovac
                    while (end_time < time) {
                        end_time = millis() - start_time;
                        i2cDriverMotor(200, -200);
                    }

                    //spustime casovac
                    start_time = millis();
                    end_time = millis() - start_time;
                    time = N;
                    //budeme otacet doleva pokud nevyprsi nam casovac nebo nenajdeme cernou caru pomoci strednich cidel
                    while (end_time < time && (napeti_cidlo[leve_str] >= BLACK && napeti_cidlo[prave_str] >= BLACK)) {
                        end_time = millis() - start_time;
                        napeti_cidlo[leve_str] = init_AD799X(2);
                        napeti_cidlo[prave_str] = init_AD799X(3);
                        napeti_cidlo[prave] = init_AD799X(1);
                        //jestli najdeme caru pomoci praveho cidla, skoncime cyklus
                        if (napeti_cidlo[prave] <= STRED)
                            break;
                        i2cDriverMotor(200, -200);
                    }
                    //jestli najdeme caru pomoci strenich cidel, podminka=5
                    if (napeti_cidlo[leve_str] <= BLACK || napeti_cidlo[prave_str] <= BLACK) {
                        delay(10);
                        flag = 5;
                        break;
                    }
                    //jestli najdeme caru pomoci praveho cidla, podminka=3
                    if (napeti_cidlo[prave] <= STRED) {
                        flag = 3;
                        break;
                    }//v pripade ze nic nenajdeme, podminka=4
                    else {
                        flag = 4;
                        break;
                    }

                    // podminka jestli jsme nasli caru pomoci praveho cidla
                case 3:
                    start_time = millis();
                    end_time = millis() - start_time;
                    time = 600;

                    //uz vidime ze nekde blizko je cara, tak ze budeme se otecet pokud ji nenajdeme
                    while ((napeti_cidlo[leve_str] >= BLACK || napeti_cidlo[prave_str] >= BLACK) || end_time < time) {
                        end_time = millis() - start_time;
                        napeti_cidlo[leve] = init_AD799X(0);
                        napeti_cidlo[leve_str] = init_AD799X(2);
                        napeti_cidlo[prave_str] = init_AD799X(3);
                        napeti_cidlo[prave] = init_AD799X(1);
                        i2cDriverMotor(200, -200);
                        if (napeti_cidlo[leve] >= WHITE && napeti_cidlo[leve_str] <= BLACK && napeti_cidlo[prave_str] <= BLACK && napeti_cidlo[prave] >= WHITE)
                            break;
                    }

                    delay(10);
                    flag = 5;
                    break;
                    // podminka jestli cara tam vubec neni
                case 4:
                    //spustime casovac
                    start_time = millis();
                    end_time = millis() - start_time;
                    time = N;
                    //budeme otacet doleva(zpatky) pokud nevyprsi casovac
                    while (end_time < time) {
                        end_time = millis() - start_time;
                        i2cDriverMotor(-200, 200);
                    }

                    //i2cDriverMotor(0, 0);
                    //spustime novy casovac
                    start_time = millis();
                    end_time = millis() - start_time;
                    time = 700;
                    //budeme jet dopredu pokud nevyprsi nam casovac nebo nenajdeme cernou caru pomoci strednich cidel
                    while (end_time < time && (napeti_cidlo[leve_str] >= BLACK && napeti_cidlo[prave_str] >= BLACK)) {
                        napeti_cidlo[leve_str] = init_AD799X(2);
                        napeti_cidlo[prave_str] = init_AD799X(3);
                        end_time = millis() - start_time;
                        i2cDriverMotor(250, 250);
                    }
                    flag = 5;
                    break;

            }
            //tady vypneme cyklus 
            if (flag == 5)
                break;
        }
    }

}
#endif