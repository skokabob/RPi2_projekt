/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zprac_poli.h
 * Author: Bobo
 *
 * Created on 8. ledna 2017, 14:34
 */

#ifndef ZPRAC_POLI_H
#define ZPRAC_POLI_H

#ifdef __cplusplus
extern "C" {
#endif

    //Pro vraceni chyby vracejte kody pomoci typu enum:

    enum TPoleError {
        EOk = 0, EWrongSize, ENoMemory, ENotAllocated, EAlreadyAllocated
    };

    static float* allocate(unsigned aN);
    static void deallocate(float *aPole);
    static void fill(float *aPole, unsigned aN, float aValue);
    enum TPoleError allocate_pole(float **aPole, unsigned aN, float aValue);
    enum TPoleError deallocate_pole(float** aPole, unsigned aN);


#ifdef __cplusplus
}
#endif

#endif /* ZPRAC_POLI_H */
