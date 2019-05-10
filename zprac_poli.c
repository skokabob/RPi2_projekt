/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "zprac_poli.h"
#include <stdio.h>
#include <stdlib.h>
//#include "check.h"

/**
 * Alokace poli
 * @param rozmer pocet poli
 * @return Navratova hodnota vytvoreneho pole
 */
static float* allocate(unsigned rozmer) {
    float *pol;

    if ((pol = (float*) malloc(rozmer * sizeof (float))) == NULL) return (NULL); // nepodaril se naalokovat vektor ukazatel�

    return (pol); // naalokovana matice
}

/**
 * Dealokovani pole
 * @param aPole pole pro dealokovani
 */
static void deallocate(float *aPole) {
    free(aPole);
    aPole = NULL;
}

/**
 * Naplneni pole hodnot
 * @param aPole druh pole
 * @param rozmer rozmer pole
 * @param aValue libovolna hodnota do kazdych poli
 */
static void fill(float *aPole, unsigned rozmer, float aValue) {
    for (unsigned n = 0; n < rozmer; ++n)
        aPole[n] = aValue;
}

/**
 * Vytvorit nove pole
 * @param aPole druh pole
 * @param aN rozmer pole
 * @param aValue libovolna hodnota do kazdych poli
 * @return Overit zda je v OK
 */
enum TPoleError allocate_pole(float **aPole, unsigned aN, float aValue) {
    if (*aPole != NULL) return (EAlreadyAllocated); // jiz naalokovano
    if (aN == 0)return (EWrongSize); // nektery ze zadanych rozmeru je spatny (nula)
    if ((*aPole = allocate(aN)) == NULL)return (ENoMemory); // funkci Allocate se nepodarilo naalokovat pamet
    fill(*aPole, aN, aValue);
    
    return (EOk); // Vse je v poradku
}

/**
 * Vyprazdnit stare pole z pameti
 * @param aPole druh pole
 * @param aN rozmer pole
 * @return Overit zda je v OK
 */
enum TPoleError deallocate_pole(float** aPole, unsigned aN) {
    if (*aPole == NULL) return (ENotAllocated);
    if (aPole == NULL) return (ENotAllocated);
    if (aN == 0) return (EWrongSize);

    deallocate(*aPole);
    //aPole = NULL; // ?

    return (EOk);
}