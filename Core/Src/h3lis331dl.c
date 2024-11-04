/***********************************************************
 * INCLUDES
 **********************************************************/
#include "h3lis331dl.h"
#include "drv_spi.h"
#include <stdbool.h>

/***********************************************************
 * MACROS
 **********************************************************/
//  Registres de config du capteur
#define H3LIS331DL_CTRL_REG1          0x20  // Registre de configuration principal pour la mise sous tension et le taux de données
#define H3LIS331DL_CTRL_REG4          0x23  // Registre de configuration pour la sélection de la plage et d'autres options

// Valeurs pour configuration
#define H3LIS331DL_CONF_REG1          0x27 // 0x27 = 00100111 pour allumer l'accéléromètre à 50Hz
#define H3LIS331DL_CONF_REG4          0x00 // 0x00 pour +/-100g, 0x10 pour +/-200g, 0x30 pour +/-400g

// Registres pour lires les accélerations sur les 3 axes
#define H3LIS331DL_ACCEL_X_L          0x28  // Valeurs basses sur l'axe x
#define H3LIS331DL_ACCEL_X_H          0x29  // Valeurs hautes sur l'axe x
#define H3LIS331DL_ACCEL_Y_L          0x2A
#define H3LIS331DL_ACCEL_Y_H          0x2B
#define H3LIS331DL_ACCEL_Z_L          0x2C
#define H3LIS331DL_ACCEL_Z_H          0x2D

#define READ_REGISTER_MASK            0x80  // Masque pour indiquer une opération de lecture en SPI

#define SPI_TRANSFER_SIZE_READ_REG    1     // Taille de la transmission et de la réception en SPI (1 octet) pour la lecture du registre

#define SPI_TRANSFER_SIZE_WRITE_REG   2     // Taille de la transmission en SPI (2 octet) pour écrire dans le registe

/***********************************************************
 * TYPEDEFS ET VARIABLES GLOBALES
 **********************************************************/

typedef struct {
    bool isLoaded;
    bool isOpen;
    bool isInit;
} Spi_StatusGlobal;

static Spi_StatusGlobal sSpiStatus;

// Configuration SPI
static SPI_Config sSpiConfig =
{
    .SPIx = SPI_2, // Utilisation de l'interface SPI 2, qui est souvent utilisée pour connecter des périphériques externes comme des capteurs.

    .Mode = MODE_MASTER, // Microcontrôleur en mode maître : le microcontrôleur initie et contrôle la communication SPI, tandis que le capteur H3LIS331DL agit en tant qu'esclave.

    .Direction = DIRECTION_2LINES, // Utilisation des deux lignes MISO (Master In Slave Out) et MOSI (Master Out Slave In) : permet une communication bidirectionnelle entre le maître et l'esclave.

    .DataSize = DATASIZE_8BIT, // Transferts de données de 8 bits : le capteur H3LIS331DL utilise des registres de 8 bits pour la communication SPI, donc la taille des données doit être configurée à 8 bits.

    .CLKPolarity = POLARITY_LOW, // Polarité du signal d'horloge (CPOL = 0) : le niveau de l'horloge est bas lorsque le SPI est inactif. Cela signifie que le signal d'horloge commence à 0.

    .CLKPhase = PHASE_1EDGE, // Phase du signal d'horloge (CPHA = 1) : les données sont échantillonnées sur le deuxième front (montant) de l'horloge. Cela est souvent utilisé pour garantir que les données sont stables et prêtes à être lues.

    .NSS = NSS_SOFT, // Gestion manuelle du NSS : la sélection du périphérique esclave (NSS) est contrôlée par logiciel, ce qui offre plus de flexibilité pour gérer plusieurs périphériques SPI.

    .BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16, // Prescaler valide : détermine la vitesse de communication SPI. La valeur choisie doit être suffisamment basse pour garantir la stabilité de la communication, tout en étant suffisamment haute pour des transferts rapides. Le prescaler de 16 est un bon compromis.

    .FirstBit = FIRSTBIT_MSB, // Envoi du bit de poids fort en premier : dans de nombreuses communications SPI, le bit le plus significatif est transmis en premier. Cela correspond généralement aux spécifications des périphériques SPI, y compris les capteurs comme le H3LIS331DL.

    .TIMode = TIMODE_DISABLE, // Désactivation du mode TI : le mode TI (Texas Instruments) n'est pas utilisé ici, donc il est désactivé pour une communication SPI standard.

    .CRCCalculation = CRCCALCULATION_DISABLE, // Désactivation du calcul CRC : le calcul CRC (Cyclic Redundancy Check) est désactivé car il n'est pas nécessaire pour cette communication SPI. Le CRC est souvent utilisé pour la vérification d'erreurs, mais ici, nous n'en avons pas besoin.

    .CRCPolynomial = 7 // Valeur du polynôme CRC : bien que le calcul CRC soit désactivé, une valeur par défaut est fournie. La valeur de 7 est une valeur standard pour les calculs CRC, mais elle n'est pas utilisée ici.
};

/***********************************************************
 * DECLARATION DES METHODES LOCALES (PRIVEES) DE LA CLASSE
 **********************************************************/
static void  h3lis331dl_local_load(void);
static void  h3lis331dl_local_open(void);
static void  h3lis331dl_local_close(void);
static int h3lis331dl_local_writeRegister(uint8_t reg, uint8_t value);
static int h3lis331dl_local_readRegister(uint8_t reg, uint8_t* pValue);
static int h3lis331dl_local_readAxisData(uint8_t low_reg, uint8_t high_reg, uint16_t* acceleration);

/***********************************************************
 * DECLARATION DES METHODES PUBLIQUES DE LA CLASSE
 **********************************************************/

/************************************************************************
 * Fonction : h3lis331dl_init
 * ----------------------------------------------------------------------
 * Description :
 * Initialiser le capteur H3LIS331DL avec les réglages par défaut.
 ************************************************************************/

void h3lis331dl_init(void)
{
	if (false == sSpiStatus.isInit)
	{
		// Initialiser le Handle de SPI
		h3lis331dl_local_load();
		// Initialiser le Hdw
		h3lis331dl_local_open();
		// Configurer le registre CTRL_REG1 pour activer l'accéléromètre
		if (0 == h3lis331dl_local_writeRegister(H3LIS331DL_CTRL_REG1, H3LIS331DL_CONF_REG1))
		{
			// Configurer la plage de mesure dans CTRL_REG4

			if (0 == h3lis331dl_local_writeRegister(H3LIS331DL_CTRL_REG4, H3LIS331DL_CONF_REG4))
			{
				// Init done
				sSpiStatus.isInit = true;
			}
			else
			{
				// Configuration error
			}

		}
	}
}

/************************************************************************
 * Fonction : h3lis331d_read_data
 * ----------------------------------------------------------------------
 * Description :
 * Lire les données des trois axes (X, Y, Z) du capteur H3LIS331DL.
 *
 * Paramètres :
 * - H3lis331d_Data* psData : Pointeur vers la structure pour stocker les données lues
 ************************************************************************/

int h3lis331d_read_data(H3lis331d_Data* psData)
{
	int status = 1;

	if(true == sSpiStatus.isInit)
	{
		// Lire les données de l'axe X
		if (0 == h3lis331dl_local_readAxisData(H3LIS331DL_ACCEL_X_L, H3LIS331DL_ACCEL_X_H, &(psData->x)))
		{
			// Lire les données de l'axe Y
			if (0 == h3lis331dl_local_readAxisData(H3LIS331DL_ACCEL_Y_L, H3LIS331DL_ACCEL_Y_H, &(psData->y)))
			{
				// Lire les données de l'axe Z
				if (0 == h3lis331dl_local_readAxisData(H3LIS331DL_ACCEL_Z_L, H3LIS331DL_ACCEL_Z_H, &(psData->z)))
				{
					status = 0;
				}
			}
		}
	}

	return status;
}

/***********************************************************
 * FONCTIONS STATIQUES (LOCALES)
 **********************************************************/

/************************************************************************
 * Fonction : h3lis331dl_local_load
 * ----------------------------------------------------------------------
 * Description :
 * Charger la configuration SPI pour le capteur H3LIS331DL.
 ************************************************************************/

 static void  h3lis331dl_local_load(void)
{
	if (false == sSpiStatus.isLoaded)
	{
		drv_spi_load(&sSpiConfig);
		sSpiStatus.isLoaded = true;
	}
}

/************************************************************************
 * Fonction : h3lis331dl_local_open
 * ----------------------------------------------------------------------
 * Description :
 * Ouvrir la communication SPI avec le capteur H3LIS331DL.
 ************************************************************************/

static void h3lis331dl_local_open(void)
{
	if((true == sSpiStatus.isLoaded) && (false == sSpiStatus.isOpen))
	{
		drv_spi_open();
		sSpiStatus.isOpen = true;
	}
}

/************************************************************************
 * Fonction : h3lis331dl_local_close
 * ----------------------------------------------------------------------
 * Description :
 * Fermer la communication SPI avec le capteur H3LIS331DL.
 ************************************************************************/

static void h3lis331dl_local_close(void)
{
	if(true == sSpiStatus.isOpen)
	{
		drv_spi_close();
		sSpiStatus.isOpen = false;
	}
}

/************************************************************************
 * Fonction : h3lis331dl_local_writeRegister
 * ----------------------------------------------------------------------
 * Description :
 * Écrire une valeur spécifique dans un registre donné du capteur H3LIS331DL.
 *
 * Paramètres :
 * - uint8_t reg : Adresse du registre
 * - uint8_t value : Valeur à écrire dans le registre
 ************************************************************************/

static int h3lis331dl_local_writeRegister(uint8_t reg, uint8_t value)
{
	int status = 1;
    uint8_t data[2] = { reg, value };
    // Sélectionner le périphérique SPI (abaisser la ligne CS)
    drv_spi_select();
    // Transmettre l'adresse du registre "reg" et la valeur à écrire "value"
    if (0 == drv_spi_transmit(data, SPI_TRANSFER_SIZE_WRITE_REG))
    {
    	// Transmission succeed
    	status = 0;

    }
    // Désélectionner le périphérique SPI (relever la ligne CS)
    drv_spi_unselect();

    return status;
}

/************************************************************************
 * Fonction : h3lis331dl_local_readRegister
 * ----------------------------------------------------------------------
 * Description :
 * Lire la valeur d'un registre donné du capteur H3LIS331DL.
 *
 * Paramètres :
 * - uint8_t reg : Adresse du registre
 *
 * Retourne :
 * - uint8_t : Valeur lue du registre
 ************************************************************************/

static int h3lis331dl_local_readRegister(uint8_t reg, uint8_t* pValue)
{
    int status = 1;
    uint8_t data = reg | READ_REGISTER_MASK; // Le bit 7 doit être mis à 1 pour une lecture
    
    // Sélectionner le périphérique SPI (abaisser la ligne CS)
    drv_spi_select();
	HAL_Delay(10);


    // Transmettre l'adresse du registre avec le bit de lecture
    if (0 == drv_spi_transmit(&data, SPI_TRANSFER_SIZE_READ_REG))
    {
        // Recevoir la valeur du registre
        if (0 == drv_spi_receive(pValue, SPI_TRANSFER_SIZE_READ_REG))
        {
        	status = 0;
        }
    }


    HAL_Delay(10);
    // Désélectionner le périphérique SPI (relever la ligne CS)
    drv_spi_unselect();

    return status;
}

/************************************************************************
 * Fonction : h3lis331dl_local_readAxisData
 * ----------------------------------------------------------------------
 * Description :
 * Lire les données d'un axe en combinant les valeurs des registres bas et haut.
 *
 * Paramètres :
 * - uint8_t low_reg : Adresse du registre contenant la partie basse de la valeur
 * - uint8_t high_reg : Adresse du registre contenant la partie haute de la valeur
 *
 * Retourne :
 * - int16_t : Valeur combinée des registres bas et haut
 ************************************************************************/

static int h3lis331dl_local_readAxisData(uint8_t low_reg, uint8_t high_reg, uint16_t* acceleration)
{
	int status = 1;
	uint8_t low = 0;
	uint8_t high = 0;

	// Lire la partie basse du registre
    if (0 == h3lis331dl_local_readRegister(low_reg, &low))
    {
    	// Lire la partie haute du registre
    	if (0 == h3lis331dl_local_readRegister(high_reg, &high))
    	{
    		status = 0;
    	}
    }

    // Combiner les deux parties pour former une valeur de 16 bits
    *acceleration = (uint16_t)((high << 8) | low);

    return status;
}

/************************************************************************************************************/
