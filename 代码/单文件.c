#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

//未定义
#define UNDEFINE_VAR	{printf("!ERROR\nLine%d:Undefined Variable: %s",g_line_num,variable);exit(0);}
//重定义
#define REDEFINE_VAR	{printf("!ERROR\nLine%d:Redefine Variable: %s",g_line_num,variable);exit(0);}
//保留变量名
#define RESERVED_VAR	{printf("!ERROR\nLine%d:Reserved Variable: %s",g_line_num,variable);exit(0);}
//非法变量名
#define INVALID_VAR		{printf("!ERROR\nLine%d:Invalid Variable Name:  %s",g_line_num,variable);exit(0);}
//非法运算子
#define INVALID_OPERAND	{printf("!ERROR\nLine%d:Invalid Operand:  %s",g_line_num,operand);exit(0);}
//非法赋值
#define INVALID_ASGN	{printf("!ERROR\nLine%d:L-value Required As Left Operand Of Assignment",g_line_num);exit(0);}
//溢出
#define OVERFLOW		{printf("!Line%d:ERROR\nOverflow",g_line_num);exit(0);}

enum { VAR_TYPE, INT_TYPE, DOUBLE_TYPE, INVALID_TYPE };
int  g_line_num = 1, g_last_value_type = VAR_TYPE;//0代表运算符,1代表运算子

/***************************************************************
						变	量	表
****************************************************************/

struct VariableList {//存储变量的链表
	char name[21];
	int type;
	int int_value;
	double double_value;
	struct VariableList *next;
};
typedef struct VariableList *VariablePointer;
VariablePointer g_var_list_head = NULL;

VariablePointer VarPosGet(char *variable) {
	VariablePointer temp_variable = g_var_list_head;
	while (temp_variable != NULL) {
		if (strcmp(variable, temp_variable->name) == 0)
			return temp_variable;
		temp_variable = temp_variable->next;
	}
	return NULL;
}

int OperandType(char *operand) {//获取变量类型
	int i = 0, alpha_num = 0, digit_num = 0, dot_num = 0;
	while (operand[i] != '\0') {
		if (isalpha(operand[i]) != 0)	++alpha_num;
		if (isdigit(operand[i]) != 0)	++digit_num;
		if (operand[i] == '.')			++dot_num;
		if (isalnum(operand[i]) == 0 && operand[i] != '.' && ((operand[i] == '-' || operand[i] == '+') && i != 0)) return INVALID_TYPE;
		i++;
	}
	if (operand[0] == '-' || operand[0] == '+') {
		++operand;
	}
	if (digit_num != 0 && dot_num == 0 && alpha_num == 0) {//int类型
		return INT_TYPE;
	}
	if ((isdigit(operand[0]) != 0 || operand[0] == '.') && digit_num != 0 && dot_num == 1 && alpha_num == 0) {//double类型
		return DOUBLE_TYPE;
	}
	if (isalpha(operand[0]) != 0 && dot_num == 0 && alpha_num != 0) {//var类型
		return VAR_TYPE;
	}
	return INVALID_TYPE;//非法类型
}

void VariableInsert(char *variable, int type) {//插入变量，并赋上类型和初值0，
	if (strcmp(variable, "int") == 0 || strcmp(variable, "double") == 0 || strcmp(variable, "print") == 0) {
		RESERVED_VAR;//保留变量名报错
	}
	if (OperandType(variable) != VAR_TYPE) {
		INVALID_VAR;//非法变量名报错
	}
	if (VarPosGet(variable) != NULL) {
		REDEFINE_VAR;//重定义变量报错
	}
	VariablePointer new_variable;
	new_variable = (VariablePointer)malloc(sizeof(struct VariableList));
	strcpy(new_variable->name, variable);
	new_variable->type = type;
	new_variable->int_value = 0;
	new_variable->double_value = 0;
	new_variable->next = g_var_list_head;
	g_var_list_head = new_variable;
}

void VariableTake(char *sentence, int type) {//获取定义语句中的变量名
	int i = 0, j = 0;
	char variable_name[21];
	while (sentence[i] != '\0') {
		if (sentence[i] == ' ') {
			++i;
			continue;
		}
		if (sentence[i] == ',') {
			VariableInsert(variable_name, type);
			++i;
			j = 0;
			continue;
		}
		variable_name[j++] = sentence[i++];
		variable_name[j] = '\0';
	}
	if (j != 0) VariableInsert(variable_name, type);
}

double ValueGet(char *variable) {//获取变量值
	VariablePointer temp = g_var_list_head;
	temp = VarPosGet(variable);
	if (temp == NULL)
		UNDEFINE_VAR;//未定义变量报错
	if (temp->type == INT_TYPE)return temp->int_value;
	if (temp->type == DOUBLE_TYPE)return temp->double_value;
	return  0;
}

void FreeVarList() {//释放变量表
	VariablePointer temp_variable = g_var_list_head;
	while (g_var_list_head != NULL) {
		g_var_list_head = g_var_list_head->next;
		free(temp_variable);
		temp_variable = g_var_list_head;
	}
}

/***************************************************************
						运	算	符	栈
****************************************************************/

struct operatorStack {//运算符栈
	char operator;
	struct operatorStack *next;
};
typedef struct operatorStack *_operator_stk_ptr;
_operator_stk_ptr g_operator_stk_top = NULL;

void PushOperator(char operator) {//运算符压栈
	_operator_stk_ptr new_operator;
	new_operator = (_operator_stk_ptr)malloc(sizeof(struct operatorStack));
	new_operator->operator = operator;
	new_operator->next = g_operator_stk_top;
	g_operator_stk_top = new_operator;
	g_last_value_type = 0;
}

char PopOperator() {//运算符出栈
	char operator;
	_operator_stk_ptr temp_operator = g_operator_stk_top;
	operator = g_operator_stk_top->operator;
	g_operator_stk_top = g_operator_stk_top->next;
	free(temp_operator);
	return operator;
}

void FreeOperator() {//释放运算符栈
	_operator_stk_ptr temp_operator = g_operator_stk_top;
	while (g_operator_stk_top != NULL) {
		g_operator_stk_top = g_operator_stk_top->next;
		free(temp_operator);
		temp_operator = g_operator_stk_top;
	}
}

/***************************************************************
						运	算	子	栈
****************************************************************/

struct OperandStack {//运算子栈
	char operand[21];
	int type;
	int int_value;
	double double_value;
	struct OperandStack *next;
};
typedef struct OperandStack *_operand_stk_ptr;
_operand_stk_ptr g_operand_stk_top = NULL;

void PushOperand(char *operand) {//运算子压栈
	_operand_stk_ptr new_operand;
	new_operand = (_operand_stk_ptr)malloc(sizeof(struct OperandStack));
	new_operand->operand[0] = '\0';
	if (OperandType(operand) == VAR_TYPE) {
		strcpy(new_operand->operand, operand);
		new_operand->int_value = ValueGet(operand);
		new_operand->double_value = ValueGet(operand);
		new_operand->type = VarPosGet(operand)->type;
	}
	else if (OperandType(operand) == INT_TYPE) {
		new_operand->type = 1;
		new_operand->int_value = atof(operand);
		new_operand->double_value = atof(operand);
		if (new_operand->double_value - new_operand->int_value > 1 || new_operand->double_value - new_operand->int_value < -1)
			OVERFLOW;//整数溢出检查
	}
	else if (OperandType(operand) == DOUBLE_TYPE) {
		new_operand->type = 2;
		new_operand->double_value = atof(operand);
		new_operand->int_value = atof(operand);
	}
	else 
		INVALID_OPERAND;//非法运算子报错
	new_operand->next = g_operand_stk_top;
	g_operand_stk_top = new_operand;
	g_last_value_type = 1;
}

double PopOperand() {//运算子出栈
	double operand;
	_operand_stk_ptr temp = g_operand_stk_top;
	if (g_operand_stk_top->type == 1)operand = g_operand_stk_top->int_value;
	else operand = g_operand_stk_top->double_value;
	g_operand_stk_top = g_operand_stk_top->next;
	free(temp);
	return operand;
}

void FreeOperand() {//释放运算子栈
	_operand_stk_ptr temp_operand = g_operand_stk_top;
	while (g_operand_stk_top != NULL) {
		g_operand_stk_top = g_operand_stk_top->next;
		free(temp_operand);
		temp_operand = g_operand_stk_top;
	}
}

/***************************************************************
						计	算
****************************************************************/

int Overflow(double a,double b) {
	if (a - b > 1 || a - b < -1) return 1;
	else return 0;
}

void FuckMinusSign() {//去除负号
	_operator_stk_ptr temp_operator = g_operator_stk_top;
	while (g_operator_stk_top != NULL)
		if (g_operator_stk_top->operator == '!') {
			temp_operator = g_operator_stk_top;
			g_operand_stk_top->int_value = -g_operand_stk_top->int_value;
			g_operand_stk_top->double_value = -g_operand_stk_top->double_value;
			g_operator_stk_top = g_operator_stk_top->next;
			free(temp_operator);
		}
		else break;
}

void Calc(char operator) {//单次计算
	double left_operand = 0, right_operand = 1, result;
	int right_type = 1, left_type = 1, temp_operator = 0;

	FuckMinusSign();
	if (g_operand_stk_top != NULL) {
		right_type = g_operand_stk_top->type;
		right_operand = PopOperand();
	}
	if (g_operator_stk_top == NULL) {
		return;
	}

	if (operator == '^') {
		temp_operator = operator;
		operator = PopOperator();
	}

	FuckMinusSign();
	if (g_operand_stk_top != NULL) {
		left_type = g_operand_stk_top->type;
		if (left_type == 1)left_operand = g_operand_stk_top->int_value;
		else left_operand = g_operand_stk_top->double_value;
	}
	if (g_operator_stk_top == NULL&&temp_operator != '^') {
		return;
	}

	if ((left_type*right_type) != 1 && g_operand_stk_top != NULL)
		g_operand_stk_top->type = 2;
	if (operator == '/'&&right_operand == 0) {
		printf("divided by ZERO\n");
		exit(0);
	}

	switch (operator) {
	case '+': {
		result = left_operand + right_operand;
		if (Overflow(left_operand, result - right_operand))//运算溢出判断，下同
			OVERFLOW;
		break;
	}
	case '-': {
		result = left_operand - right_operand;
		if (Overflow(left_operand, result + right_operand))
			OVERFLOW;
		break;
	}
	case '*': {
		result = left_operand * right_operand;
		if (right_operand != 0) {
			if (Overflow(left_operand, result / right_operand)) {
				OVERFLOW;
			}
		}
		break;
	}
	case '/': {
		result = left_operand / right_operand;
		if (Overflow(left_operand, result * right_operand))
			OVERFLOW;
		break;
	}
	case '=': {
		result = right_operand;
		if (g_operand_stk_top->operand[0] == '\0')//非法赋值
			INVALID_ASGN;
		VariablePointer temp = g_var_list_head;
		temp = VarPosGet(g_operand_stk_top->operand);
		temp->int_value = result;
		temp->double_value = result;
		break;
	}
	}
	if (g_operand_stk_top->type == 1 && ((result - (int)result)>1 || (result - (int)result)<-1))//隐式类型转换溢出判断
		OVERFLOW;
	g_operand_stk_top->int_value = result;
	g_operand_stk_top->double_value = result;
}

void SentenceCalc(char *sentence) {
	int i = 0, j = 0;
	char operand[21], ch;
	while ((ch = sentence[i++]) != '\0') {
		if (ch == ' ' || ch == '\n') {
		} else if (isalnum(ch) || ch == '.') {
			operand[j++] = ch;
			operand[j] = '\0';
		} else {
			if (j != 0) {
				PushOperand(operand);
				j = 0;
			}

			switch (ch) {
			case '+':case '-': {
				if (g_last_value_type == 0) {
					if (ch == '-')PushOperator('!');
				} else {
					while (g_operator_stk_top != NULL)
						if (g_operator_stk_top->operator == '*' || g_operator_stk_top->operator == '/')
							Calc('^');
						else {
							PushOperator(ch);
							break;
						}
				}
				break;
			}
			case '*':case '/': {
				if (g_operator_stk_top != NULL)
					if (g_operator_stk_top->operator == '*' || g_operator_stk_top->operator == '/')
						Calc('^');
				PushOperator(ch);
				break;
			}
			case '(': {
				PushOperator(ch);
				break;
			}
			case ')': {
				FuckMinusSign();
				while (g_operator_stk_top->operator != '(')
					Calc('^');
				PopOperator();
				break;
			}
			case '=': {
				PushOperator(ch);
				break;
			}
			}
			g_last_value_type = 0;
		}
	}

	if (j != 0) PushOperand(operand);
	while (g_operator_stk_top != NULL)
		Calc('^');

	FreeOperand();
	FreeOperator();
}

void PrintVariable(char *sentence) {//输出语句
	char operand[21];
	int  i = 0, j = 0;
	while (sentence[i] != ')') {
		if (sentence[i] == ' ' || sentence[i] == '(') {
			++i;
			continue;
		}
		operand[j++] = sentence[i++];
		operand[j] = '\0';
	}
	PushOperand(operand);
	if (g_operand_stk_top != NULL) {
		if (g_operand_stk_top->type == 1)
			printf("%d\n", g_operand_stk_top->int_value);
		if (g_operand_stk_top->type == 2)
			printf("%.6lf\n", g_operand_stk_top->double_value);
	}
	FreeOperand();
}

int SentenceGet(FILE *source_file, char *sentence) {//获取单句语句，并对齐行首
	int i = 0, ch;
	while ((ch = getc(source_file)) != ';') {
		if (ch == EOF) 
			return -1;
		if (ch == '\n' || ch == '\r' || (ch == ' '&&i == 0)) {
			if (ch == '\n')
				g_line_num++;
			continue;
		}
		sentence[i] = ch;
		++i;
	}
	sentence[i] = '\0';
	return 1;
}

int main(int argc, char *argv[])
{
	FILE *source_file;
	char sentence[1001];
	source_file = fopen(argv[1], "r");
	if (source_file == NULL) {
		printf("File Open Fail");
		exit(0);
	}

	while (SentenceGet(source_file, sentence) != -1) {
		if (strncmp(sentence, "int ", 4) == 0) {
			VariableTake(sentence + 3, 1);
		} else if (strncmp(sentence, "double ", 7) == 0) {
			VariableTake(sentence + 6, 2);
		} else if (strncmp(sentence, "print ", 6) == 0 || strncmp(sentence, "print(", 6) == 0)
			PrintVariable(sentence + 5);
		else SentenceCalc(sentence);
	}

	fclose(source_file);
	FreeVarList();
	return 0;
}