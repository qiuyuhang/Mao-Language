//
// Created by qyh on 16-1-2.
//

#ifndef MAO_LANGUAGE_PUBLIC_H
#define MAO_LANGUAGE_PUBLIC_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
enum { VAR_TYPE, INT_TYPE, DOUBLE_TYPE ,INVALID_TYPE};
int  g_line_num = 1,g_last_value_type = VAR_TYPE;//0代表运算符,1代表运算子

int OperandType(char *operand) {//获取变量类型
    int i = 0, alpha_num = 0, digit_num = 0, dot_num = 0;
    while (operand[i] != '\0') {
        if (isalpha(operand[i]) != 0)	++alpha_num;
        if (isdigit(operand[i]) != 0)	++digit_num;
        if (operand[i] == '.')			++dot_num;
        if (isalnum(operand[i]) == 0 && operand[i] != '.') return INVALID_TYPE;
        i++;
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

#endif //MAO_LANGUAGE_PUBLIC_H
