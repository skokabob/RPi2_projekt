/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include "opt_sensors.h"
#include "ad799x.h"
#include "i2c.h"
#include "zprac_poli.h"

/**
 * Nacte aktualni hodnotu napeti z vybraneho pinu A/D prevodniku
 * @param cPIN cislo PINu
 * @return namerena hodnota napeti
 */
float cteni_napeti_cidel_AD799X(unsigned cPIN) {
    // inicializace i2c
    int bus = i2c_init(1);

    int16_t N; // sem ulozime prectena data z vybraneho snimace cary na pinu "c"

    i2c_read_beint16(bus, ADDR_AD799X_0_F, AD799X_RESULT_CH(cPIN), &N);

    // 3.3V.... BIN (BIN = 1111 1111 1100) = 0xFFC
    // nezname napeti (V) .... (N & AD799X_RESULT_VALUE) N je hodnota, ale musi byt vymaskovana
    float napeti = 3.3 * (N & AD799X_RESULT_VALUE) / 0xFFC; // vypocet napeti

    //printf("AD(%d) N: 0x%04x %.4f\t", (N & AD799X_RESULT_CHAN) >> 12, (N & AD799X_RESULT_VALUE), napeti);
    DEBUG_MSG_SENSORS(printf("AD(%d) %.4f\t", (N & AD799X_RESULT_CHAN) >> 12, napeti));

    // uzavreni i2c
    i2c_close(bus);

    return (napeti);
}

/**
 * Fci je predano pole s cisly PINu A/D obsazenych snimaci a pole, do ktereho
 * ulozime prectene hodnoty napeti
 * @param vstup pole s cisly pouzitych pinu A/D
 * @param vystup 
 * @param rozmer_vstup rozmer vstupniho pole
 */
void read_AD799X(unsigned *vstup, float** vystup, unsigned rozmer_vstup) {
    for (unsigned a = 0; a < rozmer_vstup; ++a)
        (*vystup)[a] = cteni_napeti_cidel_AD799X(vstup[a]);
}

/**
 * Urceni vazeneho prumeru z hodnot napeti snimacu cary
 * Z napeti snimacu urcime vazeny prumer, pokud je > 2,5, musime zatacet doleva,
 * abychom se blizili k care. < 2,5 zatacime doprava. Cim dal jsme od 2,5,
 * tim dal je robot od cary. Hodnotu predame PIDu jako merenou.
 * @param sensors pole s cisly PINu A/D obsazenych snimaci cary
 * @param pocet_senzoru velikost pole sensors
 * @return ziskany vazeny prumer
 */
float ReadSensors(unsigned *sensors, unsigned pocet_senzoru) {
    float *napeti_cidla = NULL;
    if (allocate_pole(&napeti_cidla, pocet_senzoru, 0) != EOk) {
        return 0;
    }
    read_AD799X(sensors, &napeti_cidla, pocet_senzoru);
    DEBUG_MSG_SENSORS(printf("\n"));

    int pocet_nul = 0;

    for (unsigned a = 0; a < pocet_senzoru; ++a)
        if (napeti_cidla[a] == 0)
            pocet_nul += 1;

    if (pocet_nul == (pocet_senzoru + 1)) {
        deallocate_pole(&napeti_cidla, pocet_senzoru);
        return 0;
    }

    // Calculate weighted mean
    float weighted_mean = 0.0;
    float cit_wMean = 0.0; // citatl vypoctu vazeneho prumeru, soucet napeti ze snimacu, kazda hodnota se nasobi poradovym cislem snimace (vahou)
    float jmen_wMean = 0.0; // jmenovatel vypoctu vazeneho prumeru, soucet napeti ze snimacu
    for (unsigned a = 0; a < pocet_senzoru; ++a) {
        // soucet napeti odectenych ze snimacu * poradove cislo snimace
        cit_wMean += napeti_cidla[a] * (float) (a + 1);
        // soucet napeti odectenych ze snimacu
        jmen_wMean += napeti_cidla[a];
    }
    deallocate_pole(&napeti_cidla, pocet_senzoru);
    weighted_mean = cit_wMean / jmen_wMean;

    return (weighted_mean);
}

/**
 * Aritmeticky prumer hodnot napeti ze snimacu
 * @param sensors pole s cisly PINu A/D obsazenych snimaci cary
 * @param pocet_snimacu velikost pole sensors
 * @return ziskana hodnota prumerneho napeti snimacu
 */
float avgSensors(unsigned *sensors, unsigned pocet_snimacu) {
    float *napeti_cidla = NULL;
    if (allocate_pole(&napeti_cidla, pocet_snimacu, 0) != EOk) {
        return 0;
    }
    read_AD799X(sensors, &napeti_cidla, pocet_snimacu);
    DEBUG_MSG_SENSORS(printf("\n"));

    float average = 0.0;
    for (unsigned a = 0; a < pocet_snimacu; ++a)
        average += napeti_cidla[a];

    deallocate_pole(&napeti_cidla, pocet_snimacu);
    average = average / pocet_snimacu;

    return (average);
}

/**
 * Urceni nejlepsi hodnoty stredu robota, prumer
 * Zjistena hodnota je zadanou hodnotou pro regulator
 * @param sensors pole s cisly PINu A/D obsazenych snimaci cary
 * @param pocet_snimacu velikost pole sensors
 * @return ziskana nejlepsi hodnota stredu robota
 */
float CalibrateSensors(unsigned *sensors, unsigned pocet_snimacu) {
    float avgCenSen = 0.0;
    float weighted_mean = 0.0;
    unsigned pocet_mereni = 10;
    for (unsigned i = 0; i < pocet_mereni; ++i) {
        float *napeti_cidla = NULL;
        if (allocate_pole(&napeti_cidla, pocet_snimacu, 0) != EOk) {
            return 0;
        }
        read_AD799X(sensors, &napeti_cidla, pocet_snimacu);
        DEBUG_MSG_SENSORS(printf("\n"));

        int pocet_nul = 0;

        for (unsigned a = 0; a < pocet_snimacu; ++a)
            if (napeti_cidla[a] == 0)
                pocet_nul += 1;

        if (pocet_nul == (pocet_snimacu + 1)) {
            deallocate_pole(&napeti_cidla, pocet_snimacu);
            return 0;
        }

        // Calculate weighted mean
        float cit_wMean = 0.0; // citatl vypoctu vazeneho prumeru, soucet napeti ze snimacu, kazda hodnota se nasobi poradovym cislem snimace (vahou)
        float jmen_wMean = 0.0; // jmenovatel vypoctu vazeneho prumeru, soucet napeti ze snimacu
        for (unsigned a = 0; a < pocet_snimacu; ++a) {
            // soucet napeti odectenych ze snimacu * poradove cislo snimace
            cit_wMean += napeti_cidla[a] * (float) (a + 1);
            // soucet napeti odectenych ze snimacu
            jmen_wMean += napeti_cidla[a];
        }
        deallocate_pole(&napeti_cidla, pocet_snimacu);
        weighted_mean += cit_wMean / jmen_wMean;

    }
    // soucet vsech vazenych prumeru podelime poctem mereni a tuto hodnotu povazujeme za nejlepsi stred robota
    avgCenSen = weighted_mean / pocet_mereni;

    return (avgCenSen);
}