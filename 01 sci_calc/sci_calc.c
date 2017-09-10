#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct Node {

	struct Node* prev;
	struct Node* next;
	char* data;

} Node;

typedef struct List {

	struct Node *header;
	struct Node *trailer;

} List;

/*HSH*/
List* InitList();
void addLast(List*, char*);
void addAt(List*, char*, int index);
char* delete(List*, int index);
void removeNode(Node*);
Node* getNode(List*, int index);
void printList(List*);
void Discard(List* list);
void unary(Node*);
char* ftos(double num);

/*BSS*/
void getList(char* s, List* list);
char* valid(char* s);
void binary(Node* no);
int isOpUnary(char* s);
/*JNL*/
void press(char* s);
char* input();
Node* findst(Node* sh, Node* trailer);

/*TEAM*/
char *registers[26] = { NULL };
int PROGRAM_GOING = 1;
char* str_tokenizer(char* str, char** des);
void calc(List* list, Node* header, Node* trailer);
void save_register(List* list, Node* header, Node* trailer);
void set_reg_val(Node* reg, char* val);
void load_register(Node* node);
void freeRegisters();
int isOpNumber(Node* op);

/*ERRORS*/
void E_INVALID_OPERANDS(char* o1, char* o2);
void E_NO_INPUT_ERROR(char* str);
void E_BRACKET_MISMATCHED(int stack_size);
void E_INVALID_REG_NAME_ERROR(Node* reg);
void E_INVALID_REG_VAL_ERROR(Node* reg);
void E_MOST_SIGNIFICANT_ERROR(List* list);
void E_MATH_ERROR(double oprd, int u);

/*HSS impl.*/
List* InitList() {

	List* list = (List*)malloc(sizeof(List));
	list->header = (Node*)malloc(sizeof(Node));
	list->trailer = (Node*)malloc(sizeof(Node));

	list->header->next = list->trailer;
	list->header->prev = NULL;
	list->header->data = NULL;

	list->trailer->next = NULL;
	list->trailer->prev = list->header;
	list->trailer->data = NULL;
	return list;
}

void Discard(List* list) {
	Node* node = list->header->next;
	Node* end = list->trailer->next;

	while (node != end) {

		if (node->prev->data) {
			free(node->prev->data);
		}
		free(node->prev);
		node = node->next;
	}
	free(list->trailer);
	free(list);
	return;
}

void addLast(List* list, char* str) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = (char*)malloc(strlen(str) + 1);
	strcpy(newNode->data, str);

	newNode->prev = list->trailer->prev;
	newNode->next = list->trailer;
	list->trailer->prev->next = newNode;
	list->trailer->prev = newNode;
	return;
}

void addAt(List* list, char* str, int index) {
	Node* tmp = list->header->next;
	Node* newNode;

	int i = 0;
	while (i != index) {
		tmp = tmp->next;
		i++;
	}
	newNode = (Node*)malloc(sizeof(Node));
	newNode->next = tmp;
	newNode->prev = tmp->prev;
	tmp->prev->next = newNode;
	tmp->prev = newNode;
	newNode->data = (char*)malloc(strlen(str) + 1);
	strcpy(newNode->data, str);
	return;
}

char* delete(List* list, int index) {
	Node* tmp = list->header;
	char* str;
	int i = -1;

	while (i != index) {
		tmp = tmp->next;
		//if index greater than list size, return null. and then print error message.
		if (tmp == list->trailer) {
			printf("error. out of range.\n");
			return NULL;
		}
		i++;
	}

	tmp->prev->next = tmp->next;
	tmp->next->prev = tmp->prev;
	str = (char*)malloc(strlen(tmp->data) + 1);
	strcpy(str, tmp->data);
	free(tmp);
	return str;
}

void printList(List* list) {
	Node* tmp = list->header;
	if (tmp->next == list->trailer) {
		printf("Empty.\n");
		return;
	}


	while (tmp != NULL) {
		if (tmp->data != NULL)
			printf("%s ", tmp->data);
		tmp = tmp->next;
	}
	printf("\n");
	return;
}

void removeNode(Node* v) {
	v->next->prev = v->prev;
	v->prev->next = v->next;
	if (v->data)
		free(v->data);
	free(v);
	return;
}

Node* getNode(List* list, int index) {
	Node* tmp = list->header;
	int i = -1;
	while (i != index) {
		tmp = tmp->next;
		i++;
	}
	return tmp;
}

//double to String
char* ftoS(double num) {
	//case 1, positive
	if (num >= 0) {
		int intPart = (int)num;
		double tmp = (num - intPart);
		int floatPart = (int)(1000000 * tmp);

		char* buffer = (char*)malloc(sizeof(char) * 100);
		char buffer2[100];
		sprintf(buffer, "%d", intPart);
		strcat(buffer, ".");
		sprintf(buffer2, "%06d", floatPart);
		strcat(buffer, buffer2);
		return buffer;
	}
	//case 2, negative
	else {
		int intPart;
		double tmp;
		int floatPart;
		char* buffer;
		char buffer2[100];

		num = -num;
		intPart = (int)num;
		tmp = (num - intPart);
		floatPart = (int)(1000000 * tmp);

		buffer = (char*)malloc(sizeof(char) * 100);

		strcpy(buffer, "-");
		sprintf(buffer2, "%d", intPart);
		strcat(buffer, buffer2);
		strcat(buffer, ".");
		buffer2[0] = '\0';
		sprintf(buffer2, "%06d", floatPart);
		strcat(buffer, buffer2);
		return buffer;
	}
}

//unary calcuation
void unary(Node* node) { // ( - expression )

	char* opr = node->data;
	char* tmp = NULL;
	double next = atof(node->next->data);

	if (strcmp(opr, "sin") == 0) {
		tmp = ftoS(sin(next));
	}
	else if (strcmp(opr, "cos") == 0) {
		tmp = ftoS(cos(next));
	}
	else if (strcmp(opr, "log") == 0) {
		E_MATH_ERROR(next, 1);
		tmp = ftoS(log(next));
	}
	else if (strcmp(opr, "exp") == 0) {
		tmp = ftoS(exp(next));
	}
	else if (strcmp(opr, "+") == 0) {
		tmp = ftoS(next);
	}
	else if (strcmp(opr, "-") == 0) {
		tmp = ftoS(-1*next);
	}

	if (opr)
		free(opr);
	node->data = tmp;
	removeNode(node->next);
	return;
}

/*BSS impl.*/
void getList(char* s, List* list)
{
	char* destination = NULL;
	char* delegator = NULL;
	char* tmp;
	int opunary;

	E_NO_INPUT_ERROR(s);
	delegator = s;
	do
	{
		delegator = str_tokenizer(delegator, &destination);//토큰분리
		if ((opunary = isOpUnary(destination)) != 0)
		{
			tmp = (char*)malloc(2);
			strcpy(tmp, "0");
			addLast(list, tmp);

			if (opunary == 1)
			{
				tmp = (char*)malloc(2);
				strcpy(tmp, "+");
			}
			else if (opunary == 2)
			{
				tmp = (char*)malloc(2);
				strcpy(tmp, "-");
			}
			addLast(list, tmp);
			destination++;
		}
		addLast(list, destination);
	} while (delegator != NULL);
	return;
}

char * valid(char * s)
{
	int i, j;
	int s_len, to_len;//받아올 문자열 길이, 생성될 문자열 길이
	int * f_cal = NULL, *s_cal = NULL;//문자열 길이 계산시 사용->free

	char token = ' ';//들어갈 토큰
	char * re_str = NULL;//반환될 문자열

	s_len = strlen(s);
	f_cal = (int *)calloc(s_len + 1, sizeof(int));//끝 널 문자까지 포함하기위해 +1
	s_cal = (int *)calloc(s_len, sizeof(int));

	for (i = 0; i<s_len + 1; i++)
	{
		if (isdigit(s[i]) || isalpha(s[i]) || s[i] == '.') f_cal[i] = 1;
		if (s[i] == '+' || s[i] == '-')	f_cal[i] = 3;
	}

	for (i = 0; i<s_len; i++)	s_cal[i] = f_cal[i] + f_cal[i + 1];

	to_len = 0;
	for (i = 0; i<s_len; i++)
	{
		switch (s_cal[i])
		{
		case 4: if (s[i] == '+' || s[i] == '-')
		{
			if (s[i - 1] == '(')
			{
				if (isdigit(s[i + 1]))
				{
					int stop = i;
					int num = 1;
					j = 0;

					while (num != 0)
					{
						if (s[++i] == '(')	num++;
						else if (s[i] == ')') num--;
						j++;
					}
					i = stop;
					stop = j;
					j = 0;

					while (1)
					{
						i++;
						if (!isdigit(s[i]) && s[i] != '.')break;
						j++;
					}
					i -= (j + 1);

					if (stop != j + 1)
					{
						to_len += 2;
						s_cal[i] = 0;
					}
					else
					{
						to_len += 1;
						s_cal[i] = 1;
					}
				}
				else if (isalpha(s[i + 1]))
				{
					int num = 1;
					j = 0;

					while (num != 0)
					{
						if (s[++i] == '(')	num++;
						else if (s[i] == ')') num--;
						j++;
					}
					i -= j;

					if (s[i + j - 1] != ')')
					{
						to_len += 2;
						s_cal[i] = 0;
					}
					else
					{
						to_len += 1;
						s_cal[i] = 1;
					}
				}
			}
			else
			{
				to_len += (s_cal[i] - 2);
				s_cal[i] = 0;
			}
		}
				else
				{
					to_len += (s_cal[i] - 2);
					s_cal[i] = 0;
				}
				break;

		case 3: if (s[i + 1] == '>')
		{
			to_len += 1;
			s_cal[i] = 1;
		}
				else
				{
					to_len += (s_cal[i] - 1);
					s_cal[i] = 0;
				}
				break;

		case 2: to_len += (s_cal[i] - 1);     s_cal[i] = 1;	break;
		case 1: to_len += (s_cal[i] + 1);     s_cal[i] = 0;	break;
		case 0: to_len += (s_cal[i] + 2);     s_cal[i] = 0;	break;
		}
	}

	re_str = (char *)calloc(to_len + 1, sizeof(char));

	j = 0;

	for (i = 0; i<s_len; i++)
	{
		if (s_cal[i]) re_str[j++] = s[i];
		else
		{
			re_str[j++] = s[i];
			re_str[j++] = token;
		}
	}

	free(f_cal);
	free(s_cal);
	free(s);
	return re_str;
}

void binary(Node* no)
{
	double prev, next, temp;
	char data[100];

	E_INVALID_OPERANDS(no->prev->data, no->next->data);
	prev = atof(no->prev->data);
	next = atof(no->next->data);

	switch (no->data[0])
	{
	case '+':
		temp = prev + next;
		sprintf(data, "%lf\0", temp);
		break;
	case '-':
		temp = prev - next;
		sprintf(data, "%lf", temp);
		break;
	case '*':
		temp = prev * next;
		sprintf(data, "%lf", temp);
		break;
	case '/':
		E_MATH_ERROR(next, 0);
		temp = prev / next;
		sprintf(data, "%lf", temp);
		break;
	}

	no->data = (char*)realloc(no->data, strlen(data) + 1);
	strcpy(no->data, data);

	removeNode(no->prev);
	removeNode(no->next);
	return;

}

int isOpUnary(char* s)
{
	if (strcmp(s, "+sin") == 0 || strcmp(s, "+cos") == 0 || strcmp(s, "+log") == 0 || strcmp(s, "+exp") == 0)
		return 1;
	if (strcmp(s, "-sin") == 0 || strcmp(s, "-cos") == 0 || strcmp(s, "-log") == 0 || strcmp(s, "-exp") == 0)
		return 2;
	return 0;
}

/*JNL impl.*/
void press(char *s)
{
	int i = 0, j = 0, k = 0;
	int len = strlen(s);

	for (i = 0; i < len - k; i++)
	{
		if (s[i] == ' ')
		{
			for (j = i; j < len - k; j++)
			{
				s[j] = s[j + 1];
			}
			k++;
			i--;
		}
	}
	return;
}

char* input()
{
	const static char* cal = "CAL";
	const static char* _exit = "EXIT";
	char tmp[128];
	char* rtn = NULL;

	int first = 1;
	while (fgets(tmp, 100, stdin), tmp[strlen(tmp) - 1] = 0, 1)
	{
		if (first)
		{
			rtn = (char*)malloc(strlen(tmp) + 1);
			strcpy(rtn, tmp);
			first = 0;
		}
		else
		{
			rtn = (char*)realloc(rtn, strlen(rtn) + strlen(tmp) + 1);
			strcat(rtn, tmp);
		}

		press(rtn);
		if (strstr(rtn, cal) != NULL)
		{
			rtn[(int)strstr(rtn, cal) - (int)rtn] = '\0';
			break;
		}
	}

	if (strstr(rtn, _exit) != NULL)
	{
		PROGRAM_GOING = 0; //flag off
		exit(0);
	}
	return rtn;
}

Node* findst(Node* sh, Node* trailer)
{
	int stack_size = 1, flag = 1;
	const static char* open = "(", *close = ")";
	Node* node = sh->next;
	Node* rtn = NULL;

	while (node != trailer)
	{
		if (strcmp(node->data, open) == 0)
			stack_size++;
		else if (strcmp(node->data, close) == 0)
			stack_size--;

		if (stack_size == 0 && flag)
		{
			rtn = node;
			flag = 0;
		}
		node = node->next;
	}

	E_BRACKET_MISMATCHED(stack_size);
	return rtn;
}

/*TEAM impl.*/
char* str_tokenizer(char* str, char** des)
{
	char* cp = str;
	char token = ' ';
	*des = str;

	while (*cp != token && *cp != 0)//구분자 또는 널을 찾는다
		cp++;

	if (*cp == token) //구분자를 널로 만든다.
		*cp = 0;
	else //널을 참조하면 base case
		return NULL;

	return *(cp + 1) ? (cp + 1) : NULL; // 구분자의 다음이 널이면 base case2
}

void calc(List* list, Node* header, Node* trailer)
{
	Node* node = header;
	Node* sh, *st;

	while (node = node->next, node != trailer) {
		if (strcmp(node->data, "(") == 0) {

			sh = node;
			st = findst(sh, trailer);

			calc(list, sh, st);
			node = sh->prev;

			removeNode(sh);
			removeNode(st);
		}
	}

	node = header;
	while (node = node->next, node != trailer)
		if (strcmp(node->data, "[") == 0)
			load_register(node);

	node = header;
	while (node = node->next, node != trailer) {
		if (strcmp(node->data, "cos") == 0 || strcmp(node->data, "sin") == 0 || strcmp(node->data, "log") == 0 || strcmp(node->data, "exp") == 0)
			unary(node);
		else if ((strcmp(node->data, "+") == 0 || strcmp(node->data, "-") == 0) && isOpNumber(node)) //unary +, -
			unary(node);
	}

	node = header;
	while (node = node->next, node != trailer)
		if (strcmp(node->data, "*") == 0 || strcmp(node->data, "/") == 0)
			binary(node);

	node = header;
	while (node = node->next, node != trailer)
		if (strcmp(node->data, "+") == 0 || strcmp(node->data, "-") == 0)
			binary(node);
	//printList(list);
	return;
}

void save_register(List* list, Node* header, Node* trailer)
{
	Node* node = header;
	Node* reg;

	int pos;
	while (node = node->next, node != trailer)
	{
		if (strcmp(node->data, "->") == 0)
		{
			calc(list, header, node);

			reg = getNode(list, 3);
			E_INVALID_REG_NAME_ERROR(reg);

			set_reg_val(reg, node->prev->data);

			removeNode(reg->prev); //[
			removeNode(reg->next); //]
			removeNode(reg); //reg_name

			removeNode(node->prev); //reg_val
			node = header;
			removeNode(node->next);
		}//end if
	}//end while
	return;
}

void set_reg_val(Node* reg, char* val)
{
	int pos = reg->data[0] - 'a';
	if (registers[pos])
		free(registers[pos]);

	registers[pos] = (char*)malloc(strlen(val) + 1);
	strcpy(registers[pos], val);
}

void load_register(Node* reg_open)
{
	Node* reg = reg_open->next;
	int pos = reg->data[0] - 'a';

	E_INVALID_REG_NAME_ERROR(reg);
	E_INVALID_REG_VAL_ERROR(reg);

	removeNode(reg->next);//]
	removeNode(reg);//reg_name
	
	reg_open->data = (char*)realloc(reg_open->data, strlen(registers[pos]) + 1);
	strcpy(reg_open->data, registers[pos]);
	return;
}
void freeRegisters()
{
	int i;
	for (i = 0; i < 26; i++)
	{
		if (registers[i])
		{
			free(registers[i]);
		}
		registers[i] = NULL;
	}
	return;
}

int isOpNumber(Node* op)// ( - 1  )
{
	Node* node = op->prev;
	if (node && node->data)
	{
		if (strcmp(node->data, "(") == 0)
		{
			node = op->next->next;
			if (strcmp(node->data, ")") == 0)
			{
				node = op;
				if (strcmp(node->data, "+") == 0)
					return 1;
				else if (strcmp(node->data, "-") == 0)
					return 2;
			}
		}
	}
	return 0;
}

/*ERROS impl.*/
void E_NO_INPUT_ERROR(char* str)
{
	if (strlen(str) == 0)
	{
		printf("NO_INPUT_ERROR");
		exit(0);
	}
	return;
}
void E_INVALID_OPERANDS(char* o1, char* o2)
{
	if (!o1 || !o2)
	{
		printf("INVALID_OPERANDS");
		exit(0);
	}
	else if (strcmp(o1, "(") == 0 || strcmp(o2, ")") == 0)
	{
		printf("INVALID_OPERANDS");
		exit(0);
	}
	return;
}
void E_BRACKET_MISMATCHED(int stack_size)
{
	if (stack_size != 0)
	{
		printf("BRACKET_MISMATCHED\n");
		exit(0);
	}
	return;
}
void E_INVALID_REG_NAME_ERROR(Node* reg)
{
	if (strlen(reg->data) > 1 || isalpha(reg->data[0]) == 0)
	{
		printf("INVALID_REG_NAME_ERROR\n");
		exit(0);
	}
	return;
}
void E_INVALID_REG_VAL_ERROR(Node* reg)
{
	int pos = reg->data[0] - 'a';
	if (!registers[pos])
	{
		printf("INVALID_REG_VAL_ERROR\n");
		exit(0);
	}
	return;
}
void E_MOST_SIGNIFICANT_ERROR(List* list)
{
	int size = 0;
	Node* node = list->header->next;
	while (node != list->trailer)
	{
		size++;
		node = node->next;
	}

	if (size != 1)
	{
		printf("MOST_SIGNIFICANT_ERROR");
		exit(0);
	}
	else
	{
		char* data = getNode(list, 0)->data;
		int len = strlen(data), i;
		for (i = 0; i < len; i++)
		{
			if (!isdigit(data[i]))
				if (!(data[i] == '+'))
					if (!(data[i] == '-'))
						if (!(data[i] == '.'))
						{
							printf("MOST_SIGNIFICANT_ERROR");
							exit(0);
						}
		}
	}
	return;
}
void E_MATH_ERROR(double oprd, int u)
{
	if (oprd == 0. || (u && oprd < 0))
	{
		printf("MATH_ERROR");
		exit(0);
	}
	return;
}


/*main*/
int main(void)
{
	while (PROGRAM_GOING)
	{
		List* expr_list = InitList();
		char* user_expr;
		int i;
		//_CrtSetDbgFlag(0x21);//메모리 누수확인

		user_expr = input();
		user_expr = valid(user_expr);
		getList(user_expr, expr_list);

		save_register(expr_list, expr_list->header, expr_list->trailer);
		printList(expr_list);
		calc(expr_list, expr_list->header, expr_list->trailer);
		E_MOST_SIGNIFICANT_ERROR(expr_list);

		printList(expr_list);

		Discard(expr_list);
		free(user_expr);
		freeRegisters();
		fputc('\n', stdout);
	}
	return 0;
}

