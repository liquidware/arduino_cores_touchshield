//*******************************************************************************
//*		
//*	
//*	
//*******************************************************************************
//*	Detailed Edit History
//*		PLEASE put comments here every time you work on this code
//*******************************************************************************
//*	Dec 29,	2008	<?> dispBrightness changed to oled_brightness
//*	Dec 29,	2008	<CML> released v0012LW
//*	Jan  2,	2009	<MLS> Changed name to oled_slide.c
//*	Jan 19,	2009	<MLS> Got permission from Chris to start working on cleanin up slide code
//*******************************************************************************

#include	"HardwareDef.h"

#include	<avr/io.h>
#include	<inttypes.h>

#include        "display.h"
#include	"panel_oled28.h"
#include	"bitops.h"
#include	"binary.h"
#include	"graphics.h"

OLED_GAMMA	gGammaTable[BRIGHT_MAX] = {
/* Brightness level */
/* BRIGHT1 50  */ { {0x0800, 0x0980, 0x1300, 0x0C0D, 0x1F13, 0x201C, 0x1A17, 0x1A1C, 0x2517}},
/* BRIGHT2 150 */ { {0x2000, 0x2180, 0x2D80, 0x1512, 0x1C10, 0x2216, 0x1813, 0x2219, 0x2014}},
/* BRIGHT3 175 */ { {0x2380, 0x2480, 0x3100, 0x1612, 0x1B0F, 0x2115, 0x1813, 0x2219, 0x1F13}},
/* BRIGHT4 225 */ { {0x2900, 0x2a00, 0x3700, 0x1912, 0x1B0D, 0x2014, 0x1812, 0x2118, 0x2111}},
/* BRIGHT5 250 */ { {0x2B80, 0x2C80, 0x3980, 0x1912, 0x1A0D, 0x2014, 0x1811, 0x2218, 0x1E11}},
};

//*******************************************************************************
static inline void
oled28_command(unsigned char command)
{

#ifdef _DEBUG_DISPWINDOW_
//      if (gEnableDebug)
//      {
//      char    debugString[48];
//
//              sprintf(debugString, "command=%02X   ", command);
//              usart_puts3(debugString);
//      }
#endif

  CLRBIT(OLED_CTRL_PORT, OLED_DC);
  CLRBIT(OLED_CTRL_PORT, OLED_CS);
  SETBIT(OLED_CTRL_PORT, OLED_RD);
  CLRBIT(OLED_CTRL_PORT, OLED_WR);

  OLED_DATA_HIGH = 0;
  OLED_DATA_MED = (command >> 5) << 2;
  OLED_DATA_LOW = command << 3;

  SETBIT(OLED_CTRL_PORT, OLED_WR);
  SETBIT(OLED_CTRL_PORT, OLED_CS);
  SETBIT(OLED_CTRL_PORT, OLED_DC);

}

//*******************************************************************************
static inline void
oled28_data(unsigned int data)
{
  volatile unsigned char lB = (unsigned char) data;
  volatile unsigned char hB = (unsigned char) (data >> 8);

#ifdef _DEBUG_DISPWINDOW_
//      if (gEnableDebug)
//      {
//      char    debugString[48];
//
//              sprintf(debugString, "data=%04X   \r\n", data);
//              usart_puts3(debugString);
//      }
#endif

  SETBIT(OLED_CTRL_PORT, OLED_DC);
  CLRBIT(OLED_CTRL_PORT, OLED_CS);
  // SETBIT(OLED_CTRL_PORT,OLED_RD);
  CLRBIT(OLED_CTRL_PORT, OLED_WR);

  OLED_DATA_LOW = (lB << 3);
  OLED_DATA_MED = (lB >> 3);
  OLED_DATA_MED = OLED_DATA_MED | (hB << 6);
  OLED_DATA_HIGH = hB;

  SETBIT(OLED_CTRL_PORT, OLED_WR);
  SETBIT(OLED_CTRL_PORT, OLED_CS);
  // SETBIT(OLED_CTRL_PORT,OLED_DC);
}

//*******************************************************************************
static unsigned int
oled28_status(void)
{
//unsigned char low, med, high;
  unsigned char lB;

  volatile unsigned int result;

  OLED_DATA_LOW_DDR = 0x00; //in
  OLED_DATA_MED_DDR = 0x00; //in
  OLED_DATA_HIGH_DDR = 0x00; //in

  CLRBIT(OLED_CTRL_PORT, OLED_DC);
  CLRBIT(OLED_CTRL_PORT, OLED_CS);
  CLRBIT(OLED_CTRL_PORT, OLED_RD);

  lB = OLED_DATA_LOW_PIN >> 3;
  lB |= (OLED_DATA_MED_PIN >> 2) << 6;
  //hB = (OLED_DATA_MED_PIN<<1) >> 6;

  SETBIT(OLED_CTRL_PORT, OLED_RD);
  SETBIT(OLED_CTRL_PORT, OLED_CS);
  SETBIT(OLED_CTRL_PORT, OLED_DC);

  OLED_DATA_LOW_DDR = 0xFF; //output
  OLED_DATA_MED_DDR = 0xFF; //output
  OLED_DATA_HIGH_DDR = 0xFF; //output

//result = (hB << 8) + lB;
  result = lB;
  return result;
}

//*******************************************************************************
static void
oled28_brightness(unsigned char brightnessLevel)
{

  /* Range Check Brightness */
  if (brightnessLevel < BRIGHT_MAX) {
    unsigned char ii;

    /* Set the Brightness */
    for (ii = 0; ii < GREG_MAX; ii++) {
      oled28_command (kOLEDcmd_GammaTopBottomControl + ii);
      oled28_data (gGammaTable[brightnessLevel].gReg[ii]);
    }
  }
}

//*******************************************************************************
static void
oled28_init(void)
{
  COLOR black =
    {
        0,
        0,
        0 };

  volatile unsigned long x, y;

  SETBIT(V5_SHUTDOWN_DDR, V5_SHUTDOWN_PNUM);
  CLRBIT(V5_SHUTDOWN_PORT, V5_SHUTDOWN_PNUM);

  OLED_DATA_LOW_DDR = 0xFF; //output
  OLED_DATA_MED_DDR = 0xFF; //output
  OLED_DATA_HIGH_DDR = 0xFF; //output

  OLED_CTRL_DDR |= (1 << OLED_DC) | (1 << OLED_CS) | (1 << OLED_RD)
      | (1 << OLED_WR) | (1 << OLED_RESET);

  for (x = 0; x < 100; x++) {
    /* Reset OLED */
    CLRBIT(OLED_CTRL_PORT, OLED_RESET);
  }

  for (x = 0; x < 100; x++) {
    /* OLED ON */
    SETBIT(OLED_CTRL_PORT, OLED_RESET);
  }

  oled28_command (kOLEDcmd_IFselect);

  oled28_command (kOLEDcmd_RGBinterfaceControl);
  oled28_data (0x0000);

  oled28_command (kOLEDcmd_EntryMode);
  //  dispData(0x8030);
  oled28_data (0x8031);

  oled28_command (kOLEDcmd_StandbyMode); //standby off
  oled28_data (0x0000);

  //delay
  for (y = 0; y < 5000; y++)
    for (x = 0; x < 10; x++) {
      asm("nop");
    }

  SETBIT(OLED_SUPPLY_PORT, OLED_SUPPLY_PNUM);
  SETBIT(OLED_SUPPLY_DDR, OLED_SUPPLY_PNUM);
  //turn on supply

  for (y = 0; y < 5000; y++)
    for (x = 0; x < 10; x++) {
      asm("nop");
    }

  oled28_command (kOLEDcmd_DisplayControl1); //display on
  oled28_data (0x0001);

  oled28_brightness (1); //low brightness

  oled28_command (kOLEDcmd_GRAMread_write); //write / read graphics data

  dispColor (black);
  dispClearScreen ();

}

//*******************************************************************************
static inline void
oled28_pixel(void)
{
  CLRBIT(OLED_CTRL_PORT, OLED_CS);
  SETBIT(OLED_CTRL_PORT, OLED_DC);
  SETBIT(OLED_CTRL_PORT, OLED_RD);
  CLRBIT(OLED_CTRL_PORT, OLED_WR);

  OLED_DATA_LOW = GraphicsColor.blue;
  OLED_DATA_MED = GraphicsColor.green;
  OLED_DATA_HIGH = GraphicsColor.red;

  SETBIT(OLED_CTRL_PORT, OLED_WR);
  SETBIT(OLED_CTRL_PORT, OLED_CS);
}

static inline void
oled28_set_xy(int x, int y)
{
  //* Set XY location
  oled28_command (kOLEDcmd_GRAMaddressSetX); //Specify the x address in RAM
  oled28_data (0x00FF & y);

  oled28_command (kOLEDcmd_GRAMaddressSetY); //Specify the y address in RAM
  oled28_data (0x01FF & x);

  oled28_command (kOLEDcmd_GRAMread_write); //RAM write
}

static inline void
oled28_set_window(int x_start, int y_start, int x_end, int y_end)
{
  //*     Specify the Vertical start positions
  oled28_command (kOLEDcmd_VerticalRamAddrHi);
  oled28_data (x_start);

  //*     Specify the Vertical end positions
  oled28_command (kOLEDcmd_VerticalRamAddrLow);
  oled28_data (x_end);

  oled28_command (kOLEDcmd_HorizontalRamAddr);
  oled28_data ((y_start << 8) | y_end);

  //*     Specify the x address in RAM
  oled28_command (kOLEDcmd_GRAMaddressSetX);
  oled28_data (y_start);

  //*     Specify the y address in RAM
  oled28_command (kOLEDcmd_GRAMaddressSetY);
  oled28_data (x_start);
  //*     RAM write */
  oled28_command (kOLEDcmd_GRAMread_write);
}

static inline void
oled28_read(COLOR *pixel)
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
oled28_fill(unsigned long length)
{
  //*     Clear the screen
  CLRBIT(OLED_CTRL_PORT, OLED_CS);

  //*     Set the color once
  OLED_DATA_LOW = GraphicsColor.blue;
  OLED_DATA_MED = GraphicsColor.green;
  OLED_DATA_HIGH = GraphicsColor.red;

#ifndef __MWERKS__
  DUFF_DEVICE_8(length,
      asm("ldi r24,0x20             ""\n\t"
          "sts 0x0109,r24       ""\n\t" "sts 0x0109,r24 ""\n\t"
          "sts 0x0109,r24       ""\n\t" "sts 0x0109,r24 ""\n\t"
          "sts 0x0109,r24       ""\n\t" "sts 0x0109,r24 ""\n\t"
          "sts 0x0109,r24       ""\n\t" "sts 0x0109,r24 ""\n\t"
          "sts 0x0109,r24       ""\n\t" "sts 0x0109,r24 ""\n\t"
          "sts 0x0109,r24       ""\n\t" "sts 0x0109,r24 ""\n\t"
          "sts 0x0109,r24       ""\n\t" "sts 0x0109,r24 ""\n\t"
          "sts 0x0109,r24       ""\n\t" "sts 0x0109,r24 ""\n\t"
          :
          :
          : "r24" ););

  //*     SET the screen CS
  SETBIT(OLED_CTRL_PORT, OLED_CS);
#endif
}

static inline void
oled28_enable(void)
{
  oled28_command (0x05);
  oled28_data (0x01);
}

static inline void
oled28_disable(void)
{
  oled28_command (0x05);
  oled28_data (0x00);
}

static inline void
oled28_resume(void)
{
  oled28_command(0x10);
  oled28_data(0x00);
}

static inline void
oled28_suspend(void)
{
  oled28_command(0x10);
  oled28_data(0x01);
}

struct display_device oled28_driver = {
      .brightness_max = 5,

      .init = oled28_init,
      .brightness = oled28_brightness,
      .status = oled28_status,
      .command = oled28_command,
      .data = oled28_data,
      .pixel = oled28_pixel,
      .set_xy = oled28_set_xy,
      .set_window = oled28_set_window,
      .read = oled28_read,
      .resume = oled28_resume,
      .suspend = oled28_suspend,
      .enable = oled28_enable,
      .disable = oled28_disable,
      .fill = oled28_fill,
};
