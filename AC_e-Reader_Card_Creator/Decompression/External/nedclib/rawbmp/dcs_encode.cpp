#include "stdafx.h"
#include <stdio.h>
#define DCS
#include "dcs.h"

#include "../nedclib.h"

void eight_ten_modulate(void)
{
	int i, j;
	unsigned char raw0,raw1,raw2,raw3;
	unsigned char mod0,mod1,mod2,mod3,mod4,mod5,mod6,mod7;
	for (i=0;i<dotcodelen;i++)
	{
		for(j=0;j<104;j+=4)
		{
			raw0 = raw[i][j+0];
			raw1 = raw[i][j+1];
			raw2 = raw[i][j+2];
			raw3 = raw[i][j+3];
			mod0 = modtable[(raw0 & 0xF0)>>4];
			mod1 = modtable[(raw0 & 0x0F)];
			mod2 = modtable[(raw1 & 0xF0)>>4];
			mod3 = modtable[(raw1 & 0x0F)];
			mod4 = modtable[(raw2 & 0xF0)>>4];
			mod5 = modtable[(raw2 & 0x0F)];
			mod6 = modtable[(raw3 & 0xF0)>>4];
			mod7 = modtable[(raw3 & 0x0F)];
			
			_810mod[i][((j*10)/8)+0] = ((mod0 & 0x1F) << 3) + ((mod1 & 0x1C) >> 2);
			_810mod[i][((j*10)/8)+1] = ((mod1 & 0x03) << 6) + ((mod2 & 0x1F) << 1) + ((mod3 & 0x10) >> 4);
			_810mod[i][((j*10)/8)+2] = ((mod3 & 0x0F) << 4) + ((mod4 & 0x1E) >> 1);
			_810mod[i][((j*10)/8)+3] = ((mod4 & 0x01) << 7) + ((mod5 & 0x1F) << 2) + ((mod6 & 0x18) >> 3);
			_810mod[i][((j*10)/8)+4] = ((mod6 & 0x07) << 5) + ((mod7 & 0x1F));
		}
	}
}

NEDCLIB_API int dpi_multiplier = 1;
NEDCLIB_API int smooth = 0;
int fill=0;

void draw_dcs_pixel(int x, int y)
{
	int i,j;
	
	switch(dpi_multiplier)
	{
	case 1:			//300 DPI
		dcsbmp[x][y] = 1;
		break;
	case 2:			//600 DPI
		if(fill!=0)
		{
			for(i=0;i<2;i++)
				for(j=0;j<2;j++)
					dcsbmp[(x*2)+i][(y*2)+j] = 1;
		}
		else
		{
			dcsbmp[(x*2)][(y*2)] = 1;
		}
		break;
	case 4:			//1200 DPI
		if(fill!=0)
		{
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					dcsbmp[(x*4)+i][(y*4)+j] = 1;
		}
		else
		{
			for(i=1;i<3;i++)
				for(j=1;j<3;j++)
					dcsbmp[(x*4)+i][(y*4)+j] = 1;
		}
		break;
	case 8:			//2400 DPI
		if(fill!=0)
		{
			for(i=0;i<8;i++)
				for(j=0;j<8;j++)
					dcsbmp[(x*8)+i][(y*8)+j] = 1;
		}
		else
		{
			for(i=2;i<5;i++)
			{
				dcsbmp[(x*8)+i][(y*8)+1] = 1;
				dcsbmp[(x*8)+i][(y*8)+5] = 1;
			}
			for(i=1;i<6;i++)
				for(j=2;j<5;j++)
					dcsbmp[(x*8)+i][(y*8)+j] = 1;
		}
		break;
	}
}

void draw_sync_marker(int x, int y)
{
	int xx, yy;
	int i,j,k, l, m;

	xx=(x*dpi_multiplier);
	yy=(y*dpi_multiplier);
	l=(5*dpi_multiplier);

	if((dpi_multiplier==1)||(smooth==0))
	{
		fill = 1;
		for(i=1;i<4;i++)
			for(j=1;j<4;j++)
			{
				draw_dcs_pixel(x,y+j);
				draw_dcs_pixel(x+4,y+j);
				draw_dcs_pixel(x+i,y+j);
				draw_dcs_pixel(x+i,y);
				draw_dcs_pixel(x+i,y+4);
			}
		fill = 0;
	}
	else
	{
		k = (l/2)*(l/2);
		for(i=(l/2);i>=0;i--)
			for(j=(l/2);j>=0;j--)
			{
				m=((i*i)+(j*j))<=k ? 1 : 0;
				if(((i+j)==(l/2))&&((i==0)||(j==0)))
					m=0;
				dcsbmp[xx+((l/2)-1)+i][yy+((l/2)-1)+j] = m;
				dcsbmp[xx+((l/2)-1)+i][yy+((l/2)-1)-j] = m;
				dcsbmp[xx+((l/2)-1)-i][yy+((l/2)-1)-j] = m;
				dcsbmp[xx+((l/2)-1)-i][yy+((l/2)-1)+j] = m;
				//dcsbmp[xx+((l/2)-1)][yy+((l/2)-1)] = 1;
			}
	}
}

void draw_address_bar(int dotcodeblock, int address)
{
	int i,j=dotcodeblock;
	draw_dcs_pixel((j*35)+4,9);			//Render the T pixel
	draw_dcs_pixel(((j+1)*35)+4,9);		//The B pixel is not rendered.

	calc_addr(j+address);			//Calculate Error info for Address.

	for (i=0;i<16;i++)	//Render the A pixels and E pixels.  (A = Address, E = Error correction info.)
	{
		if(((((0x0001 << (i)) & addr[0]) >> (i)))==1)
		{
			draw_dcs_pixel((j*35)+4,33-i);
		}
		if(((((0x0001 << (i)) & addr[1]) >> (i)))==1)
		{
			draw_dcs_pixel(((j+1)*35)+4,33-i);
		}
	}
}


void init_dcs(void)
{
	/* Purpose - Draw the bmp template for Dotcodes. */
	/*
		 XXX                                XXX
		YYYYY                              YYYYY
		YYYYY   Z Z Z Z Z Z  Z Z Z Z Z Z   YYYYY
		YYYYY                              YYYYY
		 XXX                                XXX
		  

		  T                                  T
		  A                                  A
		  A                                  A
		  .                                  .
		  .                                  .
		  .                                  .
		  A                                  A
		  A                                  A
		  E                                  E
		  E                                  E
		  E                                  E
		  .                                  .
		  .                                  .
		  .                                  .
		  E                                  E
		  E                                  E
		  B                                  B


		 XXX                                XXX
		YYYYY                              YYYYY
		YYYYY   Z Z Z Z Z Z  Z Z Z Z Z Z   YYYYY
		YYYYY                              YYYYY
		 XXX                                XXX
	 */
	int i, j;

	int start_address;

	/*if(dotcodelen==28)
		start_address=25;
	else
		start_address=1;*/
	start_address=raw[1][1];
	
	for (j=0;j<dotcodelen;j++)
	{
		draw_sync_marker((j*35)+2,2);
		draw_sync_marker((j*35)+2,37);
		draw_sync_marker(((j+1)*35)+2,2);
		draw_sync_marker(((j+1)*35)+2,37);	//Render X/Y pixels.

		draw_address_bar(j,start_address);

		for (i=0;i<=5;i++) //Render the Z pixels. (
		{
			draw_dcs_pixel((j*35)+(10+(i*2)),4);
			draw_dcs_pixel((j*35)+(23+(i*2)),4);
			draw_dcs_pixel((j*35)+(10+(i*2)),39);
			draw_dcs_pixel((j*35)+(23+(i*2)),39);
		}
	}
}

void flipbmp(void)
{
	int i, j, width, height, temp;

	width=989 * dpi_multiplier;
	height=44 * dpi_multiplier;

	if(dotcodelen != 28)
		width = 639 * dpi_multiplier;
	
	for(i=0;i<(height/2);i++)
		for(j=0;j<width;j++)
		{
			temp = dcsbmp[j][i];
			dcsbmp[j][i] = dcsbmp[j][height-1-i];
			dcsbmp[j][height-1-i] = temp;
		}
	for(i=0;i<height;i++)
		for(j=0;j<(width/2);j++)
		{
			temp = dcsbmp[j][i];
			dcsbmp[j][i] = dcsbmp[width-1-j][i];
			dcsbmp[width-1-j][i] = temp;
		}
}


void makebmp(void)
{
	int i, j;

        int width=((dotcodelen*35)+9)*dpi_multiplier;
       // if(dotcodelen == 18)
       //         width=639*dpi_multiplier;
	
	/*if (bmplen == 0x7C)
	{*/

        for(i=0;i<352;i++)
                for(j=0;j<992;j++)
                        bmpdata[i][j] = 0;

        for(j=0;j<(44*dpi_multiplier);j++)
        {
                /*for (i=0;i<(bmplen*dpi_multiplier);i++)
                {
                        bmpdata[(44*dpi_multiplier)-1-j][i] = 0xFF;
                        bmpdata[(44*dpi_multiplier)-1-j][i] -= (dcsbmp[(i*8)+0][j]<<7)+(dcsbmp[(i*8)+1][j]<<6);
                        bmpdata[(44*dpi_multiplier)-1-j][i] -= (dcsbmp[(i*8)+2][j]<<5)+(dcsbmp[(i*8)+3][j]<<4);
                        bmpdata[(44*dpi_multiplier)-1-j][i] -= (dcsbmp[(i*8)+4][j]<<3)+(dcsbmp[(i*8)+5][j]<<2);
                        bmpdata[(44*dpi_multiplier)-1-j][i] -= (dcsbmp[(i*8)+6][j]<<1)+(dcsbmp[(i*8)+7][j]<<0);
                }*/
                for(i=0;i<width;i++)
                {
                        bmpdata[(44*dpi_multiplier)-1-j][i/8] += (1 << (7 - (i % 8)));
                        bmpdata[(44*dpi_multiplier)-1-j][i/8] -= (dcsbmp[i][j]<<(7 - (i % 8)));
                }
        }

}

void make_dcs(void)
{
	
	int i, j, k, count=0;
	fill=0;
	for(i=0;i<dotcodelen;i++)
	{
		for (j=0;j<3;j++)
			for (k=0;k<26;k++,count++)
			{
				if((_810mod[i][(int)(count/8)] >> (7 - (count % 8))) & 0x01)
					draw_dcs_pixel((i*35)+9+k,j+6);
				//dcsbmp[(i*35)+9+k][j+6] = (_810mod[i][(int)(count/8)] >> (7 - (count % 8))) & 0x01;
			}
		for (j=0;j<26;j++)
			for (k=0;k<34;k++,count++)
			{
				if((_810mod[i][(int)(count/8)] >> (7 - (count % 8))) & 0x01)
					draw_dcs_pixel((i*35)+5+k,j+9);
				//dcsbmp[(i*35)+5+k][j+9] = (_810mod[i][(int)(count/8)] >> (7 - (count % 8))) & 0x01;
			}
		for (j=0;j<3;j++)
			for (k=0;k<26;k++,count++)
			{
				//if(((_810mod[i][(int)(count/8)] >> (7 - (count % 8))))>0)
                                if((_810mod[i][(int)(count/8)] >> (7 - (count % 8))) & 0x01)
					draw_dcs_pixel((i*35)+9+k,j+35);
				//dcsbmp[(i*35)+9+k][j+35] = (_810mod[i][(int)(count/8)] >> (7 - (count % 8))) & 0x01;
			}

		count=0;
	}

}