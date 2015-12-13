// file sharp.h

#ifndef _SHARP
#define _SHARP

/* definitions */


#define SHARP 5				//AxeX
#define SHARP_LED1 _LATG6		//sel0
#define SHARP_LED2 _LATG7		//sel1
#define SHARP_LED3 _LATG8		//sel2
#define SHARP_LED4 _LATG9		//sel3
#define SHARP_LED5 _LATB6		//AxeY
#define SHARP_VIN  _LATB7		//AxeZ

//AxeX - pas de dir pour l'analogique
#define SHARP_LED1_DIR _TRISG6		//sel0
#define SHARP_LED2_DIR _TRISG7		//sel1
#define SHARP_LED3_DIR _TRISG8		//sel2
#define SHARP_LED4_DIR _TRISG9		//sel3
#define SHARP_LED5_DIR _TRISB6		//AxeY
#define SHARP_VIN_DIR  _TRISB7		//AxeZ


/* functions */

void e_init_sharp(void);   // to be called before starting using the Sharp
int e_get_dist_sharp(void); 	// to get analog value of Sharp

// fontions pour les LED et pour Vin
void e_set_sharp_led(unsigned int sharp_led_number, unsigned int value); // set sharp_led_number (1-5) to value (0=off 1=on higher=inverse) 
void e_sharp_led_clear(void);

//not working.always on. Hardware problem?
void e_sharp_on(void);
void e_sharp_off(void);


#endif
