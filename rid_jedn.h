/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   rid_jedn.h
 * Author: Bobo
 *
 * Created on 8. ledna 2017, 23:26
 */

#ifndef RID_JEDN_H
#define RID_JEDN_H

#ifdef __cplusplus
extern "C" {
#endif

#define WHITE 2.6
#define STRED 1.7
#define BLACK 0.4
#define N 220

    enum cidlo {
        leve, prave, leve_str, prave_str, pocet_cidel
    };

    float testReg_stredniCidla(float zadana_hodnota);
    float testReg_krajniCidla(float zadana_hodnota);

    void rotate();
    float rychlostPRIMO(float napeti);
    float Lrychlostrych(float napeti);
    float Lrychlostpom(float napeti);
    float Prychlostrych(float napeti);
    float Prychlostpom(float napeti);
    void cidla_test();


#ifdef __cplusplus
}
#endif

#endif /* RID_JEDN_H */

