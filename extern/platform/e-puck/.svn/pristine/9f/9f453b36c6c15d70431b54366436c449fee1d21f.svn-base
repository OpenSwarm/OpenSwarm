/*! \file
 * \ingroup camera2
 * \brief Manage camera's interrupts (three timers)
 */

/* Compile this file with maximum optimisation
 * ( -O3 )
 */


#include <p30f6014a.h>
#include "./../../motor_LED/e_epuck_ports.h"
#include "e_po3030k.h"

/*! The number of row we should take 
 * \sa e_po3030k_aApply_timer_config 
 */
static int max_row;

/*! The number of bytes per row we should take
 * \sa e_po3030k_get_bytes_per_pixel, e_po3030k_apply_timer_config
 */
static int max_col;

/*! The buffer to write to */
static char * buffer;

/*! The flag to tell, the image is ready or not
 * Zero mean capture is in progress, non-zero mean capture done.
 * \sa e_po3030k_is_img_ready
 */
static int img_ready;

/*! The current row we are */
static int current_row;
/*! The current column we are */
static int current_col;
/*! Counter which is incremented each time we acquire a byte */
static int buf_pos;

/*! Number of bytes per pixel */
static int bytes_per_pixel;

/*! The current byte we are inside a pixel */
static int bpp_current;

/*! Pixel to "jump" between each effective pixel */
static int pixel_betw_pixel;

/*! The current pixel we are between two effective pixel */
static int pbp_current;

/*! The number of blank row between each effective row */
static int blank_betw_lines;

/*! The current row we ignore between each effective row */
static int bbl_current;



/*! \brief The HSYNC interrupt.
 * This interrupt is called each time the Horizontal sync signal is asserted
 * This mean we begin the a line of the picture 
 */
void __attribute__((interrupt, auto_psv))
_T4Interrupt(void) {
	IFS1bits.T4IF = 0;

	/* Yes, this is needed, otherwise we miss half of the interrupt 
	 * Hardware bug ? */
	TMR4 = 0;
	if(!bbl_current) {	
		if(++current_row > max_row) {
			T1CONbits.TON = 0; 
			T4CONbits.TON = 0; 
			img_ready = 1;
		} else {
			/* We are in the correct column, let's enable pixel clock */
			T1CONbits.TON = 1;
			/* we must manually fire the first interrupt */
			IFS0bits.T1IF = 1;
		}
	}
	if(++bbl_current > blank_betw_lines)
		bbl_current = 0;
}


/*! \brief The VSYNC interrupt.
 * This interrupt is called every time the Vertical sync signal is asserted
 * This mean that the picture is comming from the camera ( we will have the first line soon )
 */
void __attribute__((interrupt, auto_psv))
_T5Interrupt(void) {
	IFS1bits.T5IF = 0;
	/* let's enable Hsync */
	T4CONbits.TON = 1;
	/* single shot */
	T5CONbits.TON = 0;
}


/*! \brief The Pixel Clock interrupt.
 * This interrupt is called every time the Pixel clock signal is asserted
 * This mean that the next byte is ready to be read.
 */
void __attribute__((interrupt, auto_psv))
_T1Interrupt(void) {
	int b;
	int take_it;
	static int p = 0;
	IFS0bits.T1IF = 0;
	b = CAM_DATA;

	/* Yes, this is needed, otherwise we miss half of the interrupt 
	 * Hardware bug ? */
	TMR1 = 0;
	

	if(pbp_current)
		take_it = 0;
	else 
		take_it = 1;
	
	if(++bpp_current == bytes_per_pixel) {
		bpp_current = 0;
		if(pbp_current++ >= pixel_betw_pixel) {
			pbp_current = 0;
		}
	}
		
	if(!take_it) 
		return;
	
	buffer[buf_pos++] = b >> 8;
	if(++current_col >= max_col) {
		T1CONbits.TON = 0;
		current_col = 0;
		pbp_current = 0;
	}
	if(p == 0)  {
		LED0 = 1;
		p = 1;
	} else {
		LED0 = 0;
		p = 0;
	}
}

static void init_timer5(void) {
	/* external pin, 1:1 prescaler */
	T5CON = 0x2;
	TMR5 = 0;
	PR5 = 1;
	IFS1bits.T5IF = 0;
	IEC1bits.T5IE = 1; 
	T5CONbits.TON = 1;
}

static void init_timer4(void) {
	T4CON = 0x2;
	TMR4 = 0;
	PR4 = 1;
	IFS1bits.T4IF = 0;
	T4CONbits.TON = 0;
	IEC1bits.T4IE = 1;
}

static void init_timer1(void) {
	/* external pin, 1:1 prescaler, sync */
	T1CON = 0x2;
	TMR1 = 0;
	PR1 = 1;
	IFS0bits.T1IF = 0;
	T1CONbits.TON = 0;
	T1CONbits.TSYNC = 1;
	IEC0bits.T1IE = 1;

}

/*! Launch a capture in the \a buf buffer
 * \param buf The buffer to write to
 * \sa e_po3030k_config_cam and e_po3030k_is_img_ready
 */
void e_po3030k_launch_capture(char * buf) {
	buf_pos = 0;
	current_col = 0;
	current_row = 0;
	img_ready = 0;
	bpp_current = 0;
	pbp_current = 0;
	bbl_current = 0;
	buffer = buf;
	init_timer4();
	init_timer1();
	/* Timer5 must ALWAY be initialized as the last one */
	init_timer5();
	
}

/*! Modify the interrupt configuration
 * \warning This is an internal function, use \a e_po3030k_config_cam
 * \param pixel_row The number of row to take
 * \param pixel_col The number of pixel to take each \a pixel_row
 * \param bpp The number of byte per pixel
 * \param pbp The number of pixel to ignore between each pixel
 * \param bbl The number of row to ignore between each line
 * \sa e_po3030k_get_bytes_per_pixel and e_po3030k_config_cam
 */
void e_po3030k_apply_timer_config(int pixel_row, int pixel_col, int bpp, int pbp, int bbl) {
	max_row = pixel_row;
	max_col = pixel_col * bpp;
	bytes_per_pixel = bpp;
	pixel_betw_pixel = pbp;
	blank_betw_lines = bbl;
}

/*! Check if the current capture is finished
 * \return Zero if the current capture is in progress, non-zero if the capture is done.
 * \sa e_po3030k_launch_capture
 */
int e_po3030k_is_img_ready(void) {
	return img_ready;
}
