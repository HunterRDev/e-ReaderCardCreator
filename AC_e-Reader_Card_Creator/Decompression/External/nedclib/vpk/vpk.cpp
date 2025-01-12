
#include "stdafx.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "../nedclib.h"

NEDCLIB_API FILE *log_stream;
NEDCLIB_API int verbose=0;

unsigned char *bitstore=NULL;

#define MAX_LOG_STR 256
#ifdef  __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
void log_only_write(const char* str, ...)
{
	char tmpstr[MAX_LOG_STR];
	va_list args;
	va_start(args,str);
	vsprintf_s(tmpstr,MAX_LOG_STR-1,str,args);
	va_end(args);
	if(log_stream!=NULL)
		fprintf(log_stream,tmpstr);
}

#ifdef  __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
NEDCLIB_API void log_write(const char* str, ...)
{
	char tmpstr[MAX_LOG_STR];
	va_list args;
	va_start(args,str);
	vsprintf_s(tmpstr,MAX_LOG_STR-1,str,args);
	va_end(args);
	if(verbose==1)
		printf(tmpstr);
	if(log_stream!=NULL)
		fprintf(log_stream,tmpstr);
}

struct tree_node {
	int node;
	int data;
	int count;
	int value;
	tree_node *look_up[32];
	tree_node *left;
	tree_node *right;
};

struct tree_node *movetree;
struct tree_node *sizetree;

int get_bitsize(unsigned long data)
{

	int i=data;

	int size=0;
	while(i > 0)
	{
		size++;
		i >>= 1;
	}
	return size;

}

int bitsleft_w = 32;
int bitsleft_r = 0;
unsigned long bits_w=0;
unsigned long bits_r=0;

NEDCLIB_API unsigned long bits_written=0;


int bits_buf=0;

inline unsigned long read_bits(int count, unsigned char *buf)
{
	int i;
	unsigned long tmp=0;

	for(i=0;i<count;i++)
	{
		if(bitsleft_r==0)
		{
			bits_r = (buf[bits_buf++] << 24);
			bits_r |= (buf[bits_buf++] << 16);
			bits_r |= (buf[bits_buf++] << 8);
			bits_r |= (buf[bits_buf++]);
			bitsleft_r = 32;
		}
		bitsleft_r--;
		tmp |= (((bits_r >> (bitsleft_r)) & 1) << (count-1-i));
	}
	return tmp;

}

inline void write_bits(unsigned long data, int count, FILE *f)
{
	int i;

	bits_written+=count;
	if((f==NULL)&&(bitstore==NULL))
	{
		return;
	}
	for(i=0;i<count;i++)
	{
		bitsleft_w--;
		bits_w |= (((data >> (count-1-i)) & 1) << bitsleft_w);
		if(bitsleft_w==0)
		{
			if(f!=NULL)
			{
				fputc(((bits_w & 0xFF000000) >> 24),f);
				fputc(((bits_w & 0x00FF0000) >> 16),f);
				fputc(((bits_w & 0x0000FF00) >>  8),f);
				fputc(((bits_w & 0x000000FF) >>  0),f);
			}

			if(bitstore!=NULL)
			{
				*bitstore++ = (unsigned char)((bits_w & 0xFF000000) >> 24);
				*bitstore++ = (unsigned char)((bits_w & 0x00FF0000) >> 16);
				*bitstore++ = (unsigned char)((bits_w & 0x0000FF00) >>  8);
				*bitstore++ = (unsigned char)((bits_w & 0x000000FF) >>  0);
			}
			
			bitsleft_w = 32;
			bits_w = 0;
		}
	}
}

inline void flush_bits(FILE *f)
{
	int i;

	i=32-bitsleft_w;
	bits_written+=i;
	if(f!=NULL)
	{
		if(i>0)
			fputc(((bits_w & 0xFF000000) >> 24),f);
		if(i>8)
			fputc(((bits_w & 0x00FF0000) >> 16),f);
		if(i>16)
			fputc(((bits_w & 0x0000FF00) >>  8),f);
		if(i>24)
			fputc(((bits_w & 0x000000FF) >>  0),f);
	}
	if(bitstore != NULL)
	{
		if(i>0)
			*bitstore++ = (unsigned char)((bits_w & 0xFF000000) >> 24);
		if(i>8)
			*bitstore++ = (unsigned char)((bits_w & 0x00FF0000) >> 16);
		if(i>16)
			*bitstore++ = (unsigned char)((bits_w & 0x0000FF00) >>  8);
		if(i>24)
			*bitstore++ = (unsigned char)((bits_w & 0x000000FF) >>  0);
	}
	bitsleft_w = 32;
	bits_w = 0;
}


void sort_tree(tree_node *tree[], int count)
{
	int i,j;
	tree_node *swap1, *swap2;

	for(i=0;i<count;i++)
		for(j=0;j<(count - 1);j++)
		{
			swap1 = tree[j];
			swap2 = tree[j+1];
			if(swap1[0].count < swap2[0].count)
			{
				tree[j] = swap2;
				tree[j+1] = swap1;
			}
		}
}

inline void free_huffman_tree(tree_node *tree)
{
	
	if(tree == NULL)
		return;
	if(tree[0].node == 1)
	{
		free_huffman_tree(tree[0].left);
		free_huffman_tree(tree[0].right);
		free(tree);
	}
	else
		free(tree);
}

tree_node* create_treenode()
{
	int i;
	struct tree_node *ptr;
	ptr = (tree_node*)malloc(sizeof(tree_node));
	ptr[0].count = 0;
	ptr[0].data = 0;
	ptr[0].node = 0;
	ptr[0].left = NULL;
	ptr[0].right = NULL;
	for(i=0;i<32;i++)
		ptr[0].look_up[i] = NULL;

	return ptr;
}

tree_node* get_treenode(tree_node *tree, int bitcount, int numbits=0, int value=0, int forcelookup=0)
{
	int i;
	struct tree_node *tmp;
	if(forcelookup==1)
	{
		if(tree[0].node == 1)
		{
			tmp = get_treenode(tree[0].left,bitcount,numbits+1,value + (0 << numbits),1);
			if(tmp == NULL)
			{
				tmp = get_treenode(tree[0].right,bitcount,numbits+1,value + (1 << numbits),1);
				if(tmp == NULL)
					return NULL;
				else
					return tmp;
			}
			else
				return tmp;
		}
		else
		{
			if(tree[0].data == bitcount)
			{
				tree[0].value = 0;
				for(i=0;i<numbits;i++)
					tree[0].value |= (((value >> i) & 1) << ((numbits - 1) - i));
				tree[0].value |= (numbits << 24);
				
				return tree;
			}
			else
				return NULL;
		}
	}
	else
	{
		return tree[0].look_up[bitcount];
	}
}

void print_huffman_tree(tree_node *tree, int root=1)
{
	if(tree==NULL)
	{
		log_write("0\n");
		return;
	}
	if(tree[0].node == 1)
	{
		log_write("(");
		print_huffman_tree(tree[0].left,0);
		log_write(",");
		print_huffman_tree(tree[0].right,0);
		log_write(")");

		if(root==1)
			log_write("\n");
	}
	else
	{
		log_write("%d",tree[0].data);
	}
}

void write_huffman_tree(tree_node *tree, FILE *f,  int root=1)
{
	if(tree[0].node == 1)
	{
		if(f!=NULL)
		log_write("(");

		write_huffman_tree(tree[0].left,f,0);

		if(f!=NULL)
		log_write(",");

		write_huffman_tree(tree[0].right,f,0);

		if(f!=NULL)
		log_write(")");
		write_bits(1,1,f);

		if(root==1)
		{
			write_bits(1,1,f);
			if(f!=NULL)
			log_write("\n");
		}
	}
	else
	{
		
		write_bits(0,1,f);
		write_bits(tree[0].data,8,f);
		if(root)
			write_bits(1,1,f);
		if(f!=NULL)
		log_write("%d",tree[0].data);
		if(root)
			if(f!=NULL)
				log_write("\n");
	}
	
}

NEDCLIB_API int best_move=0;
NEDCLIB_API int best_size=0;

NEDCLIB_API int skip_huffman=0;
int skip_size=0;

tree_node* create_huffman_tree(unsigned short *buf, int count, FILE *f, int method=0, int type=0)
{
	int i,j,k;
	int treenodes=0;
	struct tree_node *ptr;
	struct tree_node *temp1;
	struct tree_node *treeroot;
	struct tree_node *pointer[256];

	int max_bitcount = 0;

	int count1, count2;

	int bitcounts[32] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	};

	for(i=0;i<count;i++)
	{
		if(method==1)
		{
			j=buf[i];
			if(j%4==0)
			{
				j+=8;
				j/=4;
			}
			else
			{
				j%=4;
				j--;
				j = get_bitsize(j);
				if(max_bitcount < j)
					max_bitcount = j;
				bitcounts[j]++;
				//bitcounts[3]++;

				j=buf[i];
				j+=8;
				j/=4;
			}
			j = get_bitsize(j);
		}
		else
			j = get_bitsize(buf[i]);

		if(max_bitcount < j)
			max_bitcount = j;

		//if(j<3)
		//	bitcounts[3]++;
		//else
			bitcounts[j]++;
	}

	if(skip_huffman)
	{
		max_bitcount = 15;
	}
	if(skip_size && (type==1))
	{
		max_bitcount = 15;
	}

	treeroot = create_treenode();
	treeroot[0].count = 0;
	treeroot[0].data = 0;
	treeroot[0].node = 0;
	treeroot[0].left = NULL;
	treeroot[0].right = NULL;

	if(!skip_huffman)
	{
		if(type==0)
			k=best_move;
		else
		{
			if(!skip_size)
				k=best_size;
			else
				k=0;
		}
	}
	else
		k=0;
	for(i=0;i<32;i++)
	{
		if(bitcounts[i] > 0)
		{
			if((i < max_bitcount))
			{
				j = 0;

				if((((best_move==0)&&(type==0)) || ((best_size==0)&&(type==1)&&(!skip_size)))&&(!skip_huffman))
				{
					if((bitcounts[i+1] * 2) >= bitcounts[i])
						j=1;
					
					if(i<(max_bitcount-3))
					{
						if((bitcounts[i] < 25))
							j=1;
					}
					else
					{
						if(bitcounts[i] < 100)
							j=1;
					}
				}
				else
				{
					if(k&1)
						j=0;
					else
						j=1;
				}
				k>>=1;

				//j=0;
				if(j==1)
				{
					bitcounts[i+1] += bitcounts[i];
					treeroot[0].look_up[i] = NULL;
					continue;
				}
			}
			if(f!=NULL)
			log_write("%.5d:%.3d\n",bitcounts[i],i);
			

			ptr = create_treenode();
			ptr[0].count = bitcounts[i];
			ptr[0].data = i;
			ptr[0].left = NULL;
			ptr[0].right = NULL;
			ptr[0].node = 0;
			pointer[treenodes++] = ptr;
			treeroot[0].look_up[i] = ptr;
			for(j=(i-1);j>=0;j--)
			{
				if(treeroot[0].look_up[j] == NULL)
				{
					treeroot[0].look_up[j] = ptr;
					continue;
				}
				break;
			}

		}
		else
			treeroot[0].look_up[i] = (tree_node*)(NULL + 1);
	}
	if(f!=NULL)
	log_write("\n");
	sort_tree(pointer,treenodes);

	while(treenodes > 1)
	{
		count1 = count2 = -1;
		if(treenodes == 2)
			ptr = treeroot;
		else
			ptr = create_treenode();
		ptr[0].node = 1;
		ptr[0].data = 0;
		treenodes--;
		ptr[0].right = pointer[treenodes];
		for(i=treenodes;i>=0;i--)
		{
			temp1 = pointer[i];
			if(temp1[0].count < ptr[0].right[0].count)
				ptr[0].right = temp1;
		}
		for(i=treenodes;i>=0;i--)
		{
			if(ptr[0].right == pointer[i])
				continue;
			ptr[0].left = pointer[i];
			break;
		}
		for(i=treenodes;i>=0;i--)
		{
			temp1 = pointer[i];
			if((temp1[0].count < ptr[0].left[0].count) && (temp1 != ptr[0].right))
				ptr[0].left = temp1;
		}
		ptr[0].count = ptr[0].right[0].count + ptr[0].left[0].count;

		for(i=0;i<(treenodes);i++)
		{
			if((ptr[0].left == pointer[i]) || (pointer[i] == NULL))
			{
				pointer[i] = pointer[i+1];
				pointer[i+1] = NULL;
			}
		}
		for(i=0;i<(treenodes);i++)
		{
			if((ptr[0].right == pointer[i]) || (pointer[i] == NULL))
			{
				pointer[i] = pointer[i+1];
				pointer[i+1] = NULL;
			}
		}
		
		
		pointer[treenodes-1] = ptr;
	}

	for(i=0;i<32;i++)
	{
		if(treeroot[0].look_up[i] == (tree_node*)(NULL + 1))
			treeroot[0].look_up[i] = NULL;
		else
			get_treenode(treeroot,i,0,0,1);
	}

	write_huffman_tree(pointer[0],f);

	return pointer[0];

}

tree_node* read_huffman_tree(unsigned char *buf)
{
	int tree_pointer=0;
	int tree;
	struct tree_node *ptr;
	struct tree_node *pointer[256];

	int tree_nodes=0;
	int node_pointer = 255;
	int i;
//	memset(huffman_tree,0,sizeof(huffman_tree));

	pointer[0] = NULL;
	
	do 
	{
		i=read_bits(1,buf);
		if(i==0)
		{
			tree_nodes=1;
			tree = read_bits(8,buf);
			ptr = create_treenode();
			ptr[0].node = i;
			ptr[0].data = tree;
			ptr[0].count = 0;
			ptr[0].left = NULL;
			ptr[0].right = NULL;

			//ptr[0].left = create_treenode();
			pointer[tree_pointer++] = ptr;
		}
		else
		{
			tree_pointer--;
			if(tree_pointer >= 1)
			{
				
				ptr = create_treenode();
				ptr[0].data = 0;
				ptr[0].node = 1;
				ptr[0].left = pointer[tree_pointer - 1];
				ptr[0].right = pointer[tree_pointer];
				pointer[tree_pointer-1] = ptr;
				
//				huffman_tree[tree_pointer-1].left = tree_pointer - 1;
//				huffman_tree[tree_pointer-1].right = tree_pointer;
			}
			//tree_pointer--;
		}
			
		

	} while (tree_pointer > 0);

	if(tree_nodes > 0)
	{
		for(i=0;i<32;i++)
			ptr[0].look_up[i] = NULL;
		return ptr;
	}
	else
	{
		ptr = create_treenode();
		ptr[0].data = 0;
		ptr[0].node = 0;
		ptr[0].left = NULL;
		ptr[0].right = NULL;
		ptr[0].value = 0;
		ptr[0].count = 0;
		for(i=0;i<32;i++)
			ptr[0].look_up[i] = NULL;
		return ptr;
	}

	
	
}
 
#define NUM_BITS get_bitsize(lzsize - 1)

#define MAX_TABLE_SIZE 0x10000
#define MAX_LITERALS 0x10000

NEDCLIB_API int skip_lz77 = 0;

NEDCLIB_API int NVPK_compress (unsigned char *buf, int size, int compression_level, int lzwindow, int lzsize, int method, FILE *f, unsigned char *bitdata)
{	
	static unsigned short literals[MAX_LITERALS], move_t[MAX_TABLE_SIZE], size_t[MAX_TABLE_SIZE];
	int literals_offset=0,move_offset=0,size_offset=0;
	int buf_offset, buf_back_offset;
	int i, j, k, l, m, n;

	int lookahead;

	tree_node *tmp;

	int window_bitsize;

	window_bitsize = get_bitsize(lzwindow-1);

	if((f==NULL)&&(bitdata==NULL)&&(compression_level<3))
	{
		return -1;
	}

	if((lzwindow<16)||(lzwindow>32768))
		return -3;
	if((lzsize<16)||(lzsize>32768))
		return -4;

	if(size>0x40000) //GBA Ram is 256KB. Can't exceed this.
		return -5;

	if(!skip_lz77)
	{
	
	if(compression_level != 0)
	{
		
		if(f!=NULL)
			log_write("LZ compressing data\n");

		for(buf_offset=0;buf_offset<size;buf_offset++)
		{

			if((literals_offset == MAX_LITERALS) || (move_offset > MAX_TABLE_SIZE) || (size_offset > MAX_TABLE_SIZE))
			{
				return -2; //Compressor failure, buffer overrun error
			}

			l=m=n=0;

			//for(buf_back_offset=0;(buf_back_offset<lzwindow) && (buf_back_offset<buf_offset);buf_back_offset++)
			for(buf_back_offset=((buf_offset<lzwindow) ? buf_offset - 1 : lzwindow - 1);buf_back_offset>=0;buf_back_offset--)
			{
				i=buf_offset;
				j=buf_back_offset;
				k=0;
				while((buf[i-(j+1)]==buf[i])&&(i<size)&&(k<(lzsize-1)))
				{
					i++;
					k++;
				}
				if(k>=2)
				{
					if(k>=l)
					{
						l=k;
						m=j;
						n=1;
						//if(k==(lzsize-1))
						//	break;
					}
				}
			}

			// Look ahead one more byte, for a run, just in case there is a better run to use.
			while((n==1)&&(l<(lzsize-1)))
			{
				n=0;
				if(l>=2)
				{
					lookahead = l;
					buf_offset++;
					for(buf_back_offset=0;(buf_back_offset<lzwindow) && (buf_back_offset<buf_offset);buf_back_offset++)
					{
						i=buf_offset;
						j=buf_back_offset;
						k=0;
						while((buf[i-(j+1)]==buf[i])&&(i<size)&&(k<(lzsize-1)))
						{
							i++;
							k++;
						}
						if(k>=2)
						{
							if(k>l)
							{
								l=k;
								m=j;
							}
						}
					}
					buf_offset--;
					if(l > lookahead)
					{
						n=1;
						literals[literals_offset++] = buf[buf_offset++];
					}
					else
						l = lookahead;
				}
			}
			if(l>=2)
			{
				literals[literals_offset++] = 0x100;
				move_t[move_offset++] = m + 1;
				size_t[size_offset++] = l;
				buf_offset+=(l-1);
			}
			else
				literals[literals_offset++] = buf[buf_offset];
		}
		literals[literals_offset++] = 0x1FF;
	}
	else
	{
		for(buf_offset = 0;buf_offset<size;buf_offset++)
			literals[literals_offset++] = buf[buf_offset];
		literals[literals_offset++] = 0x1FF;
	}

	}
	else
	{
		while(literals[literals_offset]!=0x1FF)
		{
			if(literals[literals_offset++]==0x100)
			{
				move_offset++;
				size_offset++;
			}
		}
	}
		


	flush_bits(NULL);
	bitstore=bitdata;
	bits_written = 0;
	write_bits(0x76706B30,32,f);
	/*fputc('v',f);
	fputc('p',f);
	fputc('k',f);
	fputc('0',f);*/
	write_bits(size,32,f);
	write_bits(method&0xFF,8,f);
	//fputc(method&0xFF,f);

	literals_offset = 0;
	

	if(compression_level == 0)
	{
		if(f!=NULL)
		log_write("Writing vpk data\n");

		write_bits(0x03,2,f);
		for(i=0;i<size;i++)
			write_bits(literals[i],9,f);
		flush_bits(f);
	}
	else if (compression_level == 1)
	{
		if(f!=NULL)
		log_write("Writing vpk data\n");

		move_offset = 0;
		size_offset = 0;

		//Move tree is 12 (4095 byte window)
		write_bits(0,1,f);
		write_bits(window_bitsize,8,f);
		write_bits(1,1,f);

		//Size tree is 8 (255 bytes)
		write_bits(0,1,f);
		write_bits(NUM_BITS,8,f);
		write_bits(1,1,f);

		while(literals[literals_offset] != 0x1FF)
		{
			if(literals[literals_offset] == 0x100)
			{
				write_bits(1,1,f);
				if(method==1)
				{
					j=move_t[move_offset];
					if(j%4==0)
					{
						j+=8;
						j/=4;
						write_bits(j,window_bitsize,f);
					}
					else
					{
						j%=4;
						j--;
						write_bits(j,window_bitsize,f);

						j=move_t[move_offset];
						j+=8;
						j/=4;
						write_bits(j,window_bitsize,f);

					}
					move_offset++;
				}
				else
				{
					write_bits(move_t[move_offset++],window_bitsize,f);
				}
				write_bits(size_t[size_offset++],NUM_BITS,f);
			}
			else
			{
				write_bits(0,1,f);
				write_bits(literals[literals_offset],8,f);
			}
			literals_offset++;
		}
		flush_bits(f);

	}
	else if (compression_level >= 2)
	{
		if(f!=NULL)
		log_write("Writing huffman trees\n");
		if(f!=NULL)
		log_write("----- Move Tree Structure/Frequency Counts-----\n");
		movetree = create_huffman_tree(move_t,move_offset,f,method,0);
		if(f!=NULL)
		log_write("----- Size Tree Structure/Frequency Counts-----\n");
		sizetree = create_huffman_tree(size_t,size_offset,f,0,1);
		if(f!=NULL)
		log_write("Writing vpk data\n");

		move_offset = 0;
		size_offset = 0;
		
		while(literals[literals_offset] != 0x1FF)
		{
			if(literals[literals_offset] == 0x100)
			{
				write_bits(1,1,f);

				if(method==1)
				{
					j=move_t[move_offset];
					if(j%4==0)
					{
						j+=8;
						j/=4;
						
					/*	if(get_bitsize(j)<3)
						{
							tmp = get_treenode(movetree,3);
							write_bits(tmp[0].value & 0x00FFFFFF,(tmp[0].value>>24),f);
							write_bits(j,tmp[0].data,f);
						}
						else
						{*/
							tmp = get_treenode(movetree,get_bitsize(j));
							if(tmp!=NULL)
							{
								write_bits(tmp[0].value & 0x00FFFFFF,(tmp[0].value>>24),f);
								write_bits(j,tmp[0].data,f);
							}
							else
							{
								write_bits(j,movetree[0].data,f);
							}
							
							
					//	}
						
					}
					else
					{
						j%=4;
						j--;
					/*	if(get_bitsize(j)<3)
						{
							tmp = get_treenode(movetree,3);
							write_bits(tmp[0].value & 0x00FFFFFF,(tmp[0].value>>24),f);
							write_bits(j,tmp[0].data,f);
						}
						else
						{*/
							tmp = get_treenode(movetree,get_bitsize(j));
							if(tmp==NULL)
							{
								write_bits(tmp[0].value & 0x00FFFFFF,(tmp[0].value>>24),f);
								write_bits(j,tmp[0].data,f);
							}
							else
								write_bits(j,movetree[0].data,f);
							
					//	}

						j=move_t[move_offset];
						j+=8;
						j/=4;

					/*	if(get_bitsize(j)<3)
						{
							tmp = get_treenode(movetree,3);
							write_bits(tmp[0].value & 0x00FFFFFF,(tmp[0].value>>24),f);
							write_bits(j,tmp[0].data,f);
						}
						else
						{*/
							tmp = get_treenode(movetree,get_bitsize(j));
							if(tmp==NULL)
							{
								return -3; //movetree failure
							}
							write_bits(tmp[0].value & 0x00FFFFFF,(tmp[0].value>>24),f);
							write_bits(j,tmp[0].data,f);
					//	}
					}
					move_offset++;
					
				}
				else
				{
					j = get_bitsize(move_t[move_offset]);
					//if(j<3)
					//	j = 3;
					tmp = get_treenode(movetree,j);
					if(tmp!=NULL)
					{
						write_bits(tmp[0].value & 0x00FFFFFF,(tmp[0].value>>24),f);
						write_bits(move_t[move_offset++],tmp[0].data,f);
					}
					else
						write_bits(move_t[move_offset++],movetree[0].data,f);
					
				}

				j = get_bitsize(size_t[size_offset]);
				//if(j<3)
				//	j = 3;
				tmp = get_treenode(sizetree,j);
				if(tmp!=NULL)
				{
					write_bits(tmp[0].value & 0x00FFFFFF,(tmp[0].value>>24),f);
					write_bits(size_t[size_offset++],tmp[0].data,f);
				}
				else
					write_bits(size_t[size_offset++],sizetree[0].data,f);
				
			}
			else
			{
				write_bits(0,1,f);
				write_bits(literals[literals_offset],8,f);
			}
			literals_offset++;
		}
		

		flush_bits(f);
		
		free_huffman_tree(movetree);
		free_huffman_tree(sizetree);
	}

	
	
	bitstore=NULL;

	if(bitdata!=NULL)
		return (bits_written/8)+1;
	else
		return 0;
}




unsigned char decompress_buffer[0x100000];

int read_tree_value(tree_node *tree, unsigned char *buf)
{
	tree_node *ptr;
	int tmp;

	int count=0;

	ptr = tree;
	if(ptr==NULL)
	{
		return 0;
	}
	else
	{
		while(ptr[0].node == 1)
		{
			tmp = read_bits(1,buf);
			if(tmp == 0)
				ptr = ptr[0].left;
			else
				ptr = ptr[0].right;
		}
		ptr[0].count++;
		tree[0].look_up[ptr[0].data] = ptr;
		return read_bits(ptr[0].data,buf);
	}
}

NEDCLIB_API int vpk_decompress (unsigned char *vpk, FILE *f)
{
	int bitcountsmove[32] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};

	int bitcountssize[32] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};
	
	struct tree_node *ptr;

	int decompress_buffer_pointer=0;
	int size,method;

	int i;

	int tmp;

	int move_tt,size_tt;

	i=0;

	if(read_bits(8,vpk)!='v')
		i=1;
	if(read_bits(8,vpk)!='p')
		i=1;
	if(read_bits(8,vpk)!='k')
		i=1;
	if(read_bits(8,vpk)!='0')
		i=1;

	if(i!=0)
	{
		printf("Error: Invalid vpk data\n");
		return 1;
	}
	
	size=read_bits(32,vpk);
	method=read_bits(8,vpk);

	if(method>1)
	{
		printf("Error: Invalid vpk data\n");
		return 1;
	}

	movetree = read_huffman_tree(vpk);
	sizetree = read_huffman_tree(vpk);

	
	if((movetree[0].node != 0) || (movetree[0].data != 0))
	{
		log_write("***** Move Tree Structure *****\n");
		print_huffman_tree(movetree);
		log_write("\n");
	}
	if((sizetree[0].node != 0) || (sizetree[0].data != 0))
	{
		log_write("***** Size Tree Structure *****\n");
		print_huffman_tree(sizetree);
		log_write("\n");
	}

	log_only_write("***** Decompression Log ******\n");

	for(i=0;i<size;)
	{
		tmp = read_bits(1,vpk);
		if(tmp == 0)
		{
			tmp = read_bits(8,vpk);
			log_only_write("%.2X\n",tmp);
			decompress_buffer[i++] = tmp;

		}
		else
		{
			move_tt = read_tree_value(movetree,vpk);

			bitcountsmove[get_bitsize(move_tt)]++;
			
			if(method==1)
			{
				if(move_tt < 3)
				{
					tmp = read_tree_value(movetree,vpk);
					bitcountsmove[get_bitsize(tmp)]++;
					move_tt++;
					move_tt = (move_tt + (tmp*4)) - 8;
				}
				else
				{
					move_tt = (move_tt * 4) - 8;
				}
			}

			size_tt = read_tree_value(sizetree,vpk);
			bitcountssize[get_bitsize(size_tt)]++;

			log_only_write("Move=%.4d, Size=%.4d: ",move_tt,size_tt);

			while((size_tt > 0) && (i < size))
			{
				decompress_buffer[i] = \
					decompress_buffer[i - move_tt];
				log_only_write("%.2X ",decompress_buffer[i - move_tt]);
				i++;
				size_tt--;
			}
			log_only_write("\n");

			
			
		}
	}

	for(i=0;i<size;i++)
		write_bits(decompress_buffer[i],8,f);
	flush_bits(f);

	
	if((movetree[0].node != 0) || (movetree[0].data != 0))
	{
		log_write("***** Frequency Table (move) *****\n");
		/*for(i=0;i<32;i++)
			if(bitcountsmove[i]>0)
				log_write("%.5d:%.3d\n",bitcountsmove[i],i);*/
		for(i=0;i<32;i++)
		{
			if(movetree == NULL)
				break;
			ptr = movetree[0].look_up[i];
			if(ptr == NULL)
				continue;
			log_write("%.5d:%.3d\n",ptr[0].count,i);
		}
		log_write("\n");
	}

	if((sizetree[0].node != 0) || (sizetree[0].data != 0))
	{
		log_write("***** Frequency Table (size) *****\n");
		/*for(i=0;i<32;i++)
			if(bitcountssize[i]>0)
				log_write("%.5d:%.3d\n",bitcountssize[i],i);*/
		for(i=0;i<32;i++)
		{
			if(sizetree == NULL)
				break;
			ptr = sizetree[0].look_up[i];
			if(ptr == NULL)
				continue;
			log_write("%.5d:%.3d\n",ptr[0].count,i);
		}
		log_write("\n");
	}
	

	free_huffman_tree(movetree);
	free_huffman_tree(sizetree);

	return 0;
}
