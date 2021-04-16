
#ifndef __FNC_DATAJSONANALYTIC_H
#define __FNC_DATAJSONANALYTIC_H

#include "gui_app.h"
#include "config_project.h"

#ifndef _VS_SIMULATOR
#define JSON_P_DEBUG_ON		1

#define JSON_DEBUGE(fmt,arg...) 	do{\
                                         if(JSON_P_DEBUG_ON)\
                                         printf("<<-JSON-DEBUG->> "fmt"\n", ##arg);\
                                      }while(0)

#define JSON_P_ERROR(fmt,arg...) 		printf("<<-JSON-ERROR->> "fmt"\n",##arg)			

#define JSON_P_INFO(fmt,arg...) 			printf("<<-JSON-INFO->> "fmt"\n",##arg)

#define SYS_P_ERROR(fmt,arg...)		printf("<<_SYS_ERROR->> "fmt"\n",##arg)
										
#define SYS_INFO(fmt,arg...)			printf("<<-SYS-INFO->> "fmt"\n",##arg)

#define SYS_DEBUGE(fmt,arg...)		printf("<<-SYS-DEBUG->> "fmt"\n",##arg)

#else

#define JSON_P_DEBUG_ON		1
#define JSON_DEBUGE(fmt,...)   do{\
										if(JSON_P_DEBUG_ON)\
										printf("<<-JSON-DEBUG->> "fmt"\n",__VA_ARGS__);\
									}while(0)


#define JSON_P_ERROR(fmt,...)		printf("<<-JSON-ERROR->> "fmt"\n",__VA_ARGS__)

#define JSON_P_INFO(fmt,...)		printf("<<-JSON-INFO->> "fmt"\n",__VA_ARGS__)


#endif // !_VS_SIMULATOR

#define FNC_Datajson_check_P(_ptr,_tag) \
do{\
	if(_ptr == NULL)\
	{\
		printf("<Invalid Pointer>"_tag"\n");\
		return -1;\
	}\
}while(0);



//从文件中读取一个 json 对象
int fncjson_fileloader(const char *path);

// 在对象数组中添加一个子对象
// path ：     json 文件目录
// Array_str ： 要添加组的子对象的索引头
int fncjson_AddArrayChildObjectItem(char *path,char *Arrayitemname,char(*namestr)[50],char(*vlauestr)[50],int addnum);

/*====================================================================
// 根据索引名和索引号获取item中数组中的一个对象  
path 		  : json 文件路径
Arrayitemname : json  对象组名
sy_strname    : 查找子对象的索引名
sy_strnum     ：查找子对象的索引号 
outtab		  ：子对象所有类型的信息
======================================================================*/

int fncjson_GetArrayChildObjectItem(char *path,char *Arrayitemname,char *sy_strname,char *sy_strnum,char(*outtab)[50]);



// 在对象数组中删除一个子对象
// path ：     json 文件目录
// Array_str ： 要删除数组子对下的索引头
int fncjson_DetachArrayChildObjectItem(char *path,char *Arrayitemname,char *sy_strname,char *sy_strnum);



/*====================================================================
// 添加一个对象  
path : json 文件路径
name： 对象名
string：对象值
======================================================================*/
int fncjson_AddObjectItem(char *path,char *name,char *string);


/*====================================================================
// 删除一个item  
path : json 文件路径
name： 对象名
string：对象值
======================================================================*/
int fncjson_DetachObjectItem(const char *path, const char *Item_str);


/*====================================================================
// 获取指定对象的值  
path : json 文件路径
name： 对象名
string：对象值
======================================================================*/
int fncjson_GetObjectItem_oncestrValue(char *path,char *utem_name,char *out_str);


/*==============================================================================
// 获取指定对象的值  
path : json 文件路径
name： 对象名
string：对象值
================================================================================*/
int fncjson_GetObjectItem_multistrValue(char *path,char (*nametab)[50],char (*out_tab)[50],int num);


/*
******Descr: add a object to root object
***@Para->object_name: name
***@Para->object_item_name[] : group of itme name
***@Para->ovject_item_value[] : string value of each item of object added
*/
int fncjson_AddObject(const char *path, const char *object_name, const char *object_item_name[], const char *object_item_value[], int item_num);


/*=============
//修改指定对象的string值
***@Para->Arrayitename: Item Name, NULL if item is single array of root
***@Para->itemname: itme name whose string value about to change
***@Para->want_string:	string value want
***@Return : 0 if success or -1 otherwise
*/
int fncjson_SetArrayString_By_ObjectItem(const char *path, char *Arrayitemname, const char *itemname,const char *want_string);

/*
*****Descr: Get String Value of item from rootjs or object of rootjs, it can be multi-item
***@Para->ObjectName:  object name of rootjs, if NULL then use rootjs as source of item
***@Para->item_name[] : item_name group to search item
***@Para->des_item_value : destination of string value of item
***@Para->item_num: item number,it's value equals to the dimension of "item_name" or "des_item_value",
	Note: it must not larger than the dimension of item_name/des_item_value
***@Return: 0 if success or -1 othersise
*/
int fncjson_GetObjectItem_StringValue(const char *path, const char *ObjectName, const char *item_name[], const char(*des_item_value)[50], int item_num);

/*
*****Descr: change string value of object/item
***@Para-> path: json file path
***@Para-> ObjectName: object name of rootjs, NULL if object is item of rootjs, object can be multi-value, or item the single value
***@Para-> item_name: item name group whose value about to change
***@Prar-> item_set_value: string value set to item_name correspondingly
***@Para-> item_num : item number match to num of item_name/item_set_value, 1 if ObjectName is NULL
***@Return: 0 is success or -1 otherwise
*/
int fncjson_SetStringValue_by_object(const char *path, const char *ObjectName, const char *item_name[], const char *item_set_value[], int item_num);

/*
******Descr: add item into array of rootjs
***@Para-> path: json file path
***@Para->ArrayName: the name of array to add item, if array is not exist, this will create a array into rootjs
***@Para->item_name: name array of item add to arrayjs
***@Para->item_value: value array of item match ot item_name
***@Para->item_num: length of array of item_name/item_value
***@Return: 0 if success or -1 otherwise
*/
int fncjson_AddItem_IntoArray_OfRootjs(const char *path, const char *ArrayName, const char *item_name[], const char *item_value[], int item_num);

/*
******Descr: delete item in array of rootjs and reorder auto
***@Para-> path: json file path
***@Para-> ArrayName: Array name
***@Para-> copy_item_name: name of item whose string value used
***@Para-> index: index of item to delete, note: the index of array is begin with 0 and end with (array_size-1)
***@Para-> auto_del_array: whether to delete array if array is empty, true to del and false not
***Return: 0 if success of -1 otherwise
*/
int fncjson_DeleteItem_InArray_OfRootjs_by_ArrayIndex_AutoReorder(const char *path, const char *ArrayName, const char *copy_item_name, int index, bool auto_del_array);

/*
*****Descr: get array item group by index
***@Para-> path: json file path
***@Para-> ArrayName: array name
***@Para-> item_name: array item name
***@Para-> des_value: destination to get string value
***@Para-> item_num: item number
***@Para-> index_in_array: index of item in array
***Return: 0 if success or -1 otherwise
*/
int fncjson_GetItemGroup_InArray_OfRootjs_by_ArrayIndex(const char *path, const char *ArrayName, const char *item_name[], char(*des_value)[50], int item_num, int index_in_array);

/*
*****Descr: Set array item value of rootjs by array index
***@Para-> path: json file path
***@Para-> ArrayName: name
***@Para-> item_name: array iten name to set value
***@Para-> item_value: value to set
***@item_num: quanlity of item to set
***@index_in_array: item index in array
***@Return: 0 if success or -1 otherwise
*/
int fncjson_SetStringValue_InArrayItem__OfRootjs_byArrayIndex(const char *path, const char *ArrayName, const char *item_name[], const char *item_value[], int item_num, int index_in_array);

/*
*****Descr: Get seleted array size
***@Para->path: json file path
***@Para->ArrayName: name
*/
int fncjson_get_array_size(const char *path, const char *ArrayName);

/*==================
  ********Test******
====================*/
/*
*****Descr: delete item of array in rootjs by array index
***@Para-> path: json file path
***@Para-> ArrayName: Array name
***@Para-> index: index of item to delete of array
***@Para-> auto_del_array: whether to delete array if array is empty, true to del and false not
***Return: 0 if success of -1 otherwise
*/
int fncjson_DeleteItem_InArray_OfRootjs_by_ArrayIndex(const char *path, const char *ArrayName, int index, bool auto_del_array);

/*
*****Descr: copy one item from array to another by array index
***@Para-> path: json file path
***@Para-> ArrayName: as you see
***@Para-> copy_item_name: name of item whose string value used
***@Para-> des_index: index of destinatoin item
***@Pata-> src_index: index of source item
***Return: 0 if success of -1 otherwise
*/
int fncjson_CopyItemString_InArray_OfRootjs_by_ArrayIndex(const char *path, const char *ArrayName, const char *copy_item_name, int des_index, int src_index);




int fncjson_GetSystemOnceInfo(char *typeinfo);



int fncjson_AnalyticData(void);




void cjsontest_main();

void test_json_api();

#endif
