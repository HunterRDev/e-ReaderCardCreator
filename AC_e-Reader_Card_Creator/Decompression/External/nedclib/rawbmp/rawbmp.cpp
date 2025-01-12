
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>


#include "../nedclib.h"

#include "dcs.h" //RAW2BMP, BMP2RAW

void write_bmp(FILE *f)
{
	int i,j;
	int length,width;
	length=((dotcodelen*35)+9)*dpi_multiplier;
	width=44*dpi_multiplier;

	i=length;
	bmpheader[0x12] = i & 0xFF;
	bmpheader[0x13] = i >> 8;
	bmpheader[0x14] = i >> 16;
	bmpheader[0x15] = i >> 24;
	bmpheader[0x16] = width & 0xFF;
	bmpheader[0x17] = (width >> 8) & 0xFF;
	bmpheader[0x18] = (width >> 16) & 0xFF;
	bmpheader[0x19] = (width >> 24) & 0xFF;
	i/=32;
	if((length%32)>0)
		i++;
	i*=4;
	i*=(44*dpi_multiplier);
	bmpheader[2] = (i + 0x3E) & 0xFF;
	bmpheader[3] = ((i + 0x3E) >> 8) & 0xFF;
	bmpheader[4] = ((i + 0x3E) >> 16) & 0xFF;
	bmpheader[5] = ((i + 0x3E) >> 24) & 0xFF;
	bmpheader[0x22] = i & 0xFF;
	bmpheader[0x23] = (i >> 8) & 0xFF;
	bmpheader[0x24] = (i >> 16) & 0xFF;
	bmpheader[0x24] = (i >> 24) & 0xFF;

	i=0x2E23*dpi_multiplier;
	bmpheader[0x26] = (i >> 0) & 0xFF;
	bmpheader[0x27] = (i >> 8) & 0xFF;
	bmpheader[0x28] = (i >> 16) & 0xFF;
	bmpheader[0x29] = (i >> 24) & 0xFF;
	bmpheader[0x2A] = (i >> 0) & 0xFF;
	bmpheader[0x2B] = (i >> 8) & 0xFF;
	bmpheader[0x2C] = (i >> 16) & 0xFF;
	bmpheader[0x2D] = (i >> 24) & 0xFF;
	

	fwrite(bmpheader,1,62,f);
	for(j=0;j<width;j++)
		for(i=0;i<(bmplen*dpi_multiplier);i++)
			fputc(bmpdata[j][i],f);

}

void clear_dcs(void)
{
	int i, j;
	for (i=0;i<7912;i++)
		for (j=0;j<352;j++)
			dcsbmp[i][j] = 0;
	for (i=0;i<28;i++)
		for(j=0;j<130;j++)
			_810mod[i][j]=0;
}

NEDCLIB_API int raw2bmp(char *rawfile, char *bmpfile)
{
	int i,j;
	FILE *f, *g;
	char filename[256];

	if(fopen_s(&f,rawfile,"rb"))
	{
		return 1;
	}

	int num_raw=count_raw(f);
		for(i=0;i<num_raw;i++)
		{
			//switch(read_next_raw(f,&raw[0][0]))
			j=read_next_raw(f,&raw[0][0]);
			dotcodelen = j/0x68;
			bmplen=(dotcodelen*35)+9;
			if((bmplen%32)>0)
				bmplen+=(32-(bmplen%32));
			bmplen/=32;
			bmplen*=4;
			/*
			{
			case 0xB60:
				dotcodelen = 28;
				bmplen = 0x7C;
				break;
			case 0x750:
				dotcodelen = 18;
				bmplen = 0x50;
				break;
			}*/
			clear_dcs();
			init_dcs();
			eight_ten_modulate();
			make_dcs();
			//flipbmp();
			makebmp();
			if(num_raw==1)
				sprintf_s(filename,255,"%s.bmp",bmpfile);
			else
				sprintf_s(filename,255,"%s-%.2d.bmp",bmpfile,i+1);
			if(!fopen_s(&g,filename,"wb"))
			{
				write_bmp(g);
				fclose(g);
			}
		}

	if(f!=NULL)
	{
		fclose(f);
	}

	return 0;
}

NEDCLIB_API int raw2bmp_f(unsigned char *rawdata, char *bmpfile)
{
	int i;
	FILE *g;
	char filename[256];

	memcpy(&raw[0][0],rawdata,0xB60);	

	//f=fopen(rawfile,"rb");
	//if(f==NULL)
	//{
	//	return 1;
	//}

	int num_raw=1;
		for(i=0;i<num_raw;i++)
		{
			switch(raw[0][1])
			{
			case 3:
				dotcodelen = 28;
				bmplen = 0x7C;
				break;
			case 2:
				dotcodelen = 18;
				bmplen = 0x50;
				break;
			}
			clear_dcs();
			init_dcs();
			eight_ten_modulate();
			make_dcs();
			//flipbmp();
			makebmp();
			if(num_raw==1)
				sprintf_s(filename,255,"%s.bmp",bmpfile);
			else
				sprintf_s(filename,255,"%s-%.2d.bmp",bmpfile,i+1);
			if(fopen_s(&g,filename,"wb"))
			{
				write_bmp(g);
				fclose(g);
			}
		}

	//if(f!=NULL)
	//{
	//	fclose(f);
	//}

	return 0;
}

NEDCLIB_API int bmp2raw(char *bmpfile, char *rawfile)
{
	FILE *f;
	int filelen;
	if(fopen_s(&f,bmpfile,"rb"))
		return 1;
	if(!read_bmp(f))
	{
		printf("%s is not a Dotcode bmp file\n",bmpfile);
		return 1;
	}
	fclose(f);
	clear_dcs();
	reversebmp();
	reverse_dcs();
	eight_ten_demodulate();

	if(MultiStrip)
	{
		if(fopen_s(&f,rawfile,"rb+"))
			if(fopen_s(&f,rawfile,"wb"))
				return 1;
		fseek(f,0,SEEK_END);  //Append raw to end of existing raw file.
	}
	else
	{
		if(fopen_s(&f,rawfile,"wb"))
			return 1;
	}
	if(dotcodelen == 28)
	{
		fwrite(raw,1,0xB60,f);
	}
	else
	{
		fwrite(raw,1,0x750,f);
	}
	fclose(f);
	return 0;
}