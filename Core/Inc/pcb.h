

//TODO [msi] : le rajouter dans l'option de pre compilation
#define PCB_V1
#ifdef 	PCB_V1

//----------------------- LED ---------------------------//
#define LED_PORT_GREEN_1 		(GPIOA)
#define LED_GPIO_GREEN_1 		(GPIO_PIN_5)
#define LED_TYPE_OUTPUT_GREEN_1 (GPIO_MODE_OUTPUT_PP)


//---------------------- USART2 -------------------------//
#define USART2_TX_PORT           (GPIOA)
#define USART2_TX_PIN            (GPIO_PIN_2)
#define USART2_RX_PORT           (GPIOA)
#define USART2_RX_PIN            (GPIO_PIN_3)
#define USART2_AF                (GPIO_AF7_USART2)

//---------------------- SPI2 -------------------------//
#define SPI2_PORT                (GPIOB)
#define SPI2_MISO                (GPIO_PIN_14)
#define SPI2_MOSI                (GPIO_PIN_15)
#define SPI2_SCK                 (GPIO_PIN_13)
#define SPI2_CS                  (GPIO_PIN_12)
#define SPI2_AF                  (GPIO_AF5_SPI2)


#endif
