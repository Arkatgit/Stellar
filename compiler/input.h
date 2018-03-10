
//Header for the input system
 

#ifndef __INPUT_H__
#define __INPUT_H__

struct  InputRead
           {
               int (*GetNextChar)(void);
               void (*PushBack)(void);
               void (*Init)(char *);
           };		

extern struct InputRead INPUT_SYS;
#endif
 