
#include "stdafx.h"
#include "../nedclib.h"
#include "rs.h"

unsigned char ShortDotCodeHeader[0x30] = {
	0x00, 0x30, 0x01, 0x01,
	0x00, 0x01, 0x05, 0x10,
	0x00, 0x00, 0x10, 0x12,	//Constant data

	0x00, 0x00,				//Header First 2 bytes

	0x02, 0x00,				//Constant data

	0x00, 0x00,				//Header Second 2 bytes

	0x10, 0x47, 0xEF,		//Global Checksum 1

	0x19, 0x00, 0x00, 0x00, 0x08, 0x4E, 0x49,
	0x4E, 0x54, 0x45, 0x4E, 0x44, 0x4F, 0x00, 0x22,
	0x00, 0x09,				//Constant data

				0x00, 0x00, //Header, last 8 bytes
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 
				0x00,		//Header Checksum
					  0x57  //Global Checksum 2
};

unsigned char LongDotCodeHeader[0x30] = {
	0x00, 0x30, 0x01, 0x02,
	0x00, 0x01, 0x08, 0x10,
	0x00, 0x00, 0x10, 0x12, //Constant Data

	0x00, 0x00,				//Header, first 2 bytes

	0x01, 0x00,				//Constant data

	0x00, 0x00,				//Header, second 2 bytes
	0x10, 0x9A, 0x99,		//Global Checksum 1

	0x19, 0x00, 0x00, 0x00, 0x08, 0x4E, 0x49,
	0x4E, 0x54, 0x45, 0x4E, 0x44, 0x4F, 0x00, 0x22,
	0x00, 0x09,				//Constant data


				0x00, 0x00, //Header, last 8 bytes
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 
				0x00,		//Header Checksum
					  0x57  //Global Checksum 2
};

unsigned char shortheader[0x18] = {
	0x00, 0x02, 0x00, 0x01, 0x40, 0x10, 0x00, 0x1C,
	0x10, 0x6F, 0x40, 0xDA, 0x39, 0x25, 0x8E, 0xE0,
	0x7B, 0xB5, 0x98, 0xB6, 0x5B, 0xCF, 0x7F, 0x72
};
unsigned char longheader[0x18] = {
	0x00, 0x03, 0x00, 0x19, 0x40, 0x10, 0x00, 0x2C,
	0x0E, 0x88, 0xED, 0x82, 0x50, 0x67, 0xFB, 0xD1,
	0x43, 0xEE, 0x03, 0xC6, 0xC6, 0x2B, 0x2C, 0x93
};

NEDCLIB_API int signature=0;
NEDCLIB_API unsigned char signature_str[0x30];

extern unsigned short dotcodelen;
unsigned char bin_header[24];

void raw_header(unsigned char *bindata, unsigned char *data, int size)
{
	int i,j,k;
	int global1, global2;

	for(i=0;i<0x30;i++)
		data[i]=((size==0x81c)?LongDotCodeHeader[i]:ShortDotCodeHeader[i]);
	

	for(i=0,j=0;i<12;i++)
		j ^= bindata[i];

	data[0x2E] = j;
	data[0x0D] = bindata[0];
	data[0x0C] = bindata[1];
	data[0x11] = bindata[2];
	data[0x10] = bindata[3];

	data[0x26] = bindata[4];
	data[0x27] = bindata[5];
	data[0x28] = bindata[6];
	data[0x29] = bindata[7];
	data[0x2A] = bindata[8];
	data[0x2B] = bindata[9];
	data[0x2C] = bindata[10];
	data[0x2D] = bindata[11];


	data[0x12] = 0x10;  //calculate Global Checksum 1
	data[0x02] = 1;  //Do not calculate Global Checksum 2

	for(i=0x0C,j=0;i<0x81C;i++)
	{
		if(i&1)
			j += bindata[i];
		else
			j += (bindata[i] << 8);
	}
	j &= 0xFFFF;
	j ^= 0xFFFF;
	data[0x13] = (j & 0xFF00) >> 8;
	data[0x14] = (j & 0x00FF);

	for(i=0,j=0;i<0x2F;i++)
		j+=data[i];
	j &= 0xFF;
	for(i=1,global2=0;i<0x2C;i++)
	{
		for(k=0,global1=0;k<0x30;k++)
		{
			global1 ^= bindata[((i-1)*0x30)+k+0x0C];
		}
		global2 += global1;
	}
	global2 += j;
	global2 &= 0xFF;
	global2 ^= 0xFF;
	data[0x2F] = global2;
}

void interleave_dotcode(unsigned char *data, unsigned char *interleave, int dotcodepointer, int size)
{
	int i;
	int dotcodeinterleave;

	if(bin_type==2)
	{
		dotcodeinterleave = bin_header[7];
		for(i=0;i<bin_header[4];i++)
			interleave[(i*dotcodeinterleave)+dotcodepointer] = data[i];
	}
	else
	{
		dotcodeinterleave = ((dotcodelen==28)?0x2C:0x1C);
		for(i=0;i<0x40;i++)
			interleave[(i*dotcodeinterleave)+dotcodepointer] = data[i];
	}

	
}

NEDCLIB_API int bin2raw(char *binfile, char *rawfile)
{
	unsigned char data[64];
	unsigned char bin[0x840];
	unsigned char dotcodetemp[0xB60];
	unsigned char raw[0xB60];
	int dotcodepointer=0;
	int dotcodeinterleave;
	
	int result;
	int i,j,k,l,count;
	int temp;
	int size;
	FILE *f, *g;

	initialize_rs();

	if(fopen_s(&f,binfile,"rb"))
		return -1; //Input file not opened.

	count=count_bin(f);
	for(l=0;l<count;l++)
	{
		size=read_next_bin(f,bin);
		if(bin_type == BIN_TYPE_NEDC_SINGLE)
			dotcodelen = ((size==0x81C) ? 28 : 18);
		if(bin_type == BIN_TYPE_NEDC_MULTI)
			dotcodelen = ((size==0x840) ? 28 : 18);


		for(i=0;i<(dotcodelen);i++)
			for(j=0;j<2;j++)
			{
				temp = ((i*2)+j)%0x18;
				raw[(i*0x68)+j] = ((dotcodelen==28)?longheader[temp]:shortheader[temp]);
			}
		if(bin_type==0)
		{
			raw_header(bin,data,size);
			
			append_error_info(data,0x40,0x10);
			interleave_dotcode(data,dotcodetemp,0,size);
		}

		dotcodeinterleave = ((dotcodelen==28)?0x2C:0x1C);

		if(bin_type==0)
		{
			for(i=1;i<dotcodeinterleave;i++)
			{
				for(j=0;j<0x30;j++)
					data[j]=bin[((i-1)*0x30)+0x0C+j];
				append_error_info(data,0x40,0x10);
				interleave_dotcode(data,dotcodetemp,i,size);
			}
		}
		else
		{
			for(i=0;i<dotcodeinterleave;i++)
			{
				for(j=0;j<0x30;j++)
					data[j]=bin[(i*0x30)+((bin_type==1)?0:8)+j];
				append_error_info(data,0x40,0x10);
				interleave_dotcode(data,dotcodetemp,i,size);
			}
		}

		j=((dotcodelen==28)?0xB38:0x724);
		k=((dotcodelen==28)?0xB60-j:0x750-j);

		dotcodepointer = 0;

		for(i=2;i<j;i++)
		{
			if((i%0x68)==0)
				i+=2;
			raw[i]=dotcodetemp[dotcodepointer++];
		}
		for(i=j;i<(j+k);i++)
		{
			if(signature)
				raw[i]=signature_str[i-j];
			else
				raw[i]=i&0xFF;
		}

		if(MultiStrip)
		{
			if(fopen_s(&g,rawfile,"rb+"))
				if(fopen_s(&g,rawfile,"wb"))
					return -4;
			fseek(g,0,SEEK_END);  //Append raw to end of existing raw file.
		}
		else
		{
			if(fopen_s(&g,rawfile,"wb"))
				return -4;
		}
		fwrite(raw,1,j+k,g);
		fclose(g);
	}
	
	free_rs();

	return 0;
}

NEDCLIB_API int bin2raw_d(unsigned char *bin, unsigned char *raw, int size)
{
	unsigned char data[64];
	unsigned char dotcodetemp[0xB60];
	int dotcodepointer=0;
	int dotcodeinterleave;
	
	int result;
	int i,j,k,l,count;
	int temp;
	FILE *f, *g;

	initialize_rs();

	switch(size)
	{
	case 0x840:
	case 0x540:
		bin_type = BIN_TYPE_NEDC_MULTI;
		break;
	case 0x81C:
	case 0x51C:
		bin_type = BIN_TYPE_NEDC_SINGLE;
		break;
	default:
		return -1;
	}

	if(bin_type == BIN_TYPE_NEDC_SINGLE)
		dotcodelen = ((size==0x81C) ? 28 : 18);
	if(bin_type == BIN_TYPE_NEDC_MULTI)
		dotcodelen = ((size==0x840) ? 28 : 18);


	for(i=0;i<(dotcodelen);i++)
		for(j=0;j<2;j++)
		{
			temp = ((i*2)+j)%0x18;
			raw[(i*0x68)+j] = ((dotcodelen==28)?longheader[temp]:shortheader[temp]);
		}
	if(bin_type==0)
	{
		raw_header(bin,data,size);
		append_error_info(data,0x40,0x10);
		interleave_dotcode(data,dotcodetemp,0,size);
	}

	dotcodeinterleave = ((dotcodelen==28)?0x2C:0x1C);

	if(bin_type==0)
	{
		for(i=1;i<dotcodeinterleave;i++)
		{
			for(j=0;j<0x30;j++)
				data[j]=bin[((i-1)*0x30)+0x0C+j];
			append_error_info(data,0x40,0x10);
			interleave_dotcode(data,dotcodetemp,i,size);
		}
	}
	else
	{
		for(i=0;i<dotcodeinterleave;i++)
		{
			for(j=0;j<0x30;j++)
				data[j]=bin[(i*0x30)+((bin_type==1)?0:8)+j];
			append_error_info(data,0x40,0x10);
			interleave_dotcode(data,dotcodetemp,i,size);
		}
	}

	j=((dotcodelen==28)?0xB38:0x724);
	k=((dotcodelen==28)?0xB60-j:0x750-j);

	dotcodepointer = 0;

	for(i=2;i<j;i++)
	{
		if((i%0x68)==0)
			i+=2;
		raw[i]=dotcodetemp[dotcodepointer++];
	}
	for(i=j;i<(j+k);i++)
	{
		if(signature)
			raw[i]=signature_str[i-j];
		else
			raw[i]=i&0xFF;
	}

	free_rs();

	return 0;
}

NEDCLIB_API int bin2raw_f(unsigned char *bin, char *rawfile, int size)
{
	unsigned char data[64];
	unsigned char dotcodetemp[0xB60];
	unsigned char raw[0xB60];
	int dotcodepointer=0;
	int dotcodeinterleave;
	
	int result;
	int i,j,k,l,count;
	int temp;
	FILE *f, *g;

	initialize_rs();

	switch(size)
	{
	case 0x840:
	case 0x540:
		bin_type = BIN_TYPE_NEDC_MULTI;
		break;
	case 0x81C:
	case 0x51C:
		bin_type = BIN_TYPE_NEDC_SINGLE;
		break;
	default:
		return -1;
	}

	if(bin_type == BIN_TYPE_NEDC_SINGLE)
		dotcodelen = ((size==0x81C) ? 28 : 18);
	if(bin_type == BIN_TYPE_NEDC_MULTI)
		dotcodelen = ((size==0x840) ? 28 : 18);


	for(i=0;i<(dotcodelen);i++)
		for(j=0;j<2;j++)
		{
			temp = ((i*2)+j)%0x18;
			raw[(i*0x68)+j] = ((dotcodelen==28)?longheader[temp]:shortheader[temp]);
		}
	if(bin_type==0)
	{
		raw_header(bin,data,size);
		append_error_info(data,0x40,0x10);
		interleave_dotcode(data,dotcodetemp,0,size);
	}

	dotcodeinterleave = ((dotcodelen==28)?0x2C:0x1C);

	if(bin_type==0)
	{
		for(i=1;i<dotcodeinterleave;i++)
		{
			for(j=0;j<0x30;j++)
				data[j]=bin[((i-1)*0x30)+0x0C+j];
			append_error_info(data,0x40,0x10);
			interleave_dotcode(data,dotcodetemp,i,size);
		}
	}
	else
	{
		for(i=0;i<dotcodeinterleave;i++)
		{
			for(j=0;j<0x30;j++)
				data[j]=bin[(i*0x30)+((bin_type==1)?0:8)+j];
			append_error_info(data,0x40,0x10);
			interleave_dotcode(data,dotcodetemp,i,size);
		}
	}

	j=((dotcodelen==28)?0xB38:0x724);
	k=((dotcodelen==28)?0xB60-j:0x750-j);

	dotcodepointer = 0;

	for(i=2;i<j;i++)
	{
		if((i%0x68)==0)
			i+=2;
		raw[i]=dotcodetemp[dotcodepointer++];
	}
	for(i=j;i<(j+k);i++)
	{
		if(signature)
			raw[i]=signature_str[i-j];
		else
			raw[i]=i&0xFF;
	}

	if(MultiStrip)
	{
		if(fopen_s(&g,rawfile,"rb+"))
			if(fopen_s(&g,rawfile,"wb"))
				return -4;
		fseek(g,0,SEEK_END);  //Append raw to end of existing raw file.
	}
	else
	{
		if(fopen_s(&g,rawfile,"wb"))
			return -4;
	}
	fwrite(raw,1,j+k,g);
	fclose(g);

	free_rs();

	return 0;
}



void deinterleave_dotcode(unsigned char *data, unsigned char *interleave, int dotcodepointer, int size, int dotcodeinterleave)
{
	for(int i=0;i<0x40;i++)
		data[i] = interleave[(i*dotcodeinterleave)+dotcodepointer];
}

NEDCLIB_API int raw2bin(char *rawfile, char *binfile)
{
	unsigned char data[64];
	unsigned char erasure[64] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	};
	char filename[256];

	unsigned char NEDC_STR[] = "NINTENDO";
	int multistrip_type;
	unsigned char bin_header[24];
	unsigned char bin[0x10008];
	unsigned char dotcodetemp[0x15C28];
	unsigned char raw[0x15C28];
	int dotcodepointer=0;
	int dotcodeinterleave;
	
	int result;
	int i,j,k;
	int size;
	int numraw,numbin=0;
	FILE *f, *g;

	initialize_rs();

	if(fopen_s(&f,rawfile,"rb"))
		return -1; //Input file not opened.
	numraw=count_raw(f);
	if(numraw==0)
		return -2; //Invalid raw file.
	for(k=0;k<numraw;k++)
	{
		size=read_next_raw(f,raw);
		//if((size!=0x750)&&(size!=0xB60))
		if(size==0)
			continue;
			//return -2; //Invalid raw nedc size.

		for(i=0;i<0x0C;i++)
			for(j=0;j<2;j++)
				bin_header[(i*2)+j]=raw[(i*0x68)+j];
		result=correct_errors(bin_header,24,16);
		if(result<0)
			continue;
			//return -3; //NEDC Read Error

		//j=((size==0xB60)?0xB38:0x724);
		j=(bin_header[4]*bin_header[7]*0x68);
		k=j%0x66;
		j/=0x66;
		if(k>0)
			j++;
		
		dotcodeinterleave=bin_header[7];
		dotcodepointer=0;

		for(i=2;i<j;i++)
		{
			if((i % 0x68) == 0) i+=2;
			dotcodetemp[dotcodepointer++]=raw[i];
		}

		deinterleave_dotcode(data,dotcodetemp,0,size,dotcodeinterleave);
		result=correct_errors(data,0x40,0x10);
		if(result<0)
			continue;
			//return -3; //NEDC Read Error
		for(i=0;i<8;i++)
			if(data[i+0x1A]!=NEDC_STR[i])
			{
				MultiStrip = 1;
				multistrip_type = 1;
				break;
			}
		if(i==8)
		{
			multistrip_type = 0;
		}
		else
		{
			continue;
			//return -2		//Not an NEDC dotcode.
		}

		
			
		for(i=0;i<0x30;i++)
			bin[i]=data[i];

		for(i=1;i<dotcodeinterleave;i++)
		{
			deinterleave_dotcode(data,dotcodetemp,i,size,dotcodeinterleave);
			result=correct_errors(data,0x40,0x10);
			if(result<0)
				break;
				//return -3; //RS_econding failed
			for(j=0;j<0x30;j++)
				bin[(i*0x30)+j]=data[j];
		}
		if(result<0)
			continue;



		if((numraw>1)&&(MultiStrip==0))
			sprintf_s(filename,255,"%s-%.2d.bin",binfile,k+1);
		else
			sprintf_s(filename,255,"%s",binfile);

		if(MultiStrip)
		{
			if(fopen_s(&g,filename,"rb+"))
				if(fopen_s(&g,filename,"wb"))
					return -4;
			if(!count_bin(g))
			{
				fseek(g,0,SEEK_SET);
				bin_type=1;
			}
			else
			{
				fseek(g,0,SEEK_END);
			}
		}
		else
		{
			bin_type=0;
			if(fopen_s(&g,filename,"wb"))
				return -4;
		}
		//fwrite(raw,1,j+k,g);
		//fclose(g);
		if(g!=NULL)
		{
			//return -4;
			switch(bin_type)
			{
			case 0:		//Will not covert to this old style format.
			case 1:		//All 48 bytes of the header is now included
						//from now on.
				fwrite(bin,1,((size==0xB60)?0x840:0x540),g);
				break;
			case 2:
				fwrite(bin_header,1,8,g);
				fwrite(bin,1,((bin_header[4]-bin_header[5])*bin_header[7]),g);
				break;
			}
			
			fclose(g);
			numbin++;
		}
	}
	
	free_rs();
	
	return (numbin>0)?0:-1;
}

NEDCLIB_API int fixraw(char *rawfile)
{
	unsigned char data[255];

	unsigned char dotcodetemp[0xB60];
	unsigned char raw[0xB60];
	int dotcodepointer=0;
	int dotcodeinterleave;

	int num_dotcodes;
	
	int result;
	int i,j,k;
	int x,y;

	int temp;
	int size;
	FILE *f;

	initialize_rs();

	if(fopen_s(&f,rawfile,"rb+"))
		return -1; //Input file not opened.
	num_dotcodes = count_raw(f);

	for(k=0;k<num_dotcodes;k++)
	{
		size=read_next_raw(f,raw);
		dotcodelen = (size==0xB60) ? 28 : 18;

		//if((size!=0x750)&&(size!=0xB60))
		if(size==0)
			return close_raw(f,-2); //Invalid raw nedc size.

		k=0;
		do
		{
			for(i=k;(i<(k+12))&&(i<dotcodelen);i++)
				for(j=0;j<2;j++)
					data[((i-k)*2)+j]=raw[((i-k)*0x68)+j];
			result=correct_errors(data,0x18,0x10);
			k+=12;
		} while ((result < 0) && (i != dotcodelen));
		if(result<0)
			return close_raw(f,-3); //NEDC Read Error
		for(i=0;i<24;i++)
			bin_header[i]=data[i];
		x=bin_header[4];
		y=bin_header[5];

		for(i=0;i<dotcodelen;i++)
			for(j=0;j<2;j++)
			{
				temp = ((i*2)+j)%0x18;
				raw[(i*0x68)+j] = data[temp];
			}

		j=(bin_header[4]*bin_header[7]*0x68);
		k=j%0x66;
		j/=0x66;
		if(k>0)
			j++;
		dotcodeinterleave=bin_header[7];
		dotcodepointer=0;

		for(i=2;i<j;i++)
		{
			if((i % 0x68) == 0) i+=2;
			dotcodetemp[dotcodepointer++]=raw[i];
		}

		deinterleave_dotcode(data,dotcodetemp,0,size,dotcodeinterleave);
		result=correct_errors(data,x,y);
		if(result<0)
			return close_raw(f,-3); //NEDC Read Error

		interleave_dotcode(data,dotcodetemp,0,size);

		
		for(i=1;i<dotcodeinterleave;i++)
		{
			deinterleave_dotcode(data,dotcodetemp,i,size,dotcodeinterleave);
			result=correct_errors(data,x,y);
			if(result<0)
				return close_raw(f,-3); //RS_econding failed
			interleave_dotcode(data,dotcodetemp,i,size);
		}

		dotcodepointer = 0;

		for(i=2;i<j;i++)
		{
			if((i%0x68)==0)
				i+=2;
			raw[i]=dotcodetemp[dotcodepointer++];
		}
		backtrack_raw(f);
		fwrite(raw,1,size,f);
	}
	fclose(f);
	free_rs();
	return 0;

}
