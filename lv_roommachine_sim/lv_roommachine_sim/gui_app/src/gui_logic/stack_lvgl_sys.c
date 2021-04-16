#include "stack_lvgl_sys.h"
//#include<stdbool.h>  
//#include<stdlib.h>  

#define ui_malloc(size_t)   (_ui_stack *)LV_MEM_CUSTOM_ALLOC(size_t)
#define ui_free(p)           LV_MEM_CUSTOM_FREE((void *)p)

_ui_stack *active_stack = NULL;

typedef struct ui_stack_trash{
	_ui_stack *trash_head;
	_ui_stack *trash_tail;
}_ui_stack_trash;
_ui_stack_trash trash_stack;


//init the UI stack
void ui_stackInit()
{
  active_stack = NULL;
  //memset(&trash_stack,0,sizeof(_ui_stack_trash));
  trash_stack.trash_head = NULL;
  trash_stack.trash_tail = NULL;
}

//check whether current stack is empty
bool ui_stackEmpty()
{
  if(active_stack == NULL){
  	return true;
  	}
  else{
  	return false;
  	}
}

//
bool  ui_stackPush(struct screen_class  *element)
{
  _ui_stack *p = ui_malloc(sizeof(_ui_stack));
  if(NULL == p){
  	ui_printf("ui_stackPush,malloc failed");
  	return false;
  	}
  p->scr_top = element;
  p->scr_previous = active_stack;
  //p->screen_id = scr_id;
  active_stack = p;
  return true;
}

void ui_stackPop()
{
  if(!ui_stackEmpty()){
  	_ui_stack *item = active_stack;
	active_stack = active_stack->scr_previous;
    ui_free(item);
  	}
}

Landing_scr* ui_Get_StackPre()
{
  if(NULL != active_stack){
  	if(NULL != active_stack->scr_previous){
		return active_stack->scr_previous->scr_top;
  		}
	else return NULL;
  	}
  else return NULL;
}



/*=====================
*****Normal Stack******
======================*/

StackState my_stack_init(_MyStack *s, int size)
{
	s->base = (ElmType *)general_malloc(sizeof(ElmType) * size);
	if (s->base == NULL)
		return Stack_error;

	s->top = s->base;
	s->size = size;
	return Stack_ok;
}

StackState my_stack_is_empty(_MyStack *s)
{
	if (s->top == s->base)
		return Stack_empty;

	return Stack_ok;
}

StackState my_stack_get_top(_MyStack *s, ElmType *out_e)
{
	if (s->base == s->top)
		return Stack_empty;
	
	*out_e = *(s->top - 1);
	return Stack_ok;
}

StackState my_stack_push(_MyStack *s, ElmType e)
{
	if (s->top - s->base >= s->size)
	{
		//to do, overflow
		return Stack_overflow;
	}
	*(s->top)++ = e;
	return Stack_ok;
}

StackState my_stack_pop(_MyStack *s, ElmType *out_e)
{
	if (s->top == s->base)
		return Stack_empty;

	*out_e = *--(s->top);
	return Stack_ok;
}

void my_stack_destory(_MyStack *s)
{
	if (s->base != NULL)
	{
		general_free(s->base);
		s->base = s->top = NULL;
		s->size = 0;
	}	
}
