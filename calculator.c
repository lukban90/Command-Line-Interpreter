#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 256
#define WHTSPC 33
#define EXPSIZE 3
#define ASC '0'

int isOperator(char op);
int calcExpression(int a, int b, char op);

int *getInput(char *in, int *numVal) {

  //int expr[EXPSIZE];// =(int*)malloc(EXPSIZE * sizeof(int));

  if(fgets(in , BUFSIZE, stdin) == NULL) exit(1);

  int leftOperand = 0;
  int rightOperand = 0;
  char operator;
  int ptr=0;

  //skip spaces
  while(in[ptr] < WHTSPC) ptr++;

  //generate leftOperand
  if(isdigit(in[ptr])) {
    leftOperand = in[ptr] - ASC; ptr++;
    while(isdigit(in[ptr])) {
      leftOperand = ((leftOperand*10) + in[ptr]) - ASC;
      ptr++;
    }
  }
  //skip spaces
  while(in[ptr] < WHTSPC) ptr++;

  if(isOperator(in[ptr])) {operator = in[ptr]; ptr++;}

  //skip spaces
  while(in[ptr] < WHTSPC) ptr++;

  //generate rightOperand
  if(isdigit(in[ptr])) {
    rightOperand = in[ptr] - ASC; ptr++;
    while(isdigit(in[ptr])) {
      rightOperand = ((rightOperand*10) + in[ptr]) - ASC;
      ptr++;
    }
  }

  while(in[ptr] < WHTSPC) ptr++;

  numVal[0] = leftOperand; numVal[1] = rightOperand; numVal[2] = operator;

  return numVal;
}

int isOperator(char op) {
  if(op == '+' || op == '-' || op == '*' || op == '/')
    return op;

  return 0;
}

int calcExpression(int a, int b, char op) {

  switch(op) {
    case '+' : return a + b;
    case '-' : return a - b;
    case '*' : return a * b;
    case '/' :
      if(b==0){
        printf("Unable to divide by 0.\n");
        printf("Aborting...\n");
        exit(1);
      }
      return a / b;
  }
  return 0;
}

int main() {

  int leftOperand, rightOperand;
  char operator;

  char in[BUFSIZE];
  int *numVal = (int*)malloc(EXPSIZE * sizeof(int));

  printf("> calc\n");

  while(1) {
    numVal = getInput(in, numVal);
    leftOperand = numVal[0]; rightOperand = numVal[1]; operator = numVal[2];
    int result = calcExpression(leftOperand, rightOperand, operator);

    if(operator != '\0')
      printf("%d %c %d = %d\n",leftOperand, operator, rightOperand, result);
  }
  free(numVal);
}
