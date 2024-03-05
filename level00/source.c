#include <stdio.h>
#include <stdlib.h>

int main(){
	int pwd;
	puts("***********************************");
	puts("* \t     -Level00 -\t\t  *");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &pwd);
	if (5249 != pwd) {
		puts("\nAuthenticated!");
		system("/bin/sh");
		return 0;
	}
	else {
		puts("\nInvalid Password !");
		return 1;
	}
}