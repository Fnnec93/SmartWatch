#ifndef H3LIS331DL_H
#define H3LIS331DL_H

/***********************************************************
 * INCLUDES
 **********************************************************/
#include <stdint.h>

/***********************************************************
 * DATA PUBLIQUE DE LA CLASSE
 **********************************************************/
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} H3lis331d_Data;

/***********************************************************
 * METHODES PUBLIQUES DE LA CLASSE
 **********************************************************/
// Initialisation 
void h3lis331dl_init(void);

// Lecture de l'acceleration 3 axes du capteur
int h3lis331d_read_data(H3lis331d_Data* psData);

#endif // H3LIS331DL_H
