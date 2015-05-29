/*
 * grep.c
 *
 *  Created on: 29-May-2015
 *      Author: binoy
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 100
#define DEM "( )"


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
	char re[MAX] = "(abc)(de)+";
	char str[MAX]= "abcde";
*/
	char ***tokOp = parse(re);
	printTokOp(tokOp);
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
	fprintf(stderr, "log::info::%s", info);
}

void errUsage(char *progName)
{
	fprintf(stderr, "usage : %s regExpression inputString\n", progName);
	fprintf(stderr, "example : (abc)(de)+ abcede\n");
	exit(1);
}
