

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cJSON.h"
#include "fnc_DataJsonAnalytic.h"

#if 0
#define str_addressbook_path  	"/home/rfs/share/data/addressbook.json"
#define str_systemcfg_path  	"/home/rfs/share/data/systemcfg.json"
#define str_guestbook_path  	"/home/rfs/share/data/guestbook.json"
#endif
#define str_addressbook_path	"ui_data/addressbook.json"
#define str_systemcfg_path		"ui_data/systemcfg.json"
#define str_guestbook_path		"ui_data/guestbook.json"
#define str_testpro_path		"ui_data/test_pro.json"
char *datafilebuf;

static int print_preallocated(cJSON *root)
{
	/* declarations */
	char *out = NULL;
	char *buf = NULL;
	char *buf_fail = NULL;
	size_t len = 0;
	size_t len_fail = 0;

	/* formatted print */
	out = cJSON_Print(root);
	printf("print_preallocated:\n");

	/* create buffer to succeed */
	/* the extra 5 bytes are because of inaccuracies when reserving memory */
	len = strlen(out) + 5;
	buf = (char*)malloc(len);
	if (buf == NULL)
	{
		printf("Failed to allocate memory.\n");
		exit(1);
	}

	/* create buffer to fail */
	len_fail = strlen(out);
	buf_fail = (char*)malloc(len_fail);
	if (buf_fail == NULL)
	{
		printf("Failed to allocate memory.\n");
		exit(1);
	}

	/* Print to buffer */
	if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
		printf("cJSON_PrintPreallocated failed!\n");
		if (strcmp(out, buf) != 0) {
			printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
			printf("cJSON_Print result:\n%s\n", out);
			printf("cJSON_PrintPreallocated result:\n%s\n", buf);
		}
		free(out);
		free(buf_fail);
		free(buf);
		return -1;
	}

	/* success */
	printf("%s\n", buf);

	/* force it to fail */
	if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
		printf("cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
		printf("cJSON_Print result:\n%s\n", out);
		printf("cJSON_PrintPreallocated result:\n%s\n", buf_fail);
		free(out);
		free(buf_fail);
		free(buf);
		return -1;
	}

	free(out);
	free(buf_fail);
	free(buf);
	return 0;
}

//从文件中读取一个 json 对象
int fncjson_fileloader(const char *path)
{
	FILE *f;
	long len;
	f=fopen(path,"rb");
	if(f == NULL)
	{
		JSON_DEBUGE("Fail to open file! ");
		return -1;	
	}
	JSON_DEBUGE("open filedata");
	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);
	datafilebuf=(char*)malloc(len+1);
	fread(datafilebuf,1,len,f);
	fclose(f);
	return 0;	
}

// 在对象数组中添加一个子对象
// path ：     json 文件目录
// Array_str ： 要添加组的子对象的索引头
int fncjson_AddArrayChildObjectItem(char *path,char *Arrayitemname,char(*namestr)[50],char(*vlauestr)[50],int addnum)
{
	int i;
	char *json_str;
	cJSON *cjson,*json_Array,*pointTempObj;
	if(fncjson_fileloader(path))
	{
		JSON_P_ERROR("not find json file!, creat a json file");
		return 0;
	}
	json_str = datafilebuf;
	cjson=cJSON_Parse(json_str); // 打包JSON 字符串
	if (!cjson)
	{
		JSON_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}
	json_Array = cJSON_GetObjectItem(cjson , Arrayitemname);  
	// 添加对象到数组中
	cJSON_AddItemToArray(json_Array, pointTempObj = cJSON_CreateObject());
	///cJSON_AddStringToObject(pointTempObj, "country", "test");
	for(i  = 0;i < addnum;i++)
	{
		cJSON_AddStringToObject(pointTempObj, namestr[i], vlauestr[i]);	
	}
	char *strin = cJSON_Print(cjson);
	FILE *fp = fopen(path, "wb+");
	fprintf(fp, strin);
	fclose(fp);
	cJSON_Delete(cjson);  
	free(datafilebuf);
	return 0;  
}

/*====================================================================
// 添加一个对象  
path 		  : json 文件路径
Arrayitemname : json  对象组名
sy_strname    : 查找子对象的索引名
sy_strnum     ：查找子对象的索引号 
outtab		  ：子对象所有类型的信息
======================================================================*/

int fncjson_GetArrayChildObjectItem(char *path,char *Arrayitemname,char *sy_strname,char *sy_strnum,char(*outtab)[50])
{
	int i,j;
	char *json_str;
	//cJSON *ip_str;
	//cJSON *desc_str;
	if(fncjson_fileloader(path))
	{
		return -1;	
	}
	json_str = datafilebuf;
	cJSON *cjson=cJSON_Parse(json_str);  //打包JSON 字符串
	if (!cjson)
	{
		JSON_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}
	cJSON *json_Array = cJSON_GetObjectItem(cjson, Arrayitemname);  //获取数组对象
	if(!json_Array)
	{  
		JSON_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		cJSON_Delete(cjson);
		free(datafilebuf);
		return -1;
	}
	int json_Array_size = cJSON_GetArraySize(json_Array);   // 获取数组对象个数
	cJSON *json_Array_child = json_Array->child;  //  子对象
	cJSON *child_value;
	for(i = 0; i< json_Array_size; i++) 
	{        
		child_value = cJSON_GetObjectItem(json_Array_child,sy_strname);
		if(strcmp(child_value->valuestring,sy_strnum) == 0)
		{
			int child_size;
			child_size = cJSON_GetArraySize(json_Array_child);  //  获取子对下的个数
			for(j =0;j<child_size;j++)
			{
				child_value = cJSON_GetArrayItem(json_Array_child,j);//  获取所有子对象的值	
				//outtab[i] = child_value->valuestring;
				strcpy(outtab[j],child_value->valuestring);
				JSON_DEBUGE("Arraychild value [%s] , num = %d",child_value->valuestring,j);
			}
			//cJSON_GetArrayItem(json_Array,i+1)
			break;
		}
		json_Array_child = json_Array_child->next;
    }
	cJSON_Delete(cjson);  
	free(datafilebuf);
	return 0;
}



// 在对象数组中删除一个子对象
// path ：     json 文件目录
// Array_str ： 要删除数组子对下的索引头
int fncjson_DetachArrayChildObjectItem(char *path,char *Arrayitemname,char *sy_strname,char *sy_strnum)
{
	int i;
	char *json_str;
	unsigned char J_FLAG = 0;
	//cJSON *ip_str;
	//cJSON *desc_str;
	if(fncjson_fileloader(path))
	{
		return -1;	
	}
	json_str = datafilebuf;
	cJSON *cjson=cJSON_Parse(json_str);    //打包JSON 字符串
	if (!cjson)
	{
		JSON_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}
	cJSON *json_Array = cJSON_GetObjectItem(cjson, Arrayitemname);  //获取数组对象
	if(!json_Array)
	{  
		JSON_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		cJSON_Delete(cjson);  
		free(datafilebuf);
		return -1;
	}
	int json_Array_size = cJSON_GetArraySize(json_Array);   // 获取数组对象个数
	cJSON *json_Array_child = json_Array->child;  //  子对象
	JSON_DEBUGE("json_Array_size = %d",json_Array_size);
	cJSON *child_value; 
	
	for(i = 0; i< json_Array_size; i++) 
	{        
		child_value = cJSON_GetObjectItem(json_Array_child,sy_strname);
		if(strcmp(child_value->valuestring,sy_strnum) == 0)
		{
			cJSON_DetachItemFromArray(json_Array,i);
			J_FLAG = 1;
			break;
		}
		json_Array_child = json_Array_child->next;
    }
	JSON_DEBUGE("===============================================================");
	if(J_FLAG == 1)
	{
		char *strin = cJSON_Print(cjson);
		FILE *fp = fopen(path, "wb+");
		fprintf(fp, strin);
		fclose(fp);
	}
	cJSON_Delete(cjson);  
	free(datafilebuf);
	return 0;
}


/*====================================================================
// 添加一个对象  
path : json 文件路径
name： 对象名
string：对象值
======================================================================*/
int fncjson_AddObjectItem(char *path,char *name,char *string)
{
	int i = 0;
	char *json_str = NULL;
	cJSON *cjson = NULL;
	if(fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	cjson=cJSON_Parse(json_str); // 打包JSON 字符串
	if (!cjson)
	{
		JSON_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}
	// 数据创建
	cJSON_AddStringToObject(cjson, name,string);
	char *strin = cJSON_Print(cjson);
	FILE *fp = fopen(path, "wb+");
	fprintf(fp, strin);
	fclose(fp);
	cJSON_Delete(cjson);  
	free(datafilebuf);
	return 0;  
}

/*====================================================================
// 删除一个对象  
path : json 文件路径
name： 对象名
string：对象值
======================================================================*/
int fncjson_DetachObjectItem(const char *path, const char *Item_str)
{
	char *json_str = NULL;
	cJSON *cjson = NULL;
	if(fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	cjson=cJSON_Parse(json_str); // 打包JSON 字符串
	if (!cjson)
	{
		JSON_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}
	// 数据删除
	cJSON_DetachItemFromObject(cjson, Item_str);
	char *strin = cJSON_Print(cjson);
	FILE *fp = fopen(path, "wb+");
	fprintf(fp, strin);
	fclose(fp);
	cJSON_Delete(cjson);  
	free(datafilebuf);
	return 0;  
}

/*====================================================================
// 获取指定对象的值  
path : json 文件路径
name： 对象名
string：对象值
======================================================================*/
int fncjson_GetObjectItem_oncestrValue(char *path,char *utem_name,char *out_str)
{
	char *json_str;
	cJSON *cjson,*json_Array;
	if(fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	cjson=cJSON_Parse(json_str); // 打包JSON 字符串
	if (!cjson)
	{
		JSON_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}
	// 数据读取
	json_Array = cJSON_GetObjectItem(cjson, utem_name);  
	JSON_DEBUGE("SystemOnceInfo type = %s",json_Array->valuestring);
	strcpy(out_str,json_Array->valuestring);
	
	cJSON_Delete(cjson);  
	free(datafilebuf);
	return 0;  
}

/*==============================================================================
// 获取指定对象的值  
path : json 文件路径
name： 对象名
string：对象值
================================================================================*/
int fncjson_GetObjectItem_multistrValue(char *path,char (*nametab)[50],char (*out_tab)[50],int num)
{
	int i;
	char *json_str;
	cJSON *cjson,*json_Array;
	if(fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	cjson=cJSON_Parse(json_str); // 打包JSON 字符串
	if (!cjson)
	{
		JSON_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}
	// 数据读取
	for(i =0;i<num;i++)
	{
		json_Array = cJSON_GetObjectItem(cjson, nametab[i]);
		strcpy(out_tab[i],json_Array->valuestring);
		JSON_DEBUGE("get valeu = %s",json_Array->valuestring);
	}
	cJSON_Delete(cjson);  
	free(datafilebuf);
	return 0;  
}



int fncjson_GetSystemOnceInfo(char *typeinfo)
{
	char *json_str;
	cJSON *cjson,*json_Array;
	if(fncjson_fileloader(str_systemcfg_path))
	{
		goto fncjson_GetSystemOnceInfo_error;
	}
	json_str = datafilebuf;
	cjson=cJSON_Parse(json_str); // 打包JSON 字符串
	if (!cjson)
	{
		JSON_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		goto fncjson_GetSystemOnceInfo_error;
	}
	
	// 数据读取
	json_Array = cJSON_GetObjectItem(cjson, "admin_password");  
	printf("SystemOnceInfo type = %s\n",json_Array->valuestring);
	
	// 数据更改
	cJSON_ReplaceItemInObject(cjson,"security_password",cJSON_CreateString("123456"));
	cJSON *Replace_js = cJSON_GetObjectItem(cjson, "security_password");
	printf("Replace_js type = %s\n",Replace_js->valuestring);
	// 数据删除
	cJSON_DetachItemFromObject(cjson, "hobby");
	
	// 数据创建
	cJSON_AddStringToObject(cjson, "country", "test");

	
	char *strin = cJSON_Print(cjson);
	FILE *fp = fopen(str_systemcfg_path, "wb+");
	fprintf(fp, strin);
	//char *cfgh = read_config_fp(fp,NULL);
	//printf("%s\n",cfgh);
	fclose(fp);
	
	cJSON_Delete(cjson);  
	free(datafilebuf);
	return 0;  
	fncjson_GetSystemOnceInfo_error:
	free(datafilebuf);
	return -1;
}


int fncjson_AnalyticData(void)
{
	int i;
	char *json_str;
	//cJSON *ip_str;
	//cJSON *desc_str;
	return 0;
	if(fncjson_fileloader(str_addressbook_path))
	{
		JSON_P_ERROR("file load error");
		return -1;	
	}
	json_str = datafilebuf;
	cJSON *cjson=cJSON_Parse(json_str);    //打包JSON 字符串
	if (!cjson)
	{
		SYS_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
		goto fncjson_AnalyticData_error;
	}
	//获取数组对象
	cJSON *json_Array = cJSON_GetObjectItem(cjson, "addressinfo");  
	if(!json_Array)
	{  
		SYS_P_ERROR("JSON : [%s]",cJSON_GetErrorPtr());
	}
	// 获取数组对象个数
	int json_Array_size = cJSON_GetArraySize(json_Array);  
	cJSON *json_Array_child = json_Array->child;  //  子对象
	SYS_DEBUGE("json_Array_size = %d",json_Array_size);
	cJSON *child_value;
	for(i = 0; i< json_Array_size; i++) 
	{
		child_value = cJSON_GetObjectItem(json_Array_child,"addressnum");
		//ip_str = cJSON_GetObjectItem(json_Array_child,"ip");
		//desc_str = cJSON_GetObjectItem(json_Array_child,"desc");
		//json_Array_child = json_Array_child->next;
		printf("<<<<< json_Array_num >>>>> = %d\n",i);
		printf("ip_str = %s\n",child_value->valuestring);
		//printf("addressnum_str = %s\n",cJSON_GetObjectItem(json_Array_child,"addressnum")->valuestring);
		//printf("ip_str = %s\n",*ip_str);
		//printf("desc_str = %s\n",*desc_str);
		json_Array_child = json_Array_child->next;
    }
	if(cjson) 
	{ 
		cJSON_Delete(cjson);  
		free(datafilebuf);
		return 0;  
	}
	fncjson_AnalyticData_error:
	free(datafilebuf);
	return -1;
}

//make sure whether create_array need to free or not
/*
******Descr: add a object to root object
***@Para->object_name: name
***@Para->object_item_name[] : group of itme name
***@Para-?ovject_item_value[] : string value of each item of object added
*/
int fncjson_AddObject(const char *path, const char *object_name, const char *object_item_name[],const char *object_item_value[], int item_num)
{
	char *json_str = NULL;
	cJSON *json_root = NULL, *json_array = NULL;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}

	int i = 0;
	cJSON *js_array_add = cJSON_CreateObject();//cJSON_CreateArray();
	for (i = 0; i < item_num; i++)
	{
		//cJSON_AddItemToArray(js_array_add, cJSON_CreateString(array_item_name[i]));
		cJSON_AddItemToObject(js_array_add, object_item_name[i], cJSON_CreateString(object_item_value[i]));
	}
	cJSON_AddItemToObject(json_root, object_name, js_array_add);

	//rewrite data into file
	char *strin = cJSON_Print(json_root);
	FILE *fp = fopen(path, "wb+");
	if (fp != NULL)
	{
		fprintf(fp, strin);
		fclose(fp);
	}

	cJSON_Delete(json_root);
	free(datafilebuf);
	return 0;
}

/*=============
//修改指定对象的string值
***@Para->Arrayitename: Item Name, NULL if item is single array of root
***@Para->itemname: itme name whose string value about to change
***@Para->want_string:	string value want
***@Return : 0 if success or -1 otherwise
*/
int fncjson_SetArrayString_By_ObjectItem(const char *path,char *Arrayitemname,const char *itemname,const char *want_string)
{
	char *json_str = NULL;
	cJSON *json_root = NULL, *json_array = NULL;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}
	//to do
	if (Arrayitemname != NULL) {
		//
	}

	cJSON_ReplaceItemInObject(json_root, itemname, cJSON_CreateString(want_string));

	//rewrite data into file
	char *strin = cJSON_Print(json_root); 
	FILE *fp = fopen(path, "wb+");
	if (fp != NULL)
	{
		fprintf(fp, strin);
		fclose(fp);
	}
	
	cJSON_Delete(json_root);
	free(datafilebuf);
	return 0;
}

/*
*****Descr: Get String Value of item from rootjs or object of rootjs, it can be multi-item
***@Para->ObjectName:  object name of rootjs, if NULL then use rootjs as source of item
***@Para->item_name[] : item_name group to search item
***@Para->des_item_value[] : destination of string value of item
***@Para->item_num: item number,it's value equals to the dimension of "item_name" or "des_item_value",
	Note: it must not larger than the dimension of item_name/des_item_value
***@Return: 0 if success or -1 othersise
*/
int fncjson_GetObjectItem_StringValue(const char *path, const char *ObjectName, const char *item_name[], const char (*des_item_value)[50], int item_num)
{
	char *json_str = NULL;
	cJSON *json_root = NULL;
	int i = 0;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}

	cJSON *js_object = json_root;
	//cJSON *js_get_item = json_root;
	cJSON *js_item = NULL;
	if (ObjectName != NULL)
	{
		js_object = cJSON_GetObjectItem(json_root, ObjectName);
		//js_get_item = js_object;
	}	
#if 1
	for (i = 0; i < item_num; i++)
	{
		js_item = cJSON_GetObjectItem(js_object, item_name[i]);
		print_preallocated(js_item);
		if (js_item != NULL)
			strcpy(des_item_value[i], cJSON_GetStringValue(js_item));
			//JSON_P_INFO("Get item string value:%s", cJSON_GetStringValue(js_item));
	}
#endif
	cJSON_Delete(json_root);
	free(datafilebuf);

	return 0;
}

/*
*****Descr: change string value of object/item
***@Para-> path: json file path
***@Para-> ObjectName: object name of rootjs, NULL if object is item of rootjs, object can be multi-value, or item the single value
***@Para-> item_name[]: item name group whose value about to change
***@Prar-> item_set_value[]: string value set to item_name correspondingly
***@Para-> item_num : item number match to num of item_name/item_set_value, 1 if ObjectName is NULL
***@Return: 0 is success or -1 otherwise
*/
int fncjson_SetStringValue_by_object(const char *path, const char *ObjectName, const char *item_name[], const char *item_set_value[], int item_num)
{
	char *json_str = NULL;
	cJSON *json_root = NULL;
	int item_cnt = 0;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}

	cJSON *js_object = json_root;
	if (ObjectName != NULL)
	{
		js_object = cJSON_GetObjectItem(json_root, ObjectName);
	}
	
	for (item_cnt = 0; item_cnt < item_num; item_cnt++)
	{
		cJSON_ReplaceItemInObject(js_object, item_name[item_cnt], cJSON_CreateString(item_set_value[item_cnt]));
	}	

	//rewrite data into file
	char *strin = cJSON_Print(json_root);
	FILE *fp = fopen(path, "wb+");
	if (fp != NULL)
	{
		fprintf(fp, strin);
		fclose(fp);
	}

	cJSON_Delete(json_root);
	free(datafilebuf);

	return 0;
}

/*
******Descr: add item into array of rootjs
***@Para-> path: json file path
***@Para->ArrayName: the name of array to add item, if array is not exist, this will create a array into rootjs
***@Para->item_name: name array of item add to arrayjs
***@Para->item_value: value array of item match ot item_name
***@Para->item_num: length of array of item_name/item_value
***@Return: 0 if success or -1 otherwise
*/
int fncjson_AddItem_IntoArray_OfRootjs(const char *path, const char *ArrayName, const char *item_name[], const char *item_value[], int item_num)
{
	char *json_str = NULL;
	cJSON *json_root = NULL;
	int item_cnt = 0;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}

	//cJSON *js_object = json_root;

	cJSON *js_array = cJSON_GetObjectItem(json_root, ArrayName);
	bool array_exist = false;
	//cJSON *js_array = cJSON_CreateArray();
	if (js_array == NULL)
	{
		js_array = cJSON_CreateArray();
		array_exist = true;
	}
	JSON_P_INFO("array size:%d", cJSON_GetArraySize(js_array));	
	
	cJSON *ArrayItem = cJSON_CreateObject();
	
	for (item_cnt = 0; item_cnt < item_num; item_cnt++)
	{
		cJSON_AddStringToObject(ArrayItem, item_name[item_cnt], item_value[item_cnt]);
	}
	cJSON_AddItemToArray(js_array, ArrayItem);

	if(array_exist)// array not exist, add array into rootfs as item
		cJSON_AddItemToObject(json_root, ArrayName, js_array);

	//rewrite data into file
	char *strin = cJSON_Print(json_root);
	FILE *fp = fopen(path, "wb+");
	if (fp != NULL)
	{
		fprintf(fp, strin);
		fclose(fp);
	}

	cJSON_Delete(json_root);
	free(datafilebuf);

	return 0;
}

/*
******Descr: delete item in array of rootjs and reorder auto
***@Para-> path: json file path
***@Para-> ArrayName: Array name
***@Para-> copy_item_name: name of item whose string value used
***@Para-> index: index of item to delete, note: the index of array is begin with 0 and end with (array_size-1)
***@Para-> auto_del_array: whether to delete array if array is empty, true to del and false not
***Return: 0 if success of -1 otherwise
*/
int fncjson_DeleteItem_InArray_OfRootjs_by_ArrayIndex_AutoReorder(const char *path, const char *ArrayName, const char *copy_item_name, int index, bool auto_del_array)
{
	char *json_str = NULL;
	cJSON *json_root = NULL;
	int item_cnt = 0;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}

	cJSON *js_array = cJSON_GetObjectItem(json_root, ArrayName);
	//cJSON *array_sub = NULL;

	if (js_array == NULL)
	{
		JSON_P_ERROR("array delete, array->%s not exist", ArrayName);
		return -1;
	}

	int array_size = cJSON_GetArraySize(js_array);
	if (array_size <= 0 || array_size < index)
	{
		JSON_P_ERROR("array delete, array size is inappropriate");
		return -1;
	}

	cJSON *del_item = cJSON_GetArrayItem(js_array, index);
	if (del_item == NULL)
	{
		JSON_P_ERROR("array delete, delete item not exist");
	}
	//auto rearray
	cJSON *js_arrayitem_pre = NULL, *js_arrayitem_next = NULL;
	cJSON *js_pre = NULL, *js_next = NULL;
	//for (item_cnt = index; item_cnt < array_size - 1; item_cnt++)
	for (item_cnt = array_size - 2; item_cnt >= index; item_cnt--)
	{
		js_arrayitem_pre = cJSON_GetArrayItem(js_array, item_cnt);
		js_arrayitem_next = cJSON_GetArrayItem(js_array, item_cnt + 1);

		js_pre = cJSON_GetObjectItem(js_arrayitem_pre, copy_item_name);
		js_next = cJSON_GetObjectItem(js_arrayitem_next, copy_item_name);

		if (js_pre == NULL || js_next == NULL)
		{
			JSON_P_INFO("js_pre pr js_next is NULL");
			continue;
		}

		cJSON_ReplaceItemInObject(js_arrayitem_next, copy_item_name, cJSON_CreateString(cJSON_GetStringValue(js_pre)));
	}
	//delete
	cJSON_DeleteItemFromArray(js_array, index);

	if (cJSON_GetArraySize(js_array) == 0 && auto_del_array)
	{
		cJSON_DeleteItemFromObject(json_root, ArrayName);
	}

	//rewrite data into file
	char *strin = cJSON_Print(json_root);
	FILE *fp = fopen(path, "wb+");
	if (fp != NULL)
	{
		fprintf(fp, strin);
		fclose(fp);
	}

	cJSON_Delete(json_root);
	free(datafilebuf);

	return 0;
}

/*
*****Descr: delete item of array in rootjs by array index
***@Para-> path: json file path
***@Para-> ArrayName: Array name
***@Para-> index: index of item to delete of array
***@Para-> auto_del_array: whether to delete array if array is empty, true to del and false not
***Return: 0 if success of -1 otherwise
*/
int fncjson_DeleteItem_InArray_OfRootjs_by_ArrayIndex(const char *path, const char *ArrayName, int index, bool auto_del_array)
{
	char *json_str = NULL;
	cJSON *json_root = NULL;
	int item_cnt = 0;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}

	cJSON *js_array = cJSON_GetObjectItem(json_root, ArrayName);
	//cJSON *array_sub = NULL;

	if (js_array == NULL)
	{
		JSON_P_ERROR("array delete, array->%s not exist",ArrayName);
		return -1;
	}

	int array_size = cJSON_GetArraySize(js_array);
	if (array_size <= 0 || array_size < index)
	{
		JSON_P_ERROR("array delete, array size is inappropriate");
		return -1;
	}

	cJSON_DeleteItemFromArray(js_array, index);

	if (cJSON_GetArraySize(js_array) == 0 && auto_del_array)
	{
		//cJSON_DetachItemFromObject(json_root, ArrayName);
		cJSON_DeleteItemFromObject(json_root, ArrayName);
	}
	
	//rewrite data into file
	char *strin = cJSON_Print(json_root);
	FILE *fp = fopen(path, "wb+");
	if (fp != NULL)
	{
		fprintf(fp, strin);
		fclose(fp);
	}

	cJSON_Delete(json_root);
	free(datafilebuf);

	return 0;
}

/*
*****Descr: copy one item from array to another by array index
***@Para-> path: json file path
***@Para-> ArrayName: as you see
***@Para-> copy_item_name: name of item whose string value used
***@Para-> des_index: index of destinatoin item
***@Pata-> src_index: index of source item
***Return: 0 if success of -1 otherwise
*/
int fncjson_CopyItemString_InArray_OfRootjs_by_ArrayIndex(const char *path, const char *ArrayName, const char *copy_item_name, int des_index, int src_index)
{
	char *json_str = NULL;
	cJSON *json_root = NULL;
	int item_cnt = 0;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}

	cJSON *js_array = cJSON_GetObjectItem(json_root, ArrayName);
	//cJSON *array_sub = NULL;

	if (js_array == NULL)
	{
		JSON_P_ERROR("array exchange, array->%s not exist", ArrayName);
		return -1;
	}

	int array_size = cJSON_GetArraySize(js_array);
	if (array_size <= 0 || array_size < des_index || array_size < src_index)
	{
		JSON_P_ERROR("array exchange, array size not appropriate");
		return -1;
	}

	cJSON *js_arrayitem_1 = cJSON_GetArrayItem(js_array, des_index); 
	cJSON *js_arrayitem_2 = cJSON_GetArrayItem(js_array, src_index);

	if (js_arrayitem_1 == NULL || js_arrayitem_2 == NULL)
	{
		JSON_P_ERROR("array exchange, array item not found");
		return -1;
	}

	cJSON *js_item1 = cJSON_GetObjectItem(js_arrayitem_1, copy_item_name);
	cJSON *js_item2 = cJSON_GetObjectItem(js_arrayitem_2, copy_item_name);

	if (js_item1 == NULL || js_item2 == NULL)
	{
		JSON_P_ERROR("array exchange, item not found");
		return -1;
	}

	cJSON_ReplaceItemInObject(js_arrayitem_1, copy_item_name, cJSON_CreateString(cJSON_GetStringValue(js_item2)));

	//rewrite data into file
	char *strin = cJSON_Print(json_root);
	FILE *fp = fopen(path, "wb+");
	if (fp != NULL)
	{
		fprintf(fp, strin);
		fclose(fp);
	}

	cJSON_Delete(json_root);
	free(datafilebuf);

	return 0;
}

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
int fncjson_GetItemGroup_InArray_OfRootjs_by_ArrayIndex(const char *path, const char *ArrayName, const char *item_name[], char (*des_value)[50], int item_num, int index_in_array)
{
	char *json_str = NULL;
	cJSON *json_root = NULL;
	int item_cnt = 0;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}

	cJSON *js_array = cJSON_GetObjectItem(json_root, ArrayName);
	if (js_array == NULL)
	{
		JSON_P_ERROR("Get Item Group In array, array not found");
		return -1;
	}

	if (index_in_array > cJSON_GetArraySize(js_array))
	{
		JSON_P_ERROR("Get Item Group In Array, index invalid");
		return -1;
	}
	
	cJSON *js_arrayitem = cJSON_GetArrayItem(js_array, index_in_array);
	cJSON *js_item = NULL;
	if (js_arrayitem == NULL)
	{
		JSON_P_ERROR("Get Item Group In Array, array item not found");
		return -1;
	}
	
	for (item_cnt = 0; item_cnt < item_num; item_cnt++)
	{
		js_item = cJSON_GetObjectItem(js_arrayitem, item_name[item_cnt]);
		if (js_item != NULL)
			strcpy(des_value[item_cnt], cJSON_GetStringValue(js_item));
	}

	cJSON_Delete(json_root);
	free(datafilebuf);

	return 0;
}

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
int fncjson_SetStringValue_InArrayItem__OfRootjs_byArrayIndex(const char *path, const char *ArrayName, const char *item_name[], const char *item_value[], int item_num, int index_in_array)
{
	char *json_str = NULL;
	cJSON *json_root = NULL;
	int item_cnt = 0;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}

	cJSON *js_array = cJSON_GetObjectItem(json_root, ArrayName);
	FNC_Datajson_check_P(js_array,"SetArrayValue,array not found");

	cJSON *js_arrayitem = cJSON_GetArrayItem(js_array, index_in_array);
	FNC_Datajson_check_P(js_arrayitem,"SetArrayValue, arrayitem not found");

	//cJSON *js_item = NULL;

	for (item_cnt = 0; item_cnt < item_num; item_cnt++)
	{
		cJSON_ReplaceItemInObject(js_arrayitem, item_name[item_cnt], cJSON_CreateString(item_value[item_cnt]));
	}

	//rewrite data into file
	char *strin = cJSON_Print(json_root);
	FILE *fp = fopen(path, "wb+");
	if (fp != NULL)
	{
		fprintf(fp, strin);
		fclose(fp);
	}

	cJSON_Delete(json_root);
	free(datafilebuf);

	return 0;
	
}

/*
*****Descr: Get seleted array size
***@Para->path: json file path
***@Para->ArrayName: name
*/
int fncjson_get_array_size(const char *path, const char *ArrayName)
{
	char *json_str = NULL;
	cJSON *json_root = NULL;
	int item_cnt = 0;
	if (fncjson_fileloader(path))
	{
		return -1;
	}
	json_str = datafilebuf;
	json_root = cJSON_Parse(json_str);
	if (!json_root)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return -1;
	}

	cJSON *js_array = cJSON_GetObjectItem(json_root, ArrayName);

	if (js_array == NULL)
	{
		JSON_P_ERROR("array size, array->%s not exist", ArrayName);
		return -1;
	}

	int array_size = cJSON_GetArraySize(js_array);

	cJSON_Delete(json_root);
	free(datafilebuf);

	return array_size;
}


//static const char *const temp_test[] = {"region","building","unit"};
//static const char *const temp_test_value[] = {"01","001","01"};
void cjsontest_main()
{
	char outstr[10][50];
	char name_str_tab[][50] = { "index","date","timer","b_read","type" };
	char value_str_tab[][50] = { "test4","2021.1.24","1.4.1","4","4" };
	char value_str_tab_1[][50] = { "test5","2021.1.25","1.4.1","4","4" };
	memset(outstr, 0, sizeof(outstr));
	//=================================================================================
	//                 往一个json 文件指对象数组中写入子对象
	//=================================================================================
	//fncjson_AddArrayChildObjectItem(str_guestbook_path,"guestbookinfo",name_str_tab,value_str_tab,5);

	//=================================================================================
	//                 从一个json 文件指对象数组中读出所有子对象
	//=================================================================================
#if 0
	fncjson_GetArrayChildObjectItem(str_guestbook_path,"guestbookinfo","index","test3",outstr);
	int cnt_1 = 0;
	for (cnt_1 = 0; cnt_1 < 5; cnt_1++)
		JSON_P_INFO("outstr[%d]:%s", cnt_1, outstr[cnt_1]);

	fncjson_GetArrayChildObjectItem(str_addressbook_path, "addressinfo", "addressnum", "50010011010101", outstr);
	int cnt_1 = 0;
	for (cnt_1 = 0; cnt_1 < 5; cnt_1++)
		JSON_P_INFO("outstr[%d]:%s", cnt_1, outstr[cnt_1]);
#endif
	//=================================================================================
	//                 从一个json 文件 删除一个子对象
	//=================================================================================
	//fncjson_DetachArrayChildObjectItem(str_guestbook_path,"guestbookinfo","index","test2");

	//=================================================================================
	//                 从一个json 文件获取一个对象的值
	//=================================================================================
	//char itemstr[50];
	//fncjson_GetObjectItem_oncestrValue(str_systemcfg_path,"admin_password",itemstr);

	//=================================================================================
	//                 从一个json 文件获取一个对象的值
	//=================================================================================

	//=============
	//	Set String By Object Item
	//==============
	//fncjson_SetArrayString_By_ObjectItem(str_systemcfg_path, NULL, "language_setup", "2");

	/*=====Test array
	*/
	//fncjson_AddArray(str_testpro_path, "address", temp_test, temp_test_value, 3);

	/*===
	***test add/delete array item
	===*/
	const char * const array_item_name[] = { "index","date","timer","b_read" };
	const char * const array_item_value[] = { "6","2020-1-10","15:24:12","1" };
	//fncjson_AddItem_IntoArray_OfRootjs(str_testpro_path, "test array", array_item_name, array_item_value, 4);//array add
	//fncjson_DeleteItem_InArray_OfRootjs_by_ArrayIndex(str_testpro_path, "test array", 0, true);//array delete
	//fncjson_CopyItemString_InArray_OfRootjs_by_ArrayIndex(str_testpro_path, "test array", array_item_name[0], 0, 1);//array copy item
	//fncjson_DeleteItem_InArray_OfRootjs_by_ArrayIndex_AutoReorder(str_testpro_path, "test array", array_item_name[0], 1, true); //array delete auto reorder
	int array_size = fncjson_get_array_size(str_testpro_path, "test array");//array size
	printf("test array size:%d\n", array_size);
#if 0
	const char * const replace_item[] = {"region","building","unit"};
	const char * const item_value[] = {"01","002","03"};
	//fncjson_SetStringValue_by_object(str_testpro_path, "address", replace_item, item_value, 3);

	const char * const replace_country[] = {"country"};
	const char * const country_value[] = { "replace" };
	fncjson_SetStringValue_by_object(str_testpro_path, NULL, replace_country, country_value, 1);
#endif
	/*
	***Teset replace string value of array object
	*/

#if 0
	char f_name_str_tab[][50] = { "admin_password","security_password","CueTone_volume","machine_IP","machine_hw_ether" };
	char test_1[][50] = { "local_machine_id","server_ip" };
	fncjson_GetObjectItem_multistrValue(str_systemcfg_path, test_1, outstr, 2);
	ui_printf("local_machine_id:%s,server_ip:%s\n", outstr[0], outstr[1]);
#endif
	// 3 
	//fncjson_DetachArrayChildObjectItem
	//for(i =0;i<5;i++)
	//{
	//	JSON_DEBUGE("%s",outstr[i]);	
	//}

	//fncjson_AddChildObjectItem(str_guestbook_path,"guestbookinfo");
}




void printJson(cJSON * root)//以递归的方式打印json的最内层键值对
{
	printf("GetArraySize:%d\n", cJSON_GetArraySize(root));
	for (int i = 0; i < cJSON_GetArraySize(root); i++)   //遍历最外层json键值对
	{
		cJSON * item = cJSON_GetArrayItem(root, i);
		if (cJSON_Object == item->type)      //如果对应键的值仍为cJSON_Object就递归调用printJson
			printJson(item);
		else                                //值不为json对象就直接打印出键和值
		{
			printf("%s->", item->string);
			printf("%s\n", cJSON_Print(item));
		}
	}
}

static void demo_json_data_structure()
{
	cJSON * root = cJSON_CreateObject();
	cJSON * item = cJSON_CreateObject();
	cJSON * next = cJSON_CreateObject();

	cJSON_AddItemToObject(root, "rc", cJSON_CreateNumber(0));//根节点下添加
	cJSON_AddItemToObject(root, "operation", cJSON_CreateString("CALL"));
	cJSON_AddItemToObject(root, "service", cJSON_CreateString("telephone"));

	cJSON_AddItemToObject(root, "semantic", item);//root节点下添加semantic节点
	cJSON_AddItemToObject(item, "slots", next);//semantic节点下添加item节点
	cJSON_AddItemToObject(next, "name", cJSON_CreateString("张三"));//添加name节点

	cJSON *test_re = cJSON_CreateArray();
	cJSON_AddItemToArray(test_re,cJSON_CreateString("Change"));
	
	cJSON_ReplaceItemInObject(root, "operation", cJSON_CreateString("Change"));
	//cJSON_ReplaceItemInArray(root, 0, cJSON_CreateString("Change"));

	printJson(root);

	cJSON_Delete(root);
}

void test_json_api()
{
	const char test_path[] = "ui_data/guestbook.json";
	char *json_str;
	cJSON *cjson;
	if (fncjson_fileloader(test_path))
	{
		JSON_P_ERROR("not find json file!, creat a json file");
		return ;
	}
	json_str = datafilebuf;
	cjson = cJSON_Parse(json_str);
	if (!cjson)
	{
		JSON_P_ERROR("JSON : [%s]", cJSON_GetErrorPtr());
		free(datafilebuf);
		return ;
	}
//test get itme
	//json_Array = cJSON_GetObjectItem(cjson, "guestbookinfo");

	//print_preallocated(json_Array);
	//printJson(json_Array);
//test detach item
	//cJSON_DetachItemFromObject(cjson, "index");
	//print_preallocated(cjson);

//test data structure
	//printJson(cjson);
	demo_json_data_structure();

	cJSON_Delete(cjson);

	free(datafilebuf);
}


