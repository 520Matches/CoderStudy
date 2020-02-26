#include <stdio.h>
#include <string.h>

#define BUFSIZE	64

int main(int argc,char *argv[])
{
	char buf[BUFSIZE];
	printf("Buf:%p\n",&buf);
	strcpy(buf,argv[1]);
	return 0;
}
