
#include "stdafx.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define d_mm  8           /* RS code over GF(2**4) - change to suit */
#define d_nn  255          /* nn=2**mm -1   length of codeword */
#define d_tt  127           /* number of errors that can be corrected */
#define d_kk  (d_nn - (2*d_tt))   /* kk = nn-2*tt  */

#include "rs.h"

#define NO_PRINT

unsigned char mm=8;
unsigned char nn=255;
unsigned char tt=16;
unsigned char kk=239;

int pp=0x187;
unsigned char *alpha_to, *index_of, *gg;
unsigned char *recd;
unsigned char b0 = 0x78;

int rs_init=-1;
int curr_errlen=0;

void make_rev(unsigned char *data, int len)
{
	for(int i=0;i<len;i++) data[i]=index_of[data[i]];
}

void make_pow(unsigned char *data, int len)
{
	for(int i=0;i<len;i++) data[i]=alpha_to[data[i]];
}

void invert_error_bytes(unsigned char *data, int len)
{
	for(int i=0;i<len;i++) data[i]^=0xFF;
}

void reverse_byte_order(unsigned char *data, int len)
{
	for(int i=0,x=0;i<(len>>1);i++) 
	{ 
		x=data[i];
		data[i]=data[len-i-1];
		data[len-i-1]=x; 
	}
}

void zerofill(unsigned char *data, int len)
{
	for(int i=0;i<len;i++) data[i]=0;
}

void generate_gf()
{
   int i, mask ;

   if(alpha_to!=NULL)
	   free(alpha_to);
   if(index_of!=NULL)
	   free(index_of);

   alpha_to=(unsigned char*)malloc(nn+1);
   index_of=(unsigned char*)malloc(nn+1);


   mask=1;
   alpha_to[nn]=0;
   index_of[0]=nn;
   for(i=0;i<nn;i++)
   {
	   alpha_to[i]=mask;
	   index_of[mask]=i;
	   mask<<=1;
	   if(mask>=(nn+1))
		   mask^=pp;
   }
}

void gen_poly(int errlen=16)
/* Obtain the generator polynomial of the tt-error correcting, length
  nn=(2**mm -1) Reed Solomon code  from the product of (X+alpha**i), i=1..2*tt
*/
 {
   int i,j,x,y ;
   if(gg!=NULL)
	   free(gg);
   gg=(unsigned char*)malloc(errlen);
   gg[0]=alpha_to[b0];
   for(i=1;i<errlen;i++)
   {
	   gg[i]=1;
	   for(j=i;j>=0;j--)
	   {
		   if(j==0)
			   y=0;
		   else
			   y=gg[j-1];
		   x=gg[j];
		   if(x!=0)
		   {
			   x=index_of[x]+b0+i;
			   if(x>=0xFF)
				   x-=0xFF;
			   y^=alpha_to[x];
		   }
		   gg[j]=y;
	   }
   }
   make_rev(gg,errlen);
 }

void initialize_rs(int bits, int polynomial, int index, int errlen)
{
	int i, j;

	mm=bits;
	if(rs_init!=mm)
	{
		for(i=0,j=1;i<mm;i++,j<<=1);
		nn=j-1;
		pp=polynomial;
		b0=index;
		tt=errlen/2;
		kk=nn-(2*tt);

		generate_gf();
		gen_poly(errlen);

		curr_errlen=errlen;
		if(recd!=NULL)
			free(recd);
		recd=(unsigned char*)malloc(nn);
		rs_init=mm;
	}

	if(curr_errlen!=errlen)
	{
		tt=errlen/2;
		kk=nn-(2*tt);

		gen_poly(errlen);
		curr_errlen=errlen;
	}
}

void free_rs()
{
	if(alpha_to!=NULL)
		free(alpha_to);
	if(index_of!=NULL)
		free(index_of);
	if(gg!=NULL)
		free(gg);
	if(recd!=NULL)
		free(recd);

	rs_init=-1;

}

int is_rs_initialized()
{
	if(mm==rs_init)
		return 1;
	else
		return 0;
}

void append_error_info(unsigned char *data, int dtalen, int errlen)
{
	int i,j,x,y,z;
	reverse_byte_order(data,dtalen);
	zerofill(data,errlen);
	for(i=dtalen-1;i>=errlen;i--)
	{
		z = index_of[data[i] ^ data[errlen-1]];
		for(j=errlen-1;j>=0;j--)
		{
			if(j==0)
				x=0;
			else
				x=data[j-1];
			if(z!=0xFF)
			{
				y=gg[j];
				if(y!=0xFF)
				{
					y+=z; if(y>=0xFF) y-=0xFF;
					x^=alpha_to[y];
				}
			}
			data[j]=x;
		}
	}
	invert_error_bytes(data,errlen);
	reverse_byte_order(data,dtalen);
}

int correct_errors(unsigned char *data, int dtalen, int errlen, unsigned char *erasure)
{
	int i,j=0,result;
	int erase_pos[2*d_tt];
	for(i=0;i<nn;i++)
		recd[i]=0;
	if(erasure != NULL)
	{
		reverse_byte_order(erasure,dtalen);
		for(i=0,j=0;i<dtalen;i++)
			if(erasure[i])
				erase_pos[j++]=i;
	}
	for(i=0;i<dtalen;i++)
		recd[i]=data[i];
	reverse_byte_order(recd,dtalen);
	invert_error_bytes(recd,errlen);
	result=eras_dec_rs(erase_pos,j);
	if(result>0)
		if(eras_dec_rs(erase_pos,0)!=0) //0 syndrome should be returned now.
			return -1;	//Otherwise errors not correctable.

	for(i=0;i<dtalen;i++)
		data[i]=recd[i];
	invert_error_bytes(data,errlen);
	reverse_byte_order(data,dtalen);
	return result;
	
}


/* Performs ERRORS+ERASURES decoding of RS codes. If decoding is successful, writes
the codeword into recd[] itself. Otherwise recd[] is unaltered except in the
erased positions where it is set to zero. 
  First "no_eras" erasures are declared by the calling program. Then, the 
maximum # of errors correctable is t_after_eras = floor((2*tt-no_eras)/2). If the number
of channel errors is not greater than "t_after_eras" the transmitted codeword
 will be recovered. Details of algorithm can be found
in R. Blahut's "Theory ... of Error-Correcting Codes". */

int eras_dec_rs(int *eras_pos,int no_eras)

{
  int i,j,r,u,q,tmp,tmp1,tmp2,num1,num2,den,pres_root,pres_loc;
  unsigned char phi[2*d_tt+1],tmp_pol[2*d_tt+1]; /* The erasure locator in polynomial form */
  int U,syn_err,discr_r,deg_phi,deg_lambda,L,deg_omega,t_after_eras;
  unsigned char lambda[2*d_tt+1],s[2*d_tt+1],lambda_pr[2*d_tt+1];/* Err+Eras Locator poly and syndrome poly */ 
  unsigned char b[2*d_tt+1],T[2*d_tt+1],omega[2*d_tt+1];
  unsigned char syn_error=0, root[2*d_tt], z[d_tt+1], err[d_nn], reg[2*d_tt+1] ;
  unsigned char loc[2*d_tt],count = 0;

/* Maximum # ch errs correctable after "no_eras" erasures */
   t_after_eras = (int)floor((2.0*tt-no_eras)/2.0);

/* Compute erasure locator polynomial phi[x] */
  zerofill(tmp_pol,nn-kk+1);
  zerofill(phi,nn-kk+1);
  if (no_eras > 0){
      phi[0] = 1; /* index form */
      phi[1] = alpha_to[eras_pos[0]];
      for (i=1;i < no_eras;i++)
	  {
          U = eras_pos[i];
		  for (j=1;j < i+2;j++)
		  {
			 tmp1 = index_of[phi[j-1]];
			 tmp_pol[j] = (tmp1 == 0xFF)?  0 : alpha_to[(U+tmp1)%0xFF];
		  }
		  for (j=1;j < i+2;j++)
			  phi[j] = phi[j]^tmp_pol[j];
      }
  /* put phi[x] in index form */
     make_rev(phi,nn-kk+1);
  }

/* recd[] is in polynomial form, convert to index form */
   make_rev(recd,nn);

/* first form the syndromes; i.e., evaluate recd(x) at roots of g(x) namely
 @**(b0+i), i = 0, ... ,(2*tt-1) */
   for (i=1; i <= nn-kk; i++)
    { s[i] = 0 ;
      for (j=0; j < nn; j++)
        if (recd[j] != 0xFF)
          s[i] ^= alpha_to[(recd[j]+(b0+i-1)*j)%0xFF] ;      /* recd[j] in index form */
/* convert syndrome from polynomial form to index form  */
      if (s[i] != 0)  syn_error = 1 ;   /* set flag if non-zero syndrome => error */
      s[i] = index_of[s[i]] ;
    };


   if (syn_error)
   { /* if syndrome is zero, modified recd[] is a codeword */
/* Begin Berlekamp-Massey algorithm to determine error+erasure locator polynomial */
   		r = no_eras;
   		deg_phi = no_eras;
   		L = no_eras;
   		if (no_eras > 0)
		{
        	/* Initialize lambda(x) and b(x) (in poly-form) to phi(x) */
        	for (i=0;i < deg_phi+1;i++) lambda[i] = (phi[i] == 0xFF)? 0 : alpha_to[phi[i]];
        	for (i=deg_phi+1;i < 2*tt+1;i++) lambda[i] = 0;
        	deg_lambda = deg_phi;
        	for (i=0;i < 2*tt+1;i++) b[i] = lambda[i];
   		}
   		else
		{
			lambda[0] = 1;
			for (i=1;i < 2*tt+1;i++) lambda[i] = 0;
			for (i=0;i < 2*tt+1;i++) b[i] = lambda[i];
   		}
		while (++r <= 2*tt) /* r is the step number */
		{ 
			/* Compute discrepancy at the r-th step in poly-form */
			discr_r = 0;
			for (i=0;i < 2*tt+1;i++)
			{
				if ((lambda[i] != 0) && (s[r-i] != 0xFF))
				{
				   tmp = alpha_to[(index_of[lambda[i]]+s[r-i])%0xFF];
				   discr_r ^= tmp;
				}
			}
			if (discr_r == 0)
			{
			   /* 3 lines below: B(x) <-- x*B(x) */
			   tmp_pol[0] = 0;
			   for (i=1;i < 2*tt+1;i++) tmp_pol[i] = b[i-1];
			   for (i=0;i < 2*tt+1;i++) b[i] = tmp_pol[i];
			}
			else
			{
			   /* 5 lines below: T(x) <-- lambda(x) - discr_r*x*b(x) */
			   T[0] = lambda[0];
			   for (i=1;i < 2*tt+1;i++)
			   {
					tmp =  (b[i-1] == 0)? 0 : alpha_to[(index_of[discr_r]+index_of[b[i-1]])%0xFF];
					T[i] = lambda[i]^tmp;
			   }

			   if (2*L <= r+no_eras-1)
			   {
					L = r+no_eras-L;
					/* 2 lines below: B(x) <-- inv(discr_r) * lambda(x) */
					for (i=0;i < 2*tt+1;i++)
						b[i] = (lambda[i] == 0)? 0 : alpha_to[(index_of[lambda[i]]-index_of[discr_r]+nn)%0xFF];
					for (i=0;i < 2*tt+1;i++) lambda[i] = T[i]; 
			   }
			   else
			   {
					for (i=0;i < 2*tt+1;i++) lambda[i] = T[i]; 
					/* 3 lines below: B(x) <-- x*B(x) */
					tmp_pol[0] = 0;
					for (i=1;i < 2*tt+1;i++) tmp_pol[i] = b[i-1];
					for (i=0;i < 2*tt+1;i++) b[i] = tmp_pol[i];
			   }
			}
		}
/* Put lambda(x) into index form */
		make_rev(lambda,2*tt+1);
    
/* Compute deg(lambda(x)) */
		deg_lambda = 2*tt;
		while ((lambda[deg_lambda] == 0xFF) && (deg_lambda > 0)) 
			--deg_lambda;
		if (deg_lambda <= 2*tt)
		{
	/* Find roots of the error+erasure locator polynomial. By Chien Search */
			for (i=1; i < 2*tt+1; i++) reg[i] = lambda[i] ;
			count = 0 ; /* Number of roots of lambda(x) */
			for (i=1; i <= nn; i++)
			{
				 q = 1 ;
				 for (j=1; j <= deg_lambda; j++)
					if (reg[j] != 0xFF)
					{ 
						reg[j] = (reg[j]+j)%0xFF ;
						q ^= alpha_to[(reg[j])%0xFF] ;
					} ;
				 if (!q)        /* store root (index-form) and error location number */
				 { 
					root[count] = i;
					loc[count] = nn-i;
					count++;
				 };
			} ;
 
			if (deg_lambda == count) /* correctable error */
			{
/* Compute err+eras evaluator poly omega(x) = s(x)*lambda(x) (modulo x**(nn-kk)). in poly-form */
				for (i=0;i < 2*tt;i++)
				{
					omega[i] = 0;
					for (j=0;(j < deg_lambda+1) && (j < i+1);j++)
					{
						if ((s[i+1-j] != 0xFF) && (lambda[j] != 0xFF))
		       				tmp = alpha_to[(s[i+1-j]+lambda[j])%0xFF];
						else
						tmp = 0;
						omega[i] ^= tmp;	
					}
				}
				omega[2*tt] = 0;
/* Compute lambda_pr(x) = formal derivative of lambda(x) in poly-form */
				for (i=0;i < tt;i++)
				{
				   lambda_pr[2*i+1] = 0;
				   lambda_pr[2*i] = (lambda[2*i+1] == 0xFF)? 0 : alpha_to[lambda[2*i+1]];
				}
				lambda_pr[2*tt] = 0;
/* Compute deg(omega(x)) */
				deg_omega = 2*tt;
				while ((omega[deg_omega] == 0) && (deg_omega > 0)) 
					--deg_omega;
/* Compute error values in poly-form. num1 = omega(inv(X(l))), 
  num2 = inv(X(l))**(b0-1) and den = lambda_pr(inv(X(l))) all in poly-form */
				for (j=0;j < count;j++)
				{
					pres_root = root[j];
					pres_loc = loc[j];
					num1 = 0;
					for (i=0;i < deg_omega+1;i++)
					{
						if (omega[i] != 0) 
							tmp = alpha_to[(index_of[omega[i]]+i*pres_root)%0xFF];
						else
							tmp = 0;
						num1 ^= tmp;
					}
					num2 = alpha_to[(pres_root*(b0-1))%0xFF];
					den = 0;
					for (i=0;i < deg_lambda+1;i++)
					{
						if (lambda_pr[i] != 0)
							tmp = alpha_to[(index_of[lambda_pr[i]]+i*pres_root)%0xFF];
						else
							tmp = 0;
						den ^= tmp;
					}
					if (den == 0)
					{
						printf("\n ERROR: denominator = 0\n");
					}
					err[pres_loc] = 0;
					if (num1 != 0)
					{
						err[pres_loc] = alpha_to[(index_of[num1]+index_of[num2]+(nn-index_of[den]))%0xFF];
					}
				}
/* Correct word by subtracting out error bytes. First convert recd[] into poly-form */
				make_pow(recd,nn);
				for (j=0;j < count;j++)
					recd[loc[j]] ^= err[loc[j]];

				return(count);
			}
			else /* deg(lambda) unequal to number of roots => uncorrectable error detected */
				return(-2);
		}
		else /* deg(lambda) > 2*tt => uncorrectable error detected */
			return(-3);
   }
   else
   {  /* no non-zero syndromes => no errors: output received codeword */
	   make_pow(recd,nn);
       return(0);
   }
}
