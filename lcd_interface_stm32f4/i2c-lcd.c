
/** Put this in the src folder **/

#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x3F<<1 // change this according to ur setup

void lcd_send_cmd (char cmd) {
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xF0);
	data_l = ((cmd<<4)&0xF0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
	HAL_Delay(5);
}

void lcd_send_data (char data) {
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xF0);
	data_l = ((data<<4)&0xF0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_init (void) {
	lcd_send_cmd (0x02);
	lcd_send_cmd (0x28);
	lcd_send_cmd (0x0C);
	lcd_send_cmd (0x80);
	HAL_Delay(1);
}

void lcd_send_string (char *str, int n_line) {
	if(n_line != 2 && n_line != 1) {
			n_line=1;
	}
	if(strlen(str)>16){
		str[15]='\0';
	}
	if( n_line == 2 ) {
		lcd_send_cmd(0xC0);
	}else{
		lcd_send_cmd(0x80);
	}
	while (*str) lcd_send_data (*str++);
}

void lcd_clear(void){
	lcd_send_cmd(LCD_CLEARDISPLAY);
	lcd_send_cmd(LCD_RETURNHOME);
	HAL_Delay(1);
}
