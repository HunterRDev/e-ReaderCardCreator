// necmaker.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char carddata[0x81C];

unsigned char vpkdata[24564];

char title[13][33];

unsigned char cardheader[0x0C] = {
	0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
//  reg,  type,             numc, data size         flags
};

#define REGION_JAPAN 0
#define REGION_US 1
#define REGION_JAPANPLUS 2

#define APPTYPE_NES 0
#define APPTYPE_Z80 1
#define APPTYPE_GBA 2
#define APPTYPE_RAW 3

#define CARD_SHORT_NO 0
#define CARD_SHORT_YES 1
#define CARD_LONG_NO 2
#define CARD_LONG_YES 3

//Usage: necmaker [-jp, -us, -jep] [-Z80 -GBA -NES] [-stn, -sty, -ltn, -lty] [-long, -short] [-mus1, -mus2] -F {vpk file name} -B {output base name} [-T {Application Title}] [-T {Card Title/stats}]...
// {-jp, -us, -jep} - Region (default US)
// {-Z80 -GBA -NES} - Application Type (default Z80)
// {-stn, -sty, -ltn, -lty} - Application/Card Titling mode (default Long title, no individual names)
// {-long, -short} - Dotcode size; (default, long)
// {-mus1, -mus2} - Music to be played during card loading, if in short titling mode. (default music 1)
// {-save} - Allows the scanned dotcodes to be saved, (default disallowed)

void usage(int extended=0)
{

	printf("usage :\n");
	printf("  nedcmaker [options]\n");
	printf("options :\n");
	//     "  |                  |                           |-123456789-123456789-12345678"
	printf("  -i <file>          input file                  (required)\n");
	printf("  -o <file>          output name                 (optional)\n");
	printf("  -type <value>      type (0=nes 1=z80 2=gba)    (required)\n");
	printf("  -region <value>    region (0=jap 1=usa 2=jap+) (default = usa)\n");
	printf("  -name <string>     Application name            (default = none)\n");
	printf("  -title <string>    Individual Card Titles      (default = none)\n");
	printf("  -dcsize <value>    dcsize (0=long 1=short)     (default = long)\n");
	printf("  -fill <value>      fill (0=none 1=filled)      (default = none)\n");
	printf("  -save <value>      save (0=no 1=yes)           (default = no)\n");
	printf("  -titlemode <value> titlemode (0=short, no individual titles)\n");
	printf("                               (1=short, individual card titles)\n");
	printf("                               (2=long, no individual titles (default))\n");
	printf("                               (3=long, individual card titles)\n");
	printf("  -music <value>     music (0=Normal 1=cheery)   (default = normal)\n");
	printf("  -help (or -?)      Print extended usage info\n");

	if(extended)
	{
	printf("\n");
	printf("The music option is only valid for title modes 0 and 1\n\n");

	printf("If titlemode 1 is specified, you can add a pokemon serial# and hitpoints \n");
	printf("to the individual titles, in the form of [hitpoint]:X-YY-Z:[title]\n");
	printf("  [Hitpoint] = 0-150\n");
	printf("  X = A-Z,-,_\n");
	printf("  Y = 01-99,A0-A9,B0-B9,C0-C8\n");
	printf("  Z = A-G,#\n");
	printf("  [title] = Whatever you want here\n");
	printf("  For example: \"50:D-35-#:Aipom\"\n");
	printf("If your using this program in a makefile, please replace # with H, as # causes the\n");
	printf("rest of the line to be treated as a comment in a makefile.\n");
	printf("If titlemode is 3, then you only need to specify the [title] with each -title\n");
	printf("Title modes 0-1 do not have the english character set for regions 0 or 2.\n\n");

	printf("If you specify a region of 0, then Titlemode automatically becomes 1 or 3\n");
	printf("Also, the only valid application type for region 0 is z80\n\n");

	printf("If -o is not specified, the output files will be 01.bin - 12.bin\n");
	printf("Otherwise it will be <string>-01.bin - <string>-12.bin\n\n");


	printf("The Following max size tables assume your using apptype nes or z80.\n");
	printf("If your using apptype gba, then subtract 4 from each of these values\n\n");

	printf("Max size of Long dotcodes in order of title modes (US/Jap+)\n");
	printf("0=24564, 2=24372, 1=21540, 3=19620\n\n");

	printf("Max size of Short dotcodes in order of title modes (US/Jap+)\n");
	printf("0=15348, 2=15156, 1=12324, 3=10404\n\n");

	printf("Max size of Long dotcodes in order of title modes (Jap)\n");
	printf("1=%d, 3=%d\n\n", (((0x81C - 0x1D)-(0x15*(8)))*(8)),(((0x81C - 0x2D)-(0x21*(8)))*(8)));

	printf("Max size of Short dotcodes in order of title modes (Jap)\n");
	printf("1=%d, 3=%d\n\n", (((0x51C - 0x1D)-(0x15*(8)))*(8)),(((0x51C - 0x2D)-(0x21*(8)))*(8)));
	}

}

unsigned char jistable [] = 
{
	0x40, 0x49, 0x00, 0x94, 0x90, 0x93, 0x95, 0x66, 0x69, 0x6A,
	0x96, 0x7B, 0x43, 0x5D, 0x44, 0x5E, 0x4F, 0x50, 0x51, 0x52,
	0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x46, 0x47, 0x83, 0x81,
	0x84, 0x48, 0x97, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
	0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x6D,
	0x5F, 0x6E, 0x4F, 0x51, 0x4D, 0x81, 0x82, 0x83, 0x84, 0x85,
	0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99,
	0x9A, 0x6F, 0x62, 0x70, 0x60
} ;

void shiftJIS(char *str, char *output, int len)
{
	int i;
	for(i=0;i<(len/2);i++)
	{
		if (str[i] == 0)
			break;
		else if (((str[i] >= 'a') && (str[i] <= 'z')) || \
			((str[i] >= 'A') && (str[i] <= 'Z')) || \
			((str[i] >= '0') && (str[i] <= '9')) )
		{
			output[(i*2)+1] = jistable[str[i]-0x20];
			output[(i*2)] = (char)0x82;
		}
		else
		{
			output[(i*2)+1] = jistable[str[i]-0x20];
			output[(i*2)] = (char)0x81;
		}
	}
	output[(i*2)] = 0;
}

void process_title(char *str, int titlenum, int cardmode, int region)
{
	int i,j,k;
	char titlestr[8];
	char jistitle[34];

	memset(titlestr,0,8);
	memset(jistitle,0,34);
	
	if((titlenum==0)||(cardmode==CARD_LONG_NO)||(cardmode==CARD_LONG_YES))
	{
		if(region!=1)
		{
			shiftJIS(str,jistitle,33);
			for(i=0;i<33;i++)
			{
				title[titlenum][i] = jistitle[i];
				if(jistitle[i]==0)
					break;
			}
		}
		else
		{
			for(i=0;i<33;i++)
			{
				title[titlenum][i] = str[i];
				if(str[i]==0)
					break;
			}
		}
		if(i==33)
		{
			printf("Warning: Title \"%s\" will be truncated to \n\t",str);
			if(region != 1)
			{
				for(i=0;i<(33/2);i++)
				{
					if(str[i]==0)
						break;
					jistitle[i]=str[i];
				}
				jistitle[i]=0;
				printf("\"%s\"\n",jistitle);
			}
			else
			{
				for(i=0;i<33;i++)
					jistitle[i]=str[i];
				printf("\"%s\"\n",jistitle);
			}
		}
	}
	else
	{
		i = 0; j=0;
		while(str[i]!=':')
		{
			titlestr[j]=str[i];
			i++;j++;
		}
		titlestr[j] = 0;
		if(i!=0)
		{
			k=atoi(titlestr);
			title[titlenum][0] = (k / 10);
		}
		i++; j=0;
		while(str[i]!=':')
		{
			titlestr[j]=str[i];
			i++;j++;
		}
		if ((str[i] == ':') && (str[i-1] == ':'))
			title[titlenum][2] = (char)0x80;
		else
		{
			k = titlestr[0];
			if((k == '-'))
			{
				title[titlenum][1] = (char)0x80;
				title[titlenum][2] = 0x06;
			}
			else if ((k == '_'))
			{
				title[titlenum][1] = (char)0xC0;
				title[titlenum][2] = 0x06;
			}
			else
			{
				k -= 'A';
				title[titlenum][1] = k << 6;
				title[titlenum][2] = k >> 2;
			}
			j = titlestr[2];
			k = titlestr[3];
			if((j=='a')||(j=='b')||(j=='c'))
			{
				j -= 'a';
				j *= 10;
				j += 100;
			}
			else if((j=='A')||(j=='B')||(j=='C'))
			{
				j -= 'A';
				j *= 10;
				j += 100;
			}
			else
			{
				j -= '0';
				j *= 10;
			}
			k -= '0';
			j += k;
			j--;
			j &= 0x7F;

			title[titlenum][0] |= j << 7;
			title[titlenum][1] |= j >> 1;

			if((titlestr[5] == '#') || (titlestr[5] == 'H'))
				title[titlenum][0] |= 0x70;
			else
			{
				k = titlestr[5];
				if ((k - 'A') < 7)
					k -= 'A';
				else
					k -= 'a';
				k &= 0x07;
				title[titlenum][0] |= k << 4;
			}

			


		}
		i++;
		if(region!=1)
		{
			shiftJIS(str+i,jistitle,0x12);
			for(j=0;j<0x12;j++)
			{
				title[titlenum][j+3] = jistitle[j];
				if(jistitle[j]==0)
					break;
			}
			if(j==0x12)
			{
				printf("Warning: Title %s will be truncated to \n\t",str);
				for(j=0;j<(0x12/2);i++,j++)
					jistitle[j]=str[i];
				jistitle[j] = 0;
				printf("\"%s\"\n",jistitle);
			}
		}
		else
		{
			for(j=0;j<0x12;j++,i++)
			{
				title[titlenum][j+3] = str[i];
				if(str[i]==0)
					break;
			}
			if(j==0x12)
			{
				i-=0x12;
				printf("Warning: Title %s will be truncated to \n\t",str);
				for(j=0;j<0x12;i++,j++)
					jistitle[j]=str[i];
				printf("\"%s\"\n",jistitle);
			}
		}
			

	}
}

int main(int argc, char* argv[])
{
	int i,j,k;
	FILE *f;
	int region=REGION_US;
	int apptype=-1;
	int cardsize=0x81C;
	int headersize=0x2D;
	int settitlesize=33;
	int cardtitlesize=0;
	int allowsave=0;
	int mode=CARD_LONG_NO;
	int numtitles=1;
	int numcards;
	int filename;
	char fn[256];
	int filesize;
	int basename=0;
	int music=0;
	int fill=0;

	for(i=0;i<13;i++)
		for(j=0;j<33;j++)
			title[i][j]=0;
	


	printf("Nintendo E-Reader Dotcode bin maker tool\n");
	printf("Copyright CaitSith2\n\n");

	for(i=1;i<argc;i++)
	{

		if(!stricmp(argv[i],"-region"))
		{
			i++;
			region = atoi(argv[i]);
			continue;
		}
		if(!stricmp(argv[i],"-type"))
		{
			i++;
			apptype = atoi(argv[i]);
			continue;
		}
		if(!stricmp(argv[i],"-title"))
		{
			i++;
			process_title(argv[i],numtitles,mode,region);
			numtitles++;
			continue;
		}
		if(!stricmp(argv[i],"-name"))
		{
			i++;
			process_title(argv[i],0,mode,region);
			continue;
		}
		if(!stricmp(argv[i],"-o"))
		{
			i++;
			basename=i;
			continue;
		}
		if(!stricmp(argv[i],"-i"))
		{
			i++;
			filename=i;
			continue;
		}
		if(!stricmp(argv[i],"-fill"))
		{
			i++;
			fill = atoi(argv[i]);
			continue;
		}
		if(!stricmp(argv[i],"-music"))
		{
			i++;
			music=atoi(argv[i]);
			continue;
		}
		if(!stricmp(argv[i],"-titlemode"))
		{
			i++;
			mode=atoi(argv[i]);
			switch(mode)
			{
			case CARD_SHORT_NO:
				headersize=0x1D;
				settitlesize=17;
				cardtitlesize=0;
				if(region==REGION_JAPAN)
				{
					cardtitlesize=0x15;
					mode=CARD_SHORT_YES;
				}
				break;
			case CARD_SHORT_YES:
				headersize=0x1D;
				settitlesize=17;
				cardtitlesize=0x15;
				break;
			case CARD_LONG_NO:
			default:
				mode=CARD_LONG_NO;
				headersize=0x2D;
				settitlesize=33;
				cardtitlesize=0;
				if(region==REGION_JAPAN)
				{
					cardtitlesize=33;
					mode=CARD_LONG_YES;
				}
				break;
			case CARD_LONG_YES:
				headersize=0x2D;
				settitlesize=33;
				cardtitlesize=33;
				break;
			}
			continue;
		}
		if(!stricmp(argv[i],"-dcsize"))
		{
			i++;
			if(argv[i][0] == '0')
				cardsize=0x81C;
			else
				cardsize=0x51C;
			continue;
		}
		if(!stricmp(argv[i],"-save"))
		{
			i++;
			allowsave=atoi(argv[i]);
			continue;
		}

		if((!stricmp(argv[i],"-help"))||(!stricmp(argv[i],"-?")))
		{
			usage(1);
			return 1;
		}
		
		
	}
	if(argc<5)
	{
		usage();
		return 1;
	}
	if (filename==0) //Minimum required command line is -F {filename}.
	{
		printf("Required parameter missing: -i <file>\n");
		return 1;
	}
	if(apptype==-1)
	{
		printf("Required paramete missing: -type <value>\n");
		return 1;
	}

	
	f=fopen(argv[filename],"rb");
	if(f==NULL)
	{
		printf("Error: VPK file could not be opened for reading\n");
		//Could not open vpk file
		return 1;
	}
	fseek(f,0,SEEK_END);
	filesize=ftell(f);
	fseek(f,0,SEEK_SET);

		if(((filesize>24562)&&(apptype!=APPTYPE_GBA))||((filesize>24558)&&(apptype==APPTYPE_GBA)))
		{
			printf("Error: VPK data too large for even Title mode 0\n");
			//VPK file is too large for ereader card sets
			return 1;
		}


	
	if(apptype==APPTYPE_GBA)
	{
		fread(vpkdata+6,1,filesize,f);
		memcpy(vpkdata,&filesize,2);
		memset(vpkdata+2,0,4);
		filesize+=6;
	}
	else
	{
		fread(vpkdata+2,1,filesize,f);
		memcpy(vpkdata,&filesize,2);
		filesize+=2;
	}
	fclose(f);
	
	if(region!=0)
	{
		j = 12;
	}
	else
	{
		j = 8;
	}
	for(i=0;i<j;i++)
	{
	//	if(filesize<maxsizes[mode][i])
		if(filesize<(((cardsize-headersize)-(cardtitlesize*(i+1)))*(i+1)))
			break;
	}
	if(i==j)
	{
		printf("Error: VPK data %d bytes too large for selected options\n",filesize-(((cardsize-headersize)-(cardtitlesize*(j)))*(j)));
		printf("Max size for these options is %d bytes",(((cardsize-headersize)-(cardtitlesize*(j)))*(j)));
		printf("nedcmaker -help to see usage, including max size tables of \n");
		printf("various dotcode sizes/modes\n");
		//VPK data too large for card type selected
		return 1;
	}

	numcards=i+1;
	filesize *= 2;

	cardheader[0] += region;
	if(((mode&0x02)==0) && (music==0))
		cardheader[1] = 0x20;
	else if(((mode&0x02)==0) && (music==1))
		cardheader[1] = 0x40;
	else
		cardheader[1] = 0xE0;
	if(((mode&0x01)==0)&&(region!=REGION_JAPAN))
		cardheader[8] |= 0x02;
	if(apptype==APPTYPE_NES)
		cardheader[8] |= 0x04;
	if(allowsave)
		cardheader[8] |= 0x01;
	cardheader[4] = (numcards<<5);
	cardheader[5] = numcards>>3;
	cardheader[5] |= (filesize & 0xFF);
	cardheader[6] = filesize >> 8;

	if(region==0)
		cardheader[8] = 0;

	int cardoffset;

	int l = 0;
	for(i=1,cardoffset=0;i<=numcards;i++,cardoffset=0)
	{
		switch(fill)
		{
		case 0:
			memset(carddata,0,0x81C);
			break;
		case 1:
			for(j=0;j<0x81C;j++)
				carddata[j] = j & 0xFF;
			break;
		}
		cardheader[4] += 2;
		for(j=0;j<0x0C;j++)
			carddata[cardoffset++] = cardheader[j];
		
		for(j=0;j<settitlesize;j++)
		{
			carddata[cardoffset++] = title[0][j];
		}
		if((region==0)&&(settitlesize==0x11))
		{
			carddata[cardoffset-1] = 0;
			carddata[cardoffset-2] = 0;
			carddata[cardoffset-3] = 0;
			carddata[cardoffset-4] = 0;
		}
		
		for(j=1;j<=numcards;j++)
			for(k=0;k<cardtitlesize;k++)
				carddata[cardoffset++] = title[j][k];
		for(;(cardoffset<cardsize)&&(l<(filesize/2));cardoffset++,l++)
			carddata[cardoffset] = vpkdata[l];
		if(basename)
			sprintf(fn,"%s.%.2d.bin",argv[basename],i);
		else
			sprintf(fn,"%.2d.bin",i);
		f=fopen(fn,"wb");
		if(f==NULL)
		{
			printf("Unable to create Dotcode set\n");
			return 1;
		}
		fwrite(carddata,1,cardsize,f);
		fclose(f);




	}



	return 0;
}
