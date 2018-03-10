/********************************************** 
*' This source file contains procudures       *
*  for the  symbol table which is basically   * 
*  a stack with each entry being a list  of   * 
*   symbol table records                      *  
*                                             *
*/  /****************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "ast.h"
#include "parser.h"
#include "scanner.h"

#define MAXSTACK_SIZE 8

struct SymtabRec
{
	struct Symbol *symb;
	struct SymtabRec *next;
};
 static int top;
 static int scope;
static struct SymtabRec *symtab[MAXSTACK_SIZE] ;
 
 static void init(void)
{
	top  =  0;
	//printf("\n\nset top = %d   \n\n\n",top);
	int i;
	for(i = 0; i < MAXSTACK_SIZE ; i++ )
	{
		symtab[i] = NULL;
	}
}
 static void openScope(void)
{
	top++;
} 
static void closeScope(void)
{
	symtab[top--] = NULL;
}
static int getScope(void)
{
	return top;
}

struct Symbol * insertName(struct Token_info *t)
{
	//printf("\n\n\n\n Entered insert  ---   %s  ",t->lexeme);
	struct SymtabRec *p;
	struct Symbol *s;
	p = (struct SymtabRec *)malloc(sizeof( struct SymtabRec));
	s = (struct Symbol *)malloc(sizeof(struct Symbol));
	if(p == NULL||s == NULL)
	{
		return NULL;
	}
	s->t = t;
	s->k = -1;
	s->type = -1;
	s->other_info = NULL;
	s->e = NULL;
	s->generated = 0;
	s->loc = 0;
	s->scope = -1;
	if(t->Tok == Toklib)
	{
		s->scope = 5;
		if(strcmp(s->t->lexeme,"ShowMeanTable") == 0)
		{
			s->type = real; 
		}
		else if(strcmp(s->t->lexeme,"get_float") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"get_int") == 0 )
		{
			s->type = integer;
		}
		else if(strcmp(s->t->lexeme,"addints") == 0 )
		{
			s->type = integer;
		}
		else if(strcmp(s->t->lexeme,"multiplyints") == 0 )
		{
			s->type = integer;
		}
		else if(strcmp(s->t->lexeme,"get_float") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"search_index") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"factorial") == 0 )
		{
			s->type = integer;
		}
		else if(strcmp(s->t->lexeme,"permutation") == 0 )
		{
			s->type = integer;
		}
		else if(strcmp(s->t->lexeme,"combination") == 0 )
		{
			s->type = integer;
		}
		else if(strcmp(s->t->lexeme,"ap_nterm") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"gp_nterm") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"gp_suminfinity") == 0)
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"ShowStdTable") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"FC") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"AG") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"PIE") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"SL") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"mini") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"maxi") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"abs_val") == 0 )
		{
			s->type = integer;
		}
		else if(strcmp(s->t->lexeme,"getchar") == 0 )
		{
			s->type = integer;
		}
		else if(strcmp(s->t->lexeme,"ShowStdTable") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"a_circle") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"c_circle") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"a_rectangle") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"a_square") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"c_square") == 0 )
		{
			s->type = real;
		}
		else if(strcmp(s->t->lexeme,"v_cylinder") == 0 )
		{
			s->type = real;
		}
	}
	s->arg = 0;
	p->symb = s;
	//printf("\n\n\n symtab  inserrted         %s  \n\n\n",t->lexeme);
	p->next = symtab[top];
	symtab[top] = p;
	//printf("\n\n%s inserted at top = %d   \n\n\n",s->t->lexeme,top);
	return s;	
}

//Searches stack to check whether a name has already been defined
struct Symbol * findName(char name[])
{
	struct SymtabRec *t;
	int count = top;
	while(count >= 0)
	{
		for(t = symtab[count] ; t != NULL ; t = t->next)
		{
			if(strcmp(name,t->symb->t->lexeme) == 0)
			{
				return t->symb ;
			}
		}
		count--;	
	}
	return NULL;
}

static int first = 0;

struct Symbol * getNextSymb(int scope )
{
	//printf("Enter getNextSymbol \n\n");
	static struct SymtabRec *p ,*t;
	if(first  == 0)
	{
		p = symtab[scope];
		first = 1;
	}
	t = p;
	if(p)
	{
		p = p->next;
	}	
	if(t == NULL)
	{
		first = 0;
		return NULL;	
	}
	//printf("Leave Get Next Symbo with  "  );
	//printf(t->symb == NULL ? "%s\n" :"%s\n",t->symb->t->lexeme," \n");
	return t->symb;
}

struct TABLE SYMTAB = {insertName,findName,init,openScope,closeScope,getScope};
