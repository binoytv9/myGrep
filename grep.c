/*
 * grep.c
 *
 *  Created on: 29-May-2015
 *      Author: binoy
 */

#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<limits.h>
#include<stdlib.h>

#define MAX 100
#define DEM "( )"
#define DEBUG 0

/* represent each operator */
struct operator{
	int min;
	int max;
};

void mylog(char *info);
char ***parse(char *re);
void loadOperator(void);
int isOperator(char *str);
void errExit(char *errMsg);
void errUsage(char *progName);
void printTokOp(char ***tokOp);
void *check_malloc(size_t size);
char split(char *buf, struct operator *prop);

struct operator *opTable[128];	/* storing operator and its related properties */

int main(int argc, char *argv[])
{
	mylog("in main function\n");

	if(argc != 3)
		errUsage(argv[0]);

	char *re = argv[1];
	char *str = argv[2];

/*
	char re[MAX] = "(abc)(de)+(abc)*";
	char str[MAX]= "abcdedededededededeabc";
*/

	loadOperator();

	char ***tokOp = parse(re);

	if(DEBUG)
		printTokOp(tokOp);

	if(match(tokOp, str))
		printf("match !!!\n");
	else
		printf("no match !!!\n");
	
	return 0;
}

void loadOperator(void)
{
	FILE *fd = fopen("operators.config", "r");

	int numBytes;
	char buf[MAX];
	while(fgets(buf, sizeof buf, fd) != NULL){
		struct operator *prop = check_malloc(sizeof(struct operator));
		char op = split(buf, prop);
		opTable[op] = prop;
	}
}

char split(char *buf, struct operator *prop)
{
	char *de = " ";
	char *token = strtok(buf, de);
	char op = *token;

	token = strtok(NULL, de);
	prop->min = atoi(token);

	if((token = strtok(NULL, de)) == NULL)
		prop->max = INT_MAX;
	else
		prop->max = atoi(token);

	if(DEBUG)
		printf("%c %d %d\n", op, prop->min, prop->max);

	return op;
}

char ***parse(char *re)
{
	mylog("in parse function\n");

	char *str1 = re;
	char *next = NULL;

	char *prev = strtok(str1, DEM);
	if(prev == NULL)
		return NULL;

	int i = 0;
	char ***tokOp = check_malloc(MAX * sizeof(char **));
	while((next = strtok(NULL, DEM)) != NULL){
		if(isOperator(prev)){
			prev = next;
			continue;
		}

		char **arr = check_malloc(2 * sizeof(char *));
		
		arr[0] = prev;
		
		if(isOperator(next)){
			arr[1] = next;
		}
		else{
			arr[1] = NULL;
		}
		
		prev = next;
		tokOp[i++] = arr;
	}
	tokOp[i] = NULL;

	return tokOp;
}

int isOperator(char *str)
{
	mylog("in isOperator function\n");

	return opTable[*str] != NULL;
}

int match(char ***tokOp, char *str)
{
	while(*tokOp != NULL){
		char *s = (*tokOp)[0];
		char *op = (*tokOp)[1];

		if(DEBUG){
			char *tmp = op == NULL ? " " : op;
			printf("[%s, %s]\n", s, tmp);
		}

		if(op == NULL){
			if(!strStr(&str,s))
				return 0;
		}
		else{
			if(!regStr(&str, *tokOp))
				return 0;
		}
		tokOp++;
	}

	if(*tokOp == NULL && *str == '\0')
		return 1;
	else
		return 0;
}

int strStr(char **strRef, char *s)
{
	char *str = *strRef;

	if(DEBUG)
		printf("%s %s\n", str, s);

	while(*str != '\0' && *s != '\0'){
		if(*str != *s)
			return 0;
		s++;
		str++;
	}

	*strRef = str;
	if(*s == '\0')
		return 1;
	else
		return 0;
}

int regStr(char **strRef, char **tokOp)
{
	char *str = *strRef;
	char *s = tokOp[0];
	char *op = tokOp[1];

	struct operator *prop = opTable[*op];
	int min = prop->min;
	int max = prop->max;

	while(min--){
		if(!strStr(strRef, s))
			return 0;
	}

	while(max--){
		char *tmp = *strRef;
		if(!strStr(strRef, s)){
			*strRef = tmp;
			return 1;
		}
	}

	return 1;
}

void printTokOp(char ***tokOp)
{
	mylog("in printTokOp function\n");

	char **tokop;
	while((tokop = *tokOp++) != NULL){
		char *op = tokop[1] == NULL ? " " : tokop[1];
		printf("[%s, %s]\n", tokop[0], op);
	}
}

void *check_malloc(size_t size)
{
	mylog("in check_malloc function\n");

	void *new = NULL;

	if((new = malloc(size)) == NULL)
			errExit("malloc");
	return new;
}

void errExit(char *errMsg)
{
	mylog("in errExit function\n");

	perror(errMsg);
	exit(1);
}

void mylog(char *info)
{
	if(DEBUG)
		fprintf(stderr, "log::info::%s", info);
}

void errUsage(char *progName)
{
	fprintf(stderr, "usage : %s regExpression inputString\n", progName);
	fprintf(stderr, "example : (abc)(de)+ abcede\n");
	exit(1);
}
