/*
 * This File Contains the functions for Semantic Analysis
 * it checks source file for semantic correctness according to 
 * the semantic definition/specification of the source language
*/


#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parser.h"
#include "symtab.h"
#include "semantics.h"
#include "error.h"

//Debug prints
#ifdef P_DEBUG 
#define PRINT(x) x
#else 
#define PRINT(x) 
#endif

#ifdef P_DEBUG

static char *stat[]  = {"If","Assign","While","return","functioncall" }; 

#endif

static void ifSemantics(struct Statement *p);
static void whileSemantics(struct Statement *p);
static void assignSemantics(struct Statement *p);
static void returnSemantics(struct Statement *p);
static void expressionSemantics(struct Expression *p);
static void functCallSemantics(struct Statement *p);
 static int isAssignCompatible(int a,int b);
static void statementSemantics(struct Statement * p)
{
	//PRINT(printf("Enter   Mainstatment  with - '%s' line %d    \n",stat[p->k],p->line);)
	while(p != NULL)
	{
	//PRINT(printf("Enter '%s'  statment line %d    \n",stat[p->k],p->line);)
		switch(p->k)
		{
			case If:
				ifSemantics(p);		//check if statement semantics
				break;
			case While:
				whileSemantics(p);	//check while statement semantics	
				break;
			case Assignment:
				assignSemantics(p);		//check assignment statement semantics 	
				break;
			case Return:
				returnSemantics(p);		//check return statement semantics
				break;
			case FunctionCall:
				functCallSemantics(p);	//check function call semantics
				break;
				
		}
		//PRINT(printf("Enter '%s'  line %d    \n",stat[p->k],p->line);)
		p = p->next;
	}
	PRINT(printf("Leave Main - statement   \n");)
}
//array
//if statement semantics checker	
static void ifSemantics(struct Statement *p)
{
	PRINT(printf("Enter 'ifstatment' line %d  \n",p->line);)
	struct Statement *t;
	expressionSemantics(p->stat.i.c);
	if(p->stat.i.c)
	{
		if(p->stat.i.c->t !=  Boolean_t)
		{
			error("error: Line %d  'statement' condition must evaluate to a boolean value  \n",p->line);
			//fprintf(stderr,"Errror: Line %d  'statement' condition must evaluate to a boolean value  \n",p->line);   //error//error
		}	
	}
	statementSemantics(p->stat.i.then);
	
	for(t = p->stat.i.elseif; t != NULL ; t = t->next)
	{
		//ifSemantics(t);
		expressionSemantics(t->stat.i.c);
		if(t->stat.i.c)
		{
			if(t->stat.i.c->t !=  Boolean_t)
			{
				error("error: Line %d  'statement' condition must evaluate to a boolean value  \n",p->line);
				//fprintf(stderr,"Errror: Line %d  'statement' condition must evaluate to a boolean value  \n",p->line);   //error//error
				statementSemantics(t->stat.i.then);
			}	
		}
		
	}
	statementSemantics(p->stat.i.els);
	PRINT(printf("Leave 'ifstatment' line %d  \n",p->line);)
	return;
}

static void whileSemantics(struct Statement *p)
{
	PRINT(printf("Enter 'whilestatment' line %d  \n",p->line);)
	expressionSemantics(p->stat.w.c);
	//printf("\n\nherea\n\n");
	if(p->stat.w.c)
	{
		if(p->stat.w.c->t != Boolean_t)
		{
			error("error: Line %d  'while statement' condition must evaluate to a boolean value  \n",p->line);
			//fprintf(stderr,"Errror: Line %d  'while statement' condition must evaluate to a boolean value  \n",p->line);   //error
		}
	}	
	statementSemantics(p->stat.w.body);
	PRINT(printf("Leave 'whilestatment' line %d  \n",p->line);)
	return;
}

static void assignSemantics(struct Statement *p)
{
	PRINT(printf("Enter 'assignment statement' line %d  \n",p->line);)
	expressionSemantics(p->stat.a.desc);
	if(p->stat.a.desc == NULL)
	{
		 return;           //error a = 2; b[2] = 9; f(20) = 4;x 4 = 4;x
	}
	if(p->stat.a.desc->kind == Operand )
	{
		if(p->stat.a.desc->node.operand.s)
		{
			if(p->stat.a.desc->node.operand.s->k == Const)
			{
				struct Symbol *r;
				r = p->stat.a.desc->node.operand.s;
				//fprintf(stderr,"error: Line %d  assigning to a constant variable '%s' defined on line %d  Col %d\n",p->line,r->t->lexeme,r->t->line,r->t->col);
				error("error: Line %d  assigning to a constant variable '%s' defined on line %d  Col %d\n",p->line,r->t->lexeme,r->t->line,r->t->col);
			}
			PRINT(printf("hellow \n\n");)
			if(p->stat.a.desc->node.operand.s->k == Func)
			{
				struct Symbol *r;
				r = p->stat.a.desc->node.operand.s;
				//fprintf(stderr,"Error: Line %d  assigning to a function named '%s' \n",p->line,r->t->lexeme);
				error("error: Line %d  assigning to a function named '%s' \n",p->line,r->t->lexeme);
			}
		}		
	}
	
	if(p->stat.a.src == NULL)
	{
		return;
	}
	expressionSemantics(p->stat.a.src);
	
	if(p->stat.a.desc->t == -1)
	{
		p->stat.a.desc->t = p->stat.a.src->t;
	}
	if(p->stat.a.desc->kind == Operator)
	{
		if(p->stat.a.desc->node.oper.oper == Array_Access)
		{
			PRINT(printf("\n\n\n Line = %d  Source type = %d , destination type =  %d    \n\n\n",p->line,p->stat.a.src->t,p->stat.a.desc->node.oper.lop->t);)
			if(isAssignCompatible(p->stat.a.desc->node.oper.lop->t, p->stat.a.src->t) == 0)
			error("error: Line %d assignment of incompatible type to array index  \n",p->line);
			//fprintf(stderr,"Error: Line %d assignment of incompatible type to array index  \n",p->line);
		}
	}
	else
	{
		PRINT(printf("\n\n\n Line = %d  Source type = %d , destination type =  %d    \n\n\n",p->line,p->stat.a.src->t, p->stat.a.desc->t);)
		if(isAssignCompatible(p->stat.a.desc->t, p->stat.a.src->t) == 0)
		{
			error("error: Line %d assignment of incompatible types   \n",p->line);
			//fprintf(stderr,"Error: Line %d assignment of incompatible types   \n",p->line );//error
		}
		}
	PRINT(printf("Leave 'assign statment ' line %d  \n",p->line);)
}

static int isAssignCompatible(int a,int b)
{
	int flag ;
	if(a == b)
	{
		flag = 1;   //same type
	}
	
	else if( a == Real_t && b == Integer_t )
	{
		flag = 1;
	}
	else if(a == Integer_t && b == Real_t)
	{
		flag = 1;
		//warning 
	}
	else if(a == Integer_t && b == Charconst_t)
	{
		flag = 1;
	}
	else if(a == Charconst_t  && b == Integer_t)
	{
		flag = 1;
	}
	else if(a == real && b == integer)
	{
		flag = 1;
	}
	else if(a == integer && b == real)
    {
		flag = 1;
		//warning
		
	}
	else if(a == integer && b == charac)
	{
		flag = 1;
	}
	else if(a == charac && b == integer)
	{
		flag = 1;
	}
	else if(a == Integer_t && b == integer || a == integer && b == Integer_t)
	{
		flag = 1;
	}
	else if(a == Real_t && b == real || a == real && b == Real_t)
	{
		flag = 1;
	}
	else if(a == Charconst_t && b == charac ||a == charac && b == Charconst_t)
	{
		flag = 1;
	}
	else if(a == Real_t && b == integer || a == real  && b == Integer_t )
	{
		flag = 1;
	}
	else if(a == Integer_t && b == real || b == integer && b == Real_t )
	{
		flag = 1;
	}
	else if(a == integer && b == Charconst_t || a == Integer && b == charac )
	{
		flag = 1;
	}
	else if(a == Charconst_t && b ==integer )
	{
		flag = 1;
	}
	/*else if(a == integer  && b ==  Integer_t )
	{
		flag = 1;
	}*/
	else if(a == Charconst_t && b == integer || a == charac && b == Integer_t) //new
	{
		flag = 1;
		//warning;
	}
	else if(a == voi && b == Void_t ||a == Void_t && b == voi )
	{
		flag == 1;
	}
	else if(a == Boolean_t && b == boolean || a == boolean && b == Boolean_t )
	{
		flag = 1;
	}
	else
	{
		flag = 0;
	}
	return flag == 1;
}

static void returnSemantics(struct Statement *p)
{
	PRINT(printf("Enter 'returnstatment' line %d  \n",p->line);)
	//if(FunctionDef->s == NULL) error
	expressionSemantics(p->stat.r.e);
	if(FUNC == NULL)
	{
		error("error: Line %d return statement can only be applied from within a function\n",p->line);
		//fprintf(stderr,"Error: Line %d return statement can only be applied from within a function\n",p->line);
	}
	if(FUNC)
	{
		if(isAssignCompatible(p->stat.r.e->t,FUNC->s->type) == 0)
		{
			error("Error Line %d  return type and its  corresponding function '%s' return type incompactible",p->line,FUNC->s->t->lexeme);
			//fprintf(stderr,"Error Line %d  return type and its  corresponding function '%s' return type incompactible",p->line,FUNC->s->t->lexeme );//error
		}
	}	
	PRINT(printf("leave 'return' line %d  \n",p->line);)
}

static void functCallSemantics(struct Statement *p)
{
	PRINT(printf("Enter 'functioncall' line %d  \n",p->line);)
	struct Expression *e;
	struct Symbol *arg;
	int i = 1;
	if(p->stat.f.name)
	{
		return;
	}
	
if(p->stat.f.name->t->Tok != Toklib)
{	
	for(e = p->stat.f.arglist, arg = p->stat.f.name->other_info; e!=NULL && arg!=NULL; )
	{
		expressionSemantics(e);
		PRINT(printf("hellow \n\n");)
		if(isAssignCompatible(arg->type,e->t ) == 0) 
		{
		    // fprintf(stderr,"\n\n\n   Source type = %d , destination type =  %d    \n\n\n",arg->type, e->t);
			error("error:Line  %d argument %d of function '%s' defined on line %d col %d and its definition are of incompatible types\n", 
			 p->line,i,
			 p->stat.f.name->t->lexeme,p->stat.f.name->t->line,p->stat.f.name->t->col);
			//fprintf(stderr,"error:Line  %d argument %d of function '%s' defined on line %d col %d and its definition are of incompatible types\n", 
			// p->line,i,p->stat.f.name->t->lexeme,p->stat.f.name->t->line,p->stat.f.name->t->col);
		}
		e = e->next;
		arg = arg->other_info;
		i++;
	}
	if(e && !arg)
	{
		//printf("\n\nhere1 \n\n");
		//if(p->stat.f.name)
		error("error: Line %d  too many arguments in call to function '%s' defined on Line %d Col %d\n",
		p->line,p->stat.f.name->t->lexeme,
		p->stat.f.name->t->line,
		p->stat.f.name->t->col);
		//fprintf(stderr,"error: Line %d  too many arguments in call to function '%s' defined on Line %d Col %d\n",
		//p->line,p->stat.f.name->t->lexeme,p->stat.f.name->t->line,p->stat.f.name->t->col); 
	}	
	else if(arg && !e)
	{
		printf("\n\nhere2\n\n");
		//if(p->stat.f.name)
		error("error: Line %d  too few arguments in call to function '%s' defined on line Col %d\n",
		p->line,p->stat.f.name->t->lexeme,
		p->stat.f.name->t->line,
		p->stat.f.name->t->col);
		//fprintf(stderr,"error: Line %d  too few arguments in call to function '%s' defined on line Col %d\n",
		//p->line,p->stat.f.name->t->lexeme,
		//p->stat.f.name->t->line,
		//p->stat.f.name->t->col
		//); 
	}
}	
	PRINT(printf("Leave 'fucntioncall' line %d  \n",p->line);)
}



static void functDefSemantics(struct FunctionDef *f)
{
	PRINT(printf("Enter 'fucntion definition' line %d  \n",f->s->t->line);)
	struct Statement *s;
	statementSemantics(f->body);
	PRINT(printf("Leave  'fuction defintion' line %d  \n");)
	return;
}

static int isBinaryOp(enum Exop t)
{
	int flag = 1;
	if(t == Uplus || t == Uminus || t == Not)
	{
		flag = 0;
	}

	return flag;
}

static void Is_Plus_Minus_Mul(struct Expression *p)
{
     PRINT(printf("Enter isplusminus  Line %d\n",p->node.oper.line);)
	//addition,subtraction or mutiplication of or by a boolean disallowed
if(	p->node.oper.lop  &&  p->node.oper.lop)
{
	if(p->node.oper.lop->t == Boolean_t || p->node.oper.rop->t == Boolean_t)
	{
		error("error :  Line %d   Col %d  invalid argument to operator \n",p->node.oper.line,p->node.oper.col);
		//fprintf(stderr,"error :  Line %d   Col %d  invalid argument to operator \n",p->node.oper.line,p->node.oper.col);                  //error
	}
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Integer_t)
	{
		p->t = Integer_t;
	}
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Charconst_t || p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t == Integer_t)
	{
		p->t = Integer_t;
	}
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Real_t || p->node.oper.lop->t == Real_t && p->node.oper.rop->t == Integer_t)
	{
		p->t = Real_t;
	}
	if(p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t  == Charconst_t)
	{
		p->t = Charconst_t;
	}
	if(p->node.oper.lop->t == Real_t && p->node.oper.rop->t  == Real_t)
	{
		p->t = Real_t;
	}
	if(p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t == Real_t || p->node.oper.lop->t == Real_t && p->node.oper.rop->t == Charconst_t)
	{
		p->t = Real_t;
	}
}	
	PRINT(printf("Leave isplusminus  Line %d\n",p->node.oper.line);)
}

static void Division(struct Expression *p)
{
	PRINT(printf("Enter division  Line %d\n",p->node.oper.line);)
	//division of or by a boolean disallowed
	if(	p->node.oper.lop  &&  p->node.oper.lop)
{
	if(p->node.oper.lop->t == Boolean_t || p->node.oper.rop->t == Boolean_t)
	{
		error("error :  Line %d   Col %d  invalid argument to '/' \n",p->node.oper.line,p->node.oper.col);
		//fprintf(stderr,"Error :  Line %d   Col %d  invalid argument to '/' \n",p->node.oper.line,p->node.oper.col); //error
	}
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Integer_t)
	{
		p->t = Integer_t;
	}
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Charconst_t || p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t == Integer_t)
	{
		p->t = Integer_t;
	}
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Real_t || p->node.oper.lop->t == Real_t && p->node.oper.rop->t == Integer_t)
	{
		p->t = Real_t;
	}
	if(p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t  == Charconst_t)
	{
		p->t = Charconst_t;
	}
	if(p->node.oper.lop->t == Real_t && p->node.oper.rop->t  == Real_t)
	{
		p->t = Real_t;
	}
	if(p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t == Real_t || p->node.oper.lop->t == Real_t && p->node.oper.rop->t == Charconst_t)
	{
		p->t = Real_t;
	}
	
}	
	
	PRINT(printf("Leave Division  Line %d\n",p->node.oper.line);)
}

static void Modulo(struct Expression *p)
{
	PRINT(printf("Enter modulo  Line %d\n",p->node.oper.line);)
	
if(	p->node.oper.lop  &&  p->node.oper.lop)
{	
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Integer_t)
	{
		p->t = Integer_t; 
	}
	else if(p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t == Charconst_t)
	{
		p->t = Charconst_t;
	}
	else if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Charconst_t || p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t == Integer_t )
	{
		p->t = Integer_t;
	}
	else
	{
		error("error :  Line %d   Col %d  invalid argument to 'mod' \n",p->node.oper.line,p->node.oper.col);
		//fprintf(stderr,"Error :  Line %d   Col %d  invalid argument to 'mod' \n",p->node.oper.line,p->node.oper.col); //error
	}
}	
	PRINT(printf("Leave Modulo  Line %d\n",p->node.oper.line);)
}

static void Relational(struct Expression *p)
{
	PRINT(printf("Enter Relational  Line \n");)
	if(	p->node.oper.lop  &&  p->node.oper.lop)
{
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Boolean_t ||p->node.oper.lop->t == Boolean_t && p->node.oper.rop->t == Integer_t)
	{
		error("error :  Line %d   Col %d  invalid argument to relationnal operator \n",p->node.oper.line,p->node.oper.col);
		//fprintf(stderr,"Error :  Line %d   Col %d  invalid argument to relationnal operator \n",p->node.oper.line,p->node.oper.col); //errror
	}
	if(p->node.oper.lop->t == Real_t && p->node.oper.rop->t == Boolean_t ||p->node.oper.lop->t == Boolean_t && p->node.oper.rop->t == Real_t)
	{
		error("error :  Line %d   Col %d  invalid argument to relationnal operator \n",p->node.oper.line,p->node.oper.col);
		//fprintf(stderr,"Error :  Line %d   Col %d  invalid argument to relationnal operator \n",p->node.oper.line,p->node.oper.col); //error	
	}
	if(p->node.oper.lop->t == Real_t && p->node.oper.rop->t == Boolean_t ||p->node.oper.lop->t == Boolean_t && p->node.oper.rop->t == Real_t)
	{ 
		error("error :  Line %d   Col %d  invalid argument to relationnal operator \n",p->node.oper.line,p->node.oper.col);
		//fprintf(stderr,"Error :  Line %d   Col %d  invalid argument to relationnal operator \n",p->node.oper.line,p->node.oper.col); //error
	}
	if(p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t == Boolean_t ||p->node.oper.lop->t == Boolean_t && p->node.oper.rop->t == Charconst_t)
	{ 
		error("Error :  Line %d   Col %d  invalid argument to relationnal operator \n",p->node.oper.line,p->node.oper.col);
		//fprintf(stderr,"Error :  Line %d   Col %d  invalid argument to relationnal operator \n",p->node.oper.line,p->node.oper.col);//error
	}
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Integer_t)
	{
		p->t = Boolean_t;
	}
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Real_t || p->node.oper.lop->t == Real_t && p->node.oper.rop->t == Integer_t)
	{
		p->t = Boolean_t;
	}
	if(p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t  == Charconst_t)
	{
		p->t = Boolean_t;
	}
	if(p->node.oper.lop->t == Real_t && p->node.oper.rop->t  == Real_t)
	{
		p->t = Boolean_t;
	}
	if(p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t == Real_t || p->node.oper.lop->t == Real_t && p->node.oper.rop->t == Charconst_t)
	{
		p->t = Boolean_t;
	}
	
	if(p->node.oper.lop->t == Integer_t && p->node.oper.rop->t == Charconst_t || p->node.oper.lop->t == Charconst_t && p->node.oper.rop->t == Integer_t)
	{
		p->t = Boolean_t;
	}
}	
	PRINT(printf("Leave Relational  Line \n");)
}

static void Logicals(struct Expression *p)
{
	PRINT(printf("Enter Logicals  Line %d\n",p->node.oper.line);)
if(	p->node.oper.lop  &&  p->node.oper.lop)
{	
	if(p->node.oper.lop->t == Boolean_t && p->node.oper.rop->t == Boolean_t)
	{ 
		p->t = Boolean_t;
	}
	else
	{
		error("error :  Line %d   Col %d  invalid argument to  Logical operator \n",p->node.oper.line,p->node.oper.col);
		//fprintf(stderr,"Error :  Line %d   Col %d  invalid argument to  Logical operator \n",p->node.oper.line,p->node.oper.col); //error
	}
	
}	
	
	PRINT(printf("Leave Logicals  Line %d\n",p->node.oper.line);)
}

static void Unary(struct Expression *p)
{
	//PRINT(printf("Enter Unary  Line %d\n",p->node.oper.line);)
	
	if(p->node.oper.rop->t  == Boolean_t)
	{
		error("Error :  Line %d   Col %d  invalid argument to unary operator \n",p->node.oper.line,p->node.oper.col);
		//fprintf(stderr,"Error :  Line %d   Col %d  invalid argument to unary operator \n",p->node.oper.line,p->node.oper.col);//error
	}
	//PRINT(printf("Leave Unary  Line %d\n",p->node.oper.line);)
}

static void expressionSemantics(struct Expression *e)
{
	PRINT(printf("Enter expression  Line  with  kind  %d\n",1);)
     	
	if(e)
	{
		if(e->kind == Operator)
		{
			if(isBinaryOp(e->node.oper.oper))
			{
				PRINT(printf("Enter expression - bianary  Line %d \n",e->node.oper.line);)
				expressionSemantics(e->node.oper.lop);
				expressionSemantics(e->node.oper.rop);
				if (e->node.oper.oper == Plus || e->node.oper.oper == Minus ||e->node.oper.oper == Mul ) //plus,minus,multiplication  
				{
					PRINT(printf("if - isplusminus \n");)
					Is_Plus_Minus_Mul(e);
				}
				if(e->node.oper.oper == Div)  //div
				{
					PRINT(printf("if - division \n");)
					Division(e);	
				}
				if(e->node.oper.oper ==  Mod ) //modulo
				{
					PRINT(printf("if - modulo \n");)
					Modulo(e);
				}
				
				if(e->node.oper.oper == And || e->node.oper.oper == Or) //logical operators
				{
					PRINT(printf("if - logicals \n");)
					Logicals(e);
				}
				
				if (e->node.oper.oper == Array_Access)
				{
					if(e->node.oper.lop->node.operand.s->k != Array) 
					{
						error("error: Line %d  Col %d  %s not of type 'array'\n",e->node.oper.line,e->node.oper.col,e->node.oper.lop->node.operand.s->t->lexeme );
						//fprintf(stderr,"Error: Line %d  Col %d  %s not of type 'array'\n",e->node.oper.line,e->node.oper.col,e->node.oper.lop->node.operand.s->t->lexeme );
					}
				    if(e->node.oper.rop->t != Integer_t)
				   {
						error("error: Line  %d  Col %d array index must evaluate to an integer \n",e->node.oper.line,e->node.oper.col);
					 // fprintf(stderr,"Error: Line  %d  Col %d array index must evaluate to an integer \n",e->node.oper.line,e->node.oper.col);
				   }	
				}
	
				if(e->node.oper.oper == Eq ||e->node.oper.oper == Lt||e->node.oper.oper == Gt||e->node.oper.oper ==Lte||e->node.oper.oper ==Gte||e->node.oper.oper ==Neq)
				{
					PRINT(printf("\n\n if- relationals \n\n");)
					Relational(e);
				}
				
				if(e->node.oper.oper == Fcall)
				{
					PRINT(printf("if - Fcall \n");)
					if(e->node.oper.lop->node.operand.s->type  == voi)  //fuction returning nothing are not to be used in expressions
					{
						struct Symbol *s  = e->node.oper.lop->node.operand.s;
						error("error : Line  %d  Col %d  Functioin returning void used in expression \n",s->t->line,s->t->col);
						//fprintf(stderr,"ERROR : Line  %d  Col %d  Functioin returning void used in expression \n",s->t->line,s->t->col);
					}		
				}
				
			}
			if(!isBinaryOp(e->node.oper.oper))
			{
				PRINT(printf("Enter  unary\n");)
				expressionSemantics(e->node.oper.rop);
				if(e->node.oper.oper  == Uplus ||e->node.oper.oper == Uminus || e->node.oper.oper == Not)
				{
					Unary(e);
				}	
			}	
		}

	}
PRINT(printf("Leave expression  Line \n");)
}

 struct SEM_  SEM = {statementSemantics,functDefSemantics};




		/*else if(e->kind == Operand)
		{
			if(e->node.operand.t->Kind  == Integer)
			{
				e->type = Integer_t;			
			}
			else if(e->node.operand.t->Kind  == Real)
			{
				e->type = Float;  //if float literal,
			}
			else if(e->node.operand.t->kind  == Boolean )
			{
				e->type = Boolean;  //if boolean literal,
			}
			else
			{
				e->t = e->node.oprand.s->type;
			}
		}
*/

