#include "stdafx.h"

#define DCS
#include "dcs.h"

void calc_addr(int address)
{
	int start;

	int mask,bits;

	/* Thanks to Martin Korth, The method to calculate the error
    correction info on the address bars is now known. As a result
    it is now possible to calculate what should be in the long and
    short dotcodes rather than have a defined table. */

	/* This is an Address bar calculator, for "Dot Code Technology",
	which is made by Olympus Optical Co. Ltd.  The address bars consist
	of a 14 bit address, and 10 bits of reed-solomon error correction
	information.  The first valid address bar as far as I know, is 1.

	On a dotcode strip,  there is 30 dots in between the top and bottom
	sync marker, that the address bar is contained in.  Top 2 dots are dead
	space, and should be always white.  Next dot down, is black, to indicate
	that this is the top of the dotcode strip.
	Next 24 dots is the address and reed-solomon error correction info.
	Next Dot is white, to indicate this is bottom of the dotcode strip
	Last 2 dots are dead space, and should be white, for proper sync marker
	recognition. */

	/* The reed-solomon error correction on the address bar, is capable of
	correcting up to 2 bits of error in the address bar. */

	/* Nintendo e-Reader dotcodes use Addresss 1 (on left) to 19 (on right)
	for short dotcodes, and address 25 (on left) to 53 (on right) for long
	dotcodes.  Any other address ranges will cause the dotcode to be rejected
	by the Nintendo e-Reader.  The upper address limit that is decoded for
	is 60 (61 on right hand side.)  Anything higher is not decoded by nintendo
	e-Reader. */

	//addr[0] = 0x3FF;
	if((addr[0] >> 10) < address)
	{
		start = (addr[1] >> 10) + 1;
	}
	if(((addr[0] >> 10) == address)&&(address!=0))
		return;	//No need to calculate anything.
	if(((addr[0] >> 10) > address) || (addr[1] == 0x3FF))
	{
		//Must recalculate the address starting from 0.
		start = 1;
		addr[1] = 0x3FF;
	}

    for(int i=start,base=0x769;i<=(address+1);i++,base=0x769)
    {
		addr[0] = addr[1];
        addr[1] = addr[0] ^ ((i & (-i)) * base);
		for(mask=0x1FFF,bits=0x651;bits>0;mask>>=1,bits>>=1)
		{
			if((i & mask)==0)
			{
				if(bits & 1)
					addr[1] ^= base;
				base <<= 1;
			}
		} 
    }
}

/* ---- ADDRESS BAR REED SOLOMON VERIFICATION ---- */

int addr_init=0;

unsigned char Alpha_to[32];
unsigned char Index_of[32];

unsigned char multiply_table[2][24] = {
	{ 	
		0x0F, 0x15, 0x18, 0x0C, 0x06, 0x03, 0x13, 0x1B,
		0x1F, 0x1D, 0x1C, 0x0E, 0x07, 0x11, 0x1A, 0x0D,
		0x14, 0x0A, 0x05, 0x10, 0x08, 0x04, 0x02, 0x01 
	},
	{	
		0x14, 0x10, 0x02, 0x09, 0x17, 0x0F, 0x0C, 0x13,
		0x1D, 0x07, 0x0D, 0x05, 0x04, 0x12, 0x0B, 0x1E,
		0x18, 0x03, 0x1F, 0x0E, 0x1A, 0x0A, 0x08, 0x01 
	}
};

unsigned char syndrome[2];

void init_tables()
{
	int i,j;
	Alpha_to[0x1F] = 0;
	Index_of[0x00] = 0x1F;
	for(i=0,j=1;i<0x1E;i++)
	{
		Alpha_to[i]=j;
		Index_of[j]=i;
		j<<=1;
		if(j>0x1F)
			j^=0x25;
	}
}


int generate_syndrome(unsigned long addr_data)
{
	int i,j,error=0;
	unsigned long address = addr_data ^ 0x3FF;
	for(i=0;i<2;i++)
	{
		syndrome[i] = 0;
		for(j=0;j<24;j++)
		{
			if(address & (1 << (23-j)))
			{
				syndrome[i] ^= multiply_table[i][j];
			}
		}
		syndrome[i] = Index_of[syndrome[i]];
		if(syndrome[i] != 0x1F)
			error=1;
	}
	return error;
}

int rs_add(unsigned char num1, unsigned char num2)
{
	int i,j;
	if((num1==0x1F)||(num2==0x1F))
		return 0x1F;
	i=num1+num2;
	j=i-0x1F;
	if(i>0x1E)
		return j;
	else
		return i;
}

int rs_multiply(int num1, int num2)
{
	int i,j;
	for(i=0,j=0;i<num2;i++)
		j=rs_add(num1,j);
	return j;
}

int rs_sub(int num1, int num2)
{
	int i,j;
	if((num1==0x1F)||(num2==0x1F))
		return 0x1F;
	i=num1-num2;
	j=i+0x1F;
	if(i<0)
		return j;
	else
		return i;
}

int get_index(int num1, int num2)
{
	return Index_of[Alpha_to[num1]^Alpha_to[num2]];
}

unsigned char rsdata[24];
unsigned char location[2];

int find_location(int numbits, int numloops)
{
	int h,i,j,k,l,m,n=0;

	for(h=numbits-1;h>=0;h--)
	{
		m=Alpha_to[rsdata[0]];
		for(i=1;i<=numloops;i++)
		{
			k=rsdata[i];
			j=rs_multiply(h,i);
			j=rs_add(k,j);
			l=Alpha_to[j];
			m^=l;
		}
		if(m==0)
		{
			location[n++] = h;
			if(n==numloops)
				return 0;
		}
	}
	return -1;
}

int correct_address(unsigned long *addr_data)
{
	int i,j;
	int num_loops=2;
	if(addr_init==0)
		init_tables();
	generate_syndrome(addr_data[0]);
	unsigned long address = addr_data[0] ^ 0x3FF;
	for(i=0,j=0;i<num_loops;i++)
	{
		if(syndrome[i] == 0x1F)
			j++;
	}
	if(j==num_loops)
		return 0;	//No need to correct errors.
	return 3; //Don't try to correct the errors.  These bmps should be correct in this aspect.

	if(syndrome[0] == 0x1F)
		return 3;	//Uncorrectable errors detected.
	j=rs_multiply(syndrome[0],num_loops+1);

	if(j!=syndrome[1])
	{
		j=get_index(j,syndrome[1]);
		j=rs_sub(j,syndrome[0]);
		rsdata[0]=j;
		rsdata[2]=0;
		rsdata[1]=syndrome[0];
		if(find_location(24,num_loops))
			return 3; //Uncorrectable errors detected.
	}
	else
	{
		location[0]=syndrome[0];
		num_loops = 1;
	}
	
	for(i=0;i<num_loops;i++)
	{
		j=location[i];
		j=(1<<j);
		address^=j;
	}
	addr_data[0] = address ^ 0x3FF;
	generate_syndrome(addr_data[0]);
	if(syndrome[0]!=0x1F)
		return 3;
	if(syndrome[1]!=0x1F)
		return 3;
	return 1;  //Correction successful
					
}