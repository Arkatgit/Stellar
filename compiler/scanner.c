//This file cotains all the fuctions for the lexical analysis phase
//It Tokenizes the input into individual tokens allocate space for tokens
//garther relevant information as lexeme,Token,value etc
//strips comments

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include "scanner.h"
#include "input.h"
#include "error.h"
#define TOKEN_BUFF_SIZE 1024
static int Stamps[TOKEN_BUFF_SIZE];
static struct Token_info Buffer[TOKEN_BUFF_SIZE];
static int  line = 1;
static int col;
static struct Token_info * Number(void);
struct Token_info * Identifier(void);
struct Token_info * ScanChar(void);
#ifdef S_DEBUG
#define S_DEBUGPRINT(x) x  
#else
#define S_DEBUGPRINT(x)
#endif 
//This fuction allocate space for a newtoken
 static int p;        
static struct Token_info *  Alloc_Newtoken(void)
{
	struct Token_info *t;
	t = NULL;
	int count;
	for(count = 0; count < TOKEN_BUFF_SIZE  && t == NULL ; count++ )
	{
		if(Stamps[count]== 0)
        {
	        t = &Buffer[count];
            Stamps[count] = 1;  			
        } 		
    } 	
    if(t == NULL)    //In  case the allocated  buffer is exhaustered 
    {
		t = (struct Token_info *)malloc(sizeof( struct Token_info));
		if(t == NULL)
		{
			fprintf(stderr,"Internal error : Memory allocation Failure\n");
             exit(1);			
        } 		
    }	
   if(t != NULL)
   {
		t->Tok = Tokerr;
		t->line = t->col = -1;
        t->Kind = None;		
   } 
  return t;   
} 
//Reserved Words
static struct Reserved_words
                           {
						       char *p;
							   enum Tokens Tok; 
							}Reserved[] =
							              {
										     {"and",Tokand },
											 {"array",Tokarray},
											 {"bool",Tokbool},
											 {"call",Tokcall},
                                             {"char",Tokchar},
                                             {"const",Tokconst},
                                             {"do",Tokdo},
                                             {"else",Tokelse},
                                             {"elseif",Tokelseif},
                                             {"false",Tokfalse},
                                             {"float",Tokfloat},
                                             {"function",Tokfunction},
  											 {"if",Tokif},
											 {"int",Tokint},
											 {"mod",Tokmod},
											 {"or",Tokor},
											 {"return",Tokreturn},
                                             {"then",Tokthen},
                                             {"true",Toktrue},
                                             {"var",Tokvar},
                                             {"void",Tokvoid},
                                             {"while",Tokwhile},  											 
 											 };
 //Number of reserved words
 
#define RESV_SIZE  (sizeof Reserved/sizeof Reserved[0])                              							
static void Free_Token(struct Token_info *t)
{
	if( t >= &Buffer[0]  && t <  &Buffer[TOKEN_BUFF_SIZE] )
	{
	     Stamps[t - Buffer] = 0; 
    }
    else
    {                              //if space was allocated by malloc
		free(t);
    }
   return;	
}
  
static void Drive(char *infile)
{
    INPUT_SYS.Init(infile);
	return;
} 

//This fuction does the tokennization of the input
static struct Token_info * GetNextToken(void)
{
	struct Token_info  *t;
    t = NULL;
	int c;
	c = INPUT_SYS.GetNextChar();	
    while(t == NULL )
    {
		col++;
		if(isspace(c) && c != '\n' )
		{
			c = INPUT_SYS.GetNextChar();
		}
        else if(c == '\0')
        {
		    t = Alloc_Newtoken();
			t->Tok = Tokeof;
            t->lexeme[0] = '\0';			
        } 		
	    else if(c == '\n')
		{
		   line++;
           col = 0; 
		   c = INPUT_SYS.GetNextChar();	
        }
		else if(c == '$')
		{
			while((c = INPUT_SYS.GetNextChar()) != '\n' && c != EOF ) 	// skip comment
			;
			//INPUT_SYS.PushBack();
		}
        else if(isdigit(c))
        {
		    INPUT_SYS.PushBack();
			t = Number(); 
			/*printf("\n '%d'  \n",'\n');*/
			if(t == NULL)
			{
				//printf("\n  NULL\n")
				c = INPUT_SYS.GetNextChar();
				col++;
			}
        }
        else if(isalpha(c))   		
	    {
		   INPUT_SYS.PushBack();
           t = Identifier(); 		   
        }
        else if(c == '{')
        {
		   t = Alloc_Newtoken();
		   t->Tok = Toklbrace;
		   t->line = line;
           t->col = col;		   
           t->lexeme[0] = '{';
           t->lexeme[1]  = '\0';
                		   
        }
        else if(c == '}')
		{
		   t = Alloc_Newtoken();
		   t->Tok = Tokrbrace;
           t->line = line;
           t->col = col;
           t->lexeme[0] = '}';
           t->lexeme[1] = '\0';
              		   
        }
		else if(c == '[')
		{
			t = Alloc_Newtoken();
			t->Tok = Toklarr;
            t->line = line;
            t->col = col;
            t->lexeme[0] = '[';
            t->lexeme[1] = '\0';    			
		}
        else if(c == ']')
		{
			t = Alloc_Newtoken();
			t->Tok = Tokrarr;
            t->line = line;
            t->col = col;
            t->lexeme[0] = ']';
            t->lexeme[1] = '\0';    			
		}  		
        else if(c == ':')
        {   
		    t = Alloc_Newtoken();
            t->line = line; 			
			c = INPUT_SYS.GetNextChar();
		    if(c == '=')
			{
			    col++; 
				t->Tok = Tokequal;
				t->col = col;
                strcpy(t->lexeme,":="); 				
            }
           else
            {    
			    INPUT_SYS.PushBack();
			    t->Tok = Tokcolon;
                t->col = col;
                t->lexeme[0] = ':';
                t->lexeme[1] = '\0';
            }
  		   
        }
        else if( c == ';' )
	    {
		    t = Alloc_Newtoken();
			t->Tok = Toksemi;
            t->line = line;
            t->col  = col;
            t->lexeme[0] = ';';
            t->lexeme[1]  = '\0';			
		} 
        else if(c == '+')
        {
			t = Alloc_Newtoken();
			t->Tok = Tokplus;
            t->line = line;
            t->col = col;
            t->lexeme[0] = '+';
            t->lexeme[1] = '\0';
 			
        }
       else if(c == '-')
        {
			t = Alloc_Newtoken();
			t->Tok = Tokminus;
            t->line = line;
            t->col = col;
            t->lexeme[0] = '-';
            t->lexeme[1]  = '\0';		   
        } 
       else if(c == '/')
        {
			t = Alloc_Newtoken();
			t->Tok = Tokdivide;
			t->line = line;
            t->col  = col;
            t->lexeme[0] = '/';
            t->lexeme[1] = '\0';   			
        }
       else if(c == '*')
       {
			t = Alloc_Newtoken();
			t->Tok = Toktimes;
            t->line = line;
            t->col  = col;
            t->lexeme[0] = '*';
            t->lexeme[1] = '\0';    			
       }
       else if(c == '(')
       {
			t = Alloc_Newtoken();
			t->Tok = Toklparen;
            t->line = line;
            t->col = col;
            strcpy(t->lexeme,"(");   			
       }
	   else if(c == ')')
       {
			t = Alloc_Newtoken();
			t->Tok = Tokrparen;
            t->line = line;
            t->col = col;
            strcpy(t->lexeme,")");   			
       }
	   else if(c == '\'')
	   {
			int p;
			c = INPUT_SYS.GetNextChar();
			if(isprint(c))
			{
				col++;
				p = c ;
				c = INPUT_SYS.GetNextChar();
				col++;
			}
            else
            {
				//fprintf(stderr,"Error: Line %d Col %d invalid character constant '%c' \n",t->line,t->col,c);
				error("Error: Line %d Col %d invalid character constant '%c' \n",t->line,t->col,c);
				c = INPUT_SYS.GetNextChar();
				col++;
            } 			
			if(c == '\'')
			{
				t = Alloc_Newtoken();
				t->Tok = Tokcharconst;
				t->line = line;
				//t->Kind = Charac_const;
				t->col = col;
				t->lexeme[0] = '\'';
				t->lexeme[1] = p;
				t->lexeme[2] = '\'';
				t->lexeme[3] = '\0';	
			}
			else
			{
				//fprintf(stderr,"Error: Line %d Col %d missing enclosing quote before \n",t->line,t->col,t->lexeme);
				error("Error: Line %d Col %d missing enclosing quote before \n",t->line,t->col,t->lexeme);
				c = INPUT_SYS.GetNextChar();
				col++;
			}
			
	   }
	   else if(c == '=')
	   {
			t = Alloc_Newtoken();
            t->Tok = Tokcompare;
            t->line = line;
            t->col = col;
            t->lexeme[0] = '=';
            t->lexeme[1] = '\0';   			
       }
       else if(c == '!')
       {
			t = Alloc_Newtoken();
			t->line = line; 
			c = INPUT_SYS.GetNextChar();
            if(c == '=')
            {
				col++;
				t->Tok = Toknequal;
				t->col = col;
                strcpy(t->lexeme,"!="); 				
            }
            else
            {
				INPUT_SYS.PushBack();
				t->Tok = Toknot;
                t->col = col;
                t->lexeme[0] = '!';
                t->lexeme[1] = '\0'; 				
            }   			
			
       }   	   
       else if(c == '>')
       {
	        t = Alloc_Newtoken();
			t->line = line; 
			c = INPUT_SYS.GetNextChar();
			if(c == '=')
			{
				col++;
                t->Tok = Tokgthan_eq;
                t->col = col;
                strcpy(t->lexeme,">=");  				
            }
			else
            {
			   INPUT_SYS.PushBack();
			   t->Tok = Tokgreater;
               t->col = col;
               t->lexeme[0] = '>';
               t->lexeme[1] = '\0';   			   
            }
		}	
       else if( c == '<' )
       {  	   
			t = Alloc_Newtoken();
            t->line  = line;
            c = INPUT_SYS.GetNextChar();
            if(c == '=')
            {
				col++;
				t->Tok = Toklthan_eq;
				t->col = col;
                strcpy(t->lexeme,"<=");    				
            }
            else
             {
				INPUT_SYS.PushBack();
				t->Tok = Tokless;
                t->col = col;
                t->lexeme[0] = '<';
                t->lexeme[1] = '\0';    				
             } 			 
       }
     else
     {
	    //fprintf(stderr,"Error: Line %d  Col: %d  invalid character '%c' in input\n",line,col,c);
		error("Error: Line %d  Col: %d  invalid character '%c' in input\n",line,col,c);
		c = INPUT_SYS.GetNextChar();
        col++;    		
     }   	 
   		
    }  	


return t;
    
} 
/* 
struct Token_info *ScanChar(void)
{
	int state,i;
	state == 0;
	int c;
	int p;
	struct Token_info *t;
	c = INPUT_SYS.GetNextChar();
	while(state != 4 && state != -1)
	{	
		switch(state)
		{
			case 0:
			state = 1;
			i++;
			break;
			case 1:
			if(isprint(c))
			{
				state = 2;
				i++;
				p = c;
			}
			else
			{
				fprintf(stderr,"Error: Line %d Col %d invalid character in input",line,col,c);
				state = -1;
				i++; 			
			}
			break;
			case 2:
			if(c == '\'')
			{
				state = 4;
				i++;
			}
			else
			{
				fprintf(stderr,"Error: Line %d Col %d invalid character in input",line,col,c);
				state = -1;
				i++;
			}	
		}
	c = INPUT_SYS.GetNextChar();
	}
	if(state == 4)
	{
		t = Alloc_Newtoken();
		t->Tok = Tokchar;
		t->line = line;
		t->col = col;
		t->lexeme[0] = p;
		t->lexeme[1] = '\0';
       		
		return t;	
	}
	else
	return  NULL;
}
*/
/*state Machine to recognize a Number*/
/*
 * integer				--> "0" - "9" {"0" - "9"}
 *  real				--> integer "." integer
 *
*/ 
static struct Token_info * Number(void)
{
    struct Token_info *t; 
    t = Alloc_Newtoken();
	t->Tok = Toknumber;
    t->line = line;
    t->col = col; 	
	int c;
	int i;
	c = INPUT_SYS.GetNextChar(); 
	int state;
	int real;
      state = real = i = 0;	
	while(1)
	{
	   switch(state)
	   {
			case 0:              //No need to check first character because it already a number
		    state = 1;
			t->lexeme[i++] = c; 
			break;  
            case 1:
			 if(isdigit(c))
             {
			     state =  1;
				 t->lexeme[i++] = c; 
             }			 
             else if(c == '.')
			 {
			    state = 2;
				t->lexeme[i++] = c;
             }
             else
             {
			     state = 4;				 //accepting
				 INPUT_SYS.PushBack(); 
             }
			 break; 
            case 2:
             
				 if(isdigit(c))
                 {
                    state = 3;
                    t->lexeme[i++] = c;  					
              	 }		 
              	else
                {
				   //fprintf(stderr,"Error:Line %d  col %d  Number expected before '%c' \n",line,col + i,c);
				   error("Error:Line %d  col %d  Number expected before '%c' \n",line,col + i,c);
                  	state = -1;			   
                }
             break;
           case 3:
              if(isdigit(c))
              {
				  state = 3;
				  t->lexeme[i++] = c; 
              }
             else
             {
			    state = 4;				//accepting
				real = 1;
				if(c != '\0')
                INPUT_SYS.PushBack();				
             }
            break;    			 
      }	
		if(state == 4 || state == -1)
		{
			break;
		}
         c = INPUT_SYS.GetNextChar();   	  
    }
	col += i;
	if(state ==  -1)
     {
	    Free_Token(t);
		t = NULL;
		INPUT_SYS.PushBack();
		return NULL;	
     }
    else
     { 	
	    t->lexeme[i] = '\0';  
     }
     if(real)
     {
  	   t->Kind = Real;
       t->value.rval = atof(t->lexeme);
     }
     else
     {
	    t->Kind = Integer;
		t->value.ival = atoi(t->lexeme);   
     }   	 
	
   return t; 
}
//identifier           --> letter { letter | digit }
/*state Machine to Recognize an Identifier*/
/* "a" - "z" {"a" - "z" "0" - "9" "_"}
 * 
*/  
static int binsearch(char *word);

struct Token_info * Identifier(void)
{
	struct Token_info *t;
    t = Alloc_Newtoken();
    //t->Tok = Tokvariable;
    t->line = line;
    t->col =  col;
    int state;
    int i ;
	int c;
	 c = INPUT_SYS.GetNextChar();
     state = i = 0;
    while(1)
    { 	
		//c = INPUT_SYS.GetNextChar();
		//S_DEBUGPRINT(printf(" c = %c  \n",c);)
		switch(state)
	    {
			case 0:
			state = 1;
			t->lexeme[i++] = c; 
			
            break;
            case 1:
            if(isalnum(c)|| c == '_')   			
       	    {
				//S_DEBUGPRINT(printf(" ccccccc = %c  \n",c);)
				state = 1;
				t->lexeme[i++] = c; 				
            }
            else
            {
			   state =  3;			   //accepting;
			   if(c != '\0')
			   INPUT_SYS.PushBack();
			  // S_DEBUGPRINT(printf(" final = %d  \n",c);)
			  // S_DEBUGPRINT(printf(" ccccccc = %c  \n",INPUT_SYS.GetNextChar());)
			 // getchar();
 
			}
            break;
                 			
        }
		if(state == 3)
		{
			break;
		}
       c = INPUT_SYS.GetNextChar(); 		
    }
    t->lexeme[i] = '\0';             //Null terminate Lexeme
    col+= i;
	int p;	
	   //Check to see if variable is reserved
	p = binsearch(t->lexeme);
    if(p >= 0)
    {
	    t->Tok = Reserved[p].Tok;
		/*
		if(t->Tok == Toktrue || t->Tok == Tokfalse)
		{
			t->Kind = Boolean;
		}*/
    }
    else
    {
	    t->Tok = Tokvariable; 
    }  	
	   
	return t; 
}   	
static int binsearch(char *word)
{
     int high = RESV_SIZE - 1;
     int low = 0;
      int mid;
       int cond;	  
     while(low <= high)
	 {
	     mid = (high + low)/2;
         if((cond = strcmp(Reserved[mid].p,word)) < 0 )
          {
		      low = mid + 1;
               			  
          }
          else if(cond > 0)
          { 
               high = mid - 1; 		  
          }
          else
             return mid;		  
      }  	 

     return -1;
}
struct scanner LEX = {Drive,GetNextToken};

#ifdef S_DEBUG
static char * Token_set[] = {        
                              "Tokeoln","Tokeof","Toklparen","Tokrparen","Tokfunction","Tokint","Tokfloat","Tokchar","Tokbool","Tokarray","Tokvoid","Toklbrace",
                              "Tokrbrace","Toksemi","Tokcolumn","Tokvar","Tokconst","Tokif","Tokthen", "Tokelse","Tokwhile","Tokdo","Tokequal","Toknequal", 				 
                              "Tokreturn","Tokand","Tokor","Tokcompare","Tokless","Tokgreater","Toklthan_eq","Tokgthan_eq","Toplus","Toktimes","Tokminus","Tokdivide",
							  "Tokmod","Toknot","Toktrue","Tokfalse","Toknumber","Tokvariable","Tokelseif","Tokcolon","Toklarr","Tokrarr","Tokcall","Tokcharconst"
                           };
main(int argc,char *argv[])
{
   char *input = "debug.txt";
   struct Token_info *t;
   LEX.Drive(input);
   int c;
   printf("Token                 Lexeme             Line               col          value\n");  
   while((t = LEX.GetNextToken()) && (t->Tok != Tokeof))
   {
        if(t->Tok == Tokeoln)
        {
		    printf("%15s%15s%15d %15d%15s\n",Token_set[t->Tok],"newline",t->line,t->col,"N|A");
        }
      else
       { 	  
		  printf("%15s%15s%15d %15d",Token_set[t->Tok],t->lexeme,t->line,t->col);
		
		  if(t->Tok == Toknumber)
		 { 
		   if(t->Kind == Real ) 
	        printf("%15f\n",t->value.rval);
           else
          	printf("%15d\n",t->value.ival);	   
         }
        else
		  printf("%12s\n","N|A"); 		
      }
   }	   
}						   
#endif
