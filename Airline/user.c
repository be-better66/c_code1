#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include<time.h>
#include<math.h>
#include<climits>
#include "user.h"


void Init_book(Air_col* p)
{
	p->pointer = 0;
	p->history_count = 0;
	while (p->bdata[p->pointer].id != 0)
	{
		p->pointer++;
	}

}

void Init_user(Users_info* p)
{
	p->count = 0;
	while (p->users[p->count].user_num != 0)
	{
		p->count++;
	}
}

int Login(Users_info* p)
{
	char ac[20] = { 0 };
	char co[20] = { 0 };
	int flag = 0; int n = 0;
	do {
		printf("请输入账号和密码\n");
		scanf("%s", ac);
		scanf("%s", co);
		flag = 0;

		for (int i = 0; i < p->count; i++)
		{
			if (strcmp(p->users[i].admin, ac) == 0)
			{
				flag = 1;
				if (strcmp(p->users[i].paswd, co) == 0)
				{
					printf("登录成功\n");
					return 1;
				}
				else {
					printf("密码错误，请重新输入\n");
					n++;
					break;
				}
			}
		}
		if (flag == 0)
		{
			printf("账户不存在，请重新登录\n");
			n++;
		}
		if (n >= 3)
		{
			return 0;
		}
	} while (1);


}

void Rigs(Users_info* p)
{
	char a[20] = { 0 };
	char b[20] = { 0 };
	printf("请输入新账号和密码\n");
	scanf("%s", a);
	scanf("%s", b);
	strcpy(p->users[p->count].admin, a);
	strcpy(p->users[p->count].paswd, b);
	if (p->count == 0)
	{
		p->users[p->count].user_num = 1;
	}
	else
	{
		p->users[p->count].user_num = p->users[p->count - 1].user_num + 1;
	}
	printf("注册成功\n");
	p->count++;
}

void uerpanel()
{
	printf("1.查询航班\n2.预定航班\n3.航班取消\n4.航班记录\n");
}

void Searchbook()
{
	printf("查询航班\n分类：\n");
	printf("1.航班号\n2.目的地\n3.出发地\n4.出发时间\n5.座位数量\n6.编号\n0:退出\n");
}

void Search_Engine(Air_col* p)
{
	printf("请输入航班号\n");
	char a[100];
	int flag = 0;
	scanf("%s", a);
	for (int i = 0; i < p->pointer; i++)
	{

		if (strcmp(p->bdata[i].name, a) == 0)
		{
			printf("查找到：\n");
			printf("%20s\t%20s\t%20s\t%5s\t%5s\t%5s\n", "航班号", "目的地", "出发地", "出发时间", "座位数量", "编号");
			printf("%20s\t%20s\t%20s\t%5s\t%5d\t%5d\n",
				p->bdata[i].name,
				p->bdata[i].author,
				p->bdata[i].ph,
				p->bdata[i].year,
				p->bdata[i].num,
				p->bdata[i].id);
			flag = 1;

		}


	}
	if (flag == 0)
	{
		printf("没有查询到该航班\n");
	}




}

void Search_Engine1(Air_col* p)
{
	printf("请输入目的地：\n");
	char a[30] = { 0 };
	int flag = 0;
	scanf("%s", a);
	for (int i = 0; i < p->pointer; i++)
	{

		if (strcmp(p->bdata[i].author, a) == 0)
		{
			printf("查找到：\n");
			printf("%20s\t%20s\t%20s\t%5s\t%5s\t%5s\n", "航班号", "目的地", "出发地", "出发时间", "座位数量", "编号");
			printf("%20s\t%20s\t%20s\t%5s\t%5d\t%5d\n",
				p->bdata[i].name,
				p->bdata[i].author,
				p->bdata[i].ph,
				p->bdata[i].year,
				p->bdata[i].num,
				p->bdata[i].id);
			flag = 1;

		}

	}
	if (flag == 0)
	{
		printf("没有查询到该航班\n");
	}




}

void Search_Engine2(Air_col* p)
{
	printf("请输入出发地：\n");
	char a[100] = { 0 };
	int flag = 0;
	scanf("%s", a);
	for (int i = 0; i < p->pointer; i++)
	{

		if (strcmp(p->bdata[i].ph, a) == 0)
		{
			printf("查找到：\n");
			printf("%20s\t%20s\t%20s\t%5s\t%5s\t%5s\n", "航班号", "目的地", "出发地", "出发时间", "座位数量", "编号");
			printf("%20s\t%20s\t%20s\t%5s\t%5d\t%5d\n",
				p->bdata[i].name,
				p->bdata[i].author,
				p->bdata[i].ph,
				p->bdata[i].year,
				p->bdata[i].num,
				p->bdata[i].id);
			flag = 1;

		}

	}
	if (flag == 0)
	{
		printf("没有查询到该航班\n");
	}




}

void Search_Engine3(Air_col* p)
{
	printf("请输入出发时间：\n");
	char a[20] = {0};
	int flag = 0;
	scanf("%s", a);
	for (int i = 0; i < p->pointer; i++)
	{

		if (strcmp(p->bdata[i].year, a) == 0)
		{
			printf("查找到：\n");
			printf("%20s\t%20s\t%20s\t%5s\t%5s\t%5s\n", "航班号", "目的地", "出发地", "出发时间", "座位数量", "编号");
			printf("%20s\t%20s\t%20s\t%5s\t%5d\t%5d\n",
				p->bdata[i].name,
				p->bdata[i].author,
				p->bdata[i].ph,
				p->bdata[i].year,
				p->bdata[i].num,
				p->bdata[i].id);
			flag = 1;

		}

	}
	if (flag == 0)
	{
		printf("没有查询到该航班\n");
	}




}

void Search_Engine4(Air_col* p)
{
	printf("请输入座位数量：\n");
	int a = 0;
	int flag = 0;
	scanf("%d", &a);
	for (int i = 0; i < p->pointer; i++)
	{

		if (p->bdata[i].num == a)
		{
			printf("查找到：\n");
			printf("%20s\t%20s\t%20s\t%5s\t%5s\t%5s\n", "航班号", "目的地", "出发地", "出发时间", "座位数量", "编号");
			printf("%20s\t%20s\t%20s\t%5s\t%5d\t%5d\n",
				p->bdata[i].name,
				p->bdata[i].author,
				p->bdata[i].ph,
				p->bdata[i].year,
				p->bdata[i].num,
				p->bdata[i].id);
			flag = 1;

		}

	}
	if (flag == 0)
	{
		printf("没有查询到该航班\n");
	}




}

void Search_Engine5(Air_col* p)
{
	printf("请输入编号：\n");
	int a = 0;
	int flag = 0;
	scanf("%d", &a);
	for (int i = 0; i < p->pointer; i++)
	{

		if (p->bdata[i].id == a)
		{
			printf("查找到：\n");
			printf("%20s\t%20s\t%20s\t%5s\t%5s\t%5s\n", "航班号", "目的地", "出发地", "出发时间", "座位数量", "编号");
			printf("%20s\t%20s\t%20s\t%5s\t%5d\t%5d\n",
				p->bdata[i].name,
				p->bdata[i].author,
				p->bdata[i].ph,
				p->bdata[i].year,
				p->bdata[i].num,
				p->bdata[i].id);
			flag = 1;

		}

	}
	if (flag == 0)
	{
		printf("没有查询到该航班\n");
	}




}

static int Find_Book(const Air_col* p, char a[])
{
	for (int i = 0; i < p->pointer; i++)
	{
		if (0 == strcmp(p->bdata[i].name, a))
		{
			return i;
		}
	}
	return -1;
}

void Borrow_Book(Air_col* p)
{

	printf("请输入航班号\n");
	char a[100] = { 0 };
	scanf("%s", a);
	int pos = Find_Book(p, a);
	if (pos == -1)
	{
		printf("没有找到该航班\n");
	}
	else
	{
		if (p->bdata[pos].num == 0)
		{
			printf("该航班座位已满\n");
		}
		else
		{

			p->bdata[pos].num--;

			// 记录借阅历史
			strcpy(p->history[p->history_count].book_name, p->bdata[pos].name);
			p->history[p->history_count].is_returned = 0;
			p->history_count++;
			printf("预约成功\n");
		}
	}
}

void Revert_Book(Air_col* p)
{
	printf("请输入航班号\n");
	char a[100] = { 0 };
	scanf("%s", a);
	int pos = Find_Book(p, a);
	if (pos == -1)
	{
		printf("没有找到该航班\n");
	}
	else
	{
		printf("退订成功\n");
		p->bdata[pos].num++;

		// 找到该书的未归还记录，标记为已归还
		for (int i = p->history_count - 1; i >= 0; i--)
		{
			if (strcmp(p->history[i].book_name, p->bdata[pos].name) == 0
				&& p->history[i].is_returned == 0)
			{
				p->history[i].is_returned = 1;
				break;
			}
		}
	}
}

void Borrow_History(Air_col* p)
{
	if (p->history_count == 0)
	{
		printf("暂无航班记录\n");
		return;
	}

	printf("航班记录：\n");
	printf("%-5s\t%-20s\t%-8s\n", "序号", "航班号", "状态");
	for (int i = 0; i < p->history_count; i++)
	{
		printf("%-5d\t%-20s\t%-8s\n",
			i + 1,
			p->history[i].book_name,
			p->history[i].is_returned ? "已退订" : "预约中");
	}
}