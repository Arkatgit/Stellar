#ifndef __OUTPUT_H__
#define __OUTPUT_H__

//#define BUFSIZE	5120   //5k buffer buffer size
#define BUFSIZE	5000
struct	output_w
{
	int fd; 			//file descriptor
	char * outfile;		//output file
	char output[BUFSIZE];	//buffer to temporary hold input  	
	int bfptr ;			//pointer to current position in buffer
	int state;
	void (*writes)(struct output_w *,char *,... );
};

extern struct output_w * newOutput(char * file);
//extern struct output_w Out;

#endif
