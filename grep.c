/*
 * grep.c
 *
 *  Created on: 29-May-2015
 *      Author: binoy
 */

#include<stdio.h>
#include<string.h>
#include<limits.h>
#include<stdlib.h>

#define MAX 100
#define DEM "( )"
#define DEBUG 0

void mylog(char *info);
char ***parse(char *re);
int isOperator(char *str);
void errExit(char *errMsg);
void errUsage(char *progName);
void printTokOp(char ***tokOp);
void *check_malloc(size_t size);

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

	char ***tokOp = parse(re);

	if(DEBUG)
		printTokOp(tokOp);

	if(match(tokOp, str))
		printf("match !!!\n");
	else
		printf("no match !!!\n");
	
	return 0;
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

	int min, max;
	switch(*op){
		case '+':
			min = 1;
			max = INT_MAX;
			break;
		case '*':
			min = 0;
			max = INT_MAX;
			break;
	}

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

	return strcmp(str, "+") == 0 || strcmp(str, "*") == 0;
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
