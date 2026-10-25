#include <stdio.h>
#include <string.h>
struct Test{
	int len;
	char msg[50];
};
void printStruct(struct Test *test1);
int main(int argc, char *argv[]){
	struct Test tst;
	if (argc ==2){
		strncpy(tst.msg,argv[1],49);
		tst.msg[49]='\0';
		tst.len=strlen(tst.msg);
	printStruct(&tst);
}
else if (argc == 1){
	strcpy(tst.msg,"C Programming");
	tst.len=strlen(tst.msg);
	printStruct(&tst);
	}
	else{
		printf("Too many argumnets supplied. Usage: ./struct_exec <Your Message> or just ./struct_exec\n");
	}
	return 0;
}
void printStruct(struct Test *test){
	printf("Lenght of message: %d\n", test->len);
	printf("Message:%s\n",test->msg);
}
