#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#define DCS
#include "dcs.h"

#include "../nedclib.h"

unsigned char mod0,mod1,mod2,mod3,mod4,mod5,mod6,mod7;
unsigned short dotcodelen;
unsigned char bmpdata1[44][0x7C];
unsigned char bmpdata2[44][0x50];

int read_bmp(FILE *f)
{
	int filelen,i,j;
	unsigned short k;
	int x,xx,y;
	int bmp_start;
	fseek(f,0,SEEK_END);
	filelen = ftell(f);
	fseek(f,0,SEEK_SET);
	fread(&i,1,2,f);
	if((i&0xFFFF)!=0x4D42)
		return 0;	//Not a bmp file
	fread(&i,1,4,f);
	if(i!=filelen)
		return 0;	//Not a bmp file
	fread(&i,1,4,f);
	if(i!=0)
		return 0;	//Not a bmp file
	fread(&bmp_start,1,4,f);

	fseek(f,4,SEEK_CUR);
	fread(&x,1,4,f);
	fread(&y,1,4,f);
	fseek(f,2,SEEK_CUR);
	fread(&k,1,2,f);
	if(k!=1)
		return 0;	//Not a dotcode bmp
	fread(&i,1,4,f);
	if(i!=0)
		return 0;	//Compressed bmp not supported.

	fseek(f,0x36,SEEK_SET);
	fread(&bmp_invert,1,4,f);


	switch(y)
	{
	case 44:
		dpi_multiplier = 1;
		break;
	case 88:
		dpi_multiplier = 2;
		break;
	case 176:
		dpi_multiplier = 4;
		break;
	case 352:
		dpi_multiplier = 8;
		break;
	default:
		return 0;  //Not a dotcode bmp.
	}


	fseek(f,0x36,SEEK_SET);
	fread(&bmp_invert,1,4,f);
	fseek(f,62,SEEK_SET);

	for(i=0;i<y;i++)
	{
		for(j=0,xx=x;xx>0;j+=4,xx-=32)
		{
			fread(&bmpdata[i][j],1,4,f);
		}
	}
	bmplen = j;
	for(dotcodelen=0,xx=x-5;xx>(35*dpi_multiplier);dotcodelen++,xx-=(35*dpi_multiplier));

	/*if (filelen == 0x158E)
	{
		fseek(f,62,SEEK_SET);
		fread(bmpdata1,0x7C,44,f);
		dotcodelen = 28;
		bmplen = 0x7C;
	}
	else
	{
		fseek(f,62,SEEK_SET);
		fread(bmpdata2,0x50,44,f);
		dotcodelen = 18;
		bmplen = 0x50;
	}*/

	return 1;	//Dotcode bmp read successfully
}

int read_dcs_pixel(int x, int y, int source=1)
{
	int xx,yy;
	switch (dpi_multiplier)
	{
	case 1:
		xx = x;
		yy = y;
		break;
	case 2:
		xx = x * 2;
		yy = y * 2;
		break;
	case 4:
		xx = (x * 4) + 1;
		yy = (y * 4) + 1;
		break;
	case 8:
		xx = (x * 8) + 2;
		yy = (y * 8) + 2;
		break;
	}
	switch (source)
	{
	case 0:		//From bmpdata
		return (bmpdata[yy][xx/8] & (1 << (7 - (xx%8)))) >> (7-(xx%8));
		break;
	case 1:		//From dcsbmp
	default:
			return dcsbmp[xx][yy];
	}

}

unsigned char bitflip(unsigned char byte)
{
	int i,j=0;
	for(i=0;i<5;i++)
	{
		if(byte&(1<<i))
			j|=0x10>>i;
	}
	return j;
}

void invertmod(void)
{
	mod7^=0x1F;
	mod6^=0x1F;
	mod5^=0x1F;
	mod4^=0x1F;
	mod3^=0x1F;
	mod2^=0x1F;
	mod1^=0x1F;
	mod0^=0x1F;
}

void read_reverse(int ii, int jj, int inverse)
{
	int i=(dotcodelen-1-ii);
	int j=(104-4-jj);
	mod0 = _810mod[i][((j*10)/8)+4] & 0x1F;
	mod1 = (_810mod[i][((j*10)/8)+4] >> 5) & 0x07;
	mod1 += (_810mod[i][((j*10)/8)+3] << 3) & 0x18;
	mod2 = (_810mod[i][((j*10)/8)+3] >> 2) & 0x1F;
	mod3 = (_810mod[i][((j*10)/8)+3] >> 7) & 0x01;
	mod3 += (_810mod[i][((j*10)/8)+2] << 1) & 0x1E;
	mod4 = (_810mod[i][((j*10)/8)+2] >> 4) & 0x0F;
	mod4 += (_810mod[i][((j*10)/8)+1] << 4) & 0x10;
	mod5 = (_810mod[i][((j*10)/8)+1] >> 1) & 0x1F;
	mod6 = (_810mod[i][((j*10)/8)+1] >> 6) & 0x03;
	mod6 += (_810mod[i][((j*10)/8)+0] << 2) & 0x1C;
	mod7 = (_810mod[i][((j*10)/8)+0] >> 3) & 0x1F;
	mod0=bitflip(mod0);
	mod1=bitflip(mod1);
	mod2=bitflip(mod2);
	mod3=bitflip(mod3);
	mod4=bitflip(mod4);
	mod5=bitflip(mod5);
	mod6=bitflip(mod6);
	mod7=bitflip(mod7);

	if(inverse)
		invertmod();
}
void read_forward(int i, int j, int inverse)
{
	mod7 = _810mod[i][((j*10)/8)+4] & 0x1F;
	mod6 = (_810mod[i][((j*10)/8)+4] >> 5) & 0x07;
	mod6 += (_810mod[i][((j*10)/8)+3] << 3) & 0x18;
	mod5 = (_810mod[i][((j*10)/8)+3] >> 2) & 0x1F;
	mod4 = (_810mod[i][((j*10)/8)+3] >> 7) & 0x01;
	mod4 += (_810mod[i][((j*10)/8)+2] << 1) & 0x1E;
	mod3 = (_810mod[i][((j*10)/8)+2] >> 4) & 0x0F;
	mod3 += (_810mod[i][((j*10)/8)+1] << 4) & 0x10;
	mod2 = (_810mod[i][((j*10)/8)+1] >> 1) & 0x1F;
	mod1 = (_810mod[i][((j*10)/8)+1] >> 6) & 0x03;
	mod1 += (_810mod[i][((j*10)/8)+0] << 2) & 0x1C;
	mod0 = (_810mod[i][((j*10)/8)+0] >> 3) & 0x1F;

	if(inverse)
		invertmod();
}

int checkmod(void)
{
	read_forward(0,0,0);
	if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x12))
	{
		read_forward(1,0,0);
		if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x01)&&(mod3==0x09))
		{
			return 0;
		}
	}
	if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x02))
	{
		read_forward(1,0,0);
		if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x01))
		{
			return 0;
		}
	}
	read_forward(0,0,1);
	if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x12))
	{
		read_forward(1,0,1);
		if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x01)&&(mod3==0x09))
		{
			return 1;
		}
	}
	if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x02))
	{
		read_forward(1,0,1);
		if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x01))
		{
			return 1;
		}
	}
	read_reverse(0,0,0);
	if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x12))
	{
		read_reverse(1,0,0);
		if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x01)&&(mod3==0x09))
		{
			return 2;
		}
	}
	if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x02))
	{
		read_reverse(1,0,0);
		if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x01))
		{
			return 2;
		}
	}
	read_reverse(0,0,1);
	if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x12))
	{
		read_reverse(1,0,1);
		if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x01)&&(mod3==0x09))
		{
			return 3;
		}
	}
	if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x02))
	{
		read_reverse(1,0,1);
		if((mod0==0x00)&&(mod1==0x00)&&(mod2==0x00)&&(mod3==0x01))
		{
			return 3;
		}
	}

	return -1;

}


void eight_ten_demodulate(void)
{
	int i, j;
	unsigned char raw0,raw1,raw2,raw3;
	int modtype=0;

	modtype=checkmod();
	if(modtype==-1)
	{
		printf("This is not a nintendo e-reader dotcode\n");
		exit(1);
	}
	for (i=0;i<dotcodelen;i++)
	{
		for(j=0;j<104;j+=4)
		{
			//switch(modtype)
			//{
			//case 0:
				read_forward(i,j,0);
			/*	break;
			case 1:
				read_forward(i,j,1);
				break;
			case 2:
				read_reverse(i,j,0);
				break;
			case 3:
				read_reverse(i,j,1);
				break;
			}*/

			raw3 = (demodtable[mod7] + (demodtable[mod6] << 4));
			raw2 = (demodtable[mod5] + (demodtable[mod4] << 4));
			raw1 = (demodtable[mod3] + (demodtable[mod2] << 4));
			raw0 = (demodtable[mod1] + (demodtable[mod0] << 4));

			raw[i][j+3] = raw3;
			raw[i][j+2] = raw2;
			raw[i][j+1] = raw1;
			raw[i][j+0] = raw0;
		}
	}
}
			

void reversebmp(void)
{	
	int i, j, bmp_flip=1;
	int length,mask,width;

	length=(dotcodelen*35)+9;
	width=44*dpi_multiplier;

	if(!bmp_invert)
	{
		/*if(bmplen == 0x7C)
			bmp_flip = ((bmpdata1[9][0] & 0x08)>>3)==0;
		else
			bmp_flip = ((bmpdata2[9][0] & 0x08)>>3)==0;*/
		bmp_flip = (read_dcs_pixel(4,9,0)==0);
	}
	else
	{
		/*if(bmplen == 0x7C)
			bmp_flip = ((bmpdata1[9][0] & 0x08)>>3)==1;
		else
			bmp_flip = ((bmpdata2[9][0] & 0x08)>>3)==1;*/
		bmp_flip = (read_dcs_pixel(4,9,0)==1);
	}

	//if (bmplen == 0x7C)
	//{
		for (i=0;i<bmplen;i++)
		{
			for(j=0;j<width;j++)
			{
				bmpdata[((44*dpi_multiplier)-1)-j][i] ^= ((bmp_invert == 0) ? 0xFF : 0);
				dcsbmp[(i*8)+0][j] = (bmpdata[(width-1)-j][i] >> 7) & 0x01;
				dcsbmp[(i*8)+1][j] = (bmpdata[(width-1)-j][i] >> 6) & 0x01;
				dcsbmp[(i*8)+2][j] = (bmpdata[(width-1)-j][i] >> 5) & 0x01;
				dcsbmp[(i*8)+3][j] = (bmpdata[(width-1)-j][i] >> 4) & 0x01;
				dcsbmp[(i*8)+4][j] = (bmpdata[(width-1)-j][i] >> 3) & 0x01;
				dcsbmp[(i*8)+5][j] = (bmpdata[(width-1)-j][i] >> 2) & 0x01;
				dcsbmp[(i*8)+6][j] = (bmpdata[(width-1)-j][i] >> 1) & 0x01;
				dcsbmp[(i*8)+7][j] = (bmpdata[(width-1)-j][i] >> 0) & 0x01;
			}
		}
			
		if (bmp_flip)
		{
			for (i=0;i<width/2;i++)
			{
				for(j=0;j<length;j++)
				{
					dcsbmp[j][i] ^= dcsbmp[length-1-j][width-1-i];
					dcsbmp[length-1-j][width-1-i] ^= dcsbmp[j][i];
					dcsbmp[j][i] ^= dcsbmp[length-1-j][width-1-i];
				}
			}
		}
				
	/*}
	if (bmplen == 0x50)
	{
		for (i=0;i<bmplen;i++)
		{
			for(j=0;j<44;j++)
			{
				if (i==0x4F)
				{
					bmpdata2[43-j][i] ^= ((bmp_invert == 0) ? 0xFE : 0);
					dcsbmp[(i*8)+0][j] = (bmpdata2[43-j][i] >> 7) & 0x01;
					dcsbmp[(i*8)+1][j] = (bmpdata2[43-j][i] >> 6) & 0x01;
					dcsbmp[(i*8)+2][j] = (bmpdata2[43-j][i] >> 5) & 0x01;
					dcsbmp[(i*8)+3][j] = (bmpdata2[43-j][i] >> 4) & 0x01;
					dcsbmp[(i*8)+4][j] = (bmpdata2[43-j][i] >> 3) & 0x01;
					dcsbmp[(i*8)+5][j] = (bmpdata2[43-j][i] >> 2) & 0x01;
					dcsbmp[(i*8)+6][j] = (bmpdata2[43-j][i] >> 1) & 0x01;
				}
				else
				{
					bmpdata2[43-j][i] ^= ((bmp_invert == 0) ? 0xFF : 0);
					dcsbmp[(i*8)+0][j] = (bmpdata2[43-j][i] >> 7) & 0x01;
					dcsbmp[(i*8)+1][j] = (bmpdata2[43-j][i] >> 6) & 0x01;
					dcsbmp[(i*8)+2][j] = (bmpdata2[43-j][i] >> 5) & 0x01;
					dcsbmp[(i*8)+3][j] = (bmpdata2[43-j][i] >> 4) & 0x01;
					dcsbmp[(i*8)+4][j] = (bmpdata2[43-j][i] >> 3) & 0x01;
					dcsbmp[(i*8)+5][j] = (bmpdata2[43-j][i] >> 2) & 0x01;
					dcsbmp[(i*8)+6][j] = (bmpdata2[43-j][i] >> 1) & 0x01;
					dcsbmp[(i*8)+7][j] = (bmpdata2[43-j][i] >> 0) & 0x01;
				}
			}
		}
		if (bmp_flip)
		{
			for (i=0;i<44/2;i++)
			{
				for(j=0;j<639;j++)
				{
					dcsbmp[j][i] ^= dcsbmp[639-1-j][44-1-i];
					dcsbmp[639-1-j][44-1-i] ^= dcsbmp[j][i];
					dcsbmp[j][i] ^= dcsbmp[639-1-j][44-1-i];
				}
			}
		}
				
	}*/

}

void reverse_dcs(void)
{
	int i, j, k, count=0;
	for(i=0;i<dotcodelen;i++)
	{
		for (j=0;j<3;j++)
			for (k=0;k<26;k++,count++)
				_810mod[i][(int)(count/8)] += (read_dcs_pixel((i*35)+9+k,j+6) << (7 - (count % 8)));
				//_810mod[i][(int)(count/8)] += (dcsbmp[(i*35)+9+k][j+6] << (7 - (count % 8)));
		for (j=0;j<26;j++)
			for (k=0;k<34;k++,count++)
				_810mod[i][(int)(count/8)] += (read_dcs_pixel((i*35)+5+k,j+9) << (7 - (count % 8)));
				//_810mod[i][(int)(count/8)] += (dcsbmp[(i*35)+5+k][j+9] << (7 - (count % 8)));
		for (j=0;j<3;j++)
			for (k=0;k<26;k++,count++)
				_810mod[i][(int)(count/8)] += (read_dcs_pixel((i*35)+9+k,j+35) << (7 - (count % 8)));
				//_810mod[i][(int)(count/8)] += (dcsbmp[(i*35)+9+k][j+35] << (7 - (count % 8)));
		

		count=0;
	}
}
