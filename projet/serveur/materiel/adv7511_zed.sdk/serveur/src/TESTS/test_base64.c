#include <stdio.h>
#include <string.h>

#include "base64_converter.h"

void test_base64(int details)
{
	char base64[100];
	char test[] = "I'm killing your brain like a poisonous mushroom";
	char expected[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

	ascii_to_base64(test, strlen(test), base64);

	if (details)
	{
		printf("testing ascii_to_base64...\n");
		printf("input:    %s\n", test);
		printf("output:   %s\n", base64);
		printf("expected: %s\n", expected);
	}

	if (strcmp(base64, expected) == 0)
		printf("ascii_to_base64: test passed.\n");
	else
		printf("ascii_to_base64: test failed.\n");
}