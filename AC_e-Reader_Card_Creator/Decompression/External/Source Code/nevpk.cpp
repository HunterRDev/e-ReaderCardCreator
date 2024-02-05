

#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "../nedclib/nedclib.h"


// If compiling on VS 2005, comment out fopen_s and vsprintf_s completely.
// It was easier to rewrite these fuctions rather than rewrite the code
// to be compilable on older VS versions.

//int fopen_s(FILE ** f, char *name, char *spec)
//{
//	if(f==NULL)
//		return 1;
//	*f=fopen(name,spec);
//	if(*f==NULL)
//		return 1;
//	else
//		return 0;
//	
//}
//
//int vsprintf_s(char *string, int sizeinbytes, const char *format, va_list ap)
//{
//	
//	vsprintf(string,format,ap);
//	return 0;
//}

//////////////////////////////////////////////////////////////////////



void usage(void)
{
	printf("usage :\n");
	printf("  nvpktool [options]\n");
	printf("options :\n");
	printf("  -i <file>            input file                              (Required)\n");
	printf("  -o <file>            output file                             (Required)\n");
	printf("  -v                   verbose                                 (Optional)\n");
	printf("  -c                   compress                                (Required *)\n");
	printf("  -d                   decompress                              (Required *)\n");
	printf("  -level <value>       compression level (0=store 1=med 2=max  (Default = 2)\n");
	printf("                       3 = Discover best lzwindow/lzsize/method)\n");
	printf("  -log <file>          Decompression log                       (Default = none)\n");
	printf("  (following options are only valid for compression levels 1 & 2)\n");
	printf("  -method <value>      compression method (0 or 1)             (Default = 0)\n");
	printf("  -lzwindow <value>    lz window size                          (Default = 4096)\n");
	printf("  -lzsize <value>      lz max repeat size                      (Default = 256)\n");
}

int main(int argc, char* argv[])
{
	int i,j,k;
	unsigned char *vpk_buf;
	FILE *f;

	int file_in=0;
	int file_out=0;
	int operation=0;
	int level=2;
	int method=0;
	int lzwindow=4096;
	int lzsize=256;
	unsigned long bitsize = 0xFFFFFFFF;

	printf("Nintendo e-Reader VPK Tool Version %d.%d\n",NEVPK_MAJOR,NEVPK_MINOR);
	printf("Copyright CaitSith2\n\n");
	nedclib_version();

	if(argc==1)
	{
		usage();
		return 1;
	}

	for(i=1;i<argc;i++)
	{
		if(!_stricmp(argv[i],"-i"))
		{
			i++;
			file_in=i;
			continue;
		}
		if(!_stricmp(argv[i],"-o"))
		{
			i++;
			file_out=i;
			continue;
		}
		if(!_stricmp(argv[i],"-c"))
		{
			if(operation == 2)
			{
				printf("You cannot use -c and -d at the same time\n");
				return 1;
			}
			operation = 1;
			continue;
		}
		if(!_stricmp(argv[i],"-d"))
		{
			if(operation == 1)
			{
				printf("You cannot use -c and -d at the same time\n");
				return 1;
			}
			operation = 2;
			continue;
		}
		if(!_stricmp(argv[i],"-level"))
		{
			level=strtoul(argv[++i],0,0);
			if(level>3)
			{
				printf("Max compression level is 3\n");
				return 1;
			}
			continue;
		}
		if(!_stricmp(argv[i],"-method"))
		{
			method=strtoul(argv[++i],0,0);
			if(method>1)
			{
				printf("Invalid compression method\n");
				return 1;
			}
			continue;
		}
		if(!_stricmp(argv[i],"-lzwindow"))
		{
			lzwindow=strtoul(argv[++i],0,0);
			/*if(lzwindow > 32768)
			{
				printf("Max lz window is 32768\n");
				return 1;
			}*/
			continue;
		}
		if(!_stricmp(argv[i],"-lzsize"))
		{
			lzsize=strtoul(argv[++i],0,0);
			/*if(lzsize> 32768)
			{
				printf("Max lz size is 32768\n");
				return 1;
			}*/
			continue;
		}
		if(!_stricmp(argv[i],"-verbose"))
		{
			verbose = 1;
		}

		if(!_stricmp(argv[i],"-log"))
		{
			i++;
			if(fopen_s(&log,argv[i],"w")!=0)
			{
				printf("Failed to open log file\n");
				log=NULL;
			}
		}

	}

	if(file_in == 0)
	{
		printf("Required parameter -i missing\n");
		return 1;
	}
	if(file_out == 0)
	{
		printf("Required parameter -o missing\n");
		return 1;
	}
	if(operation==0)
	{
		printf("-c or -d (not both) required\n");
		return 1;
	}

	/*
	in=stream_open(argv[file_in],"rb");
	out=stream_open(argv[file_out],"wb");
	stream_seek(in,0,SEEK_END);
	i=stream_tell(in);
	stream_seek(in,0,SEEK_SET);
	lz77_encode(out,in,i,9);
	stream_close(in);
	stream_close(out);
	in=stream_open(argv[file_out],"rb");
	out=stream_open(argv[file_in],"wb");
	stream_seek(in,0,SEEK_END);
	i=stream_tell(in);
	stream_seek(in,0,SEEK_SET);
	lz77_decode(out,in,i);
	stream_close(in);
	stream_close(out);
	*/
	//return 0;
	
	log_write("Input file: %s\n",argv[file_in]);
	log_write("Output file: %s\n",argv[file_out]);
	log_write("Operation: %s\n",(operation==1)?"Compress":"Decompress");
	if(operation == 1)
	{
		log_write("Compression Level: %d\n",level);
			if(level > 0)
		{
			if(level != 3)
			{
				log_write("Compression Method: %d\n",method);
				log_write("LZ Window: %d\n",lzwindow);
				log_write("LZ Size: %d\n",lzsize);
			}
		}
	}
	log_write("\n");
	

	//f=;
	if(fopen_s(&f,argv[file_in],"rb")!=0)
	{
		printf("Unable to open input file %s\n",argv[file_in]);
		return 1;
	}
	fseek(f,0,SEEK_END);
	i = ftell(f);
	fseek(f,0,SEEK_SET);
	vpk_buf = (unsigned char *)malloc(i);
	if(vpk_buf==NULL)
	{
		printf("Not enough memory to allocate compression buffer\n");
		return 1;
	}
	fread(vpk_buf,1,i,f);
	fclose(f);
	//f=;

	int b_move = 0;
	int b_size = 0;

	if(operation == 1)
	{
		if(level==3)
		{
			int result;
			for(j=16;j<65536;j<<=1)
			{
				for(k=16;k<65536;k<<=1)
				{
					//best_move=0x80000000;
					//best_size=0x80000000;
					//skip_huffman=1;
					/*for(l=1;l<(65536*2);l+=2)
					{
						if((l>j)&&(l<65536))
						{
							l=65535;
							continue;
						}
						if((l>(65536+k)))
						{
							break;
						}
						if(l==65536) continue;
						if(l<65536)
						{
							best_move = l;
							best_size = 0;
						}
						else
						{
							best_move = b_move;
							best_size = l - 65536;
						}*/
						printf("Filesize: %d, lzwindow: %d, lzsize: %d          ",((bitsize==0xFFFFFFFF)?0:bitsize/8),k,j);
						printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
						printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
						printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
						result=NVPK_compress(vpk_buf,i,level,k,j,method,NULL);
						if(result==0)
						{
							if(bitsize>bits_written)
							{
								lzwindow=k;
								lzsize=j;
								bitsize=bits_written;
								b_move=best_move;
								b_size=best_size;
							}
						}
						else if (result==3)  //Window size is invalid, break out of the 
						{					 //Size loop, and try next window size.
							break;
						}
						else if (result==4)  //Repeat size is invalid, try next repeat size.
						{
							continue;
						}
						else if (result==2)  //Buffer overrun error. (Data execution prevention)
						{
							continue;
						}
					//}
				}
			}
			if(bitsize==0xFFFFFFFF)
			{
				printf("Compression failed\n");
				return 1;
			}
			skip_huffman=0;
			NVPK_compress(vpk_buf,i,level,lzwindow,lzsize,method,NULL);

			if(bitsize>bits_written)
			{
				bitsize=bits_written;
			}

			//skip_size=1;
			skip_lz77=1;
			for(j=1;j<lzwindow;j+=2)
			{
				for(k=1;k<lzsize;k+=2)
				{
					printf("Filesize: %d, %d, %d                                     ",((bitsize==0xFFFFFFFF)?0:bitsize/8),(lzwindow-1-j),(lzsize-1-k));
							printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
							printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
							printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
					best_move=j;
					best_size=k;
					result=NVPK_compress(vpk_buf,i,level,lzwindow,lzsize,method,NULL);
					if(result==0)
					{
						if(bitsize>bits_written)
						{
							b_size=best_size;
							b_move=best_move;
							bitsize=bits_written;
						}
					}
				}
			}
			/*best_move=b_move;
			skip_size=0;
			for(j=1;j<lzsize;j++)
			{
				printf("Filesize: %d, %d                                               ",((bitsize==0xFFFFFFFF)?0:bitsize/8),(lzsize-1-j));
						printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
						printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
						printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
				best_size=j;
				result=NVPK_compress(vpk_buf,i,level,lzwindow,lzsize,method,NULL);
				if(result==0)
				{
					if(bitsize>bits_written)
					{
						b_size=best_size;
						bitsize=bits_written;
					}
				}
			}*/
			best_size=b_size;
			best_move=b_move;
			best_size=b_size;
			log_write("Compression Method: %d\n",method);
			log_write("LZ Window: %d\n",lzwindow);
			log_write("LZ Size: %d\n",lzsize);
		}
		if(fopen_s(&f,argv[file_out],"wb")!=0)
		{
			printf("Unable to open output file %s\n",argv[file_out]);
			return 1;
		}
		i=NVPK_compress(vpk_buf,i,level,lzwindow,lzsize,method,f);
	}
	else
	{	
		if(fopen_s(&f,argv[file_out],"wb")!=0)
		{
			printf("Unable to open output file %s\n",argv[file_out]);
			return 1;
		}
		i=vpk_decompress(vpk_buf,f);
	}
	fclose(f);
	if(log!=NULL)
		fclose(log);

	j=i;

	free(vpk_buf);
	
	return 0;
}
