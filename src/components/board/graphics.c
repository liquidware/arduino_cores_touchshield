//*******************************************************************************
//*
//*
//*
//*******************************************************************************
//*	Detailed Edit History
//*	<inthebitz>	=	Matt <inthebitz @ antipastohw.blogspot.com & liquidware.com>
//*	<CML>		=	Chris  <Chris @ antipastohw.blogspot.com & liquidware.com>
//*	<MLS>		=	Mark Sproul <msproul@jove.rutgers.edu>
//* <TWH>		=	Thom Holtquist, ctsfutures.com
//*******************************************************************************
//*		PLEASE put comments here every time you work on this code
//*******************************************************************************
//*	Dec 27,	2008	<MLS> Copied some bounds check from version 0.9 for dispRectangle
//*	Dec 27,	2008	<MLS> the new version of dispRectangle does not work, using old versioon
//*	Dec 27,	2008	<MLS> Changed RECT_T to RECT
//*	Dec 28,	2008	<MLS> Changed SCREEN_T to SCREEN
//*	Dec 29,	2008	<CML> released v0012LW
//*	Dec 31,	2008	<MLS> Copied dispBrightness from 0012
//*	Jan  1,	2009	<MLS> dispRectangle -  Fixed overflow problem on large rectangles
//*	Jan  2,	2009	<MLS> Added negitive checking to w/h of dispRectangle
//*	Jan  3,	2009	<MLS> dispRectangle, changed xLoc, yLoc back to signed ints
//*	Jan 14,	2009	<MLS> Changed all 320/240/319/239 number constants to define constants
//*	Jan 19,	2009	<MLS> Got permission from Chris to start working on cleanin up slide code
//*	Jan 19,	2009	<MLS> Added more bounds checking to dispRectangle, fixed offscreen center problem
//*	Dec 17, 2009	<MLS> portrait mode support added, not finished
//*	Dec 18, 2009	<MLS> added kScreenOrientation_Portrait180
//*	Dec 18, 2009	<MLS> added SetScreenOrientation
//*	Dec 23,	2009	<MLS> Finished orientation support
//*	Dec 26,	2009	<MLS> Fixed dispRead to support for screen rotation
//*	Jan  8,	2009	<TWH/MLS> Added in TWHs code to graphcis.c
//*	Jan  9,	2009	<TWH/MLS> Adding TWHs clipping code with _ENABLE_CLIPPING_
//*	Jan 10,	2010	<TWH/MLS> Added TWHs hardare scroll with _ENABLE_HARDWARE_SCROLL_
//*	Jan 10,	2010	<TWH/MLS> Added dispPowerLCD and dispStandby
//*	Jan 11,	2010	<MLS> Extended clipping to dispSetWindow
//*	Jan 16,	2010	<MLS> dispSetWindow now retunrs TRUE/FALSE, very important for clipping
//*	Jan 16,	2010	<MLS> Added gClippingEnabled so all of the testing doenst have to be done if its not being used
//*******************************************************************************


//*******************************************************************************
//*	Includeds
//*******************************************************************************
#include	<avr/io.h>
#include	<avr/pgmspace.h>
#include	<avr/interrupt.h>

#include	"HardwareDef.h"
#include        "display.h"
#include	"graphics.h"

#ifndef SUBPGRAPHICS_H
	#include	"SubPGraphics.h"
#endif
#include	"bitops.h"
#include	"font.h"

#define incx() x++, dxt += d2xt, t += dxt
#define incy() y--, dyt += d2yt, t += dyt




COLOR	GraphicsColor;
SCREEN	screen	=	{
					kSCREEN_X_size,			//*	Width
					kSCREEN_Y_size};		//*	Height


#ifdef _ENABLE_CLIPPING_
	//*	globals for TWHs clipping code
	//*	_ENABLE_CLIPPING_ is defined graphics.h
	boolean	gClippingEnabled	=	false;
	int		gClipX1				=	0;
	int		gClipY1				=	0;
	int		gClipX2				=	kSCREEN_X_size;
	int		gClipY2				=	kSCREEN_Y_size;
#endif

//#define	_DEBUG_DISPWINDOW_

#ifdef _DEBUG_DISPWINDOW_
	char	gEnableDebug	=	false;
#endif


//*******************************************************************************
//*	Slide specific stuff
//*******************************************************************************


//*******************************************************************************
void	dispColor(COLOR c)
{
	GraphicsColor.red	=	c.red;
	GraphicsColor.green	=	c.green;
	GraphicsColor.blue	=	c.blue;
}

//*******************************************************************************
void	dispPix()
{
        display->pixel();
}


#ifdef _ENABLE_SCREEN_ROTATION_

	char	gScreenOrientation	=	kDefaultOrienation;
#endif


//*******************************************************************************
int	GetScreenWidth(void)
{
int	theScreenWidth;

#ifdef _ENABLE_SCREEN_ROTATION_
	switch(gScreenOrientation)
	{
		case  kScreenOrientation_Portrait:
		case  kScreenOrientation_Portrait180:
			theScreenWidth	=	kSCREEN_Y_size;
			break;

		default:
			theScreenWidth	=	kSCREEN_X_size;
			break;
	}
#else
	theScreenWidth  =       kSCREEN_X_size;
#endif

	return(theScreenWidth);
}

//*******************************************************************************
int	GetScreenHeight(void)
{
int	theScreenHeight;

#ifdef _ENABLE_SCREEN_ROTATION_
	switch(gScreenOrientation)
	{
		case  kScreenOrientation_Portrait:
		case  kScreenOrientation_Portrait180:
			theScreenHeight	=	kSCREEN_X_size;
			break;

		default:
			theScreenHeight	=	kSCREEN_Y_size;
			break;
	}
#else
	theScreenHeight	=	kSCREEN_Y_size;
#endif

	return(theScreenHeight);
}



#ifdef _ENABLE_SCREEN_ROTATION_
//*******************************************************************************
//*	returns OLD orientation
byte	SetScreenOrientation(char newOrienation)
{
byte	oldOrientation;

	oldOrientation		=	gScreenOrientation;
	gScreenOrientation	=	newOrienation;
	if (gScreenOrientation >= kScreenOrientation_Last)
	{
		gScreenOrientation	=	0;
	}
	//*	SubPgraphics globals
	gWidth	=	GetScreenWidth();
	gHeight	=	GetScreenHeight();

#ifdef _ENABLE_CLIPPING_
	//*	we need to call removeClip to set the new boundries
	dispRemoveClip();
#endif

	return(oldOrientation);
}

//*******************************************************************************
//*	returns True if within bounds
static void	TranslateScreenOrientation(int xx, int yy, int *newXX, int *newYY)
{
#ifdef _ENABLE_SCREEN_ROTATION_
	switch(gScreenOrientation)
	{
		case  kScreenOrientation_Portrait:
			*newXX	=	yy;
			*newYY	=	(kSCREEN_Y_size - xx);
			break;

		case  kScreenOrientation_Portrait180:
			*newXX	=	(kSCREEN_X_size - yy);
			*newYY	=	xx;
			break;

		case	kScreenOrientation_Landscape180:
			*newXX	=	kSCREEN_X_size - xx;
			*newYY	=	kSCREEN_Y_size - yy;
			break;

		//*	this is the default, dont do anything except Copy
		default:
			*newXX	=	xx;
			*newYY	=	yy;
			break;
	}
#else
	//*	this is the default, dont do anything except Copy
	*newXX	=	xx;
	*newYY	=	yy;
#endif

}
#endif		//_ENABLE_SCREEN_ROTATION_




//*******************************************************************************
//*	returns TRUE if all OK, FALSE if we are aborting
unsigned char	dispSetWindow(int xx, int yy, int argWidth, int argHeight)
{
uint16_t	yyStart, yyEnd;
uint16_t	xxStart, xxEnd;
int			myXX, myYY;
int			myWidth, myHeight;

#ifdef DEBUG
  char out[100];
  usart_puts("dispSetWindow\n");

  sprintf(out, "xx=%d, yy=%d, argWidth=%d, argHeight=%d\n",
      xx, yy, argWidth, argHeight);
  usart_puts(out);
#endif

#ifdef _DEBUG_DISPWINDOW_
char		debugString[48];
	gEnableDebug	=	true;
	if ((xx < (gWidth / 2)) && (yy > 10))
	{
		gEnableDebug	=	true;
	}
	if (gEnableDebug)
	{
		sprintf(debugString, "xx=%3d yy=%3d argWidth=%3d argHeight=%3d\r\n", xx, yy, argWidth, argHeight);
		usart_puts3(debugString);
	}
#endif

#ifdef _ENABLE_CLIPPING_
	if (gClippingEnabled)
	{
		//*	if clipping is enabled, we have to adjust all the parameters
		//*	first see if it is outside the clip bounds
		if ( (xx >= gClipX2) || (yy >= gClipY2))
		{
#ifdef DEBUG
		        usart_puts("dispSetWindow ex1\n");
#endif
			return(FALSE);
		}
		if ( ((xx + argWidth) <= gClipX1) || ((yy + argHeight) <= gClipY1))
#ifdef DEBUG
		  usart_puts("dispSetWindow ex2\n");
#endif
			return(FALSE);
		}
		//*	is the left edge outside to the left
		if (xx < gClipX1)
		{
		int	rightEdge;

			rightEdge	=	xx + argWidth;

			xx			=	gClipX1;
			argWidth	=	rightEdge - xx;

		}
		//*	is the right edge outside to the right
		if ((xx + argWidth) > gClipX2)
		{
			argWidth	=	gClipX2 - xx;
		}


		if (yy < gClipY1)
		{
		int	bottomEdge;

			bottomEdge	=	yy + argHeight;

			yy			=	gClipY1;
			argHeight	=	bottomEdge - yy;

		}
		if ((yy + argHeight) > gClipY2)
		{
			argHeight	=	gClipY2 - yy;
		}

		if ((argWidth <= 0) || (argHeight <= 0))
		{
#ifdef DEBUG
		  usart_puts("dispSetWindow ex3\n");
#endif
			return(FALSE);
		}
	}
#endif


	//* Bounds checking
	if ((argWidth <= 0) || (argHeight <= 0) )
	{
#ifdef DEBUG
	  sprintf(out,"dispSetWindow ex4 argWidth=%d, argHeight=%d\n",argWidth, argHeight);
	  usart_puts(out);
#endif
		return(FALSE);
	}
	if ((xx > gWidth) || (yy > gHeight))
	{
#ifdef DEBUG
	  usart_puts("dispSetWindow ex5\n");
#endif
		return(FALSE);
	}
	if (xx <= 0)
	{
		argWidth	+=	xx;
		xx			=	0;
	}
	if (yy <= 0)
	{
		argHeight	+=	yy;
		yy			=	0;
	}
	if ((xx + argWidth) > gWidth)
	{
		argWidth	=	gWidth - xx;
	}

	if ((yy + argHeight) > gHeight)
	{
		argHeight	=	gHeight - yy;
	}

	if ((argWidth <= 0) || (argHeight <= 0))
	{
#ifdef DEBUG
	  usart_puts("dispSetWindow ex6\n");
#endif
		return(FALSE);
	}


#ifdef _DEBUG_DISPWINDOW_
	if (gEnableDebug)
	{
		usart_puts3("--------------\r\n");

		sprintf(debugString, "xx=%3d yy=%3d\r\n", xx, yy);
		usart_puts3(debugString);


		sprintf(debugString, "gScreenOrientation=%2x\r\n", gScreenOrientation);
		usart_puts3(debugString);
		sprintf(debugString, "xx=%3d yy=%3d argWidth=%3d argHeight=%3d\r\n", xx, yy, argWidth, argHeight);
		usart_puts3(debugString);
	}
#endif

#ifdef _ENABLE_SCREEN_ROTATION_


	TranslateScreenOrientation(xx, yy, &myXX, &myYY);

	switch(gScreenOrientation)
	{
		case  kScreenOrientation_Portrait:
		case  kScreenOrientation_Portrait180:
			myWidth		=	argHeight;
			myHeight	=	argWidth;
			break;

		//*	this is the default, dont do anything except Copy
		default:
			myWidth		=	argWidth;
			myHeight	=	argHeight;
			break;
	}
#else
	//*	normal operation
	myXX		=	xx;
	myYY		=	yy;
	myWidth		=	argWidth;
	myHeight	=	argHeight;
#endif

#ifdef _DEBUG_DISPWINDOW_
	if (gEnableDebug)
	{

		sprintf(debugString, "myXX=%3d myYY=%3d myWidth=%3d myHeight=%3d\r\n", myXX, myYY, myWidth, myHeight);
		usart_puts3(debugString);
	}
#endif


#ifdef _ENABLE_SCREEN_ROTATION_

#ifdef _DEBUG_DISPWINDOW_
	if (gEnableDebug)
	{
	char	debugString[48];

		sprintf(debugString, "myXX=%3d myYY=%3d myWidth=%3d myHeight=%3d\r\n", myXX, myYY, myWidth, myHeight);
		usart_puts3(debugString);
	}
#endif
	switch(gScreenOrientation)
	{
		case  kScreenOrientation_Portrait:
		   	yyStart	=	myYY - myHeight;
		   	yyEnd	=	myYY;
			xxStart	=	myXX;
		   	xxEnd	=	myXX + myWidth;
			break;


		case  kScreenOrientation_Portrait180:
		   	yyStart	=	myYY;
		   	yyEnd	=	myYY + myHeight;
			if (yyStart <= 0)
			{
				yyStart	=	1;
			}
			if (yyEnd > kSCREEN_Y_size)
			{
				yyEnd	=	kSCREEN_Y_size - 1;
			}
			xxStart	=	myXX - myWidth;
		   	xxEnd	=	myXX;
			break;

		case	kScreenOrientation_Landscape180:
			yyStart	=	myYY - myHeight;
			yyEnd	=	myYY ;

			xxStart	=	myXX - myWidth;
		   	xxEnd	=	myXX;
			break;

		default:
			yyStart	=	myYY;
			yyEnd	=	myYY + myHeight;

			xxStart	=	myXX;
		   	xxEnd	=	myXX + myWidth;
			break;
	}
#else
	if ((myYY + myHeight) >= kSCREEN_Y_size)
	{
		myHeight	=	(kSCREEN_Y_size - 1) - myYY;
	}

	yyStart	=	myYY;
	yyEnd	=	myYY + myHeight;

	xxStart	=	myXX;
   	xxEnd	=	myXX + myWidth;
#endif

	//**************************************
	//*	check absolute bounds
	//*	from page 63 of the LCD manual
	//*		Window Address Range
	//*			0 <= HSA <= HEA <= 0xEF  (239)
	//*			0 <= VSA <= VEA <= 0x13F  (319)
	//*	yyStart	=	HSA	(Horizontal Start Address)
	//*	yyEnd	=	HEA	(Horizontal End Address)
	//*	xxStart	=	VSA	(Vertical Start Address)
	//*	xxEnd	=	VEA	(Vertical End Address)
	//**************************************
	if (yyStart < 0)
	{
		yyStart	=	0;
	}
	if (yyEnd >= kSCREEN_Y_size)
	{
		yyEnd	=	kSCREEN_Y_size - 1;
	}
	if (xxStart < 0)
	{
		xxStart	=	0;
	}
	if (xxEnd >= kSCREEN_X_size)
	{
		xxEnd	=	kSCREEN_X_size - 1;
	}

#ifdef _DEBUG_DISPWINDOW_
        if (gEnableDebug)
        {
          sprintf(debugString, "xxStart=%3d xxEnd=%3d yyStart=%3d yyEnd=%3d \r\n", xxStart, xxEnd, yyStart, yyEnd);
          usart_puts3(debugString);
        }
#endif

	display->set_window(xxStart, yyStart, xxEnd, yyEnd);

#ifdef _DEBUG_DISPWINDOW_
	if (gEnableDebug)
	{
		usart_puts3("\r\n");
		gEnableDebug	=	false;
	}
#endif

	return(TRUE);
}

//*	portrait mode support added by Mark Sproul, Dec 17, 2009
//*	it is not finished, the FILL routines and TOUCH routines need to be modified



//*******************************************************************************
void	dispPixel(int xx, int yy)
{
int	myXX, myYY;


#ifdef _ENABLE_SCREEN_ROTATION_
	//*	we have to check this to make sure beforw we do the subtraction
	if ( (xx < 0) || (yy < 0))
	{
		return;
	}
	TranslateScreenOrientation(xx, yy, &myXX, &myYY);

#else
	myXX	=	xx;
	myYY	=	yy;
#endif

#ifdef _ENABLE_CLIPPING_
	if (gClippingEnabled)
	{
		if ( (xx < gClipX1) || (yy < gClipY1))
		{
			return;
		}
		if ( (xx > gClipX2) || (yy > gClipY2))
		{
			return;
		}
	}
#endif

	//* check to make sure its on the screen
	if ( (myXX >= kSCREEN_X_size) || (myYY >= kSCREEN_Y_size))
	{
		return;
	}
	if ( (myXX < 0) || (myYY < 0))
	{
		return;
	}

	display->set_xy(myXX, myYY);

	display->pixel();
}

//*******************************************************************************
//*	dispPixel2 added by <TWH> for aliasing functions
void	dispPixel2(int xx, int yy)
{
#if 0
	if (iDisplayMode == 1)
	{
		int t	=	xx;
		xx		=	yy;
		yy		=	kSCREEN_Y_size - t;	// If you don't do this your image gets mirrored
	}
#endif


	//* check to make sure its on the screen
	if ( (xx >= kSCREEN_X_size) || (yy >= kSCREEN_Y_size))
	{
		return;
	}
	if ( (xx < 0) || (yy < 0))
	{
		return;
	}

	display->set_xy(xx, yy);

	display->pixel();
}


//*******************************************************************************
//*	Jan 29, 2009	<MLS> Changed args to int (were uint16_t)
//*	Dec 26,	2009	<MLS> Added support for screen rotation
//*******************************************************************************
void	dispRead(COLOR *buffer, int xx, int yy)
{
int	myXX, myYY;


#ifdef _ENABLE_SCREEN_ROTATION_
	//*	we have to check this to make sure beforw we do the subtraction
	if ( (xx < 0) || (yy < 0))
	{
		return;
	}
	TranslateScreenOrientation(xx, yy, &myXX, &myYY);

#else
	myXX	=	xx;
	myYY	=	yy;
#endif


	//* check to make sure its on the screen
	if ((myXX < 0) || (myYY < 0) || (myXX >= kSCREEN_X_size) || (myYY >= kSCREEN_Y_size))
	{
		//*	set the value to something
		buffer->blue	=	0;
		buffer->green	=	0;
		buffer->red		=	0;
		return;
	}

	display->set_xy(myXX, myYY);

	display->read(buffer);
}

//*******************************************************************************
//*	Jan  1,	2009	<MLS> dispRectangle -  Fixed overflow problem on large rectangles
//*	Jan  3,	2009	<MLS> dispRectangle, changed xLoc, yLoc back to signed ints
//*******************************************************************************
void	dispRectangle(int  xLoc,  int  yLoc,   int rectWidth,   int  rectHeight)
{
//int32_t len	=	(width*height);	//*	if width or height get modified, this is messed up
long	len;
long	myWidth;
long	myHeight;
boolean	okToContinue;

	myWidth		=	rectWidth;
	myHeight	=	rectHeight;
	//*	Jan  2,	2009	<MLS> Added negitive checking to w/h of dispRectangle
	if ((xLoc >= gWidth)  || (yLoc >= gHeight) || (rectWidth <= 0) || (rectHeight <= 0))
	{
		return;
	}
	//*	Jan 19,	2009	<MLS> Added more bounds checking to dispRectangle, fixed offscreen center problem
	if (((xLoc + myWidth) < 0) || ((yLoc + myHeight) < 0))
	{
		return;
	}

#ifdef _ENABLE_SCREEN_ROTATION_
	switch(gScreenOrientation)
	{
		case  kScreenOrientation_Portrait:
		case  kScreenOrientation_Portrait180:
			if ((xLoc + myWidth) >= kSCREEN_Y_size)
			{
				myWidth	=	(kSCREEN_Y_size - 1) - xLoc;
			}
			if ((yLoc + myHeight) >= kSCREEN_X_size)
			{
				myHeight	=	(kSCREEN_X_size - 1) - yLoc;
			}
			break;

		default:
			if ((xLoc + myWidth) >= kSCREEN_X_size)
			{
				myWidth	=	(kSCREEN_X_size - 1) - xLoc;
			}
			if ((yLoc + myHeight) >= kSCREEN_Y_size)
			{
				myHeight	=	(kSCREEN_Y_size - 1) - yLoc;
			}
			break;
	}
#else
	if ((xLoc + myWidth) >= kSCREEN_X_size)
	{
		myWidth	=	(kSCREEN_X_size - 1) - xLoc;
	}
	if ((yLoc + myHeight) >= kSCREEN_Y_size)
	{
		myHeight	=	(kSCREEN_Y_size - 1) - yLoc;
	}
#endif




	//*	Set XY location
	okToContinue	=	dispSetWindow(xLoc, yLoc, myWidth, myHeight);
	if (okToContinue)
	{
	        len   =       (myWidth * myHeight);
	        display->fill(len);
	}
}


//*******************************************************************************
void dispClearScreen()
{
  unsigned long len = kSCREEN_X_size;

  len *= kSCREEN_Y_size;

  display->set_xy(0,0);

  display->fill(len);
}

#pragma mark -
#pragma mark rectange clipping

#ifdef _ENABLE_CLIPPING_
//*******************************************************************************
//* May 13, 2009	<TWH> Added support for [rectangle] clipping
//*******************************************************************************
void	dispSetClip(int x, int y, int width, int height)
{
	if ((x < 0) || (y < 0) || (width < 1) || (height < 1))
	{
		return;
	}
	if (x > gWidth || y > gHeight)
	{
		return;
	}
	gClipX1	=	x;
	gClipY1	=	y;
	gClipX2	=	gClipX1 + width;
	gClipY2	=	gClipY1 + height;
	if (gClipX2 > gWidth)
	{
		gClipX2	=	gWidth;
	}
	if (gClipY2 > gHeight)
	{
		gClipY2	=	gHeight;
	}
	gClippingEnabled	=	true;
}

//*******************************************************************************
void	dispRemoveClip()
{
	gClipX1				=	0;
	gClipY1				=	0;
	gClipX2				=	gWidth;
	gClipY2				=	gHeight;
	gClippingEnabled	=	false;
}
#endif


#pragma mark -
#pragma mark Hardware Scroll

#ifdef _ENABLE_HARDWARE_SCROLL_
//*******************************************************************************
void	dispSetScrollArea(int startRow, int endRow)
{
	if (startRow < 0 || startRow > 319) return;
	if (endRow < 0 || endRow > 319) return;

	if (startRow < endRow)
	{
		dispCommand(kOLEDcmd_VerticalScrollCtrlHi);		// Scroll Start
		dispData(startRow);
		dispCommand(kOLEDcmd_VerticalScrollCtrlLo);		// Scroll End
		dispData(endRow);
	}
	else
	{
		dispCommand(kOLEDcmd_VerticalScrollCtrlHi);		// Scroll Start
		dispData(endRow);
		dispCommand(kOLEDcmd_VerticalScrollCtrlLo);		// Scroll End
		dispData(startRow);
	}
}

//*******************************************************************************
void	dispScrollLeft(int steps)
{
	if (steps < 0 || steps > 319) return;
	dispCommand(kOLEDcmd_VerticalScrollCtrl2);		// Perform Scroll
	dispData(steps);
}

//*******************************************************************************
void	dispScrollRight(int steps)
{
	dispScrollLeft(319-steps);
}

//*******************************************************************************
void	dispScrollStop()
{
	dispScrollLeft(0);
}
#endif


#if 0
//*******************************************************************************
//*	this code by MLS, will be deleted once TWHs code is working
void	dispScrollVertical(short startRow, short endRow, short numRows)
{
//*	this worked but veritcal is actually horzontal in the way this code works
//*	also, once scrolled, the cordinates are messed up and it stays scrolled,
//*	in other words, USELESS!!!!
#ifdef _TOUCH_SLIDE_

	dispCommand(kOLEDcmd_VerticalScrollCtrlHi);
	dispData(0);

	dispCommand(kOLEDcmd_VerticalScrollCtrlLo);
	dispData(300);

	dispCommand(kOLEDcmd_VerticalScrollCtrl2);
	dispData(10);
#endif

}
#endif
