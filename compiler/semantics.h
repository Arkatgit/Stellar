#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

struct SEM_
{
	void (*statementSemantics)(struct Statement *);
	void (*functDefSemantics)(struct FunctionDef  *);
	//struct FunctionDef * FUNC;
};

extern struct SEM_  SEM; 
//extern static struct FunctionDef * FUNC;

#endif
