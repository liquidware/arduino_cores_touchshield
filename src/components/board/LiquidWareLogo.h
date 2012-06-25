//*******************************************************************************
//#include	"LiquidWareLogo.h"

#ifndef _STARTUP_H_
	#include	"startup.h"
#endif


#ifdef _STARTUPSCREEN_LIQUIDWARE_

#ifndef __PGMSPACE_H_
	#include	<avr/pgmspace.h>
#endif

prog_uchar gLWstartupLogo[] PROGMEM  =	{
	
	
0,75,0,75,	//xSize_hi, xSize_lo ,ySize_hi, ySize_lo
150,207,14,0,		//*	background color
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,37,  183,183,183,1,  150,207,14,37,  		0,0,0,0,
150,207,14,36,  183,183,183,3,  150,207,14,36,  		0,0,0,0,
150,207,14,35,  183,183,183,5,  150,207,14,35,  		0,0,0,0,
150,207,14,35,  183,183,183,5,  150,207,14,35,  		0,0,0,0,
150,207,14,34,  183,183,183,7,  150,207,14,34,  		0,0,0,0,
150,207,14,33,  183,183,183,9,  150,207,14,33,  		0,0,0,0,
150,207,14,33,  183,183,183,10,  150,207,14,32,  		0,0,0,0,
150,207,14,32,  183,183,183,12,  150,207,14,31,  		0,0,0,0,
150,207,14,31,  183,183,183,12,  0,0,0,3,  150,207,14,29,  		0,0,0,0,
150,207,14,31,  183,183,183,12,  0,0,0,4,  150,207,14,28,  		0,0,0,0,
150,207,14,30,  183,183,183,12,  0,0,0,6,  150,207,14,27,  		0,0,0,0,
150,207,14,29,  183,183,183,13,  0,0,0,7,  150,207,14,26,  		0,0,0,0,
150,207,14,29,  183,183,183,12,  0,0,0,8,  150,207,14,26,  		0,0,0,0,
150,207,14,28,  183,183,183,12,  182,181,177,1,  0,0,0,4,  4,7,0,1,  0,5,0,1,  0,0,0,3,  150,207,14,25,  		0,0,0,0,
150,207,14,27,  183,183,183,13,  0,0,0,7,  68,79,19,1,  0,0,0,3,  150,207,14,24,  		0,0,0,0,
150,207,14,27,  183,183,183,13,  0,0,0,1,  4,1,10,1,  0,0,0,2,  0,2,0,1,  0,7,0,1,  94,111,17,1,  150,207,14,1,  34,46,0,1,  0,0,0,3,  150,207,14,23,  		0,0,0,0,
150,207,14,26,  183,183,183,13,  0,0,0,6,  26,39,0,1,  168,195,56,1,  150,207,14,1,  125,144,26,1,  1,13,0,1,  0,0,0,2,  150,207,14,23,  		0,0,0,0,
150,207,14,26,  183,183,183,13,  1,1,0,1,  0,0,0,1,  4,0,15,1,  0,0,0,1,  0,2,0,1,  0,0,0,1,  110,132,23,1,  150,207,14,3,  53,70,0,1,  0,0,0,2,  0,2,0,1,  150,207,14,22,  		0,0,0,0,
150,207,14,25,  183,183,183,13,  0,0,0,1,  1,0,0,1,  1,0,4,1,  0,0,0,3,  43,60,0,1,  150,207,14,5,  0,0,0,3,  150,207,14,22,  		0,0,0,0,
150,207,14,25,  183,183,183,12,  0,11,0,1,  1,0,0,1,  0,0,0,5,  128,154,19,1,  150,207,14,5,  60,79,0,1,  0,0,0,3,  150,207,14,21,  		0,0,0,0,
150,207,14,24,  183,183,183,13,  0,0,0,6,  60,78,2,1,  150,207,14,7,  0,0,0,1,  0,11,0,2,  150,207,14,21,  		0,0,0,0,
150,207,14,24,  183,183,183,12,  0,0,0,3,  1,0,5,1,  0,0,0,2,  3,18,0,1,  134,161,46,1,  150,207,14,7,  74,98,0,1,  0,10,0,1,  0,11,0,1,  0,0,0,1,  150,207,14,20,  		0,0,0,0,
150,207,14,23,  183,183,183,13,  0,0,0,6,  57,82,0,1,  150,207,14,9,  0,0,0,1,  0,5,0,1,  0,0,0,1,  150,207,14,20,  		0,0,0,0,
150,207,14,23,  183,183,183,12,  0,0,0,6,  0,19,0,1,  150,207,14,10,  79,103,9,1,  0,9,0,1,  0,11,0,1,  0,0,0,1,  150,207,14,19,  		0,0,0,0,
150,207,14,22,  183,183,183,13,  0,0,0,6,  56,80,0,1,  150,207,14,3,  158,210,3,1,  150,207,14,7,  0,0,0,3,  150,207,14,19,  		0,0,0,0,
150,207,14,22,  183,183,183,12,  0,0,0,5,  0,7,0,1,  0,0,0,1,  130,162,29,1,  150,207,14,11,  79,104,2,1,  0,8,0,1,  0,0,0,2,  150,207,14,18,  		0,0,0,0,
150,207,14,21,  183,183,183,13,  0,11,0,2,  0,3,8,1,  0,0,5,1,  0,5,1,1,  0,13,0,1,  58,86,0,1,  150,207,14,7,  150,209,0,1,  150,207,14,1,  156,210,0,1,  150,207,14,3,  0,0,0,4,  150,207,14,17,  		0,0,0,0,
150,207,14,21,  183,183,183,12,  0,11,0,2,  0,2,0,1,  0,4,3,1,  0,2,1,1,  0,4,0,1,  0,14,0,1,  120,154,15,1,  150,207,14,2,  152,206,0,1,  150,207,14,7,  154,206,0,1,  150,207,14,3,  0,8,0,1,  0,0,0,2,  150,207,14,17,  		0,0,0,0,
150,207,14,20,  183,183,183,3,  181,181,181,1,  183,183,183,9,  0,11,0,1,  0,0,0,2,  0,5,0,1,  0,4,0,1,  0,8,0,1,  43,65,0,1,  150,207,14,3,  153,209,0,1,  150,207,14,6,  151,207,10,1,  153,209,12,1,  150,207,14,3,  0,0,0,3,  150,207,14,17,  		0,0,0,0,
150,207,14,20,  183,183,183,1,  181,181,181,1,  183,183,183,10,  0,0,0,1,  0,11,0,2,  0,0,0,4,  108,135,38,1,  150,207,14,5,  148,211,8,1,  147,212,4,1,  150,207,14,5,  151,210,6,1,  150,207,14,2,  50,85,0,1,  0,0,0,1,  0,2,0,1,  150,207,14,17,  		0,0,0,0,
150,207,14,20,  183,183,183,12,  0,0,0,6,  15,39,0,1,  150,207,14,2,  153,205,8,1,  149,207,1,1,  148,208,0,1,  150,207,14,12,  0,11,0,1,  3,9,7,1,  0,11,0,1,  150,207,14,16,  		0,0,0,0,
150,207,14,19,  183,183,183,13,  0,0,0,2,  0,11,0,1,  0,0,0,1,  0,4,0,1,  0,0,0,1,  79,111,2,1,  150,207,14,17,  30,52,0,1,  0,6,0,1,  0,11,0,1,  150,207,14,16,  		0,0,0,0,
150,207,14,19,  183,183,183,2,  181,181,181,1,  183,183,183,9,  0,0,0,5,  0,5,0,1,  0,0,0,1,  138,179,41,1,  150,207,14,18,  0,9,0,1,  0,2,0,1,  0,11,0,1,  150,207,14,15,  		0,0,0,0,
150,207,14,19,  183,183,183,12,  0,0,0,3,  6,6,4,1,  0,0,0,2,  40,68,0,1,  156,203,47,1,  150,207,14,18,  0,23,0,1,  0,7,0,1,  0,11,0,1,  150,207,14,15,  		0,0,0,0,
150,207,14,18,  183,183,183,13,  0,0,0,6,  93,129,31,1,  148,201,33,1,  150,207,14,17,  147,194,52,1,  43,74,0,1,  0,9,0,1,  0,11,0,1,  150,207,14,15,  		0,0,0,0,
150,207,14,18,  183,183,183,12,  0,0,0,3,  1,0,5,1,  0,3,0,1,  0,0,0,2,  150,207,14,21,  0,11,0,1,  0,2,0,1,  150,207,14,15,  		0,0,0,0,
150,207,14,17,  183,183,183,13,  0,0,0,6,  31,67,0,1,  151,202,61,1,  150,207,14,9,  138,188,37,1,  150,207,14,10,  0,14,0,1,  0,2,0,1,  0,11,0,1,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  0,0,0,1,  1,0,5,1,  0,0,0,4,  60,101,0,1,  147,205,43,1,  150,207,14,8,  98,142,18,1,  38,81,0,1,  136,189,37,1,  150,207,14,9,  11,40,0,1,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  0,11,0,1,  0,0,4,1,  5,0,11,1,  0,0,5,1,  0,6,0,1,  0,0,0,1,  150,207,14,10,  38,73,0,1,  0,18,0,1,  90,133,18,1,  139,193,45,1,  150,207,14,8,  45,77,4,1,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  0,11,0,1,  1,1,3,1,  7,0,0,2,  0,0,0,2,  150,207,14,8,  141,197,26,1,  104,147,16,1,  0,0,0,1,  0,11,0,1,  0,0,0,1,  138,186,64,1,  150,207,14,8,  72,106,29,1,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  2,2,2,2,  1,0,0,1,  5,7,4,1,  0,0,0,2,  150,207,14,9,  25,59,0,1,  0,8,0,1,  0,0,0,2,  85,124,31,1,  150,207,14,9,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  0,0,0,2,  1,0,0,1,  7,9,4,1,  0,0,0,2,  150,207,14,8,  95,137,25,1,  0,18,0,1,  2,10,0,1,  0,2,2,1,  0,0,0,1,  7,40,0,1,  132,183,42,1,  150,207,14,8,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  0,0,0,3,  7,0,0,1,  0,0,0,2,  150,207,14,7,  134,195,32,1,  30,65,0,1,  0,14,0,1,  4,8,7,1,  0,0,0,1,  0,12,8,1,  0,14,0,1,  85,134,6,1,  133,196,21,1,  150,207,14,7,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  0,0,0,2,  1,1,0,1,  0,4,0,1,  0,0,0,2,  150,207,14,7,  105,163,16,1,  0,28,0,1,  0,0,0,5,  31,71,0,1,  123,182,28,1,  150,207,14,7,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  0,0,0,3,  3,5,0,1,  0,7,2,1,  0,0,0,1,  150,207,14,7,  61,117,0,1,  0,0,0,7,  112,166,52,1,  150,207,14,7,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  7,0,0,2,  0,0,0,1,  7,0,0,1,  0,0,0,2,  150,207,14,7,  21,72,0,1,  0,0,0,7,  84,136,38,1,  150,207,14,7,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  0,11,0,1,  1,1,1,1,  0,0,0,3,  0,10,0,1,  150,207,14,7,  9,55,0,1,  0,0,0,7,  51,99,0,1,  150,207,14,6,  3,31,0,1,  0,6,0,1,  0,0,0,1,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  0,11,0,1,  0,0,0,1,  0,1,0,1,  0,0,0,2,  0,8,0,1,  0,0,0,1,  150,207,14,6,  8,57,0,1,  0,0,0,7,  62,114,6,1,  150,207,14,6,  0,0,0,2,  0,4,0,1,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  0,11,0,2,  0,0,0,5,  150,207,14,6,  35,91,0,1,  7,0,0,1,  0,0,0,5,  0,20,0,1,  96,155,35,1,  150,207,14,5,  21,62,0,1,  0,0,0,1,  0,8,0,1,  0,5,0,1,  150,207,14,14,  		0,0,0,0,
150,207,14,17,  183,183,183,14,  0,11,0,1,  0,0,0,6,  150,207,14,5,  88,154,22,1,  22,77,0,1,  0,0,0,5,  22,77,0,1,  106,175,33,1,  150,207,14,5,  0,0,0,2,  0,5,2,1,  0,6,5,1,  150,207,14,14,  		0,0,0,0,
150,207,14,17,  183,183,183,14,  0,11,0,2,  0,0,0,5,  150,207,14,6,  71,135,13,1,  0,51,0,1,  0,29,0,1,  0,21,0,1,  0,25,0,1,  22,85,0,1,  98,172,23,1,  150,207,14,5,  0,0,0,1,  0,6,0,1,  0,0,0,3,  150,207,14,14,  		0,0,0,0,
150,207,14,17,  183,183,183,15,  0,11,0,2,  11,0,4,1,  0,0,0,4,  150,207,14,17,  0,8,0,1,  0,0,0,3,  150,207,14,15,  		0,0,0,0,
150,207,14,17,  183,183,183,15,  0,11,0,2,  4,0,2,1,  0,0,0,2,  7,6,12,1,  0,0,0,2,  150,207,14,14,  0,0,0,2,  0,3,2,1,  0,0,0,3,  150,207,14,15,  		0,0,0,0,
150,207,14,18,  183,183,183,14,  0,11,0,2,  4,4,2,1,  0,0,0,6,  0,17,0,1,  150,207,14,11,  0,0,0,5,  6,0,0,1,  150,207,14,16,  		0,0,0,0,
150,207,14,19,  183,183,183,14,  0,11,0,1,  0,0,0,5,  1,1,0,1,  0,0,0,18,  150,207,14,17,  		0,0,0,0,
150,207,14,19,  183,183,183,15,  0,11,0,3,  0,0,0,21,  150,207,14,17,  		0,0,0,0,
150,207,14,20,  183,183,183,16,  0,11,0,3,  3,0,0,1,  0,0,0,17,  150,207,14,18,  		0,0,0,0,
150,207,14,20,  183,183,183,18,  0,11,0,4,  0,4,0,1,  0,3,0,1,  0,0,0,8,  0,0,5,1,  0,0,0,2,  0,11,0,1,  150,207,14,19,  		0,0,0,0,
150,207,14,21,  183,183,183,21,  0,11,0,1,  0,0,0,12,  150,207,14,20,  		0,0,0,0,
150,207,14,22,  183,183,183,24,  0,0,0,8,  150,207,14,21,  		0,0,0,0,
150,207,14,22,  183,183,183,31,  150,207,14,22,  		0,0,0,0,
150,207,14,23,  183,183,183,29,  150,207,14,23,  		0,0,0,0,
150,207,14,24,  183,183,183,27,  150,207,14,24,  		0,0,0,0,
150,207,14,25,  183,183,183,24,  150,207,14,26,  		0,0,0,0,
150,207,14,26,  183,183,183,22,  150,207,14,27,  		0,0,0,0,
150,207,14,28,  183,183,183,19,  150,207,14,28,  		0,0,0,0,
150,207,14,34,  183,183,183,8,  150,207,14,33,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0

//total values=716,  
//EOF
};
#endif
