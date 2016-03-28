//
// Created by qyh on 16-1-2.
//

#include "public.h"
#include "error.h"
#include "operator_stack.h"
#include "variable_list.h"
#include "operand_stack.h"
#include "calc.h"

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
        if (g_operand_stk_top->type == 1)printf("%d\n", g_operand_stk_top->int_value);
        if (g_operand_stk_top->type == 2)printf("%.6lf\n", g_operand_stk_top->double_value);
    }
    FreeOperand();
}

int SentenceGet(FILE *source_file, char *sentence) {//获取单句语句，并对齐行首
    int i = 0, ch;
    while ((ch = getc(source_file)) != ';') {
        if (ch == EOF) return -1;
        if (ch == '\n' || ch == '\r' || (ch == ' '&&i == 0)) {
            if(ch=='\n')
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
        printf("File Open Fail!");
        exit(0);
    }

    while (SentenceGet(source_file, sentence) != -1){
        if (strncmp(sentence, "int ", 4) == 0) {
            VariableTake(sentence + 3, 1);
        }
        else if (strncmp(sentence, "double ", 7) == 0) {
            VariableTake(sentence + 6, 2);
        }
        else if (strncmp(sentence, "print ", 6) == 0 || strncmp(sentence, "print(", 6) == 0)
            PrintVariable(sentence + 5);
        else SentenceCalc(sentence);
    }

    fclose(source_file);
    FreeVarList();
    return 0;
}