/*
@Platform: Friendly Arm tiny6410
@CPU: S3C6410
@Function: LCD
@Written by Armstrong Wall
@Start wrirte date: 2016.3.15
@Daily: 		2016.3.14  Finish Uart

*****************Attention*************
Some code	Ref to	http://m.blog.csdn.net/article/details?id=42213717
						
*/

#define GPICON		 		(*(volatile unsigned long*) 0x7F008100)
//Port I Configuration Register
#define GPJCON	 	 		(*(volatile unsigned long*) 0x7F008120)
//Port J Configuration Register
#define VIDCON0	 			(*(volatile unsigned long*) 0x77100000)
//Video control 0 register
#define VIDCON1	 	 		(*(volatile unsigned long*) 0x77100004)
//R/W Video control 1 register
#define VIDTCON0	 		(*(volatile unsigned long*) 0x77100010)
//Video time control 0 register
#define VIDTCON2	 		(*(volatile unsigned long*) 0x77100018)
//R/W Video time control 2 register
#define VIDTCON1	 		(*(volatile unsigned long*) 0x77100014)
//R/W Video time control 1 register
#define VIDW00ADD0B0	(*(volatile unsigned long*) 0x771000A0)
//R/W Window 0’s buffer start address register, buffer 0 
#define VIDW00ADD1B0	(*(volatile unsigned long*) 0x771000D0)
//R/W Window 0’s buffer end address register, buffer 0
#define VIDW00ADD2	 	(*(volatile unsigned long*) 0x77100100)
//R/W Window 0’s buffer size register
#define WINCON0	 			(*(volatile unsigned long*) 0x77100020)
//R/W Window 0 control register
#define WPALCON	 			(*(volatile unsigned long*) 0x771001A0)
//R/W Window Palette control register
#define MIFPCON	 			(*(volatile unsigned long*) 0x7410800C)
//R/W Modem Interface Port Control register
#define SPCON	 				(*(volatile unsigned long*) 0x7F0081A0)
//R/W Special Port Configuration Register
#define GPFCON	 			(*(volatile unsigned long*) 0x7F0080A0)
//  R/W Port F Configuration Register
#define VIDOSD0A	 		(*(volatile unsigned long*) 0x77100040)
//  R/W Video Window 0’s position control register 0x0000_0000
#define VIDOSD0B	 		(*(volatile unsigned long*) 0x77100044)
//R/W Video Window 0’s position control register 0x0000_0000
#define VIDOSD0C	 		(*(volatile unsigned long*) 0x77100048)
//  R/W Video Window 0’s size control register 0x0000_0000
 
/*
Frame Rate = 
1/ [ { (VSPW+1) + (VBPD+1) + (LIINEVAL + 1) + (VFPD+1) } x {(HSPW+1) + (HBPD +1)
+ (HFPD+1) + (HOZVAL + 1) } x { ( CLKVAL+1 ) / ( Frequency of Clock source ) } ]
*/
#define VSPW  2 //VSPW+1=3  tHP
#define VBPD  28//VBPD+1=29 tHP
#define VFPD	12//VFPD+1=tVP - tVW - tVBP - tw=525-3-29-480=13 tHP

 
#define HSPW	47//HSPW+1=48 tCLK
#define HBPD  39//HBPD+1=40 tCLK
#define HFPD  39//HBPD+1=tHP - tHW - tHBP - tHV=928-48-40-800=40 tCLK


#define LINEVAL  479// LINEVAL = (Vertical display size) –1=480
#define HOZVAL   799// HOZVAL = (Horizontal display size) -1=800

unsigned short LCDBUFFER[480][800];// 480 row *800 colum *2 bytes

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned long  U32;

void lcd_port_init()
{
		GPICON |= 0xaaaaaaaa;
//10 = LCD VD[0]
		GPJCON |= 0xaaaaaa;

		MIFPCON &= (0<<3);
/*
SEL_BYPASS [3] Select (mux) control for LCD bypass ( from Modem/Host Interface
to LCD i80 interface)
‘0’ = Normal mode
‘1’ = Bypass mode (initial value)   set value =0
*/
		SPCON   &= ~(0x3);
	  SPCON   |= (1<<0);
/*
LCD_SEL [1:0] Select LCD I/F pin configure 00
6410X_UM GPIO
10-43
00 = Host I/F style 01 = RGB I/F style,selcet this mode
10 = 601/656 sytle 11 = reserved.
*/

	}

void lcd_control_init()
{
		VIDCON0 &= ~((3<<26) | (3<<17) | (0xff<<6)  | (3<<2));     

/* RGB I/F, RGB Parallel format,  */
		VIDCON0 |= ((3<<6) | (1<<4) );    
		
		
/*				
According to LCD-A70 datasheet P11,DCLK =33.3Mhz
RGB_VCLK (Hz) =HCLK/ (CLKVAL+1) where CLKVAL >= 1
							=133Mhz/(CLKVAL+1) =33.3Mhz
							so CLKVAL+1=4 -->  CLKVAL=3
							
CLKVAL_F [13:6] Determine the rates of VCLK and CLKVAL[7:0]
VCLK = Video Clock Source / (CLKVAL+1) where CLKVAL >= 1
Note. 1. The maximum frequency of VCLK is 66MHz.
			2. Video Clock Source is selected by CLKSEL_F register
CLKSEL_F [3:2] Select the Video Clock source,00 = HCLK

so VIDCON0=0b0000 1100 0000= 0xC0
*/
    VIDCON1 &= ~(1<<7);   
/* IVCLK [7] This bit controls the polarity of the VCLK active edge.
	0 = RGB type LCD driver gets the video data at VCLK falling edge
	1 = RGB type LCD driver gets the video data at VCLK rising edge */
		VIDCON1 = ((1)<<6) | ((1)<<5) |((0)<<4);
		
/*
IHSYNC [6] This bit indicates the HSYNC pulse polarity.
0 = normal 1 = inverted  , choose inverted

IVSYNC [5] This bit indicates the VSYNC pulse polarity.
0 = normal 1 = inverted  , choose inverted

IVDEN [4] This bit indicates the VDEN signal polarity.
0 = normal 1 = inverted  , choose normal
*/
		
		
		VIDTCON0 = (VBPD<<16) | (VFPD<<8) | (VSPW<<0);
/*
VBPD [23:16] Vertical back porch is the number of inactive lines at the start of a
frame, after vertical synchronization period.
VFPD [15:8] Vertical front porch is the number of inactive lines at the end of a
frame, before vertical synchronization period.
VSPW [7:0] Vertical sync pulse width determines the VSYNC pulse's high level
width by counting the number of inactive lines.	
*/	
    VIDTCON1 = (HBPD<<16) | (HFPD<<8) | (HSPW<<0);
/*
HBPD [23:16] Horizontal back porch is the number of VCLK periods between the
falling edge of HSYNC and the start of active data.
HFPD [15:8] Horizontal front porch is the number of VCLK periods between the
end of active data and the rising edge of HSYNC.
HSPW [7:0] Horizontal sync pulse width determines the HSYNC pulse's high
level width by counting the number of the VCLK.
*/
    
    VIDTCON2 = (LINEVAL<<11) | (HOZVAL<<0);
/*
LINEVAL [21:11] These bits determine the vertical size of display
HOZVAL [10:0] These bits determine the horizontal size of display 
*/

		WINCON0  = (1<<17) | (5<<2);
/*
BYTSWP [17] Byte swaps control bit.
0 = Swap Disable 1 = Swap Enable
BPPMODE_F [5:2] Select the BPP (Bits Per Pixel) mode Window image
0101 = 16 BPP (non-palletized, R: 5-G:6-B:5 )
*/

  	VIDOSD0A = ((0<<11) | (0<<0));
/*  	
OSD_LeftTopX_F [21:11] Horizontal screen coordinate for left top pixel of OSD image 0
OSD_LeftTopY_F [10:0] Vertical screen coordinate for left top pixel of OSD image
*/
		VIDOSD0B = ((799<<11) | (479<<0));
/*
OSD_RightBotX_F [21:11] Horizontal screen coordinate for right bottom pixel of OSD
image
OSD_RightBotY_F [10:0] Vertical screen coordinate for right bottom pixel of OSD image*/
		VIDOSD0C = 800*480; 
//Eq. Height * Width (Number of Word)




                 /****Frame Buffer init*****/



		VIDW00ADD0B0 = (((U32)LCDBUFFER>>24)<<24) | (((U32)LCDBUFFER)&0xffffff);
/*
VBANK_F [31:24] These bits indicate A[31:24] of the bank location for the video buffer
in the system memory.
VBASEU_F [23:0] These bits indicate A[23:0] of the start address of the Video frame
buffer.
*/

		VIDW00ADD1B0 = (((U32)LCDBUFFER)&0xffffff+ (800*2+0)*480);
/*		
VBASEL_F [23:0] These bits indicate A[23:0] of the end address of the Video frame
buffer.
VBASEL = VBASEU +
(PAGEWIDTH+OFFSIZE) x (LINEVAL+1)*/

		VIDW00ADD2  = (0<<13) | 800*2;
/*
OFFSIZE_F [25:13] Virtual screen offset size (the number of byte).
This value defines the difference between the address of the last byte
displayed on the previous Video line and the address of the first byte
to be displayed in the new Video line.
OFFSIZE_F must have value that is multiple of 4-byte size or 0.

PAGEWIDTH_F [12:0] Virtual screen page width (the number of byte).
This value defines the width of the view port in the frame..
PAGEWIDTH must have bigger value than the burst size and the
size must be aligned word boundary.
*/



	}
	
void lcd_init()
{
		lcd_port_init();
		lcd_control_init();
		
		//打开LCD电源
		
		//LCD控制器
		VIDCON0 |= (3<<0);
		WINCON0 |= (1<<0);  
/*
ENVID [1] Video output and the logic immediately enable/disable.
0 = Disable the video output and the Display control signal.
1 = Enable the video output and the Display control signal.
0
ENVID_F [0] Video output and the logic enable/disable at current frame end.
0 = Disable the video output and the Display control signal.
1 = Enable the video output and the Display control signal.
* If set on and off this bit, then you will read “H” and video controller
enable until the end of current frame.*/
		
	}
	
void point(unsigned int x,unsigned int y,unsigned int color)
{
	unsigned int red,green,blue;
	
	red = (color>>19)  &0x1f;
	green= (color>>10) &0x3f;
	blue= (color>>3)   &0x1f;

	LCDBUFFER[y][x] = (unsigned short)((red<<11) | (green<<5) |blue);

}


void Paint_Bmp(U16 x0,U16 y0,U16 wide,U16 high,const U8 *bmp)
{
    U16 x,y;
    U16 c;
    U32 p = 0;
    
    for( y = y0 ; y < y0+high ; y++ )
    {
    	for( x = x0 ; x < x0+wide ; x++ )
    	{
    	    c = bmp[p] | (bmp[p+1]<<8) ;										
	    
	    if ( ( x < 800) && ( y < 480) )
	        LCDBUFFER[y][x] = c ;
		
	    p = p + 2 ;	
    	}
    }
}
extern unsigned char bmp[90200];

void lcd_test()
{
		int x;
		
		for(x=0;x<800;x++)
		{
			point(x++,320,0xB3EE3A);
			
			}
			
		Paint_Bmp(0,0,220,220,bmp);
		
	}

void clearSrc(unsigned int color)
{
		
	}