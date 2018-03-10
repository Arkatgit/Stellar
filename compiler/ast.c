#include "ast.h"
#include<stdio.h>

struct Statement * newStatement(void)
{
	struct Statement *p;
	p = (struct Statement *)malloc(sizeof(struct Statement));
	if(p != NULL)
	{
		p->k = -1;
		p->next = NULL;
	}
	return p;
}

struct FunctionDef * newFunctionDef(void)
{
	struct FunctionDef * temp;
	temp = (struct FunctionDef  *)malloc(sizeof(struct FunctionDef));
	if(temp != NULL)
	{
		temp->next = NULL;
		temp->body = NULL;
		temp->s = NULL;	
	}
	return temp;	
}

struct Expression * newExpression(void)  
{
	struct Expression *temp;
	temp = (struct Expression *)malloc(sizeof(struct Expression));	
	if(temp != NULL)
	{
		temp->kind = temp->t = -1;
		temp->next = NULL;
	}
	return temp;
}

struct AST_alloc AST_ALLOC  = {newStatement , newFunctionDef , newExpression}; 
