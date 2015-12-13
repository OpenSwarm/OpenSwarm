
#include <motor_led/e_epuck_ports.h>
#include <stdio.h>
#include <uart/e_uart_char.h>
#include <bluetooth/e_bluetooth.h>
#include <motor_led/e_init_port.h>
#include <string.h>
#include "runbreitenberg.h"

#define uart2_send_static_text(msg) { e_send_uart2_char(msg,sizeof(msg)-1); while(e_uart2_sending()); }

/*this programm is an exemple how to used bluetooth library, if selectore is on 0,
* you can comunicate through normal RS232 conncection (need cable) and test bluetooth function
* other wise e-puck will connect to nearest robot (that must have sercom program) and make a obstacle
* avoidance sending also motor command to the other e-puck*/



int getselector() {
	return SELECTOR0 + 2*SELECTOR1 + 4*SELECTOR2 + 8*SELECTOR3;
}


int main(void)
{
	 char message[50];
	
	
	 char	command[30], response[80];
	 int	c;
	 int	i, version,j;
	 int nb_bt_device,error;//max 10
	
	
	 e_init_port();   //Configure port pins
	 e_init_uart1();   //Initialize UART to 115200Kbaud
	 e_init_uart2(); //Initialize UART to 115200Kbaud
	
	 if(RCONbits.POR)	//Reset if Power on (some problem for few robots)
	 {
		RCONbits.POR=0;
		__asm__ volatile ("reset");
	 }
	 
	 if(getselector()==0)//getselector()==0
	 {
	
		
		 sprintf(message, "\n\n\rWELCOME to the protocol on e-Puck Bluetooth calibration");
		 e_send_uart2_char(message,strlen(message));  //send string
		 sprintf(response, "\n\rPress H (return) for help");
		 e_send_uart2_char(response,strlen(response));  //send string
		 
		
		while(1)
		{	
		   i = 0;
			c=0;
		   do
		   {
		     	if (e_getchar_uart2(&command[i]))
				{	
					c=command[i];
		    		i++;
				}	
		   }
		   while (((char)c != '\n')&&((char)c != '\x0d'));
		   command[i]='\0';
			
		   switch (command[0]) 
		   { 
		   	case 'A':	sprintf(message,"\n\rSearch begin");
						e_send_uart2_char(message,strlen(message)); 
						nb_bt_device=e_bt_find_epuck();
						for(j=0;j<nb_bt_device;j++){
							sprintf(response,"\n\r%d. e_puck_%c%c%c%c BT_address:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x",j,(unsigned char)e_bt_present_epuck[j].number[0],(unsigned char)e_bt_present_epuck[j].number[1],(unsigned char)e_bt_present_epuck[j].number[2],(unsigned char)e_bt_present_epuck[j].number[3],(unsigned char)e_bt_present_epuck[j].address[0],(unsigned char)e_bt_present_epuck[j].address[1],( unsigned char)e_bt_present_epuck[j].address[2],( unsigned char)e_bt_present_epuck[j].address[3],( unsigned char)e_bt_present_epuck[j].address[4],(unsigned char)e_bt_present_epuck[j].address[5]);
							e_send_uart2_char(response,strlen(response)); 
							while(e_uart2_sending());
						}
						sprintf(response,"\n\rInquiry finished");
						break;
			case 'B':	sprintf(message,"\n\rSearch e-puck");
						e_send_uart2_char(message,strlen(message)); 
						if(e_bt_connect_epuck()==0)
							sprintf(response,"\n\rc, connected");
						else
							sprintf(response,"\n\rc, connection failed"); 
						break;
			case 'P':	e_bt_read_local_pin_number(message);
						sprintf(response,"\n\rPIN code = %s",message);
						break;
			case 'O':	sscanf(command,"O,%s\n",message);
						if(e_bt_write_local_pin_number(message))
							sprintf(response,"\n\rError writting PIN");
						else
							sprintf(response,"\n\rPIN code = %s",message);
						break;
			case 'M':	sscanf(command,"M,%s\n",message);
						if(e_bt_write_local_name(message))
							sprintf(response,"\n\rError writting Name");
						else
							sprintf(response,"\n\rFriendly name = %s",message);
						break;
			case 'S':	sscanf(command,"S,%s\n",message);
						if(e_bt_write_local_pin_number(message))
							sprintf(response,"\n\rError writting PIN");
						else
							sprintf(response,"\n\rPIN code = %s",message);
						e_send_uart2_char(response,strlen(response)); 
						sprintf(command,"e-puck_%s",message);
						if(e_bt_write_local_name(command))
							sprintf(response,"\n\rError writting Name");
						else
							sprintf(response,"\n\rFriendly name = %s",command);
						break;
			case 'N':	e_bt_read_local_name(message);
						sprintf(response,"\n\rFriendly name = %s",message);
						break;
			case 'R':	version=e_bt_reset();
						sprintf(response,"\n\rReset ok Firmware = %d",version);
						break;
			case 'H':	uart2_send_static_text("\n\r \"A\" find e-puck");
						uart2_send_static_text("\n\r \"B\" connect to nearest e-puck");	
						uart2_send_static_text("\n\r \"C,#\" Connect SPP device # (make inquiry first)");
						uart2_send_static_text("\n\r \"D,\%s\" Send data");
						uart2_send_static_text("\n\r \"E\" Release SPP conection");
						uart2_send_static_text("\n\r \"F,#\" Ask friendly name local device # (make inquiry first)");
						uart2_send_static_text("\n\r \"I\" Inquiry local device");
						uart2_send_static_text("\n\r \"K\" List local paired device");
						uart2_send_static_text("\n\r \"L,#\" Remove pairing device device # (make list local paired device first)");
						uart2_send_static_text("\n\r \"M,Name\" Write Name for Friendly Bluetooth name");
						uart2_send_static_text("\n\r \"N\" Read actual Friendly Bluetooth name");
						uart2_send_static_text("\n\r \"O,#\" Write # PIN number");
						uart2_send_static_text("\n\r \"P\" Read actual PIN number"); 
						uart2_send_static_text("\n\r \"R\" Soft reset Bluetooth module");	
						uart2_send_static_text("\n\r \"S,#\" Write # PIN number and same time e-puck_#");	
						uart2_send_static_text("\n\r \"T\" Enter transparent mode");
						uart2_send_static_text("\n\r \"U\" Exit transparent mode");
						
						response[0]='\n';
						response[1]='\0';
						break;
			case 'I':	sprintf(message,"\n\rInquiry begin");
						e_send_uart2_char(message,strlen(message)); 
						nb_bt_device=e_bt_inquiry(e_bt_present_device);
						for(j=0;j<nb_bt_device;j++){
							sprintf(response,"\n\rDevice %d BT_address:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x Type:%2.2x:%2.2x:%2.2x",j,(unsigned char)e_bt_present_device[j].address[0],(unsigned char)e_bt_present_device[j].address[1],( unsigned char)e_bt_present_device[j].address[2],( unsigned char)e_bt_present_device[j].address[3],( unsigned char)e_bt_present_device[j].address[4],(unsigned char)e_bt_present_device[j].address[5],(unsigned char)e_bt_present_device[j].class[0],(unsigned char)e_bt_present_device[j].class[1],(unsigned char)e_bt_present_device[j].class[2]);
							e_send_uart2_char(response,strlen(response)); 
							while(e_uart2_sending());
						}
						sprintf(response,"\n\rInquiry finished");
						break;
			case 'F':	sscanf(command,"F,%d\r",&j);
						e_bt_get_friendly_name(&e_bt_present_device[j]);
						sprintf(response,"\n\rBT_address:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x Friendly Name:%s",(unsigned char)e_bt_present_device[j].address[0],(unsigned char)e_bt_present_device[j].address[1],( unsigned char)e_bt_present_device[j].address[2],( unsigned char)e_bt_present_device[j].address[3],( unsigned char)e_bt_present_device[j].address[4],(unsigned char)e_bt_present_device[j].address[5],&e_bt_present_device[j].friendly_name[0]);
						break;
			case 'K':	nb_bt_device=e_bt_list_local_paired_device();
						for(j=0;j<nb_bt_device;j++){
							sprintf(response,"\n\rDevice %d BT_address:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x",j,(unsigned char)e_bt_local_paired_device[0+j*6],(unsigned char)e_bt_local_paired_device[1+j*6],( unsigned char)e_bt_local_paired_device[2+j*6],( unsigned char)e_bt_local_paired_device[3+j*6],( unsigned char)e_bt_local_paired_device[4+j*6],(unsigned char)e_bt_local_paired_device[5+j*6]);
							e_send_uart2_char(response,strlen(response)); 
							while(e_uart2_sending());
							}
						response[0]='\n';
						response[1]='\0';	
						break;
			case 'L':	sscanf(command,"L,%d\r",&j);
						e_bt_remove_local_paired_device(j);
						sprintf(response,"\n\rl,erase paring");
						break;
			case 'D':	sscanf(command,"D,%s\r",&message);
						e_bt_send_SPP_data(message,strlen(message));
						sprintf(response,"\n\rd,data send");
						break;
			case 'C':	sscanf(command,"C,%d\r",&j);
						if(e_bt_etablish_SPP_link(&e_bt_present_device[j].address[0])==0)
							sprintf(response,"\n\rc, connected");
						else
							sprintf(response,"\n\rc, connection failed"); 
						break;
			case 'E':	if(e_bt_release_SPP_link()==0x1f)
							sprintf(response,"\n\ry, no connection");
						else	
							sprintf(response,"\n\ry, released");
						break;
			case 'T':	e_bt_tranparent_mode();
						sprintf(response,"\n\rt, tranparent mode");
						break;
			case 'U':	e_bt_exit_tranparent_mode();
						sprintf(response,"\n\ru, exit tranparent mode");
						break;
			case 'X':	e_bt_factory_reset();
						sprintf(response,"\n\rx, factory reset");
						break;
		   	default:    sprintf(response,"\n\rz,Command not found");
		                 	break;
		   }
		   e_send_uart2_char(response,strlen(response));
		   while(e_uart2_sending());
		 }
	}
	else
	{
		LED0=1;
		error=e_bt_connect_epuck();
		if(error==0){
			e_bt_tranparent_mode();
			run_breitenberg();	
		}
		LED3=1;
	}
	LED0=0;
	
		while(1)
		{
			LED4!=LED4;
			for(j=0;j<30000;j++);
		}
	
	return 1;
}

