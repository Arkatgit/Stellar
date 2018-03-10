#include "error.h"
#include<stdio.h>
#include<stdarg.h>
#include<string.h>

static int ecount; 

#ifndef MAXE_SIZE

#define  MAXE_SIZE  5120

#endif

static char errorlog[5120];
static  int i = 0;


 void error(char * fmt,... )
{
	va_list ap;
	va_start(ap,fmt);
	i += vsprintf(&errorlog[i],fmt,ap);
	ecount++; 	
}

FILE * fp;

void errorlogger(void)
{
	if((fp = fopen("err.txt","w")) == NULL)
		{
			fprintf(stdout,"Cannot open Errorlog \n");
			return;
		}	
		fprintf(fp,"%s",errorlog);
}


void printerrors(void)
{
	
	if(ecount > 0)
	{
		fprintf(stderr,"%s",errorlog);
		
		
	}
	
			
} 


int getecount(void)
{
	return ecount;
}

