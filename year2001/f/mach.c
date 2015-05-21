#include<stdio.h>
#include<stdlib.h>

#define LDA    0
#define STA    1
#define SWP    2
#define ADD    3
#define INC    4
#define DEC    5
#define BEZ    6
#define BRA    7
#define STP    8

#define LENGTH 256

#define verbose 1

char FILE_IN[] = "mach.in";
char FILE_OUT[] = "mach.out";
int memory[LENGTH];

void read(void);
void write(void);
void inc_pc(int* pc);
char letter(int num);
char number(int let);

/******************************************************************
main
*******************************************************************/
void main(void)
{
  int pc; /* program counter */
  int acc_A;  /* accumulator A */
  int acc_B;  /* accumulator B */
  int stop;

  /* Read memory. */
  read();

  /* Set program counter to beginning of program. */
  pc = 0;
  /* Initialize accumulators to 0. */
  acc_A = 0;
  acc_B = 0;

  /* Execute program */
  stop = 0;
  while (stop == 0) {
    int command;
    int temp;
    int add1, add2, address;

    if (verbose)
      printf("%d\t", pc);

    /* Get the next command. */
    command = memory[pc];
    inc_pc(&pc);

    switch(command)
      {
      case LDA : /* Load A */
	add1 = memory[pc];
	inc_pc(&pc);
	add2 = memory[pc];
	inc_pc(&pc);
	address = 16 * add1 + add2;
	acc_A = memory[address];
	if (verbose)
	  printf("Loading A from %d, A := %d\n", address, acc_A);
	break;

      case STA : /* Store A */
	add1 = memory[pc];
	inc_pc(&pc);
	add2 = memory[pc];
	inc_pc(&pc);
	address = 16 * add1 + add2;
	memory[address] = acc_A;
	if (verbose)
	  printf("Storing A to %d\n", address);
	break;

      case SWP : /* Swap accumulators */
	temp = acc_B;
	acc_B = acc_A;
	acc_A = temp;
	if (verbose)
	  printf("Swapping, A := %d, B := %d\n", acc_A, acc_B);
	break;

      case ADD : /* Add accumulators, low in A, high in B*/
	temp = acc_A + acc_B;
	acc_A = temp % 16;
	acc_B = temp / 16;
	if (verbose)
	  printf("Adding, A := %d, B := %d\n", acc_A, acc_B);
	break;

      case INC : /* Increment accumulator A */
	acc_A = (acc_A + 1) % 16;
	if (verbose)
	  printf("Incrementing A, A := %d\n", acc_A);
	break;

      case DEC : /* Decrement accumulator A */
	acc_A --;
	if (acc_A < 0)
	  acc_A += 16;
	if (verbose)
	  printf("Decrementing A, A := %d\n", acc_A);
	break;

      case BEZ : /* Branch if A == 0 */
	add1 = memory[pc];
	inc_pc(&pc);
	add2 = memory[pc];
	inc_pc(&pc);
	address = 16 * add1 + add2;
	if (acc_A == 0)
	  pc = address;
	if (verbose)
	  printf("Branching if A == 0, A = %d\n", acc_A);
	break;

      case BRA : /* Branch always */
	add1 = memory[pc];
	inc_pc(&pc);
	add2 = memory[pc];
	inc_pc(&pc);
	address = 16 * add1 + add2;
	pc = address;
	if (verbose)
	  printf("Branch always\n");
	break;

      case STP : /* Stop */
	stop = 1;
	if (verbose)
	  printf("Stop\n");
	break;

      default :
	printf("Unknown command %d\n", command);
      }
  }

  /* Write memory to the file. */
  write();
}

/******************************************************************
Read memory from file.
******************************************************************/
void read(void)
{
  FILE* fp;
  int i;
  char letter;

  fp = fopen(FILE_IN, "r");
  if (fp == NULL)
    {
      printf("Could not open file '%s' for reading\n", FILE_IN);
      exit(-2);
    }

  for (i = 0; i < LENGTH; i++)
    {
      fscanf(fp, "%c", &letter);
      memory[i] = number(letter);
    }

  fclose(fp);
}

/******************************************************************
Write contents of memory to file.
******************************************************************/
void write(void)
{
  FILE* fp;
  int i;

  fp = fopen(FILE_OUT, "w");
  if (fp == NULL)
    {
      printf("Error: could not open file '%s' for writing\n", FILE_OUT);
      exit(-1);
    }

  for (i = 0; i < LENGTH; i++)
    fprintf(fp, "%c", letter(memory[i]));
  fprintf(fp, "\n");
  fclose(fp);
}

/*******************************************************************
Increment the program counter.
*******************************************************************/
void inc_pc(int* pc)
{
  *pc = *pc + 1;
  if (*pc >= LENGTH)
    *pc = 0;
}

/*******************************************************************
Map 0 to 15 to alpha-numeric symbol.
*******************************************************************/
char letter(int num)
{
  if (num < 10)
    return '0' + num;
  return 'A' + num - 10;
}

/*******************************************************************
Map alpha-numeric symbol to number.
*******************************************************************/
char number(int let)
{
  if (let <= '9')
    return let - '0';
  return let - 'A' + 10;
}
