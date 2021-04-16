#include "message_mechanism.h"

#include "stdio.h"

#include "gui_app.h"

#ifndef _VS_SIMULATOR
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif // !_VS_SIMULATOR

#include "data_tencell.h"

/*
***Global
*/
static const char*visitor_mesg_dir = "visitor_mesg";
static const char *visitor_dir = "VisitorMesg";
/*
****List Storage****
*/
_mesg_mech_list_t  message_box_visitor;
 _mesg_mech_list_t *message_box_list_get(message_mech_type_t type)
{
	switch (type)
	{
	case message_mech_type_visitor:
		return &message_box_visitor;
		break;

	default:
		return NULL;
		break;
	}
}


/*===================================
************File Operation***********
====================================*/
static bool universal_file_write(const char *path, uint8_t *data_stream, uint32_t data_len)
{
	FILE *file = fopen(path, "w");
	if (file == NULL)
	{
		ui_printf("universal_file_write,open fail\n");
		return false;
	}
	fwrite(data_stream, data_len, 1, file);
	fclose(file);
	return true;
}

static int universal_file_del(const char *path)
{
	if (is_null_ptr(path))
		return -1;
	
	ui_printf("universal_file_del,:%s\n", path);

		if (remove(path) == 0) {
			ui_printf("remove file:%s\n", path);
			return 0;
		}
			
		else
			ui_printf("remove file fail:%s\n", path);

		return -1;
}

/*====================================================
*******Message Information Display Mechanism**********
=====================================================*/
/*Prefix: mesg_display_mech_
*/
#if 0
///eg: 2021.01.21 16:47:01 , visitor_2021_01_21_16_47_01.jpg
static bool universal_date_time_to_file_name(_ui_date_time_t *_time, char *file_name_addr, visitor_file_type_t type)
{
	if (is_null_ptr(file_name_addr)) return false;
	if (type == visitor_file_type_picture)
	{
		sprintf(file_name_addr, "visitor_%04d_%02d_%02d_%02d_%02d_%02d.jpg", _time->date.year, _time->date.month, _time->date.day, _time->time.hour, _time->time.minute, _time->time.second);
		return true;
	}
	else if (type == visitor_file_type_audio)
	{
		sprintf(file_name_addr, "visitor_%04d_%02d_%02d_%02d_%02d_%02d.wav", _time->date.year, _time->date.month, _time->date.day, _time->time.hour, _time->time.minute, _time->time.second);
		return true;
	}
	return false;
}

static bool universal_date_time_to_visitor_file_path(_ui_date_time_t *_time, visitor_file_type_t type, char *file_path_addr)
{
	char file_name[50] = { 0 };
	if (universal_date_time_to_file_name(_time, file_name, type))
	{
		sprintf(file_path_addr, "%s/%s",visitor_mesg_dir, file_name);
		return true;
	}
	return false;
}
#endif

#if 0
/*
***Whether directory exist or not
***@return: true if exist
*/
static int message_mech_is_dir_exist(const char *dir_path)
{
#ifndef _VS_SIMULATOR
	if (dir_path == NULL)
		return -1;
	DIR *dirp = NULL;
	dirp = opendir(dir_path);
	if (dirp == NULL)
		return -1;

	closedir(dirp);
	return 0;

#else
	return 0;
#endif // !_VS_SIMULATOR

}

/*
*****Write pic or audio data to file,
***@_time: date time
***@ type: picture or audio, two choose one,can't be both
***@ data: date write to file
***@ data_len: data lenth
***@ return : true if success or false conversely
*/
static bool message_mech_visitor_write_to_file(_ui_date_time_t *_time, visitor_file_type_t type, uint8_t *data, uint32_t data_len)
{
	if (is_null_ptr(data) || data_len == 0) return false;

	bool b_ret = false;
	//char file_name[50] = { 0 };
	char file_path[100] = { 0 };
	//const char dir_path[] = "visitor_mesg";
	if (universal_date_time_to_visitor_file_path(_time, type, file_path))
	{
		int dir_ret = message_mech_is_dir_exist(visitor_mesg_dir);
		if(dir_ret == -1){
			ui_printf("visitor_mesg dir not exist\n");
#ifndef _VS_SIMULATOR
			dir_ret = mkdir(visitor_mesg_dir, 0775);
#endif // !_VS_SIMULATOR			
			}
		if(dir_ret < 0) return false;
		b_ret = universal_file_write(file_path, data, data_len);
	}

	return b_ret;
}
#endif

static _mesg_mech_node *mesg_mech_new_node_get(_ui_display_time_t *date_time, const char *file_prefix)
{
	_mesg_mech_node *node = (_mesg_mech_node *)general_malloc(sizeof(_mesg_mech_node));
	if (is_null_ptr(node)) return NULL;

	memset(node, 0, sizeof(_mesg_mech_node));

	node->index = 0;
	node->b_read = false;
	node->pre = NULL;
	node->next = NULL;

	node->file_prefix = (char *)general_malloc(sizeof(char) *(strlen(file_prefix) + 1)); //remember to free this memory when mesg delete
	strcpy(node->file_prefix, file_prefix);

	if (!is_null_ptr(date_time))
		memcpy(&node->_time, date_time, sizeof(_ui_display_time_t));
	else
		memset(&node->_time, 0, sizeof(_ui_display_time_t));

	return node;
}

static void mesg_mech_tail_insert(_mesg_mech_list_t *mesg_list, _mesg_mech_node *new_node)
{
	if (is_null_ptr(mesg_list->head))
	{
		new_node->index = 1;
		mesg_list->head = mesg_list->tail = new_node;
	}
	else
	{
		new_node->index = mesg_list->tail->index + 1;
		new_node->pre = mesg_list->tail;
		mesg_list->tail->next = new_node;
		mesg_list->tail = new_node;
	}
}

static void mesg_mech_node_del(_mesg_mech_node *del_node, _mesg_mech_list_t *mesg_list)
{
	_mesg_mech_node *pre_node = del_node->pre;
	_mesg_mech_node *next_node = del_node->next;
	general_free(del_node->file_prefix); //first free memory of file_prefix
	ui_printf_plus("free del_node->file_prefix");

	if (pre_node == NULL || next_node == NULL)//del_node is head or tail both
	{
		if (pre_node == NULL && next_node == NULL)//both
		{
			mesg_list->head = mesg_list->tail = NULL;
		}
		else if (pre_node == NULL)//del_node is list head
		{
			next_node->pre = NULL;
			mesg_list->head = next_node;
		}
		else//del_node is list tail
		{
			pre_node->next = NULL;
			mesg_list->tail = pre_node;
		}
		general_free(del_node);
	}
	else
	{
		pre_node->next = next_node;
		next_node->pre = pre_node;
		general_free(del_node);
	}
	//after delete this node, index of all nodes after this node should decrease 1
	while (next_node != NULL)
	{
		next_node->index--;
		next_node = next_node->next;
	}
}
#if 0
/*
***@return: true if the last_time equal new_time, or false rather than
*/
static bool mesg_mech_compare_time(_ui_date_time_t *last_time, _ui_date_time_t *new_time)
{
	int size = sizeof(_ui_date_time_t);
	//uint8_t i = 0;
	char *cmp_last = (char *)last_time;
	char *cmp_new = (char *)new_time;
	size--;
	while (size >= 0)
	{
		if (*(cmp_last + size) != *(cmp_new + size))
			return false;

		size--;
	}
	return true;
}
#endif
//exterior call function
static  _mesg_mech_node *mesg_mech_list_in(_mesg_mech_list_t *mesg_list, _ui_display_time_t *date_time, const char *file_prefix)
{
#if 0 //audio and picture data come in separately, this will check the date_time with (last node)'s date_time so that to decide whether create a new node or just return the last node
	bool b_last = false;
	_mesg_mech_node *return_node = NULL;
	if (!is_null_ptr(mesg_list->tail)) {//make sure the list is not empty
		b_last = mesg_mech_compare_time(&mesg_list->tail->_time,date_time);//compare whether time is equal or not
	}
	if(!b_last)
	{
		_mesg_mech_node *node = mesg_mech_new_node_get(date_time);
		if (is_null_ptr(node)) return NULL;

		mesg_mech_tail_insert(mesg_list, node);
		return_node = node;
	}
	else//与前面一个节点时间相同，即一次图片，一次语音
	{
		return_node = mesg_list->tail;
	}	

	return return_node;
#endif
	_mesg_mech_node *node = mesg_mech_new_node_get(date_time, file_prefix);
	if (is_null_ptr(node)) return NULL;

	mesg_mech_tail_insert(mesg_list, node);

	return node;

}

/*
*****Descr: get the file name of prefix
*/

static int mesg_mech_visitor_get_file_path(const char *file_prefix, char *filename, message_file_type type)
{
	char file_suffix[5] = { 0 };
	if (type == visitor_file_type_audio)
		sprintf(file_suffix, ".wav");
	else if (type == visitor_file_type_picture)
		sprintf(file_suffix, ".jpg");
	else
		return -1;

	sprintf(filename, "%s/%s%s", visitor_dir, file_prefix, file_suffix);
	return 0;
}

static void mesg_mech_visitor_file_del(const char *file_prefix,visitor_file_type_t type)
{
	ui_printf("mesg_mech,visitor file del, type:0x%x\n", type);
	if ((type & 0x0F) == visitor_file_type_picture)
	{
		char picture_file_path[100] = { 0 };
		//universal_date_time_to_visitor_file_path(_time, visitor_file_type_picture, picture_file_path);
		mesg_mech_visitor_get_file_path(file_prefix, picture_file_path, visitor_file_type_picture);
		universal_file_del(picture_file_path);
	}
	
	if ((type & 0xF0) == visitor_file_type_audio)
	{
		char audio_file_path[100] = { 0 };
		//universal_date_time_to_visitor_file_path(_time, visitor_file_type_audio, audio_file_path);
		mesg_mech_visitor_get_file_path(file_prefix, audio_file_path, visitor_file_type_audio);
		universal_file_del(audio_file_path);
	}
	
}

void mesg_mech_list_del_node(_mesg_mech_node *del_node, _mesg_mech_list_t *mesg_list, message_mech_type_t type)
{
	mesg_mech_visitor_file_del(del_node->file_prefix, del_node->type);
	//to do, delelte information in json file
	datajs_del_message_mech_array(del_node->index, type);
	ui_printf_plus("datajs del message success");

	mesg_mech_node_del(del_node, mesg_list);// free del_node
	ui_printf_plus("mesg_mech_node_del success");
}

#define MESG_DISPLAY_PAGE_ROW_MAX		6
//get current page head node
//正序获取当前页的第一个node
_mesg_mech_node *mesg_mech_list_get_page_head_node(_mesg_mech_list_t *mesg_list)
{
	if (is_null_ptr(mesg_list) || is_null_ptr(mesg_list->tail)) return NULL;

	uint8_t page_head_index = mesg_list->page_cnt *MESG_DISPLAY_PAGE_ROW_MAX + 1;
	_mesg_mech_node *page_head_node = NULL;

	if (mesg_list->tail->index < page_head_index)
		return NULL;

	page_head_node = mesg_list->head;

	while (page_head_node->index != page_head_index && page_head_node != NULL)
	{
		page_head_node = page_head_node->next;
	}

	if (page_head_node->index == page_head_index)
		return page_head_node;
	else
		return NULL;

	return NULL;
}

/*
//倒序获得当前页的第一个node
//获取方使用该node前溯->pre依次逆序获取节点
//此函数不判断前溯次数是否合法，换言之，page_turn note中保证了page的合法性，page_cnt的值的准确性保证了这里次数的可靠性，否则会导致这里操作到空指针
*/
_mesg_mech_node *mesg_mech_list_get_page_reverse_head_node(_mesg_mech_list_t *mesg_list)
{
	if (is_null_ptr(mesg_list) || is_null_ptr(mesg_list->tail)) return NULL;

	_mesg_mech_node *reverse_head_node = mesg_list->tail;//首先选中链表尾节点
	_mesg_mech_node *temp_last_head_node = NULL;
	
	uint8_t i = mesg_list->page_cnt * MESG_DISPLAY_PAGE_ROW_MAX;//确定需要回溯的次数
	uint8_t j = (mesg_list->page_cnt - 1) * MESG_DISPLAY_PAGE_ROW_MAX;
	bool b_quit = false;
	while (i != 0)
	{
		if (!b_quit)
		{
			if (j == 0)
			{
				b_quit = true;
				temp_last_head_node = reverse_head_node;
			}
			else
			{
				j--;
			}
		}
		
		reverse_head_node = reverse_head_node->pre;
		i--;
	}
	if(is_null_ptr(reverse_head_node) && mesg_list->page_cnt > 0)
	{
		reverse_head_node = temp_last_head_node;
		mesg_list->page_cnt--;
	}
	return reverse_head_node;
}

bool mesg_mech_list_page_turn(mesg_display_mech_page_cmd_t page_cmd, _mesg_mech_list_t *mesg_list)
{
	if (is_null_ptr(mesg_list) || is_null_ptr(mesg_list->tail)) return false;
	if (page_cmd == mesg_display_mech_page_cmd_next)
	{
		if ((mesg_list->tail->index - mesg_list->head->index) / MESG_DISPLAY_PAGE_ROW_MAX > mesg_list->page_cnt)
		{
			mesg_list->page_cnt++;
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (page_cmd == mesg_display_mech_page_cmd_pre)
	{
		if (mesg_list->page_cnt > 0)
		{
			mesg_list->page_cnt--;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


#if 0
bool mesg_mech_receive_in(_ui_date_time_t *date_time, message_mech_type_t mesg_type, message_file_type file_type, uint8_t *data, uint32_t data_len)
{
	_mesg_mech_list_t *mesg_list = message_box_list_get(mesg_type);
	if (is_null_ptr(mesg_list)) return false;


	_mesg_mech_node *node = mesg_mech_list_in(date_time, mesg_list);//先将数据进离链表
	if (is_null_ptr(node)) return false;

	bool write_success = false;

	if (mesg_type == message_mech_type_visitor)
	{
		write_success = message_mech_visitor_write_to_file(date_time, file_type, data, data_len);//将数据保存成文件
		if (write_success)//保存文件成功
		{
			//to to
			node->type = node->type | file_type;
			ui_printf("file type:0x%x,node->type:0x%x\n", file_type, node->type);
			//to do
			//write list information to json file
		}
	}

	return write_success;
}


bool mesg_mech_receive_come_in(_ui_date_time_t *date_time, message_mech_type_t mesg_type, uint8_t *audio_data, uint32_t audio_data_len, uint8_t *picture_data, uint32_t picture_data_len)
{
	if (is_null_ptr(audio_data) && is_null_ptr(picture_data))
		return false;

	_mesg_mech_list_t *mesg_list = message_box_list_get(mesg_type);
	if (is_null_ptr(mesg_list)) return false;


	_mesg_mech_node *node = mesg_mech_list_in(date_time, mesg_list);//first put data in list
	if (is_null_ptr(node)) return false;

	if (message_mech_type_visitor == mesg_type)
	{
		if (message_mech_visitor_write_to_file(date_time, visitor_file_type_audio, audio_data, audio_data_len)) //write audio data to file
		{
			node->type = node->type | visitor_file_type_audio;
		}

		if (message_mech_visitor_write_to_file(date_time, visitor_file_type_picture, picture_data, picture_data_len))//write picture data to file
		{
			node->type = node->type | visitor_file_type_picture;
		}
		//write mesg information to json file
		datajs_message_mech_array_come_in(node, message_mech_type_visitor);

		return true;
	}
	else if(message_mech_type_block == mesg_type)
	{
	}

	return false;
}
#endif

/*
*****Descr: true data put in mesg list, you should decide whether to insert mesg before call this fun
***@Para->mest_type: message_mech_type_visitor or message_mech_type_block
***@Para->date_time: date & time
***@Para->path: full path of file
***@Para->write_json: whether to write this information to json file, if called by power on read, no need to write json file
***Return: 0 if success of -1 otherwise
*/
static int mesg_mech_data_in(message_mech_type_t mesg_type, _ui_display_time_t *date_time, const char *file_prefix, bool write_json)
{
	ui_printf_plus("[mesg data in]date:%s,time:%s,file_prefix:%s", date_time->date, date_time->time, file_prefix);
	_mesg_mech_list_t *mesg_list = message_box_list_get(mesg_type);
	if (is_null_ptr(mesg_list)) return -1;

	_mesg_mech_node *node = mesg_mech_list_in(mesg_list, date_time, file_prefix);//first put data in list
	if (is_null_ptr(node)) return -1;
	node->type = visitor_file_type_picture_and_audio;

	if (write_json)
	{
		if (message_mech_type_visitor == mesg_type)
		{
			//write mesg information to json file
			datajs_message_mech_array_come_in(node, message_mech_type_visitor);
			return 0;
		}
		else if(message_mech_type_block == mesg_type)
		{

		}

	}
	return -1;
}

static char current_info[30] = { 0 };
/*
*****Descr: extract file tag info from path, for example: "VisitorMes/A_20120129_101812_1.wav" to "A_20120129_101812_1"
***@Para->path: file path
***Return: true if new-come path is same with @current_info which means one audio and another picture as one message, or false otherwise as a new message
*/
static bool mesg_mech_extract_info_from_path(const char *path)
{
	int i = 0;
	int dot_pos = 0;
	for (i = (int)strlen(path) -1; i != 0; --i)
	{
		if (path[i] == '.')
			dot_pos = i;

		if (path[i] == '/')
			break;
	}

	char file_info[30] = { 0 };
	memcpy(file_info, path + i + 1, dot_pos - i - 1);
	
	if (strcmp(current_info, file_info) != 0)
	{
		strcpy(current_info, file_info);
		return false;
	}
	
	return true;
}

/*
*****Descr: extract date and time from name
***@Para->filename: for example: A_20120129_101812_1
***@_time_addr: addr to get _time
*/
static bool mesg_mech_extract_date_time_from_name(const char *filename, _ui_display_time_t *_time_addr)
{
	ui_printf_plus("filename:%s", filename);
	char str_year[5] = { 0 }, str_month[3] = { 0 }, str_day[3] = { 0 };
	char str_hour[3] = { 0 }, str_minute[3] = { 0 }, str_second[3] = { 0 };
	
	memcpy(str_year, &filename[0 + 2], 4);
	memcpy(str_month, &filename[0 + 2 + 4], 2);
	memcpy(str_day, &filename[0 + 2 + 4 + 2], 2);

	memcpy(str_hour, &filename[11 + 0], 2);
	memcpy(str_minute, &filename[11 + 2], 2);
	memcpy(str_second, &filename[11 + 4], 2);
	ui_printf_plus("hour:%s", str_hour);
	
	sprintf(_time_addr->date, "%s-%s-%s", str_year, str_month, str_day);
	sprintf(_time_addr->time, "%s:%s:%s", str_hour, str_minute, str_second);

	return true;
}

int mesg_mech_receive_path_come_in(const char *file_path)
{
	if (mesg_mech_extract_info_from_path(file_path)) //the old second message
		return 0;
	
	_ui_display_time_t _time;
	memset(&_time, 0, sizeof(_ui_display_time_t));

	mesg_mech_extract_date_time_from_name(current_info, &_time);

	return mesg_mech_data_in(message_mech_type_visitor, &_time, current_info, true);
}

/*
*****Descr: retrieve all list data from json file when power on init
*/
void mesg_mech_retrieve_list_data(message_mech_type_t mesg_type)
{
	int node_index = 0;

	_mesg_mech_list_t *mesg_list = message_box_list_get(mesg_type);
	if (is_null_ptr(mesg_list)) return ;

#if 0
	_mesg_mech_node *node = mesg_mech_list_in(date_time, mesg_list);//first put data in list
	if (is_null_ptr(node)) return false;
#endif
#if 0 //test date
	_mesg_mech_node temp_node;
	datajs_get_message_mech_array_item_by_index(&temp_node, 0, message_mech_type_visitor);
	_ui_date_time_t date_time = temp_node._time;
	ui_printf_plus("[retrieve list data]%d-%d-%d", date_time.date.year, date_time.date.month, date_time.date.day);
#endif
	int array_size = datajs_get_message_mech_array_size(message_mech_type_visitor);
	
	if (array_size <= 0) return;
	
	//_ui_display_time_t date_time;
	char file_prefix[25] = { 0 };
	_mesg_mech_node temp_node;
	_mesg_mech_node *new_node = NULL;
	//memset(&date_time, 0, sizeof(_ui_display_time_t));
	memset(&temp_node, 0, sizeof(_mesg_mech_node));

	for (node_index = 1; node_index <= array_size; node_index++) //attention: node index is begin with 1, and index of array will decrease 1 auto
	{
#if 0
		node = mesg_mech_list_in(&date_time, mesg_list);//first put data in list
		if (is_null_ptr(node)) continue;

		datajs_get_message_mech_array_item_by_index(node, node_index, message_mech_type_visitor);
#endif
		if (0 == datajs_get_message_mech_array_item_by_index(&temp_node, node_index, message_mech_type_visitor, file_prefix)) //get the information from json file
		{
			new_node = mesg_mech_list_in(mesg_list, &temp_node._time, file_prefix);
			if (!is_null_ptr(new_node))
			{
				new_node->b_read = temp_node.b_read;
				new_node->type = temp_node.type;
			}
		}
	}
}

/*
*****Descr:set mesg node as read
***@Para-> node
***@Para-> mesg_type
*/
void mesg_mech_set_node_read(_mesg_mech_node *node, message_mech_type_t mesg_type)
{
	if (!node->b_read)
	{
		node->b_read = true;
		datajs_set_message_mech_read_item(mesg_type, node->index);
	}
}
