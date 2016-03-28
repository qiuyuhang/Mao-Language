//
// Created by qyh on 16-1-2.
//

#ifndef MAO_LANGUAGE_OPERATOR_STACK_H
#define MAO_LANGUAGE_OPERATOR_STACK_H

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

#endif //MAO_LANGUAGE_OPERATOR_STACK_H
