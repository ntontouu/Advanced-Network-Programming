#include <stdio.h>
#include <string.h>
struct Test{
int len;
char msg[50];
};
void printStruct(struct Test *test1);
int main(){
struct Test tst;
strcpy(tst.msg,"C Programming");
tst.len=strlen(tst.msg);
printStruct(&tst);
return 0;
}
void printStruct(struct Test *test){
printf("Length of Message: %d\n", test->len);
printf("Message: %s\n",test->msg);
}
