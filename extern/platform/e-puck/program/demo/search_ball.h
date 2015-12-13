//*********************************************
//CONSTANTES
#define PIC_FOUND 1
#define PIC_NOT_FOUND -1
#define IS_SEARCHING_BALL 0
#define IS_FOLLOWING_BALL 1

//*********************************************
//structure définissant l'epuck 
#ifndef Epuck  					
typedef struct
{
  	char state;
	int dist_ball;
	int angle_ball;
	int lin_speed;
	int angle_speed;
} Epuck;
#endif

//*********************************************
//fcts accessible de l'exterieur de search_ball
void epuck_init(Epuck *epuck);

void normalize(unsigned char buffer[], int nb_val);

int search_ball(Epuck *epuck, unsigned char buffer[], int nb_val);

void goto_ball(Epuck *epuck);

void ARW();
