//*******************************************************************************
// panel_hx8347
//
// (C) Chris Ladden <chris.ladden@liquidware.com>
//*******************************************************************************

#include	"HardwareDef.h"

#include	<avr/io.h>
#include	<inttypes.h>

#include        "display.h"
#include	"panel_oled28.h"
#include	"bitops.h"
#include	"binary.h"
#include	"graphics.h"

#define LCD_DNC_SCL_PORT PORTD
#define LCD_DNC_SCL_DDR  DDRD
#define LCD_DNC_SCL_PIN  PD0

static inline void
hx8347_command(unsigned char cmd)
{

  //CLRBIT(OLED_CTRL_PORT,OLED_DC);
  CLRBIT(LCD_DNC_SCL_PORT, LCD_DNC_SCL_PIN);
  CLRBIT(OLED_CTRL_PORT, OLED_CS);
  SETBIT(OLED_CTRL_PORT, OLED_RD);
  CLRBIT(OLED_CTRL_PORT, OLED_WR);

  OLED_DATA_LOW = (cmd << 2);
  OLED_DATA_MED = (cmd >> 6) << 2;
//  OLED_DATA_HIGH = 0;

  SETBIT(OLED_CTRL_PORT, OLED_WR);
//  SETBIT(OLED_CTRL_PORT, OLED_CS);
  //SETBIT(OLED_CTRL_PORT,OLED_DC);
}

static inline void
hx8347_data(unsigned char data)
{
//  volatile unsigned char lb = (unsigned char) data;
//  volatile unsigned char hb = (unsigned char) (data >> 8);

  //SETBIT(OLED_CTRL_PORT,OLED_DC);
  SETBIT(LCD_DNC_SCL_PORT, LCD_DNC_SCL_PIN);
  CLRBIT(OLED_CTRL_PORT, OLED_CS);
  // SETBIT(OLED_CTRL_PORT,OLED_RD);
  CLRBIT(OLED_CTRL_PORT, OLED_WR);

  OLED_DATA_LOW = (data << 2);
  OLED_DATA_MED = (data >> 6) << 2;
//  OLED_DATA_HIGH = 0;

  SETBIT(OLED_CTRL_PORT, OLED_WR);
 // SETBIT(OLED_CTRL_PORT, OLED_CS);
  // SETBIT(OLED_CTRL_PORT,OLED_DC);
}

static inline void
hx8347_com_data(unsigned char com, unsigned char data)
{
  hx8347_command (com);
  hx8347_data (data);
}

//*******************************************************************************
static unsigned int
hx8347_status(void)
{
  return 0;
}

//*******************************************************************************
static void
hx8347_brightness(unsigned char brightnessLevel)
{
  OCR0B = 255 - brightnessLevel;
}

//*******************************************************************************
static inline void
hx8347_pixel(void)
{
  SETBIT(LCD_DNC_SCL_PORT, LCD_DNC_SCL_PIN);
//  CLRBIT(OLED_CTRL_PORT,OLED_CS);
  CLRBIT(OLED_CTRL_PORT,OLED_WR);

  OLED_DATA_LOW = 255-GraphicsColor.red;
  OLED_DATA_MED = 255-GraphicsColor.green;
  OLED_DATA_HIGH = 255-GraphicsColor.blue;

  SETBIT(OLED_CTRL_PORT,OLED_WR);
//  SETBIT(OLED_CTRL_PORT,OLED_CS);
}

static inline void
hx8347_set_xy(int x, int y)
{
//  unsigned int dx_start = x;
//  unsigned int dy_start = y;//319 - x;

#ifdef DEBUG
  char out[100];
  sprintf(out, "hx8347_set_xy x=%d, y=%d, dx=%d, dy=%d\n", x,y, dx_start, dy_start);
  usart_puts(out);
#endif

  hx8347_com_data (0x0002, x >> 8); // Column address start2
  hx8347_com_data (0x0003, x); // Column address start1
//  hx8347_com_data (0x0006, dy_start >> 8); // Row address start2
  hx8347_com_data (0x0007, y); // Row address start1

#if 0
  hx8347_com_data (0x0004, 319 >> 8); // Column address end2
  hx8347_com_data (0x0005, 319); // Column address end1
  hx8347_com_data (0x0008, 234 >> 8); // Row address end2
  hx8347_com_data (0x0009, 234); // Row address end1
#endif
  hx8347_command (0x0022);  //ram write
}

static inline void
hx8347_read(COLOR *pixel)
{
  /* Input Direction */
  OLED_DATA_LOW_DDR = 0x00;
  OLED_DATA_MED_DDR = 0x00;
  OLED_DATA_HIGH_DDR = 0x00;

  /* Read pixel */
  SETBIT(OLED_CTRL_PORT, OLED_WR);
  CLRBIT(OLED_CTRL_PORT, OLED_CS);
  CLRBIT(OLED_CTRL_PORT, OLED_RD);

  CLRBIT(OLED_CTRL_PORT, OLED_RD);
  CLRBIT(OLED_CTRL_PORT, OLED_RD);

  pixel->blue = OLED_DATA_LOW_PIN;
  pixel->green = OLED_DATA_MED_PIN;
  pixel->red = OLED_DATA_HIGH_PIN;

  SETBIT(OLED_CTRL_PORT, OLED_RD);
  SETBIT(OLED_CTRL_PORT, OLED_CS);

  /* Output direction */
  OLED_DATA_LOW_DDR = 0xFF;
  OLED_DATA_MED_DDR = 0xFF;
  OLED_DATA_HIGH_DDR = 0xFF;
}

static inline void
hx8347_enable(void)
{
  hx8347_command (0x05);
  hx8347_data (0x01);
}

static inline void
hx8347_disable(void)
{
  hx8347_command (0x05);
  hx8347_data (0x00);
}

static inline void
hx8347_resume(void)
{
  hx8347_command(0x10);
  hx8347_data(0x00);
}

static inline void
hx8347_suspend(void)
{
  hx8347_command(0x10);
  hx8347_data(0x01);
}

static inline void
hx8347_fill(unsigned long length)
{
  SETBIT(LCD_DNC_SCL_PORT, LCD_DNC_SCL_PIN);
//  CLRBIT(OLED_CTRL_PORT, OLED_CS);

  //*     Set the color once
  OLED_DATA_LOW = 255 - GraphicsColor.red;
  OLED_DATA_MED = 255 - GraphicsColor.green;
  OLED_DATA_HIGH = 255 - GraphicsColor.blue;

  DUFF_DEVICE_8(length,
    /* toggle the WR pin */
    asm("ldi r24,0x20               ""\n\t"
        "sts 0x0109,r24 ""\n\t"
        "sts 0x0109,r24 ""\n\t"
        "sts 0x0109,r24 ""\n\t"
        "sts 0x0109,r24 ""\n\t"
        "sts 0x0109,r24 ""\n\t"
        "sts 0x0109,r24 ""\n\t"
        "sts 0x0109,r24 ""\n\t"
        "sts 0x0109,r24 ""\n\t"
        :
        :
        : "r24" ););

//  SETBIT(OLED_CTRL_PORT, OLED_CS);
}

static inline void
hx8347_set_window(int x_start, int y_start, int x_end, int y_end)
{

#ifdef DEBUG
  char out[100];
  sprintf(out, "hx8347_set_window xs=%d, ys=%d, xe=%d, ye=%d\n",x_start, y_start, x_end, y_end);
  usart_puts(out);
#endif

  hx8347_com_data (0x0002, x_start >> 8); // Column address start2
  hx8347_com_data (0x0003, x_start); // Column address start1
  hx8347_com_data (0x0004, x_end >> 8); // Column address end2
  hx8347_com_data (0x0005, x_end); // Column address end1

//  hx8347_com_data (0x0006, y_start >> 8); // Row address start2
  hx8347_com_data (0x0007, y_start); // Row address start1
//  hx8347_com_data (0x0008, y_end >> 8); // Row address end2
  hx8347_com_data (0x0009, y_end); // Row address end1
  hx8347_command (0x0022);
}

static void
hx8347_setup_pins(void)
{
#define LCD_VLED_A_PORT PORTG
#define LCD_VLED_A_DDR  DDRG
#define LCD_VLED_A_PIN  PG5

  //      print("hw_init\r\n");

  SETBIT(V5_SHUTDOWN_DDR, V5_SHUTDOWN_PNUM);
  CLRBIT(V5_SHUTDOWN_PORT, V5_SHUTDOWN_PNUM);

  SETBIT(OLED_SUPPLY_PORT, OLED_SUPPLY_PNUM);
  SETBIT(OLED_SUPPLY_DDR, OLED_SUPPLY_PNUM);
  //turn on supply

  OLED_DATA_LOW_DDR = 0xFF; //output
  OLED_DATA_MED_DDR = 0xFF; //output
  OLED_DATA_HIGH_DDR = 0xFF; //output

  OLED_CTRL_DDR |= (1 << OLED_DC) | (1 << OLED_CS) | (1 << OLED_RD)
      | (1 << OLED_WR) | (1 << OLED_RESET);

  //LED backlight
  SETBIT(LCD_VLED_A_DDR, LCD_VLED_A_PIN);
  CLRBIT(LCD_VLED_A_PORT, LCD_VLED_A_PIN);

  TCCR0A |= (1 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (1 << WGM00);
  TCCR0B |= (0 << WGM02);
  hx8347_brightness(255);

  SETBIT(LCD_DNC_SCL_DDR, LCD_DNC_SCL_PIN);
}

static void
hx8347_init(void)
{
  hx8347_setup_pins ();

//      print("hx_init\r\n");


  /* Reset OLED */
  CLRBIT(OLED_CTRL_PORT, OLED_RESET);
  delay_ms (200);
  SETBIT(OLED_CTRL_PORT, OLED_RESET);

  //hx8347_com_data (0x0016,0x0008 | (5<<5)); // MY=0, MX=0, MV=0, ML=1, BGR=0, TEON=0

  // Display Setting
  hx8347_com_data (0x0001, 0x0006); // IDMON=0, INVON=1, NORON=1, PTLON=0
//  hx8347_com_data (0x0016, 0x00C8); // MY=1, MX=1, MV=0, ML=1, BGR=0, TEON=0   0048

  hx8347_com_data (0x0023, 0x0095); // N_DC=1001 0101
  hx8347_com_data (0x0024, 0x0095); // PI_DC=1001 0101
  hx8347_com_data (0x0025, 0x00FF); // I_DC=1111 1111

  hx8347_com_data (0x0027, 0x0002); // N_BP=0000 0010
  hx8347_com_data (0x0028, 0x0002); // N_FP=0000 0010
  hx8347_com_data (0x0029, 0x0002); // PI_BP=0000 0010
  hx8347_com_data (0x002a, 0x0002); // PI_FP=0000 0010
  hx8347_com_data (0x002C, 0x0002); // I_BP=0000 0010
  hx8347_com_data (0x002d, 0x0002); // I_FP=0000 0010

  hx8347_com_data (0x003a, 0x0001); // N_RTN=0000, N_NW=001    0001
  hx8347_com_data (0x003b, 0x0000); // P_RTN=0000, P_NW=001
  hx8347_com_data (0x003c, 0x00f0); // I_RTN=1111, I_NW=000
  hx8347_com_data (0x003d, 0x0000); // DIV=00
  delay_ms (20);
  hx8347_com_data (0x0035, 0x0038); // EQS=38h
  hx8347_com_data (0x0036, 0x0078); // EQP=78h
  hx8347_com_data (0x003E, 0x0038); // SON=38h
  hx8347_com_data (0x0040, 0x000F); // GDON=0Fh
  hx8347_com_data (0x0041, 0x00F0); // GDOFF

  // Power Supply Setting
  hx8347_com_data (0x0019, 0x0049); // CADJ=0100, CUADJ=100, OSD_EN=1 ,60Hz
  hx8347_com_data (0x0093, 0x000F); // RADJ=1111, 100%
  delay_ms(10);
  hx8347_com_data (0x0020, 0x0040); // BT=0100
  hx8347_com_data (0x001D, 0x0007); // VC1=111   0007
  hx8347_com_data (0x001E, 0x0000); // VC3=000
  hx8347_com_data (0x001F, 0x0004); // VRH=0011

  //VCOM SETTING
  hx8347_com_data (0x0044, 0x004D); // VCM=101 0000  4D
  hx8347_com_data (0x0045, 0x000E); // VDV=1 0001   0011
  delay_ms(10);
  hx8347_com_data (0x001C, 0x0004); // AP=100
  delay_ms(20);

  hx8347_com_data (0x001B, 0x0018); // GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=0, STB=0
  delay_ms(40);
  hx8347_com_data (0x001B, 0x0010); // GASENB=0, PON=1, DK=0, XDK=0, VLCD_TRI=0, STB=0
  delay_ms(40);
  hx8347_com_data (0x0043, 0x0080); //set VCOMG=1
  delay_ms(100);

  // Display ON Setting
  hx8347_com_data (0x0090, 0x007F); // SAP=0111 1111
  hx8347_com_data (0x0026, 0x0004); //GON=0, DTE=0, D=01
  delay_ms(40);
  hx8347_com_data (0x0026, 0x0024); //GON=1, DTE=0, D=01
  hx8347_com_data (0x0026, 0x002C); //GON=1, DTE=0, D=11
  delay_ms(40);
  hx8347_com_data (0x0026, 0x003C); //GON=1, DTE=1, D=11

  // INTERNAL REGISTER SETTING
  hx8347_com_data (0x0057, 0x0002); // TEST_Mode=1: into TEST mode
  hx8347_com_data (0x0095, 0x0001); // SET DISPLAY CLOCK AND PUMPING CLOCK TO SYNCHRONIZE
  hx8347_com_data (0x0057, 0x0000); // TEST_Mode=0: exit TEST mode
  //main_W_com_data(0x0021,0x0000);

  hx8347_com_data (0x0016,0x0008 | (5<<5)); // MY=0, MX=0, MV=0, ML=1, BGR=0, TEON=0

  //  Gamma
  hx8347_com_data (0x0046, 0x00A4);
  hx8347_com_data (0x0047, 0x0053);
  hx8347_com_data (0x0048, 0x0000);
  hx8347_com_data (0x0049, 0x0044);
  hx8347_com_data (0x004a, 0x0004);
  hx8347_com_data (0x004b, 0x0067);
  hx8347_com_data (0x004c, 0x0033);
  hx8347_com_data (0x004d, 0x0077);
  hx8347_com_data (0x004e, 0x0012);
  hx8347_com_data (0x004f, 0x004C);
  hx8347_com_data (0x0050, 0x0046);
  hx8347_com_data (0x0051, 0x0044);

  hx8347_set_window(0,0,319,239);

  long len = 320;
  len *=240;

  GraphicsColor.red = 0;
  GraphicsColor.green = 0;
  GraphicsColor.blue = 0;
  hx8347_fill(len);
}

struct display_device hx8347_driver = {
      .brightness_max = 5,

      .init = hx8347_init,
      .brightness = hx8347_brightness,
      .status = hx8347_status,
      .command = hx8347_command,
      .data = hx8347_data,
      .pixel = hx8347_pixel,
      .set_xy = hx8347_set_xy,
      .set_window = hx8347_set_window,
      .read = hx8347_read,
      .resume = hx8347_resume,
      .suspend = hx8347_suspend,
      .enable = hx8347_enable,
      .disable = hx8347_disable,
      .fill = hx8347_fill,
};
