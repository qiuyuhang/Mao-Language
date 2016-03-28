//
// Created by qyh on 16-1-2.
//

#ifndef MAO_LANGUAGE_CALC_H
#define MAO_LANGUAGE_CALC_H

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
    int right_type = 1, left_type = 1,temp_operator = 0;

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
    if (g_operator_stk_top == NULL&&temp_operator !='^') {
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
            if ((int)left_operand != (int)(result - right_operand))//运算溢出判断，下同
                OVERFLOW;
            break;
        }
        case '-': {
            result = left_operand - right_operand;
            if ((int)left_operand != (int)(result + right_operand))
                OVERFLOW;
            break;
        }
        case '*': {
            result = left_operand * right_operand;
            if(right_operand != 0){
                if ((int)left_operand != (int)(result / right_operand))
                    OVERFLOW;
            }
            break;
        }
        case '/': {
            result = left_operand / right_operand;
            if ((int)left_operand != (int)(result * right_operand))
                OVERFLOW;
            break;
        }
        case '=': {
            result = right_operand;
            if (g_operand_stk_top->operand[0] == '\0')
                INVALID_ASGN;
            VariablePointer temp = g_var_list_head;
            temp = VarPosGet(g_operand_stk_top->operand);
            temp->int_value = result;
            temp->double_value = result;
            break;
        }
    }
    if (g_operand_stk_top->type==1&&((result - (int)result)>1|| (result - (int)result)<-1))//隐式类型转换溢出判断
        OVERFLOW;
    g_operand_stk_top->int_value = result;
    g_operand_stk_top->double_value = result;
}


void SentenceCalc(char *sentence) {
    int i = 0, j = 0;
    char operand[21], ch;
    while ((ch = sentence[i++]) != '\0') {
        if (ch == ' ' || ch == '\n' ) {
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


#endif //MAO_LANGUAGE_CALC_H
