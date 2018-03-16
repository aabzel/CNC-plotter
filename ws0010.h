
/**
 * @file ws0010.h
 * @author Thanasis Georgiou
 * @brief Defines functions and constants for the WS0010 component.
 *
 * This component is designed to be mostly compatible with the stock Character LCD component. There are
 * some quirks here and there but transition should be painless.
 */
    
#ifndef WS0010_WS0010_H
#define WS0010_WS0010_H

#include "gpio.h"

#define _BV(bit) (1 << (bit))
   
#define BLACK 0
#define WHITE 1   

#define LINE_SIZE 8
#define MAX_LINES 2

//embedded RAM dimentions
#define RS0010_WIDTH    100
#define RS0010_HEIGHT   16
#define BUF_SIZE        (RS0010_WIDTH * RS0010_HEIGHT / 8) // 200

   
#define DISPLAY_ON_OFF_BIT      (3)
#define DISPLAY_ON_D_BIT        (2)
//FUNCTION_SET register
#define FUNCTION_SET_BIT        (5)   
#define FUNCTION_SET_DL_BIT     (4)   
#define FUNCTION_SET_N_BIT      (3)
#define FUNCTION_SET_F_BIT      (2)
   
// Control commands
#define LCD_CLEAR_DISPLAY (1<<0)      /** Clears the display **/
#define LCD_RETURN_HOME (1<<1)        /** Returns the cursor to the home position */
#define LCD_FUNCTION_SET 0x28
#define LCD_SET_CGRAM_ADDR 0x40     /** Set a specific character RAM address, use to add custom characters */
#define LCD_SET_DDRAM_ADDR 0x80     /** Set a specific DDRAM address */

// flags for display entry mode
#define LCD_ENTRY_MODE_SET 0x04             /** Use with entry mode flags to change how the text is inserted */
#define LCD_ENTRY_RIGHT 0x00                /** Right-to-left text */
#define LCD_ENTRY_LEFT 0x02                 /** Left-to-right text*/
#define LCD_ENTRY_SHIFT_INCREMENT 0x01      /** 'Right justify' text */
#define LCD_ENTRY_SHIFT_DECREMENT 0x00      /** 'Left justify' text */

// flags for display on/off control
#define LCD_DISPLAY_CONTROL (1<<3)    /** Use to turn the display on/off and toggle cursor/blink. */
#define LCD_DISPLAY_ON (1<<2)  /** Turn the display on */
#define LCD_DISPLAY_OFF 0x00        /** Turn the display off */
#define LCD_CURSOR_ON   (1<<1)      /** Turn the cursor visible */
#define LCD_CURSOR_OFF 0x00         /** Turn the cursor invisible */
#define LCD_BLINK_ON  (1<<0)           /** Turn on cursor blinking */
#define LCD_BLINK_OFF 0x00          /** Turn off cursor blinking */

// flags for display/cursor shift
#define LCD_CURSOR_SHIFT 0x10   /** Use with LCD_DISPLAY_MOVE to scroll the display without chaning the RAM. */
#define LCD_DISPLAY_MOVE 0x08   /** Use with LCD_CURSOR_SHIFT to scroll the display without chaning the RAM. */
#define LCD_CURSOR_MOVE 0x00    /** Move cursor */
#define LCD_MOVE_RIGHT 0x04     /** Scroll the display right */
#define LCD_MOVE_LEFT 0x00      /** Scroll the display left */

// Power and display modes
#define LCD_POWER_ON 0x07       /** Use with LCD_CURSOR_SHIFT to turn on the power */
#define LCD_POWER_OFF 0x03      /** Use with LCD_CURSOR_SHIFT to turn off the power */
#define LCD_GRAPHIC_MODE 0x0B   /** Use with LCD_CURSOR_SHIFT to go in graphic mode (don't forget power) */
#define LCD_CHARACTER_MODE 0x03 /** Use with LCD_CURSOR_SHIFT to go in graphic mode (don't forget power) */

// flags for function set
#define LCD_8BIT_MODE   0x10
#define LCD_4BIT_MODE   0x00
#define LCD_JAPANESE    0x00
#define LCD_EUROPEAN_I  0x01
#define LCD_RUSSIAN     0x02
#define LCD_EUROPEAN_II 0x03

// High/Low
#define HIGH    1
#define LOW     0

// R/W Pin
#define WS0010_WriteMode() WS0010_Pin_RW_Write(LOW);
#define WS0010_ReadMode() WS0010_Pin_RW_Write(HIGH);

// RS Pin
#define WS0010_DataMode() WS0010_Pin_RS_Write(HIGH);
#define WS0010_ControlMode() WS0010_Pin_RS_Write(LOW);

uint8_t read_status();
void clear_screen();
// ======== Low level commands ========
/**
 * Send one nibble (half a byte) to the display.
 * @param nibble  The nibble to send. The last 4 bits will be ignored since this function will only sends 4 bits.
 * @see WS0010_SendByte()
 */
void WS0010_SendNibble(unsigned char nibble);
void set_cursor(uint8_t x, uint8_t y);
/**
 * Send a whole byte to the display. Since we are in 4-bit mode, it will first send the high nibble and then
 * the low one.
 * @param byte  The byte to send
 * @see WS0010_SendNibble()
 * @see WS0010_SendCommand()
 * @see WS0010_SendData()
 */
void WS0010_SendByte(unsigned char byte);
void display_buf(void);
void display_buf_diff(void) ;
void test_display();
/**
 * Send a command to the display. Commands are 1-byte long.
 * @param cmd   The command to send.
 * @see WS0010_SendData()
 */
void WS0010_WriteControl(unsigned char cmd);

/**
 * Send data to the display. Use this to write a single character at the cursor's position.
 * @param data  The character to write
 * @see WS0010_SendCommand()
 */
void WS0010_WriteData(unsigned char data);
/**
 * Alias for stock LCD compatibility,
 * @see WS0010_SendData()
 */
#define WS0010_PutChar(data) WS0010_SendData(data);

/**
 * Stalls code execution until the display is ready for the next command.
 */
void WS0010_IsReady(void);

// ======== High level commands ========
/**
 * Prepares the display for use. For now, just an alias for WS0010_Init().
 * @see WS0010_Init()
 */
void WS0010_Start(void);

void WS0010_WriteControl(unsigned char cmd) ;
/**
 * Initializes the display for use. This will:
 * - Restart the display.
 * - Set the display in 4-bit mode
 * - Turn the display off and then clear the contents.
 * - Turn it back on.
 */
void WS0010_Init(void);

/**
 * Sets the cursor to the given position.
 * @param row       The row to set. If larger than the display, it will reset to the first row.
 * @param column    The column position.
 */
void WS0010_Position(uint8_t column, uint8_t row);

/**
 * Turns off the display. In sleep mode, the display should draw around 0.8mA.
 * @see WS0010_Wakeup()
 * @warning This command ignores 'show cursow' and 'blink cursor' settings. If these must persist, you must manually put the display
 *          to sleep using your own LCD_DISPLAY_CONTROL command. 
 */
void WS0010_Sleep();

void WS0010_draw_dot(int x, int y, int col);
/**
 * Turns on the display.
 * @see WS0010_Sleep()
 */
void WS0010_Wakeup();

/**
 * Write a string at the current cursor position.
 * @param string String to write.
 */
void WS0010_PrintString(char *string);

/** 
 * Write a byte at the current cursor position in hexdecimal notation.
 * @param byte Byte to write.
 */
void WS0010_PrintByte(unsigned char byte);

void clear_buf(void) ;

void  my_draw_pixel(int8_t x, int8_t y, uint8_t color);
void draw_line(int8_t x0, int8_t y0, int8_t x1, int8_t y1, uint8_t color);
void set_CGRAM_address(unsigned char cgram_addr);
void set_DDRAM_address(unsigned char ddram_addr);


// For stock LCD compatibility
#define WS0010_ClearDisplay() WS0010_WriteControl(LCD_CLEAR_DISPLAY);
#define WS0010_DisplayOn() WS0010_WriteControl(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
#define WS0010_DisplayOff() WS0010_WriteControl(LCD_DISPLAY_CONTROL | LCD_DISPLAY_OFF);

void draw_down();
void draw_up();
void draw_x_left();
void draw_x_right();
void draw_y_left();
void draw_y_right();

#endif // WS0010_H
