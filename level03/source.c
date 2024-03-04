#include <time.h>
#include <stdlib.h>

void	decrypt(int key) {
	char	buffer[] = "Q}|u`sfg~sf{}|a3";

	for (int i = 0; buffer[i] != '\0'; i++) {
		buffer[i] ^= key;
	}
	if (!strncmp("Congratulations!", buffer, 17))
		system("/bin/sh");
	else
		puts("\nInvalid Password");
}

void	test(int input, int nb) {
	int	key;

	key = nb - input;
	if (key >= 21) {
		decrypt(key);
		return ;
	}
	decrypt(rand());
}

int		main() {
	int	input;

	srand(time(0));
	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf("Password:");
	__isoc99_scanf("%d", &input);
	test(input, 322424845);
	return (0);
}