//
// Created by qyh on 16-1-2.
//

#ifndef MAO_LANGUAGE_OPERAND_STACK_H
#define MAO_LANGUAGE_OPERAND_STACK_H

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
        OVERFLOW;
    }
    else if (OperandType(operand) == DOUBLE_TYPE) {
        new_operand->type = 2;
        new_operand->double_value = atof(operand);
        new_operand->int_value = atof(operand);
    }
    else INVALID_OPERAND;//非法运算子报错
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
#endif //MAO_LANGUAGE_OPERAND_STACK_H
