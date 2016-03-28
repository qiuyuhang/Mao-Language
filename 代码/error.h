//
// Created by qyh on 16-1-2.
//

#ifndef MAO_LANGUAGE_ERROR_H
#define MAO_LANGUAGE_ERROR_H

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

#endif //MAO_LANGUAGE_ERROR_H
