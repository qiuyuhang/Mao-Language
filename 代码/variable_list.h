//
// Created by qyh on 16-1-2.
//

#ifndef MAO_LANGUAGE_VARIABLE_LIST_H
#define MAO_LANGUAGE_VARIABLE_LIST_H

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

void VariableInsert(char *variable, int type) {//插入变量，并赋上类型和初值0，
    if (strcmp(variable, "int") == 0 || strcmp(variable, "double") == 0 || strcmp(variable, "print") == 0) {
        RESERVED_VAR;//保留变量名报错
    }
    if (OperandType(variable) != VAR_TYPE) {
        INVALID_VAR;//非法变量名报错
    }
    if(VarPosGet(variable)!=NULL) {
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
    if(temp==NULL)
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

#endif //MAO_LANGUAGE_VARIABLE_LIST_H
