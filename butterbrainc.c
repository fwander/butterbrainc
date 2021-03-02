#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <ctype.h>

typedef struct LLNode LLNode;

struct LLNode{
  LLNode* next;
  int data;
};

typedef struct{
  LLNode* tail;
  LLNode* head;
} Stack;

int buf;
char* code;
char* start;
int memsize;
int fsize;
int gnum();
int stn();
FILE* outfile;
int looplbl = 0;
int iflbl  = 0;
Stack* looplbls;
Stack* iflbls;


LLNode* push(Stack* s, int data){
  LLNode* new = (LLNode*) malloc(sizeof(LLNode));
  new->next = s->head;
  s->head = new;
  new->data = data;
  return new;
}

int pop(Stack* s){
	int ret = s->head->data;	
	LLNode* h = s->head;
	s->head = h->next;
	free(h);
	return ret;
}

Stack* newLL(){
  Stack* new = (Stack*) malloc(sizeof(Stack));
  new->tail = push(new, 0);
  new->head = push(new, 0);
  new->head->next = new->tail;
  return new;
}

void and(int b){fprintf(outfile,"and eax, ebx\n");}
void or(int b){fprintf(outfile,"or eax, ebx\n");}
void xor(int b){fprintf(outfile,"xor eax, ebx\n");}
void plus(int b){fprintf(outfile,"add eax, ebx\n");}
void minus(int b){fprintf(outfile,"sub eax, ebx\nneg eax\n");}
void mul(int b){fprintf(outfile,"imul ebx\n");}
void divi(int b){fprintf(outfile,"xor edx, edx\nmov ecx, eax\nmov eax, ebx\ndiv ecx\n");}
void eq(int b){fprintf(outfile,"cmp eax, ebx\nsete al\n");}
void less(int b){fprintf(outfile,"cmp eax, ebx\nsetg al\n");}
void greater(int b){fprintf(outfile,"cmp eax, ebx\nsetl al\n");}
void not(int b){fprintf(outfile,"mov eax, ebx\nsub eax, 1\nsbb eax, eax\nand eax, 1\n");}
void modulo(int b){fprintf(outfile,"xor edx, edx\nmov ecx, eax\nmov eax, ebx\ndiv ecx\nmov eax, edx\n");}
void rightp(int b){fprintf(outfile, "mov rcx, r8\nneg rcx\nsar ecx, 3\nadd eax, ecx\n");}
void leftp(int b){fprintf(outfile, "mov rcx, rax\nmov rax, r8\nneg rax\nsar eax, 3\nsub eax, ecx\n");}
void at(int b){fprintf(outfile,"neg rax\nmov rax, [rbp + rax * 8]\n");}
void here(int b){fprintf(outfile,"mov rax, r8\nneg rax\nshr rax, 3\n");}


typedef void (*funptr)(int);
funptr map[256];
void initMap(){
  map['&'] = and;
  map['|'] = or;
  map['^'] = xor;
  map['+'] = plus;
  map['-'] = minus;
  map['*'] = mul;
  map['/'] = divi;
  map['='] = eq;
  map['<'] = less;
  map['>'] = greater;
  map['!'] = not;
  map['%'] = modulo;
  map[')'] = rightp;
  map['('] = leftp;
  map['@'] = at;
  map['v'] = here;
}
void g(){
	fprintf(outfile,"mov rbx, [rbp + r8]\n");
	gnum();
	fprintf(outfile,"shl rax, 3\nneg rax\nmov r8, rax\n");
}
void p(){
	fprintf(outfile,"mov rax, [rbp + r8]\ncall print\n");
}
void n(){
	fprintf(outfile,"call newline\n");
}
void i(){
	fprintf(outfile,"call get\nmov [rbp + r8], rax\n");
}

void s(){
	fprintf(outfile,"mov rbx, [rbp + r8]\n");
	gnum();
	fprintf(outfile,"mov [rbp + r8], rax\n");
}


void os(){
  fprintf(outfile,"loop%d:\n",looplbl);
  fprintf(outfile,"cmp dword [rbp + r8], dword 0\nje loopend%d\n",looplbl);
  push(looplbls,looplbl++);
}
void cs(){
  int temp = pop(looplbls);
  fprintf(outfile,"cmp dword [rbp + r8], dword 0\njne loop%d\n",temp);
  fprintf(outfile,"loopend%d:\n",temp);
}
void oc(){
  fprintf(outfile,"cmp dword [rbp + r8], dword 0\nje if%d\n",iflbl);
  push(iflbls,iflbl++);
}
void cc(){
  fprintf(outfile,"if%d:\n",pop(iflbls));
}

void a(){
	fprintf(outfile,"mov eax, dword [rbp + r8]\n call printc\n");
}

typedef void (*spec)();

spec special[256];
void initSpecial(){
  special['g'] = g;
  special['p'] = p;
  special['n'] = n;
  special['i'] = i;
  special['['] = os;
  special[']'] = cs;
  special['s'] = s;
  special['{'] = oc;
  special['}'] = cc;
  special['a'] = a;
}



int stn(){  // string to number
  int result = 0;
  int size = 0;
  while(isdigit(*code)){

    result *= 10;
    result += (int) (*code - '0');
    code++;

  }
  code--;
  return result;
}

int gnum(){

  code++;
  char* start = code;


  if(map[*start] != NULL){
    map[*start](gnum());
	return 1;
  }


  int temp = stn();
  fprintf(outfile,"mov rax, qword %d\n", temp);
  return 1;
}

void loopP(int toomuch){
  while (code-start < toomuch-1){
    (special[*code])();
    code++;
  }
}
int init(int argc, char** argv){
  initMap();
  initSpecial();
  if (argc <= 2){
    printf("format for use:\n ./a.out [file] [memory use]\n");
    return 1;
  }
  FILE* file = fopen(argv[1],"r");
  outfile = fopen("out.s","w");
  code = argv[2];
  memsize = stn();
  looplbls = newLL();
  iflbls = newLL();

  if (file == NULL){
    printf("could not find file\n");
    return 1;
  }

  fprintf(outfile,begin);
  for (int i = 0; i < memsize; i++){
	  fprintf(outfile,"push 0\n");
  }
  fprintf(outfile,"xor r8, r8\n");
  fseek(file, 0, SEEK_END); // seek to end of file
  fsize = ftell(file); // get current file pointer
  fseek(file, 0, SEEK_SET); // seek back to beginning of fil
  code = (char*) malloc(fsize * sizeof(char));
  start = code;
  char op;
  do
    {
      op = (char)fgetc(file);

      if(!isspace(op)){

        *code = op;
        code++;

      }


    } while(op != EOF);
  fclose(file);
  return 0;
}

int main(int argc, char** argv){
  if (init(argc, argv)){return 1;}
  int toomuch = code-start;
  code = start;

  loopP(toomuch);

  fprintf(outfile,"mov rbp,0\nmov rax,1\nint 0x80");
  fclose(outfile);

  free(start);
  return 0;
}
