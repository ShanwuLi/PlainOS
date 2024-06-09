/* MIT License

Copyright (c) 2023 PlainOS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define CFG_LINE_MAX_LEN        512
#define CFG_HEADER_TITLE        "#ifndef __PLAINOS_CONFIG_H__\n#define __PLAINOS_CONFIG_H__\n\n"
#define CFG_HEADER_TITLE_END    "\n#endif /* __PLAINOS_CONFIG_H__ */\n"
#define CFG_HEADER_VAR_PREFIX   "#define CONFIG_"
#define CFG_HEADER_FILE_NAME    "config.h"
#define CFG_MAKEFILE_FILE_NAME  "config.mk"

#define container_of(ptr, struct_type, member) \
	((struct_type *)((char *)(ptr) - (char *)(&(((struct_type *)0)->member))))

enum cfg_parse_err {
	CFG_PARSE_ERR_COMMENT = -1,
	CFG_PARSE_ERR_VAR_NAME = -2,
	CFG_PARSE_ERR_VAR_ASSIGN_SYMBOL = -3,
	CFG_PARSE_ERR_VAR_VALUE = -4,
	CFG_PARSE_ERR_VAR_END = -5,
	CFG_PARSE_ERR_SYNTAX = -6
};

enum assign_type {
	ASSIGN_TYPE_DIRECT = 0,   /*  = */
	ASSIGN_TYPE_IMMEDIATE,    /* := */
	ASSIGN_TYPE_CONDITIONAL,  /* ?= */
	ASSIGN_TYPE_ADDITION,     /* += */
};

enum value_type {
	VALUE_TYPE_YES = 0, /* 'y' */
	VALUE_TYPE_NO,      /* 'n' */
	VALUE_TYPE_OTHER,   /* others */
};

struct list_node {
	struct list_node *next;
	struct list_node *prev;
};

struct plain_cfg {
	uint32_t assign_type;
	uint32_t value_type;
	uint16_t value_len;
	uint16_t varible_name_len;
	char *varible_name;
	char *value;
	struct list_node node;
};

static struct list_node plain_cfg_head = {
	.next = &plain_cfg_head,
	.prev = &plain_cfg_head
};

static char *cfg_file_buff = NULL;
static char *cfg_file_name = NULL;
static int cfg_file_line_num = 0;
static int cfg_file_col_num = 0;
static char cfg_var_name[CFG_LINE_MAX_LEN];
static char cfg_var_value[CFG_LINE_MAX_LEN];

struct plain_cfg *plain_cfg_alloc_init(uint32_t assign_type,
                      char *var_name, uint16_t var_name_len,
                      char *var_value, uint16_t var_value_len)
{
	struct plain_cfg *cfg;

	cfg = malloc(sizeof(struct plain_cfg) + var_name_len + var_value_len);
	if (cfg == NULL)
		return NULL;

	cfg->value_type = VALUE_TYPE_OTHER;
	if (var_value_len == 2 && var_value[0] == 'y')
		cfg->value_type = VALUE_TYPE_YES;

	if (var_value_len == 2 && var_value[0] == 'n')
		cfg->value_type = VALUE_TYPE_NO;

	cfg->varible_name_len = var_name_len;
	cfg->value_len = var_value_len;
	cfg->assign_type = assign_type;
	cfg->varible_name = (char *)cfg + sizeof(struct plain_cfg);
	cfg->value = cfg->varible_name + var_name_len;
	memcpy(cfg->varible_name, var_name, var_name_len);
	memcpy(cfg->value, var_value, var_value_len);

	return cfg;
}
static int plain_cfg_add_to_list(struct plain_cfg *cfg)
{
	if (cfg == NULL)
		return -1;

	cfg->node.next = &plain_cfg_head;
	cfg->node.prev = plain_cfg_head.prev;
	plain_cfg_head.prev->next = &cfg->node;
	plain_cfg_head.prev = &cfg->node;

	return 0;
}

static void plain_cfg_list_dump(void)
{
	struct list_node *node;
	struct plain_cfg *cfg;

	if (plain_cfg_head.next == &plain_cfg_head)
		return;

	for (node = plain_cfg_head.next; node != &plain_cfg_head; node = node->next) {
		cfg = container_of(node, struct plain_cfg, node);
		printf("assign_type:%d, value_type:%d, var_name:%s, len:%d, cvalue:%s, len:%d\n",
		       cfg->assign_type, cfg->value_type, cfg->varible_name, cfg->varible_name_len,
		       cfg->value, cfg->value_len);
	}
}

static void plain_cfg_list_destroy(void)
{
	struct list_node *pos;
	struct list_node *temp;
	struct plain_cfg *cfg;

	if (plain_cfg_head.next == &plain_cfg_head)
		return;

	for (pos = plain_cfg_head.next, temp = pos->next;
		 pos != &plain_cfg_head; pos = temp, temp = pos->next) {
		cfg = container_of(pos, struct plain_cfg, node);
		free(cfg);
	}

	plain_cfg_head.next = &plain_cfg_head;
	plain_cfg_head.prev = &plain_cfg_head;
}

static int plain_cfg_get_next_state(int state, char c)
{
	int next_state = 0;

	switch (state) {
	case 0:
		if (c == ' ' || c =='\r' || c == '\n' || c == '\t') {
			next_state = 0;
			break;
		}

		if (c == '/') {
			next_state = 1;
			break;
		}

		if ((c >= 'A' && c <= 'Z') || c == '_') {
			next_state = 11;
			break;
		}

		next_state = CFG_PARSE_ERR_SYNTAX;
		break;

	/* comment with state 1 ~ 3 */
	case 1:
		if (c == '*') {
			next_state = 2;
			break;
		}

		next_state = CFG_PARSE_ERR_COMMENT;
		break;
	
	case 2:
		if (c == '*') {
			next_state = 3;
			break;
		}

		next_state = 2;
		break;
	
	case 3:
		if (c == '/') {
			next_state = 0;
			break;
		}

		next_state = 2;
		break;

	case 11:
		if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
			next_state = 11;
			break;
		}

		if (c == ' ' || c == '\t') {
			next_state = 12;
			break;
		}

		next_state = CFG_PARSE_ERR_VAR_NAME;
		break;

	/* assign type with state 12 ~ 15 */
	case 12:
		if (c == ' ' || c == '\t') {
			next_state = 12;
			break;
		}

		if (c == '+') {
			next_state = 13;
			break;
		}

		if (c == '?') {
			next_state = 14;
			break;
		}

		if (c == ':') {
			next_state = 15;
			break;
		}

		if (c == '=') {
			next_state = 16;
			break;
		}

		next_state = CFG_PARSE_ERR_VAR_ASSIGN_SYMBOL;
		break;

	case 13:
		if (c == '=') {
			next_state = 16;
			break;
		}

		next_state = CFG_PARSE_ERR_VAR_ASSIGN_SYMBOL;
		break;

	case 14:
		if (c == '=') {
			next_state = 16;
			break;
		}

		next_state = CFG_PARSE_ERR_VAR_ASSIGN_SYMBOL;
		break;

	case 15:
		if (c == '=') {
			next_state = 16;
			break;
		}

		next_state = CFG_PARSE_ERR_VAR_ASSIGN_SYMBOL;
		break;

	/* value with state 16 ~ 17 */
	case 16:
		if (c == ' ' || c == '\t') {
			next_state = 17;
			break;
		}
		break;

	case 17:
		if (c == ' ' || c == '\t') {
			next_state = 17;
			break;
		}

		next_state = 18;
		break;

	/* value end with state 18 */
	case 18:
		if (c == ' ' || c == '\r' || c == '\n' || c == '\t') {
			next_state = 0;
			break;
		}

		next_state = 18;
		break;

	default:
		next_state = CFG_PARSE_ERR_SYNTAX;
		break;
	}

	return next_state;
}


static int plain_cfg_err_check(int state)
{
	if (state >= 0)
		return 0;

	cfg_file_line_num = cfg_file_line_num + 1;
	switch (state) {
	case CFG_PARSE_ERR_COMMENT:
		printf("%s parse error in line[%d], col[%d], you must use /* */ to comment\n",
		       cfg_file_name, cfg_file_line_num, cfg_file_col_num);
		break;

	case CFG_PARSE_ERR_VAR_NAME:
		printf("%s parse error in line[%d], col[%d], format error of varible's name\n" ,
		       cfg_file_name, cfg_file_line_num, cfg_file_col_num);
		break;

	case CFG_PARSE_ERR_VAR_ASSIGN_SYMBOL:
		printf("%s parse error in line[%d], col[%d], only '+=' || '?=' || ':=' || '=' is valid\n",
		       cfg_file_name, cfg_file_line_num, cfg_file_col_num);
		break;

	case CFG_PARSE_ERR_VAR_VALUE:
		printf("%s parse error in line[%d], col[%d], format error of varible's value\n",
		       cfg_file_name, cfg_file_line_num, cfg_file_col_num);
		break;
	
	case CFG_PARSE_ERR_VAR_END:
		printf("%s parse error in line[%d], col[%d], you must use ' ' to end your value\n",
		       cfg_file_name, cfg_file_line_num, cfg_file_col_num);
		break;

	case CFG_PARSE_ERR_SYNTAX:
		printf("%s has syntax error in line[%d], col[%d]\n",
		       cfg_file_name, cfg_file_line_num, cfg_file_col_num);
		break;

	default:
		printf("%s unknow error, fatal error\n", cfg_file_name);
		break;
	}

	return state;
}

static int plain_cfg_alloc_init_and_add_list(struct plain_cfg *cfg_info)
{
	int ret;
	struct plain_cfg *cfg;

	/* copy cfg_var_value to plain_cfg */
	cfg = plain_cfg_alloc_init(cfg_info->assign_type, cfg_info->varible_name,
	                           cfg_info->varible_name_len, cfg_info->value,
	                           cfg_info->value_len);
	if (cfg == NULL) {
		printf("alloc cfg failed\n");
		return -1;
	}

	ret = plain_cfg_add_to_list(cfg);
	if (ret < 0) {
		printf("add cfg to list failed\n");
		return -2;
	}

	return 0;
}

static int plain_cfg_parse(char *cfg_chars_in)
{
	int ret;
	int char_idx = 0;
	int curr_state = 0;
	int next_state = 0;
	uint16_t var_name_len = 0;
	uint16_t val_len = 0;
	uint32_t assign_type = ASSIGN_TYPE_DIRECT;
	struct plain_cfg cfg;

	while(*cfg_chars_in && next_state >= 0) {
		if (*cfg_chars_in == '\n') {
			cfg_file_line_num++;
			cfg_file_col_num = 0;
		} else {
			cfg_file_col_num++;
		}

		next_state = plain_cfg_get_next_state(curr_state, *cfg_chars_in);
		ret = plain_cfg_err_check(next_state);
		if (ret < 0) {
			printf("curr char:%c, next_state:%d\n", *cfg_chars_in, next_state);
			return -1;
		}

		switch (next_state) {
		case 0:
			if (curr_state == 18) {
				cfg_var_value[char_idx] = '\0';
				val_len++;

				cfg.assign_type = assign_type;
				cfg.varible_name = cfg_var_name;
				cfg.varible_name_len = var_name_len;
				cfg.value = cfg_var_value;
				cfg.value_len = val_len;

				ret = plain_cfg_alloc_init_and_add_list(&cfg);
				if (ret < 0) {
					printf("alloc cfg failed\n");
					return -2;
				}
			}

			char_idx = 0;
			var_name_len = 0;
			val_len = 0;
			assign_type = ASSIGN_TYPE_DIRECT;
			break;

		case 11:
			cfg_var_name[char_idx] = *cfg_chars_in;
			char_idx++;
			var_name_len++;
			break;

		case 13:
			assign_type = ASSIGN_TYPE_ADDITION;
			break;

		case 14:
			assign_type = ASSIGN_TYPE_CONDITIONAL;
			break;

		case 15:
			assign_type = ASSIGN_TYPE_IMMEDIATE;
			break;

		case 16:
			var_name_len++;
			cfg_var_name[char_idx] = '\0';
			char_idx = 0;
			break;

		case 18:
			cfg_var_value[char_idx] = *cfg_chars_in;
			char_idx++;
			val_len++;
			break;

		default:
			break;
		}

		curr_state = next_state;
		cfg_chars_in++;
	}

	return next_state;
}
static int plain_malloc_file_buff(char *file_name, char **buff)
{
	int ret;
	struct stat st;

	ret = stat(file_name, &st);
	if (ret < 0) {
		printf("no such file %s\n", file_name);
		return ret;
	}

	*buff = malloc(st.st_size + 10);
	if (*buff == NULL) {
		printf("malloc failed\n");
		return -2;
	}

	return 0;
}
static void plain_free_file_buff(char **buff)
{
	if (*buff == NULL)
		return;

	free(*buff);
	*buff = NULL;
}
static int plain_read_file_to_buff(char *file_name, char *buff)
{
	FILE *fp;
	size_t ret;
	size_t offset = 0;

	fp = fopen(file_name, "r");
	if (fp == NULL) {
		printf("Fail to open file!\n");
		return -1;
	}

	while (!feof(fp)) {
		ret = fread(buff + offset, 1, 1024, fp);
		offset += ret;
	}

	buff[offset] = '\0';
	fclose(fp);
	return 0;
}

static int plain_write_cfg_to_file(char *mk_file_name, char *head_file_name)
{
	size_t ret;
	FILE *mk_fp;
	FILE *head_fp;
	struct list_node *node;
	struct plain_cfg *cfg;

	mk_fp = fopen(mk_file_name, "w");
	if (mk_fp == NULL) {
		printf("Fail to open file!\n");
		return -1;
	}

	head_fp = fopen(head_file_name, "w");
	if (head_fp == NULL) {
		fclose(mk_fp);
		printf("Fail to open file!\n");
		return -1;
	}

	if (plain_cfg_head.next == &plain_cfg_head) {
		fclose(mk_fp);
		fclose(head_fp);
		return 0;
	}

	printf("writing %s\n", mk_file_name);
	printf("writing %s\n", head_file_name);
	fwrite(CFG_HEADER_TITLE, strlen(CFG_HEADER_TITLE), 1, head_fp);
	for (node = plain_cfg_head.next; node != &plain_cfg_head; node = node->next) {
		cfg = container_of(node, struct plain_cfg, node);

		/* write cfg to header file */
		if (cfg->assign_type == ASSIGN_TYPE_DIRECT) {
			if (cfg->value_type == VALUE_TYPE_YES) {
				fwrite(CFG_HEADER_VAR_PREFIX, strlen(CFG_HEADER_VAR_PREFIX), 1, head_fp);
				fwrite(cfg->varible_name, cfg->varible_name_len - 1, 1, head_fp);
				fwrite("\n", strlen("\n"), 1, head_fp);
			}

			if (cfg->value_type == VALUE_TYPE_OTHER) {
				fwrite(CFG_HEADER_VAR_PREFIX, strlen(CFG_HEADER_VAR_PREFIX), 1, head_fp);
				fwrite(cfg->varible_name, cfg->varible_name_len - 1, 1, head_fp);
				fwrite(" ", strlen(" "), 1, head_fp);
				fwrite(cfg->value, cfg->value_len -1, 1, head_fp);
				fwrite("\n", strlen("\n"), 1, head_fp);
			}
		}

		/* write cfg to makefile file */
		fwrite(cfg->varible_name, cfg->varible_name_len - 1, 1, mk_fp);
		fwrite(" ", strlen(" "), 1, mk_fp);
		if (cfg->assign_type == ASSIGN_TYPE_IMMEDIATE)
			fwrite(":", strlen(":"), 1, mk_fp);

		if (cfg->assign_type == ASSIGN_TYPE_CONDITIONAL)
			fwrite("?", strlen("?"), 1, mk_fp);

		if (cfg->assign_type == ASSIGN_TYPE_ADDITION)
			fwrite("+", strlen("+"), 1, mk_fp);

		fwrite("= ", strlen("= "), 1, mk_fp);
		fwrite(cfg->value, cfg->value_len -1, 1, mk_fp);
		fwrite("\n", strlen("\n"), 1, mk_fp);
	}

	fwrite(CFG_HEADER_TITLE_END, strlen(CFG_HEADER_TITLE_END), 1, head_fp);
	fclose(mk_fp);
	fclose(head_fp);

	return 0;
}



int main(int argc, char **argv)
{
	int ret;

	if (argc != 2) {
		printf("invalid argument\n");
		return -1;
	}

	cfg_file_name = argv[1];
	ret = plain_malloc_file_buff(cfg_file_name, &cfg_file_buff);
	if (ret < 0) {
		printf("file buffer alloc fialed, ret:%d", ret);
		return ret;
	}

	ret = plain_read_file_to_buff(cfg_file_name, cfg_file_buff);
	if (ret < 0) {
		plain_free_file_buff(&cfg_file_buff);
		printf("file read failed, ret:%d", ret);
		return ret;
	}

	ret = plain_cfg_parse(cfg_file_buff);
	if (ret < 0) {
		goto done;
	}

	//plain_cfg_list_dump();

	ret = plain_write_cfg_to_file(CFG_MAKEFILE_FILE_NAME, CFG_HEADER_FILE_NAME);
	if (ret < 0) {
		printf("generate config file failed, ret:%d\n", ret);
	}

done:
	plain_cfg_list_destroy();
	plain_free_file_buff(&cfg_file_buff);
	//printf("cfg list destroy\n");
	return ret;
}
