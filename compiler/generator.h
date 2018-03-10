#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "output.h"
#include "ast.h"

struct codegen
			{
				struct output_w  *writer;

			};
extern void EmitAll(struct codegen *g,struct Statement * p);	
extern void EmitGlobalDeclaration(struct codegen  * g );
extern struct codegen  * newGenerator(struct output_w * writer);
extern void EmitFunction(struct codegen *g,struct FunctionDef *f );
void emitLocalDeclaration(struct codegen * g , struct Symbol *s);
 void EmitHeading(struct codegen  * g);
#endif