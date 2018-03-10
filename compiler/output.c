
#include "output.h"
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdarg.h> 
#include<stdio.h>

#ifdef P_DEBUG
#define PRINT(x) x
#else
#define PRINT(x)
#endif


static void flushoutput(struct output_w *e)
{
	PRINT(printf(" Enter Output::flushouput \n");)
	if( (e->fd  > 0) && (e->bfptr > 0) )
	{
		write(e->fd , e->output , e->bfptr);
	}
	e->bfptr =  0; 
	PRINT(printf(" Leave Output::flushouput \n");)
}
 
static void writes(struct output_w  * e, char * s ,...)
{
/*This procedure takes multiple strings and write them to temporary 
 * buffer or write them to  file if  the buffer full....
 * 
*/



	
PRINT(printf(" Enter Output::writes \n");)
	va_list ap;
	char * a;
	int len;
	char * p;
	if(e->state == 0 )	// first entrance into write function, we open the file 		 
	{
		//e->fd = open(e->outfile,O_RDONLY|O_WRONLY|O_APPEND ,066);
		e->fd = creat( e->outfile, 0666 ); 
		if(e->fd == -1)
		{
			fprintf(stderr,"Cannot open output file '%s' ",e->outfile);
			exit(1);
		}
		e->state = 1;
	}
	if( s != NULL)
	{
		len =  strlen(s);
		if( (e->bfptr + len) >  BUFSIZE )	//buffer is full
		{
			flushoutput(e);	
		}
		va_start(ap,s);
		p = s;
		PRINT(printf("Write:: ");)
		do
		{
			while(*p != '\0')
			{
				PRINT(printf("%c", *p); )
				e->output[e->bfptr++] = *p++;	
			}
			/*if(e->bfptr >= BUFSIZE -1)
			{
				flushoutput(e);
			}*/
			p = va_arg(ap , char *);	
		}while(p);
		va_end(ap);
		PRINT(printf("\n");)
	}
	else
	{			//last entry into write function so we write(flush) to file and close
		flushoutput(e);
	
	}
PRINT(printf(" Leave Output::writes \n");)	
}

#define NUM_WRITERS 10
struct output_w  Writers[NUM_WRITERS]; 
static int Current = 0;
struct output_w * newOutput(char * file)
{
	PRINT(printf(" Enter Output::newoutput \n");)
	int i = Current;
	if( i < NUM_WRITERS)
	{
		Writers[i].fd = -1;
		Writers[i].outfile = file;
		Writers[i].bfptr = 0;
		Writers[i].state = 0;
		Writers[i].writes = writes;	
	}
	Current++;
	PRINT(printf(" Leave Output::newoutput \n");)
	return &Writers[i];
}


	