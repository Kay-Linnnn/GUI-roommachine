#ifndef _STACK_LVGL_SYS_H
#define _STACK_LVGL_SYS_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "../../gui_app.h"
#include "GUI_self_define.h"
#include "../gui_sys_control/gui_scr_sys.h"

typedef struct ui_stack{  
    struct screen_class *scr_top;  
    struct ui_stack *scr_previous;
}_ui_stack;

void ui_stackInit(void);
bool ui_stackEmpty(void);
bool  ui_stackPush(struct screen_class *element);
void ui_stackPop(void);
struct screen_class* ui_Get_StackPre(void);


//my stack
typedef int ElmType;

typedef struct MyStack {
	ElmType *base;
	ElmType *top;
	int size;
}_MyStack;

enum {
	Stack_empty,
	Stack_error,
	Stack_ok,
	Stack_overflow,
};
typedef int StackState;

StackState my_stack_init(_MyStack *s, int size);
StackState my_stack_is_empty(_MyStack *s);
StackState my_stack_get_top(_MyStack *s, ElmType *out_e);
StackState my_stack_push(_MyStack *s, ElmType e);
StackState my_stack_pop(_MyStack *s, ElmType *out_e);
void my_stack_destory(_MyStack *s);



#ifdef __cplusplus
 }
#endif
 
#endif

