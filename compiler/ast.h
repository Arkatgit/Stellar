#ifndef __AST_H__
#define __AST_H__

#include "scanner.h"
#include "parser.h"
#include "symtab.h"


enum Exop
{
	And,Or,
	Eq,Lt,Gt,Lte,Gte,Neq,
	Plus,Minus,
	Mul,Div,Mod,
	Uplus,Uminus,Not,
	Fcall,
	Array_Access
};

enum NodeKind
{
	Operator,
	Operand,
};

struct Operator
{
	enum Exop  oper;
	struct Expression *lop;
	struct Expression *rop;
	int line;
	int col;
}; 
struct Operand
{
	struct Token_info *t;
	struct Symbol *s;
	int p;
};

enum StatKind
{
	If,
	Assignment,
	While,
	Return,
	FunctionCall,	
};

struct Expression
{
	enum NodeKind kind;
	enum Type t;
	//int t;  //type
	union
	{
		struct Operator oper;
		struct Operand operand;
	}node;	
	struct Expression *next;
};

struct FunctionDef
{
	struct Symbol *s;
	struct Statement *body;
	struct FunctionDef *next;
};


struct Ifstat
{
	struct Expression *c; 
	struct Statement *then;
	struct Statement *elseif;
	struct Statement *els;
};

struct AssignStat
{
	struct Expression *desc;
	struct  Expression *src; 
}; 

struct WhileStat 
{
	struct Expression *c;
	struct Statement *body;
};
struct ReturnStat
{
	struct Expression *e;
};
/*
struct ArgList
{
	struct Expression *e;
	struct ArgList *next;
};
*/
struct FunctCall
{
	struct Symbol *name;
	struct Expression *arglist;
};


struct Statement
{
	enum StatKind k;
	int line;
	union
	{
		struct AssignStat a;
		struct WhileStat w;
		struct FunctCall f;
		struct ReturnStat r;
		struct Ifstat i;
	}stat;
	struct Statement *next;
};

struct AST_alloc 
{
	struct Statement * (*newStatement)(void);
	struct FunctionDef * (*newFunctionDef)(void);
	struct Expression * (*newExpression)(void);	
};
extern struct AST_alloc AST_ALLOC;
#endif
