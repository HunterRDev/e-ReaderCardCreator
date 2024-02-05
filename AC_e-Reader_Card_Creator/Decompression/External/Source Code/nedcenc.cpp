// nedcenc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>

#include "../nedclib/nedclib.h"


int main(int argc, char* argv[])
{
	int i,j,k,temp,hexinput;

	int encode=0,decode=0,fix=0;
	int infile=0,outfile=0;

	printf("Nintendo eReader Dotcode encoder/decoder v%d.%d\n",NEDCENC_MAJOR,NEDCENC_MINOR);
	printf("Copyright 2007 CaitSith2\n\n");
	nedclib_version();

	if(argc<3)
	{
		printf("Usage: %s [options] [-i] infile [[-o] outfile]\n\n",argv[0]);
		printf("[options]\n");
		printf("\t-i\tIn file (required)\n");
		printf("\t-o\tOut file (required)\n");

		printf("\t-e\tEncode bin 2 raw, Outfile required (Default operation)\n");
		printf("\t-d\tDecode raw 2 bin. Outfile required\n");
		printf("\t-f\tRepair raw file\n");
		printf("\t-s\tDot code signature (encoding only)\n\t\t< - Input hex.\n\t\t<< - ");
		printf("Input <\n\t\t> - If inputting hex, end hex input, otherwise input >");
		return 1;
	}

	for(i=1;i<argc;i++)
	{
		if(argv[i][0]!='-')
			continue;
		switch(argv[i][1])
		{
		case 'e':
			encode=1;
			break;
		case 'd':
			decode=1;
			break;
		case 'f':
			fix=1;
			break;
		case 's':
			signature=i+1;
			i++;
			break;
		case 'i':
			infile=i+1;
			i++;
			break;
		case 'o':
			outfile=i+1;
			i++;
			break;
		}
	}
	if((encode+decode+fix)==0)
	{
		encode=1;
	}
	if((encode+decode+fix)>1)
	{
		printf("You cannot specify more than one operation\n");
		return 1;
	}

	if(signature)
	{
		i=0;
		j=0;
		k=0;
		temp=0;
		hexinput=0;
		while((argv[signature][i])&&(j<0x2C))
		{
			switch(argv[signature][i])
			{
			case '<':
				if(hexinput)
				{
					hexinput = 0;
					signature_str[j++]=argv[signature][i];
					break;
				}
				hexinput=1;
				break;
			case '>':
				if(hexinput)
				{
					hexinput = 0;
					break;
				}
				signature_str[j++]=argv[signature][i];
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(hexinput)
				{
					if(k)
					{
						temp += (argv[signature][i]-'0');
						signature_str[j++] = temp;
						k=0;
					}
					else
					{
						temp = ((argv[signature][i]-'0')<<4);
						k++;
					}
					break;
				}
				signature_str[j++]=argv[signature][i];
				break;
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
				if(hexinput)
				{
					if(k)
					{
						temp += (argv[signature][i]-'a')+10;
						signature_str[j++] = temp;
						k=0;
					}
					else
					{
						temp = (((argv[signature][i]-'a')+10)<<4);
						k++;
					}
					break;
				}
				signature_str[j++]=argv[signature][i];
				break;
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				if(hexinput)
				{
					if(k)
					{
						temp += (argv[signature][i]-'A')+10;
						signature_str[j++] = temp;
						k=0;
					}
					else
					{
						temp = (((argv[signature][i]-'A')+10)<<4);
						k++;
					}
					break;
				}
				signature_str[j++]=argv[signature][i];
				break;
			default:
				if(hexinput)
					break;
				signature_str[j++]=argv[signature][i];
				break;	
			}
			i++;
		}
	}

	if(encode)
		i=bin2raw(argv[infile],argv[outfile]);
	if(decode)
		i=raw2bin(argv[infile],argv[outfile]);
	if(fix)
		i=fixraw(argv[infile]);

	switch(i)
	{
	case 0:
		if(encode)
			printf("bin successfully encoded to raw\n");
		if(decode)
			printf("raw successfully decoded to bin\n");
		if(fix)
			printf("dotcode raw successfully repaired\n");
		break;
	case -1:
		printf("Unable to open input file %s\n",argv[infile]);
		break;
	case -2:
		if(encode)
			printf("Invalid bin file\n");
		if(decode)
			printf("Invalid raw file\n");
		if(fix)
			printf("Invalid raw file\n");
		break;
	case -3:
		if(encode)
			printf("Unable to encode bin file to raw\n");
		if(decode)
			printf("Unable to decode raw file to bin\n");
		if(fix)
			printf("Unable to repair raw file\n");
		break;
	case -4:
		printf("Unable to open output file %s\n",argv[outfile]);
		break;
	default:
		printf("Unknown error\n");
	}
	if(i!=0)
		return i;
	

	
	

	return 0;
}

