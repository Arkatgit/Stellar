/*
 * This file contains predefined library functions
 *
 *
 *
*/ 



#include<stdarg.h>
#include<math.h>
#include<stdio.h>
#include<ctype.h>

/* Common Mathematical Constansts */

// PI- mahematical Constant

	
	
		
/**Input  && output routines */ 

#define  DEG(x) (( 3.142/180) * x)

float sine(float x)
{
	float me = DEG(x);
	return sin(me);
}

float cose(float x)
{
   float me = DEG(x);
   return cos(me);
}

float tane(float x)
{
	float me = DEG(x);
	return tan(me);
}



void printbool(int a)
{
	if(a == 1)
	{
		printf("%s\n"," True ");
	}
	else
	{
		printf("%s\n"," False ");
	}
}
void printfloat(float a)
{
	printf(" %f \n", a);
}

float get_float( void)
{
	char a[10];
	gets(a);
	float m;
	printf("Enter float \n");
	scanf("%f",&m);
	//= atof(a);
	printf("from getint :  %f \n" ,m);	
	return m ;	
	
	
}

int get_int( void)
{
	char a[10];
	gets(a);
	int  m = atoi(a);
    printf("from getint :  %d \n" ,m);	
	return m;
	
}



//Add Arbitrary Number of integers with a zero at the end
int addints(int a,...)
{
	va_list p;
	va_start(p,a);
	int b = a;
	int  ans = 0;	
	while( b)
	{
		ans += b;
		b = va_arg(p,int);		
	}
	va_end(p);
	
	return ans;
}
//Multiply  arbitrary number of integers with a one at the end
int multiplyints(int a,...)
{
	va_list p;
	va_start(p,a);
	int b = a;
	int  ans = 1;	
	while( b != 1)
	{
		ans *= b;
		b = va_arg(p,int);		
	}
	va_end(p);
	
	return ans;
}
//make a duplicate copy 'b'  of an integer array 'a' 
void dupintarray(int *a,int *b,int  l)
{
	int i;
	for(i = 0;i < l ; i++ )
	{
		b[i] = a[i]; 
	}
	return;
}
//make a duplicate copy 'b'  of an float array 'a'
void dupfloatarray(float *a,float *b,int  l)
{
	int i;
	for(i = 0;i < l ; i++ )
	{
		b[i] = a[i]; 
	}
	return;
}

//search an array for  index of key

int search_index(float *a,int l , float key )
{
	int i = 0;
	while(i < l)
	{
		if(a[i] == key){return  i;}
		i++;
	}
	return -1;
}


//sort an integer array 
void sortintarray(int a[],int l)
{
	int i,j;
	int temp;
	for(i = 0 ; i < l ; i++)
	{
		for(j = i + 1 ; j < l; j++)
		{
			if(a[i] > a[j])
			{
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}	
	}
	return;
}
//sort a float array
void sortfloatarray(float a[],int l)
{
	int i,j;
	float temp;
	for(i = 0 ; i < l ; i++)
	{
		for(j = i + 1 ; j < l; j++)
		{
			if(a[i] > a[j])
			{
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}	
	}
	return;
}

/* 
 *  procedures for permutation  and combination
 *
 */
 
 int  factorial(int a)
{
	int fact = 0;
	if( a > 0)
	{
		fact = 1;
		int i;
		for(i = 1; i <= a; i++)
		{
			fact *= i;
		}
	}
	return fact;
} 
 
 #define COMB(n,r)  (factorial(n)/(factorial(r) * factorial(n - r)) )
/*
int  factorial(int a)
{
	int fact = 0;
	if( a > 0)
	{
		fact = 1;
		int i;
		for(i = 1; i <= a; i++)
		{
			fact *= i;
		}
	}
	return fact;
} 
*/
int permutation(int n,int r)
{
	return factorial(n)/factorial(n - r);
}

int combination(int n,int r)
{
	return COMB(n,r);
}

/* procedures for series and sequence */

 
 // first term  = a; 
 //common difference = d;
 //common ratio = r
 //
 
 
 //nth term of an Arithmetic progression
 
 float ap_nterm(float a,float d,float n)
{
	float p;
	p = a + (n - 1) * d;
	return p;
} 
 
 
 //Sum of the first nterms of an AP
 
 float ap_sumnterms(float a,float d,float n)
{
	float s;
	s = (n /2) *( 2 * a  +   d *(n - 1));
	return s;
}

float gp_nterm(float a,float r, float n )
{
	return a * pow(r,n - 1);
}

float gp_suminfinity(float a,float r)
{
	if(r > 1 || r < -1)
	{
		return 0;
	}
	if(r > 0)
	{
		return (a / r - 1);
	}
}
 
//procedures for statistical analysis


//Standard deviation and mean computation


//mean
struct Values
{
	float vals;
	int freq;
	 int fx;
};
//standard deviation
struct Valuess
{
	float vals;
	int freq;
	 int fx;
	 int fx2;
};

void init(struct Values vals[] ,int len)
{
	int i;
	for(i = 0 ; i < len ; i++)
	{
		vals[i].vals = 0;
		vals[i].freq = 0 ;
		vals[i].fx = 0;
	}
}

void init2(struct Valuess vals[] ,int len)
{
	int i;
	for(i = 0 ; i < len ; i++)
	{
		vals[i].vals = 0;
		vals[i].freq = 0 ;
		vals[i].fx = 0;
		vals[i].fx2 = 0;
	}
}

//compute mean  show table of computation and return mean 
float ShowMeanTable(float * a ,int len )
{
	float mean;
	struct Values  vals[len];
	int i, j; 
	int k = 0;
	int v = 0;
	float sum;
	float freqsum;
	int index = 0;
	sum = freqsum = 0;
	init(vals,len);
	
	for(i = 0 ; i <  len ; i++)
	{ 
		k = v;
		index = 0;
		while( k >= 0  )
		{
			
			if(a[i]  == vals[k].vals)
			{
				//printf("vals ==  %f \n",vals[k].vals );
				index = 1;
				 break;
				 
			}
			k--;
		}
		
		
		if(index != 1)
		{
			//printf("  v2 = %d\n", i);
			vals[v].vals = a[i];
			vals[v].freq = 0;	
			for(j = 0 ; j <  len ; j++)
			{
				if( vals[v].vals == a[j]  )
				{
					vals[v].freq++; 
				}
	
			}
				v++;
		}		
	}
			for(i = 0 ; i <  v ; i++)
			{
				vals[i].fx = vals[i].vals * vals[i].freq; 
				sum  += vals[i].fx;
				freqsum += vals[i].freq;	
			}
			printf("-------------------------------------------\n");
			printf(" x         |      f         |      fx     | \n");
			printf("-------------------------------------------\n");
			for(i = 0 ; i <  v ; i++)
			{
				printf("|");
				printf("%5.2f     |   %5d       |    %5d     |\n",vals[i].vals,vals[i].freq ,vals[i].fx );
	   
		}
			printf("-------------------------------------------\n");
		mean = sum/ freqsum;
		 //printf("%f\n",freqsum);
		return mean;
}

//standard deviation
float ShowStdTable(float * a,int len )
{
	float sum2 , par;
	float std;
	struct Valuess  vals[len];
	int i, j; 
	int k = 0;
	int v = 0;
	float sum;
	float freqsum;
	int index = 0;
	sum = freqsum = sum2 = 0;
	init2(vals,len);
	
	for(i = 0 ; i <  len ; i++)
	{ 
		k = v;
		index = 0;
		while( k >= 0  )
		{
			
			if(a[i]  == vals[k].vals)
			{
				//printf("vals ==  %f \n",vals[k].vals );
				index = 1;
				 break;
				 
			}
			k--;
		}
		
		
		if(index != 1)
		{
			//printf("  v2 = %d\n", i);
			vals[v].vals = a[i];
			vals[v].freq = 0;	
			for(j = 0 ; j <  len ; j++)
			{
				if( vals[v].vals == a[j]  )
				{
					vals[v].freq++; 
				}
	
			}
				v++;
		}		
	}
			for(i = 0 ; i <  v ; i++)
			{
				vals[i].fx = vals[i].vals * vals[i].freq;
				vals[i].fx2 = vals[i].fx * vals[i].vals;	
				sum  += vals[i].fx;
				sum2 += vals[i].fx2;
				freqsum += vals[i].freq;	
			}
			printf("------------------------------------------------------\n");
			printf(" x         |      f         |      fx     |     fx2   |  \n");
			printf("-------------------------------------------------------  \n");
			for(i = 0 ; i <  v ; i++)
			{
				printf("|");
				printf("%5.2f     |   %5d       |    %5d     |    %5d  |\n",vals[i].vals,vals[i].freq ,vals[i].fx ,vals[i].fx2);
	   
		}
			printf("-------------------------------------------------------\n");
	
	par = (sum2/ freqsum) - pow(sum/freqsum , 2);
	std = pow(par ,0.5);
	//printf("sum2 = %f\n",sum2);
	//printf("sum = %f\n",sum);
	return std;
} 
 
 
 /**Trignometric functions **/
 
 
 
 
 
 
 
