#include<stdio.h>

int
main(int argc, char **argv)
{
	argc--;
	argv++;

	for(int i = 0; i < argc; i++){
		printf("argv: %s\n", argv[i]);
	}

	return 0;
}
