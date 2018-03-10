#ifndef __SYMTAB_H__
#define __SYMTAB_H__


#include "scanner.h"
#include "parser.h"

enum SymKind
{
	Var,
	Const,
	Func,
	Array,
};

enum SymType
{
	integer,
	real,
	charac,
	boolean,
	voi,	
};
struct Symbol
{
	struct Token_info *t;
	enum SymKind k;
	enum SymType type;
	struct Expression *e;
	struct Symbol *other_info;
	int scope;
	int arg;  
	int loc;
	int generated;	
};

struct TABLE
{
	struct Symbol * (*insertName)(struct Token_info *);
	struct Symbol *(*findName)(char name[]);
	void  (*init)(void);
	void  (*openScope)(void);
	void  (*closeScope)(void);
	int  (*getScope)(void);	
};
struct Symbol * getNextSymb(int scope );
extern struct TABLE SYMTAB;
#endif
