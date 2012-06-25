//*******************************************************************************
//*
//*
//*
//*******************************************************************************
//*	Detailed Edit History
//*		PLEASE put comments here every time you work on this code
//*******************************************************************************
//*	Jan  2,	2009	<MLS> Changed name to oled_slealth.c
//*******************************************************************************

#include	"HardwareDef.h"

#include	<avr/io.h>
#include	<inttypes.h>
#include	<stdlib.h>
#include	<inttypes.h>
#include	<avr/interrupt.h>
#include	<avr/pgmspace.h>
#include	<string.h>
#include	<avr/eeprom.h>


#include	"panel_oled15.h"

#include	"touchscreen.h"
#include	"graphics.h"
#include	"bitops.h"
#include	"usart.h"
#include	"font.h"
#include	"dataflash.h"
#include	"image_interface.h"
#include	"bmp.h"

#include "binary.h"


/* VARIABLES */
int clipWidth	=	127;  //our clipping region width ending pixel
int clipHeight	=	127; //our clipping region height ending pixel
COLOR GraphicsColor;

//*******************************************************************************
void oled15_rect(LCD_RECT r, COLOR outline_color, COLOR fill_color)
{
    oled15_rectangle(r.left, r.top, r.right, r.bottom, outline_color, fill_color);
}

//*******************************************************************************
void oled15_clear(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2, COLOR * color)
{

    oled15_setColumn(x1, x2);  //set column start/ end
    oled15_setRow(y1,y2);  //set row start /end


    GraphicsColor.red	=	color->red;
    GraphicsColor.green	=	color->green;
    GraphicsColor.blue	=	color->blue;


    unsigned int count	=	((x2-x1)+1)*((y2-y1)+1);
    unsigned int pix;

    oled15_write_C(0x5C);      //enable write RAM

    for (pix=0;pix<count;pix++)
        dispPix();

    oled15_setColumn(0, 127);  //set column start/ end
    oled15_setRow(0,127);  //set row start /end

}


//*******************************************************************************
void oled15_dimWindow(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    oled15_write_C(0x8C);

    oled15_write_D(x1);
    oled15_write_D(y1);
    oled15_write_D(x2);
    oled15_write_D(y2);
}

//*******************************************************************************
void oled15_xy_pixel(int x, int y, COLOR pixel_color)
{
    /* Bounds clipping */
    if ((x<0) || (x>=128) ||
        (y<0) || (y>=128))
    {
        return;
    }

    oled15_setColumn(x,127);   //set column start/ end
    oled15_setRow(y,127);  //set row

    GraphicsColor.red	=	pixel_color.red;
    GraphicsColor.green	=	pixel_color.green;
    GraphicsColor.blue	=	pixel_color.blue;

    dispPix(); //draw the pixel
}



//*******************************************************************************
void oled15_rectangle(int x1, int y1, int x2, int y2, COLOR  outline_color, COLOR  fill_color)
{

    int16_t len	=	((x2-x1)*(y2-y1));

    /* Set XY location   */
    oled15_setColumn(x1, x2);
    oled15_setRow(y1, y2);

    /* Enable write of gram */
    oled15_write_C(0x5C);

    /* Clear the screen */
    SETBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);


    /* Set the color once */
    LCD_DATA_LOW	=	fill_color.blue;
    LCD_DATA_MED	=	fill_color.green;
    LCD_DATA_HIGH	=	fill_color.red;

    /* Start the clocking of the WR pin */

    DUFF_DEVICE_8(len,
                  PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
                 PING |= (1 << LCD_CS); PING |= (1 << LCD_CS););

    SETBIT(LCD_CTRL_PORT,LCD_CS);
    /* Set XY location   */
    oled15_setColumn(0, 127);
    oled15_setRow(0, 127);



//color format = RGB
/*
oled15_write_C(0x84);
oled15_write_D(x1);
oled15_write_D(y1);
oled15_write_D(x2);
oled15_write_D(y2);

//line color
oled15_write_D( (outline_color.red & B11111000) | (outline_color.green >> 5));
oled15_write_D( ((outline_color.green<<3) & B11100000) | (outline_color.blue>>3) );

//fill color
oled15_write_D( (fill_color.red & B11111000) | (fill_color.green >> 5));
oled15_write_D( ((fill_color.green<<3) & B11100000) | (fill_color.blue>>3) );

delay_ms(1);
*/


}





//*******************************************************************************
unsigned char oled15_read_status(void)
{
    unsigned char msg=0;

    PORTC	=	0x00;
    PORTD	=	0x00;

    DDRC	=	0x00;
    DDRD	=	0x00;

    CLRBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    CLRBIT(LCD_CTRL_PORT,LCD_RD);   //assert read
    SETBIT(LCD_CTRL_PORT,LCD_WR);

    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    msg	=	(PINC & B00001100) << 4;

    SETBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_DC);
    SETBIT(LCD_CTRL_PORT,LCD_RD);

    DDRC	=	0xFF;
    DDRD	=	0xFF;

    return msg;
}

//*******************************************************************************
void oled15_write_C(unsigned char command)
{
    CLRBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_RD);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);

    LCD_DATA_MED	=	command;
    LCD_DATA_MED	>>=	4;     //shift right by four
    LCD_DATA_LOW	=	command;
    LCD_DATA_LOW	<<=	2;     //shift left by two

    SETBIT(LCD_CTRL_PORT,LCD_WR);
    SETBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_DC);
}

//*******************************************************************************
void oled15_setContrast(unsigned char red, unsigned char green, unsigned char blue)
{
    oled15_write_C(0xC1); //Contrast levels for R, G, B
    oled15_write_D(0x55);    //Red
    oled15_write_D(0x52);    //Green
    oled15_write_D(0xA1);    //Blue
}


//*******************************************************************************
void oled15_write_D(unsigned char data)
{

    SETBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_RD);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);


    LCD_DATA_MED	=	data;
    LCD_DATA_MED	>>=	4;     //shift right by four
    LCD_DATA_LOW	=	data;
    LCD_DATA_LOW	<<=	2;     //shift left by two


    SETBIT(LCD_CTRL_PORT,LCD_WR);
    SETBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_DC);
}



//*******************************************************************************
static void oled15_init(void)
{
//set output values
    SETBIT(LCD_CTRL_PORT,LCD_DC);
    SETBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_WR);
    SETBIT(LCD_CTRL_PORT,LCD_RD);
    LCD_DATA_LOW	=	0x00;
    LCD_DATA_MED	=	0x00;
    LCD_DATA_HIGH	=	0x00;

//setup data direction registers
    SETBIT(LCD_CTRL_DDR,LCD_DC); //output
    SETBIT(LCD_CTRL_DDR,LCD_CS); //output
    SETBIT(LCD_CTRL_DDR,LCD_WR); //output
    SETBIT(LCD_CTRL_DDR,LCD_RD); //output
    SETBIT(LCD_CTRL_DDR,LCD_RESET); //output

    LCD_DATA_LOW_DDR	=	0xFF; //output
    LCD_DATA_MED_DDR	=	0xFF; //output
    LCD_DATA_HIGH_DDR	=	0xFF; //output

//init the LCD
    CLRBIT(LCD_CTRL_PORT,LCD_RESET);
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    SETBIT(LCD_CTRL_PORT,LCD_RESET);



    oled15_write_C(0xAE); //display off

    oled15_write_C(0xca); // Duty
    oled15_write_D(0x7f); // 1/128

    oled15_write_C(0xA1); //Set display start line
    oled15_write_D(0);    //0x00 start

    oled15_write_C(0xA2); //Display offset
    oled15_write_D(0x80);    //0x00 start


    oled15_write_C(0xA0); //Color Remap
    oled15_write_D(B10111100);    //65k color, 18-bit

    oled15_write_C(0xC7); //Master current control
    oled15_write_D(0x0A);    //0x07 Low Brightness
    //0x0A Typical
    //0x0E High Brightness


    oled15_write_C(0xC1); //Contrast levels for R, G, B
    oled15_write_D(0x55);    //Red
    oled15_write_D(0x52);    //Green
    oled15_write_D(0xA1);    //Blue


    oled15_write_C(0xB1); //Phase Adjust
    oled15_write_D(0x22);    //

    oled15_write_C(0xB3); //Frame Rate
    oled15_write_D(0xF0);    //85 Hz?
//oled15_write_D(0x10);    //85 Hz?

    oled15_write_C(0xBB); //Set Pre-carge level for R, G, B
    oled15_write_D(0x00);    //Red
    oled15_write_D(0x00);    //Green
    oled15_write_D(0x00);    //Blue


    oled15_write_C(0xAD); //Master Config
    oled15_write_D(0x8E);    //

    oled15_write_C(0xB0);  //Current Saving
    oled15_write_D(0x00);    //

    oled15_write_C(0xBE);  //VCOMH Setting
    oled15_write_D(0x1C);    //

    oled15_write_C(0xA6);  //Inverse Display mode off
    oled15_write_C(0xD1); //Normal Display
    oled15_write_D(0x02);    //

    oled15_write_C(0xAF); //Display On

    oled15_write_C(0x92);
    oled15_write_D(1);


    SETBIT(CHARGE_PUMP_DDR, CHARGE_PUMP_PIN); //turn on the charge pump
    SETBIT(CHARGE_PUMP_PORT,CHARGE_PUMP_PIN);


    COLOR c	=	{0,0,0};
    oled15_clearScreen(c);

}

//*******************************************************************************
void oled15_clearScreen(COLOR c)
{

    unsigned int i=512;

    /* Set XY location   */
    oled15_setColumn(0, 127);
    oled15_setRow(0, 127);

    /* Enable write of gram */
    oled15_write_C(0x5C);

    /* Clear the screen */
    SETBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);


    /* Set the color once */
    LCD_DATA_LOW	=	c.blue;
    LCD_DATA_MED	=	c.green;
    LCD_DATA_HIGH	=	c.red;

    /* Start the clocking of the WR pin */

    while (i--)
    {
        /* Set & Clear */
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);

        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);

        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);

        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
    }

    SETBIT(LCD_CTRL_PORT,LCD_CS);

}

//*******************************************************************************
void oled15_setColumn(int start,int end)
{

/* Bounds clipping */
    if (start < 0 )
    {
        start	=	0;
    }
    if (start >= 128 )
    {
        start	=	127;
    }
    if (end < 0)
    {
        end		=	0;
    }
    if (end >= 128)
    {
        end		=	127;
    }

//set column (x)
    oled15_write_C(0x15);
    oled15_write_D(start);
    oled15_write_D(end);
}

//*******************************************************************************
void oled15_setRow(int start,int end)
{

/* Bounds clipping */
    if (start < 0 )
    {
        start	=	0;
    }
    if (start >= 128 )
    {
        start	=	127;
    }
    if (end < 0)
    {
        end		=	0;
    }
    if (end >= 128)
    {
        end		=	127;
    }

//set row (y)
    oled15_write_C(0x75);
    oled15_write_D(start);
    oled15_write_D(end);
}



//*******************************************************************************
char pointInRect(POINT p, LCD_RECT r)
{
    if (p.x >= r.left)
        if (p.x <= r.right)
            if (p.y >= r.top)
                if (p.y <= r.bottom)
                    return 1;

    return 0;
}
#if 0
//*******************************************************************************
void dispBrightness(uint8_t brightnessLevel)
{
    oled15_setBrightness(brightnessLevel);
}

//*******************************************************************************
void dispColor(COLOR c)
{
    GraphicsColor.red	=	c.red;
    GraphicsColor.green	=	c.green;
    GraphicsColor.blue	=	c.blue;
}

//*******************************************************************************
void dispClearScreen()
{
    oled15_clearScreen(GraphicsColor);
}

//*******************************************************************************
void dispPixel(int x, int y)
{
    oled15_pixel(x, y, GraphicsColor);
}

//*******************************************************************************
void dispRectangle( int  x,  int  y,  int width,  int  height)
{
    int32_t len	=	width*height;

    /* Set the rectangle window */
    dispSetWindow(x,y,width,height);

    /* Enable write of gram */
    oled15_write_C(0x5C);

    /* Clear the screen */
    SETBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);

    /* Set the color once */
    LCD_DATA_LOW	=	GraphicsColor.blue;
    LCD_DATA_MED	=	GraphicsColor.green;
    LCD_DATA_HIGH	=	GraphicsColor.red;

    /* Start the clocking of the WR pin */
    DUFF_DEVICE_8(len,
                  PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
                  PING |= (1 << LCD_CS); PING |= (1 << LCD_CS););

    SETBIT(LCD_CTRL_PORT,LCD_CS);

    /* Set back a wide window   */
    oled15_setColumn(0, 127);
    oled15_setRow(0, 127);
}

//*******************************************************************************
void dispClip(int width, int height)
{
    clipWidth	=	width;
    clipHeight	=	height;
}

//*******************************************************************************
int dispGetClipWidth()
{
    return clipWidth;
}

//*******************************************************************************
int dispGetClipHeight()
{
    return clipHeight;
}

//*******************************************************************************
unsigned char dispSetWindow(int x, int y, int width, int height)
{
    uint16_t temp;

    /* Bounds clipping */
    if (x < 0 ){ width += x-0; x = 0;  }
    if (y < 0 ){ height += y-0; y = 0; }
    if (x > clipWidth ){ x = clipWidth; }
    if (y > clipHeight ){ y = clipHeight; }
    if (x+width  >= clipWidth+1){ width = clipWidth-x; }
    if (y+height >= clipHeight+1){ height = clipHeight-y; }

    /* Set XY location   */
    oled15_setColumn(x, x+width);
    oled15_setRow(y, y+height);
}
#endif

static void
oled15_brightness(unsigned char brightnessLevel)
{
  oled15_write_C(0xC7); //Master current control

  if (brightnessLevel < 15)
  {
      if (brightnessLevel==0)
      {
          /* Turn off display */
          CLRBIT(PORTE, PE3);
      }
      else
      {
          /* Turn on display */
          SETBIT(PORTE, PE3);
      }

      //0x0A Typical
      //0x0E High Brightness
      oled15_write_D(brightnessLevel);

  } else
  {
      oled15_write_D(15); //max brightness
  }
}

static unsigned int
oled15_status()
{
  return oled15_read_status();
}

static inline void
oled15_command(unsigned char command)
{
  oled15_write_C(command);
}

static inline void
oled15_data(unsigned int data)
{
  oled15_write_D(data);
}

//*******************************************************************************
static inline void
oled15_pixel(void)
{

  oled15_write_C (0x5C); //enable write RAM

  SETBIT(LCD_CTRL_PORT, LCD_DC);
//LCD_CTRL_PORT ^= (1 << LCD_CS) | (1 << LCD_WR);
  CLRBIT(LCD_CTRL_PORT, LCD_CS);
  CLRBIT(LCD_CTRL_PORT, LCD_WR);

  LCD_DATA_HIGH = GraphicsColor.red;
  LCD_DATA_MED = GraphicsColor.green;
  LCD_DATA_LOW = GraphicsColor.blue;

//LCD_CTRL_PORT ^= (1 << LCD_CS) | (1 << LCD_WR);

  SETBIT(LCD_CTRL_PORT, LCD_WR);
  SETBIT(LCD_CTRL_PORT, LCD_CS);
  SETBIT(LCD_CTRL_PORT, LCD_DC);

}

static inline void
oled15_set_xy(int x, int y)
{
  oled15_setColumn (x, 127); //set column start/ end
  oled15_setRow (y, 127); //set row
}

static inline void
oled15_set_window(int x_start, int y_start, int x_end, int y_end)
{
  oled15_setColumn (x_start, x_end); //set column start/ end
  oled15_setRow (y_start, y_end); //set row
}

static inline void
oled15_read(COLOR *pixel)
{
}

static inline void
oled15_fill(unsigned long length)
{
}

static inline void
oled15_enable(void)
{
}

static inline void
oled15_disable(void)
{
}

static inline void
oled15_resume(void)
{
}

static inline void
oled15_suspend(void)
{
}

struct display_device oled15_driver = {
      .brightness_max = 15,

      .init = oled15_init,
      .brightness = oled15_brightness,
      .status = oled15_status,
      .command = oled15_command,
      .data = oled15_data,
      .pixel = oled15_pixel,
      .set_xy = oled15_set_xy,
      .set_window = oled15_set_window,
      .read = oled15_read,
      .resume = oled15_resume,
      .suspend = oled15_suspend,
      .enable = oled15_enable,
      .disable = oled15_disable,
      .fill = oled15_fill,
};
