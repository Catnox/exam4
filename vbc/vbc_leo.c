#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

int parse_sum();
char *x;

int parse_factor()
{
	if (*x == '\0')
	{
		printf("Unexpected end of input\n");
		exit(0);
	}
	if (isdigit((unsigned char)*x))
		return *x++ - '0';
	if (*x == '(')
	{
		++x;
		int v = parse_sum();
		if (*x == '\0')
		{
			printf("Unexpected end of input\n");
			exit(0);
		}
		if (*x != ')')
		{
			printf("Unexpected token: '%c'\n", *x);
			exit(0);
		}
		++x;
		return (v);
	}
	printf("Unexpected token: '%c'\n", *x);
	exit(0);
}

int parse_product()
{
	int fac1 = parse_factor();
	while (*x == '*')
	{
		++x;
		int fac2 = parse_factor();
		fac1 *= fac2;
	}
	return fac1;
}

int parse_sum()
{
	int prod1 = parse_product();
	while (*x == '+')
	{
		++x;
		int prod2 = parse_product();
		prod1 += prod2;
	}
	return prod1;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	x = argv[1];
	int result = parse_sum();
	if (*x != '\0')
	{
		printf("Unexpected token: '%c'\n", *x);
		return 0;
	}
	printf("%d\n", result);
	return 0;
}