#include "stdafx.h"
#include "nedclib.h"

char DMCA_data[0x19] = "\0\0DMCA NINTENDO E-READER";

NEDCLIB_API unsigned short nes_dec(unsigned short NMI_vector)
{
	unsigned short NMI = NMI_vector;
	int i,j;

	for(i=0;i<0x18;i++)
	{
		NMI ^= (DMCA_data[i] << 8);
		for(j=0;j<8;j++)
		{
			if(NMI & 0x8000)
			{
				NMI <<= 1;
				NMI ^= 0x0C8D;
			}
			else
			{
				NMI <<= 1;
			}
		}
	}

	return NMI;
}

NEDCLIB_API unsigned short nes_enc(unsigned short NMI_vector)
{
	unsigned short NMI = NMI_vector;
	int i,j;

	for(i=0;i<0x18;i++)
	{
		for(j=0;j<8;j++)
		{
			if(NMI & 0x0001)
			{
				NMI >>= 1;
				NMI ^= 0x8646;
			}
			else
			{
				NMI >>= 1;
			}
		}
		NMI ^= (DMCA_data[0x17-i] << 8);
	}

	return NMI;
}

NEDCLIB_API int make_nes(unsigned char *nesdata)
{
	unsigned short nmi;
	if((nesdata[0]!='N')||(nesdata[1]!='E')||(nesdata[2]!='S')||(nesdata[3]!=0x1A))
		return 1;
	if((nesdata[4] != 1) || (nesdata[5] != 1) || ((nesdata[6] & 0xFE) != 0) || (nesdata[7] != 0))
		return 2;

	nmi = (nesdata[0x3FFB+16] << 8) + (nesdata[0x3FFA+16]);
	nmi = nes_enc(nmi);
	nesdata[0x3FFB+16] = (nmi >> 8) & 0xFF;
	nesdata[0x3FFA+16] = nmi & 0xFF;

	if(nesdata[6] & 1)
		nesdata[0x3FFD + 16] &= 0x7F;
	else
		nesdata[0x3FFD + 16] |= 0x80;

	return 0;

}