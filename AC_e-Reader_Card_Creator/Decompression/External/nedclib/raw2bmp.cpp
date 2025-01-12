// raw2bmp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../nedclib/nedclib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int InFileList[256],OutFileList[256];

void usage (void)
{
	printf("Usage :\n  DcsTool [options]\n");
	printf("Options :\n");
	printf("  -i <file>\t\tInput File\t\t\t(Required)\n");
	printf("  -o <file>\t\tOutput File\t\t\t(Required)\n");
	printf("  -dpi <dpi>\t\tDPI Setting\t\t\t(Optional, Default 300)\n");
	printf("  -MultiStrip\t\tMultistrip raw file mode\t\t(Optional)\n");
	printf("\n");
}

int main(int argc, char* argv[])
{
	int i,j;
	int OptI=0;
	int OptO=0;
	int OptD=0;
	int OptR=0;

	printf("Nintendo e-Reader dotcode strip tool Version %d.%d\n",RAW2BMP_MAJOR,RAW2BMP_MINOR);
	printf("Copyrighted by CaitSith2\n\n");
	nedclib_version();

	for (i=1;i<argc;i++)
	{
		if(!_stricmp(argv[i],"-i"))
		{
			if((i+1)==argc) continue;
			i++;
			while(argv[i][0] != '-')
			{
				InFileList[OptI++] = i;
				i++;
				if((i)==argc) break;
			}
			i--;
			continue;
		}
		if(!_stricmp(argv[i],"-o"))
		{
			if((i+1)==argc) continue;
			i++;
			while(argv[i][0] != '-')
			{
				OutFileList[OptO++] = i;
				i++;
				if((i)==argc) break;
			}
			i--;
			continue;
		}
		if(!_stricmp(argv[i],"-dpi"))
		{
			if((i+1)==argc) continue;
			i++;
			switch(atoi(argv[i]))
			{
			case 300:
			case 360:
			default:
				dpi_multiplier = 1;
				break;
			case 600:
			case 720:
				dpi_multiplier = 2;
				break;
			case 1200:
			case 1440:
				dpi_multiplier = 4;
				break;
			case 2400:
			case 2880:
				dpi_multiplier = 8;
				break;
			}
			continue;
		}
		if(!_stricmp(argv[i],"-multistrip"))
		{
			MultiStrip = 1;
			continue;
		}
		if(!_stricmp(argv[i],"-smooth"))
		{
			smooth = 1;
			continue;
		}
	}
	
	if((OptI < 1) || (OptO < 1) || ((OptI != OptO) && (MultiStrip == 0)))
	{
		usage();
		return 1;
	}

	for(i=0,j=0;(i<OptI)&&(j<OptO);i++)
	{
		if(is_bmp(argv[InFileList[i]]))
		{
			if(MultiStrip)
				if(is_bmp(argv[OutFileList[j]]))
				{
					i--;
					j++;
					continue;
				}
			if((i==0)&&(MultiStrip))
			{
				MultiStrip = 0;
				if(bmp2raw(argv[InFileList[i]],argv[OutFileList[j]]))
				{
					printf("Error converting %s to %s\n",argv[InFileList[i]],argv[OutFileList[j]]);
				}
				MultiStrip = 1;
			}
			else
			{
				if(bmp2raw(argv[InFileList[i]],argv[OutFileList[j]]))
				{
					printf("Error converting %s to %s\n",argv[InFileList[i]],argv[OutFileList[j]]);
				}
			}
			if(!MultiStrip)
				j++;
		}
		else
		{
			if(MultiStrip)
				j++;
			if(raw2bmp(argv[InFileList[i]],argv[OutFileList[j]]))
			{
				printf("Error converting %s to %s\n",argv[InFileList[i]],argv[OutFileList[j]]);
			}
			
		}
	}

	return 0;
}

