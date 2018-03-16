#include "gpio.h"
#include "ws0010.h"

#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
    // Offset for each row in DDRAM
unsigned char row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

uint8_t realBuffer[BUF_SIZE+10] = {
	0x3C,0xE0,0x8C,0x3B,0xC0,0x30,0x18,0x0E,0x00,0x00,0x00,0xE0,0x18,0x18,0xE0,0x00,
	0x00,0xE0,0x40,0x20,0x00,0xC0,0x20,0x20,0x20,0xFC,0x00,0xE0,0x00,0x00,0x00,0xE0,
	0x00,0xE8,0x00,0xE0,0x20,0x20,0x20,0xC0,0x00,0xC0,0x20,0x20,0x20,0xC0,0x00,0x00,
	0x00,0x00,0xE0,0x00,0x80,0x60,0x80,0x00,0xE0,0x00,0x00,0x00,0x00,0xFC,0x00,0xC0,
	0xA0,0xA0,0xA0,0xC0,0x00,0xC0,0xA0,0x20,0x20,0x00,0xE8,0x00,0xC0,0xA0,0xA0,0xA0,
	0xC0,0x00,0x00,0x00,0x03,0x0C,0x38,0xC0,0x00,0xFF,0xF1,0x98,0x86,0x42,0x60,0x30,
	0x08,0x08,0x08,0x00,0x00,0x00,0x03,0x7E,0xDB,0xDE,0x04,0x00,0x00,0x00,0x07,0x01,
	0x01,0x01,0x01,0x07,0x00,0x07,0x00,0x00,0x00,0x03,0x04,0x04,0x04,0x07,0x00,0x03,
	0x04,0x04,0x04,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x03,0x04,0x04,
	0x04,0x03,0x00,0x00,0x00,0x00,0x01,0x06,0x01,0x00,0x01,0x06,0x01,0x00,0x00,0x00,
	0x00,0x07,0x00,0x03,0x04,0x04,0x04,0x02,0x00,0x04,0x04,0x05,0x03,0x00,0x07,0x00,
	0x03,0x04,0x04,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x7F,0xFC,0x87,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

uint8_t desBuffer[BUF_SIZE+10] = {
	0x3C,0xE0,0x8C,0x3B,0xC0,0x30,0x18,0x0E,0x00,0x00,0x00,0xE0,0x18,0x18,0xE0,0x00,
	0x00,0xE0,0x40,0x20,0x00,0xC0,0x20,0x20,0x20,0xFC,0x00,0xE0,0x00,0x00,0x00,0xE0,
	0x00,0xE8,0x00,0xE0,0x20,0x20,0x20,0xC0,0x00,0xC0,0x20,0x20,0x20,0xC0,0x00,0x00,
	0x00,0x00,0xE0,0x00,0x80,0x60,0x80,0x00,0xE0,0x00,0x00,0x00,0x00,0xFC,0x00,0xC0,
	0xA0,0xA0,0xA0,0xC0,0x00,0xC0,0xA0,0x20,0x20,0x00,0xE8,0x00,0xC0,0xA0,0xA0,0xA0,
	0xC0,0x00,0x00,0x00,0x03,0x0C,0x38,0xC0,0x00,0xFF,0xF1,0x98,0x86,0x42,0x60,0x30,
	0x08,0x08,0x08,0x00,0x00,0x00,0x03,0x7E,0xDB,0xDE,0x04,0x00,0x00,0x00,0x07,0x01,
	0x01,0x01,0x01,0x07,0x00,0x07,0x00,0x00,0x00,0x03,0x04,0x04,0x04,0x07,0x00,0x03,
	0x04,0x04,0x04,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x03,0x04,0x04,
	0x04,0x03,0x00,0x00,0x00,0x00,0x01,0x06,0x01,0x00,0x01,0x06,0x01,0x00,0x00,0x00,
	0x00,0x07,0x00,0x03,0x04,0x04,0x04,0x02,0x00,0x04,0x04,0x05,0x03,0x00,0x07,0x00,
	0x03,0x04,0x04,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x7F,0xFC,0x87,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};



void WS0010_Start(void) {
    WS0010_Init();
}


void set_8_bit_mode()
{
      delay_ms(500);
    
    // Put in 8-bit mode
    //DL=1  N=1 F=0 FT1_FT0 10
    WS0010_WriteControl(0x02|(1<<FUNCTION_SET_BIT)|
                        (1<<FUNCTION_SET_DL_BIT)|
                        (1<<FUNCTION_SET_N_BIT)
                        );
    //Display ON/OFF Control 
    //D=1 C=1 B=0
    WS0010_WriteControl(0x0F);// 
    //Clear Display
    WS0010_WriteControl(0x01);// 
    //Entry Mode Set I/D=1 S=0
    WS0010_WriteControl((1<<2)|(1<<1));// 
    //
}

void switch_off_display()
{
  // Turn display off
    WS0010_WriteControl((1<<DISPLAY_ON_OFF_BIT));//выключили экран
}

void switch_on_display()
{
  // Turn display off
    WS0010_WriteControl((1<<DISPLAY_ON_OFF_BIT)|(1<<DISPLAY_ON_D_BIT)|(1<<1)|(1<<0));//выключили экран
        //DL=1  N=1 F=0 FT1_FT0 10
    WS0010_WriteControl(0x02|(1<<FUNCTION_SET_BIT)|
                        (1<<FUNCTION_SET_DL_BIT)|
                        (1<<FUNCTION_SET_N_BIT)
                        );
}

void set_entry_mode(int ID, int S)
{

  uint8_t cmd=0x00;
  cmd |= (1<<2);
  if(ID){
    cmd |= (1<<1);
  }
  if(S){
    cmd |= (1<<0);
  }
  WS0010_WriteControl(cmd);
}


void set_cirillic_alpabet()
{
  //Function Set
  WS0010_WriteControl((1<<5)|(1<<4)|(1<<3)|(1<<1));
  
}

void set_char_mode()
{
  //off display
  switch_off_display();
  //Cursor/Display Shift/Mode/Pwr
  WS0010_WriteControl((1<<4)|(1<<2)|(1<<1)|(1<<0));
  //on display
  switch_on_display();
}

void set_graph_mode()
{
  switch_off_display();
  //Cursor/Display Shift/Mode/Pwr
  //CURSOR/DISPLAY SHIFT INSTRUCTION
  WS0010_WriteControl((1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0));
  switch_on_display();
  clear_buf();
}


void WS0010_Init(void) {
  //http://we.easyelectronics.ru/blog/lcd_gfx/2270.html
  //http://we.easyelectronics.ru/blog/lcd_gfx/2236.html
    // Wait for power-on

    set_8_bit_mode();
    
    //switch_off_display();
    
    WS0010_WriteControl(LCD_RETURN_HOME);

    WS0010_WriteControl(LCD_CLEAR_DISPLAY);

    set_cirillic_alpabet();
    
    set_char_mode();

    //set_DDRAM_address(0x00);

    //set_entry_mode(0, 0);

    WS0010_WriteControl(0x0F);//
    
}

void pulse()
{
  HAL_GPIO_WritePin(E_PORT   ,E_PIN, GPIO_PIN_RESET);
  delay_ms(1);
  HAL_GPIO_WritePin(E_PORT   ,E_PIN, GPIO_PIN_SET);
  delay_ms(1);
  HAL_GPIO_WritePin(E_PORT   ,E_PIN, GPIO_PIN_RESET);
  delay_ms(1);
}

void clear_screen()
{
    WS0010_WriteControl(LCD_RETURN_HOME);
    WS0010_WriteControl(LCD_CLEAR_DISPLAY);
    clear_buf();
    WS0010_WriteControl(0x40);
    WS0010_WriteControl(0x80);
    WS0010_WriteControl(0x02|(1<<FUNCTION_SET_BIT)|
                        (1<<FUNCTION_SET_DL_BIT)|
                        (1<<FUNCTION_SET_N_BIT)
                        );
}



void WS0010_WriteControl(unsigned char data) {
  uint8_t stat;
  int i=0;
  HAL_GPIO_WritePin(E_PORT   ,E_PIN, GPIO_PIN_RESET);
  delay_ms(1);
  HAL_GPIO_WritePin(RS_PORT  ,RS_PIN, GPIO_PIN_RESET);
  delay_ms(1);
  HAL_GPIO_WritePin(R_W_PORT ,R_W_PIN, GPIO_PIN_RESET);
  delay_ms(1);
  WS0010_Set_Data(data);
  delay_ms(1);
  
  pulse();
  
  do{
    stat = read_status();
    i++;
  }while(stat&(1<<7));
  if(2<i){
    int a=0;
  }
    
  WS0010_Set_Data(0x00);
}


void WS0010_WriteData(unsigned char data) {
  uint8_t stat;
  int i=0;
  HAL_GPIO_WritePin(E_PORT   ,E_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RS_PORT  ,RS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(R_W_PORT ,R_W_PIN, GPIO_PIN_RESET);
  delay_ms(1);
  WS0010_Set_Data(data);
  delay_ms(1);
  
  pulse();
  
  do{
    stat = read_status();
    i++;
  }while(stat&(1<<7));
  if(2<i){
    int a=0;
  }
    
  WS0010_Set_Data(0xFF);

}    



uint8_t read_status()
{
  uint8_t data =0;
  HAL_GPIO_WritePin(R_W_PORT ,R_W_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(RS_PORT  ,RS_PIN, GPIO_PIN_RESET);
  set_data_dir_read();
  pulse();
  data = read_data();
  return data;
  
}


void set_CGRAM_address(uint8_t cgram_addr)
{
  uint8_t cmd=0;
  cmd = 0x3F & cgram_addr;
  cmd |= (1<<6);
  WS0010_WriteControl(cmd);
}

void set_DDRAM_address(uint8_t ddram_addr)
{
  uint8_t cmd=0;
  cmd = 0x7F & ddram_addr;
  cmd |= (1<<7);
  WS0010_WriteControl(cmd);  
}


void WS0010_draw_dot(int x, int y, int col)
{
  set_graph_mode();
  // User can fill in 100x16 data in embedded RAM to display graphic. 
  // Graphic mode addressing is different from character mode. 
  // Use DDRAM address instruction to set X-axis address of Graphic mode and CGRAM address
  // instruction to set Y-axis of Graphic mode.
  set_CGRAM_address(y);
  set_DDRAM_address(x); 
}


void WS0010_IsReady(void) {

    delay_us(1000);
}

void WS0010_Position(uint8_t column, uint8_t row) {
  if ( row >= 2 ) {
    row = 0;  //write to first line if out off bounds
  }
  
  WS0010_WriteControl(LCD_SET_DDRAM_ADDR | (column + row_offsets[row]));
}

void WS0010_Sleep() {
    WS0010_WriteControl(LCD_DISPLAY_CONTROL | LCD_DISPLAY_OFF);
    WS0010_WriteControl(LCD_CURSOR_SHIFT | LCD_POWER_OFF);
}

void WS0010_Wakeup() {
    WS0010_WriteControl(LCD_CURSOR_SHIFT | LCD_POWER_ON | LCD_CHARACTER_MODE);
    WS0010_WriteControl(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
}
 
void WS0010_Print2ndString(char *string) {
  unsigned char index = 0;
  set_char_mode();
  clear_screen();
  WS0010_WriteControl(0x41);
  WS0010_WriteControl(0x80);
  while (string[index] != '\0') {
    if ('\n'==string[index]) {
    }else if ('\r'==string[index]) {
    }else {
      WS0010_WriteData(string[index]);
    }
    index++;
  }
}

void WS0010_PrintString(char *string) {
  unsigned char index = 0;
  set_char_mode();
  clear_screen();
  while (string[index] != '\0') {
    if ('\n'==string[index]) {
      WS0010_WriteControl(0x41);
    }else if ('\r'==string[index]) {
      WS0010_WriteControl(0x80);
    }else {
      WS0010_WriteData(string[index]);
    }
    index++;
  }
}

void WS0010_PrintByte(unsigned char byte) {
    char string[3];
    string[0] = NibbleToAscii(byte >> 4);
    string[1] = NibbleToAscii(byte & 0x0F);
    string[2] = '\0';
    
    WS0010_PrintString(string);
}

void test_display1()
{
  int i=0;
  //set_graph_mode();
  set_cursor(0, 0);
  for(i=0; i<100; i++)
  {
    WS0010_WriteData(0x81);
  }
  
  set_cursor(0, 1);
  for(i=0; i<100; i++)
  {
    WS0010_WriteData(0x18);
  }
  
}
                        
void test_display2()
{
  int i=0,first,second;
  WS0010_WriteControl(LCD_CLEAR_DISPLAY);

  for(i=0; i<50; i+=2)  {
    first=i;
    second=i+1;
    set_cursor(second, 0);
    WS0010_WriteData(0x81);
    set_cursor(first, 0);
    WS0010_WriteData(0x81);
  }
  
  for(i=0; i<50; i+=2) {
    first=i;
    second=i+1;
    set_cursor(second, 1);
    WS0010_WriteData(0x81);
    set_cursor(first, 1);
    WS0010_WriteData(0x81);
  } 
}
                        
                        

void set_cursor(uint8_t x, uint8_t y)
{
   
 //set_DDRAM_address(x); 
 //set_CGRAM_address(y);
  
 // command(RS_SETDDRAMADDR | (col + row_offsets[row]));
 WS0010_WriteControl((1<<7) | x);
 
 delay_ms(7);
 //set y
 if ( y == 0  )  WS0010_WriteControl(0x40);
 else WS0010_WriteControl(0x41);//установка второй строки
 delay_ms(7);
}


void  my_draw_pixel(int8_t x, int8_t y, uint8_t color)
{
  if ((x < 0) || (x >= RS0010_WIDTH) || (y < 0) || (y >= RS0010_HEIGHT))
    return;
  if (color) {
    desBuffer[x+ (y/8) * RS0010_WIDTH] |= (1<<(y%8));
  }
  else {
    desBuffer[ x + (y/8) * RS0010_WIDTH] &= ~(1<<(y%8));
  }
  if(y<8){
    set_CGRAM_address(0);
  }else{
    set_CGRAM_address(1);
  }
  set_DDRAM_address(x); 
  WS0010_WriteData(desBuffer[ x + (y/8) * RS0010_WIDTH]);
  
} 
  
void draw_pixel(int8_t x, int8_t y, uint8_t color) {
	 
  if ((x < 0) || (x >= RS0010_WIDTH) || (y < 0) || (y >= RS0010_HEIGHT))
    return;
  if (color) {
    desBuffer[x+ (y/8) * RS0010_WIDTH] |= (1<<(y%8));
  }
  else {
    desBuffer[ x + (y/8) * RS0010_WIDTH] &= ~(1<<(y%8));
  }
  //if(y<8){
  //  set_CGRAM_address(0);
  //}else{
  //  set_CGRAM_address(1);
 // }
  //set_DDRAM_address(x); 
  //WS0010_WriteData(desBuffer[ x + (y/8) * RS0010_WIDTH]);
}

void test_display4()
{
  int8_t x, y;
  clear_buf();
  display_buf();
  for(x=0; x<40; x++) {
    for (y=0; y < 16; y++)  {
      //clear_buf();
      //draw_pixel(x,   y, 1);
      //display_buf();
    }
  }
  for(x=0; x<40; x++) {
    for (y=0; y < 16; y++)  {
      clear_buf();
      draw_pixel(x,   y, 1);
      display_buf_diff();
    }
  }
}

void display_buf_diff(void) 
{
  uint16_t size_of_buff;
  size_of_buff = (RS0010_WIDTH * RS0010_HEIGHT/8);
  //set_cursor(0,0);
  for (uint16_t i = 0; i < size_of_buff; i++) {
    if (desBuffer[i]!=realBuffer[i]) {
      if (size_of_buff/2<=i) {
        set_cursor(i-size_of_buff/2, 1);
      } else {
        set_cursor(i, 0);
      }
      //set_DDRAM_address(i); 
      WS0010_WriteData(desBuffer[i]);
      realBuffer[i]= desBuffer[i];
    }
  }
}

                        



void display_buf(void) {
  uint16_t size_of_buff;
  set_DDRAM_address(0); 
  //delay_ms(7);
  set_CGRAM_address(0);

  size_of_buff = (RS0010_WIDTH * RS0010_HEIGHT/8);
  set_cursor(0,0);
  for (uint16_t i = 0; i < size_of_buff; i++){
      WS0010_WriteData(desBuffer[i]);
  }
}



void clear_buf(void) 
{
  memset(desBuffer, 0x00, (BUF_SIZE));
}

void draw_line(int8_t x0, int8_t y0, int8_t x1, int8_t y1, uint8_t color) {
  //set_graph_mode();
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      draw_pixel(y0, x0, color);
    } else {
      draw_pixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}
                        
uint8_t getDesPixel(unsigned char col, unsigned char line)
{
  if(desBuffer[col+ (line/8)*RS0010_WIDTH] & _BV((line%8)))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}


void draw_frame()
{
   draw_pixel(0, 0, 1);
   draw_pixel(1, 0, 1);
   draw_pixel(2, 0, 1);
   draw_pixel(3, 0, 1);
   draw_pixel(4, 0, 1);
   draw_pixel(4, 1, 1);
   draw_pixel(4, 2, 1);
   draw_pixel(4, 3, 1);
   draw_pixel(4, 4, 1);
   draw_pixel(3, 4, 1);
   draw_pixel(2, 4, 1);
   draw_pixel(1, 4, 1);
   draw_pixel(0, 4, 1);
   draw_pixel(0, 3, 1);
   draw_pixel(0, 2, 1);
   draw_pixel(0, 1, 1);
}      
        
void draw_down()
{
  draw_frame();
  draw_pixel(2, 2, 1);
}

void draw_up()
{
  draw_frame();
  draw_pixel(2, 2, 0);
}

void draw_x_left()
{
  draw_frame();
  draw_pixel(1, 2, 1);
  draw_pixel(3, 2, 0);
}



void draw_x_right()
{
  draw_frame();
  draw_pixel(1, 2, 0);
  draw_pixel(3, 2, 1);
}

void draw_y_left()
{
  draw_frame();
  draw_pixel(2, 1, 1);
  draw_pixel(2, 3, 0);

}

void draw_y_right()
{
  draw_frame();      
  draw_pixel(2, 1, 0);
  draw_pixel(2, 3, 1);
}