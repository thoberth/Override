#include <stdio.h>
#include <stdlib.h>

int		main(void) {
	char	buffer[100];

	fgets(buffer, 100, stdin);
	for (int i = -1; buffer[i] != '\0'; ++i)
	{
		if (buffer[i] > 64 && buffer[i] <= 90)
			buffer[i] ^= 32;
	}
	printf(buffer);
	exit(0);
}