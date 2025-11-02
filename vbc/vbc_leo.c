#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

// Version reussie en examen.

int parse_sum();
char *s;

int parse_factor()
{
	if (*s == '\0')
	{
		printf("Unexpected end of input\n");
		exit(0);
	}
	if (isdigit((unsigned char)*s))
		return *s++ - '0';
	if (*s == '(')
	{
		++s;
		int v = parse_sum();
		if (*s == '\0')
		{
			printf("Unexpected end of input\n");
			exit(0);
		}
		if (*s != ')')
		{
			printf("Unexpected token: '%c'\n", *s);
			exit(0);
		}
		++s;
		return (v);
	}
	printf("Unexpected token: '%c'\n", *s);
	exit(0);
}

int parse_product()
{
	int fac1 = parse_factor();
	while (*s == '*')
	{
		++s;
		int fac2 = parse_factor();
		fac1 *= fac2;
	}
	return fac1;
}

int parse_sum()
{
	int prod1 = parse_product();
	while (*s == '+')
	{
		++s;
		int prod2 = parse_product();
		prod1 += prod2;
	}
	return prod1;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	s = argv[1];
	int result = parse_sum();
	if (*s != '\0')
	{
		printf("Unexpected token: '%c'\n", *s);
		return 0;
	}
	printf("%d\n", result);
	return 0;
}