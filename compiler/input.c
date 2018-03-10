 
//input system functions
//This input system reads a whole file into a buffer named 'Buffer'
//by first determining the size of the file and then allocating space for 
// it



/***********************************************/

#include<stdio.h>
#include "input.h"
#include<stddef.h>
#ifdef DEBUG 
#define DEBUGPRINT(x)  x
#else
#define DEBUGPRINT(x) 
#endif

static char *Buffer;  // buffer for the input
static int BuffLen;   // hold the length of the buffer
static  char * BuffPtr;   // Points to the current position within the file

   /*  Fuctions   */
 //this fuction returns the next character int the buffer 
static int GetNextChar(void)
{
	int c ;
	c = '\0';
    if(Buffer && BuffPtr < Buffer + BuffLen )
    {
	     c = *BuffPtr++;
    } 
    return c;	
}  
 
//This fuction  Pushback an unwanted charcter back into the buffer

static void PushBack(void)
{
	if(BuffPtr >= &Buffer[0] )
    {
		BuffPtr--;
    }
  return;   
} 
//This fuction checks the size of a file 
//allocate space for it 
//and read it into 'Buffer'
static void Init(char *inputFile)
{
   FILE *fp;
   if((fp = fopen(inputFile,"rb")) == NULL)
   {
		fprintf(stderr,"Cannot read file  %s  \n",inputFile);
   }   
   else
   {
        size_t size;
		fseek(fp,0,SEEK_END);    //move file file position indicator to the end of file
		size = ftell(fp);        //determine the bytes of characters within the file  
        rewind(fp);		        //reset the file postion indicator  
  		Buffer = (char *)malloc(sizeof(char) * size);     
       if(!Buffer)
       {
			fprintf(stderr,"Internal Error: Memory allocation Failure");
			exit(1); 
       }  	   
       else
	   {
	        BuffPtr = Buffer;      //set BuffPtr equal to buffer
			size_t  read_chars;
	        read_chars = fread(Buffer,sizeof(char),size,fp);
			DEBUGPRINT(printf(" read_chars= %d   size = %d  \n",read_chars,size);)
			if(size > read_chars)
			{
			  read_chars = size;   //just in case of any character conversion 
            }			
            BuffLen =  read_chars;
			fclose(fp); 
	   } 
  	   
   }   

  return;
}   

struct InputRead INPUT_SYS = {GetNextChar,PushBack,Init};

//Debug prints

#ifdef DEBUG
 main()
{
   char *infile = "Grammer1.txt";
   Init(infile);
   int c;
   while((c = INPUT_SYS.GetNextChar()) != '\0')
   printf("%c",c);  
   
}  

#endif
