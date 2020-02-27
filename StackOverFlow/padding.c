int main(int argc,char *argv[])
{
	void *p;
	char buf[100];
	switch(argv[1][0])
	{
		case '1':
			memset(buf,'\x90',100);
			buf[30] = 0;
			printf("%s",buf);
			break;
		case '2':
			p = (void *)0x7fffffffdd51;
			memset(buf,0,16);
			memcpy(buf,&p,8);
			printf("%s",buf);
			break;
	}
	return 0;
}
