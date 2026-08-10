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
#include "adm.h"

void Adminior_panel()
{
	printf("1.增加航班\n2.取消航班\n3.更新航班\n4.显示航班信息\n5.添加用户\n6.删除用户\n7.显示用户\n8.更新用户\n0.退出\n");
}

void Add_Book(Air_col* p)
{
	if (p->pointer == MAX_BOOKS)
	{
		printf("系统已满\n");
		return;
	}
	char a[50] = { 0 };
	char b[30] = { 0 };
	char c[50] = { 0 };
	char d[20] = {0};
	int e = 0;
	printf("请输入航班号\n");
	scanf("%s", a);
	printf("请输入目的地：\n");
	scanf("%s", b);
	printf("请输入出发地：\n");
	scanf("%s", c);
	printf("请输入出发时间：\n");
	scanf("%s", d);
	printf("请输入座位数量：\n");
	scanf("%d", &e);
	strcpy(p->bdata[p->pointer].name, a);
	strcpy(p->bdata[p->pointer].author, b);
	strcpy(p->bdata[p->pointer].ph, c);
	strcpy(p->bdata[p->pointer].year, d);
	p->bdata[p->pointer].num = e;
	if (p->pointer == 0)
	{
		p->bdata[p->pointer].id = 1;
	}
	else
	{
		p->bdata[p->pointer].id = p->bdata[p->pointer - 1].id + 1;
	}

	p->pointer++;

}
int Find_Book1(const Air_col* p, char a[])
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

static int Find_user(const Users_info* p, char a[])
{
	for (int i = 0; i < p->count; i++)
	{
		if (0 == strcmp(p->users[i].admin, a))
		{
			return i;
		}
	}
	return -1;
}

static int Find_user_by_num(const Users_info* p, int num)
{
	for (int i = 0; i < p->count; i++)
	{
		if (p->users[i].user_num == num)
		{
			return i;
		}
	}
	return -1;
}

void Delete_Book(Air_col* p)
{
	printf("请输入航班号\n");
	char a[50] = { 0 };
	scanf("%s", a);
	int pos = Find_Book1(p, a);
	if (pos == -1)
	{
		printf("无此航班\n");
		return;
	}
	for (int i = pos; i < p->pointer - 1; i++)
	{
		p->bdata[i] = p->bdata[i + 1];
	}
	p->pointer--;
	printf("删除成功\n");

}


void Show_Book(Air_col* p)
{
	printf("%20s\t%20s\t%20s\t%5s\t%5s\t%5s\n", "航班号", "目的地", "出发地", "出发时间", "座位数量", "编号");
	for (int i = 0; i < p->pointer; i++)
	{

		printf("%20s\t%20s\t%20s\t%20s\t%20d\t%20d\n",
			p->bdata[i].name,
			p->bdata[i].author,
			p->bdata[i].ph,
			p->bdata[i].year,
			p->bdata[i].num,
			p->bdata[i].id);
	}

}
void Updata_Book(Air_col* p)
{
	char ar[50] = { 0 };
	char b[30] = { 0 };
	char c[50] = { 0 };
	char d[20] = {0};
	int e = 0;
	int f = 0;
	int input = 0;
	printf("请输入要更改的航班号\n");
	char a[50] = { 0 };
	scanf("%s", a);
	int pos = Find_Book1(p, a);
	if (pos == -1)
	{
		printf("无此航班\n");
		return;
	}
	do {
		printf("请选择要更改的信息：\n");
		printf("1.航班号\n2.目的地\n3.出发地\n4.出发时间\n5.座位数量\n6.编号\n0:退出\n");
		switch (input)
		{
		case 0:printf("退出\n"); break;
		case 1:
			printf("请输入航班号\n");
			scanf("%s", ar);
			strcpy(p->bdata[pos].name, ar); break;
		case 2:
			printf("请输入目的地：\n");
			scanf("%s", b);
			strcpy(p->bdata[pos].author, b); break;
		case 3:
			printf("请输入出发地：\n");
			scanf("%s", c);
			strcpy(p->bdata[pos].ph, c); break;
		case 4:
			printf("请输入出发时间：\n");
			scanf("%s", d);
			strcpy(p->bdata[pos].year, d); break;
		case 5:
			printf("请输入座位数量：\n");
			scanf("%d", &e);
			p->bdata[pos].num = e; break;
		case 6:
			printf("请输入编号：\n");
			scanf("%d", &f);
			p->bdata[pos].id = f; break;
		default:printf("输入错误\n"); continue;
		}

	} while (input);




}

void Add_User(Users_info* p)
{
	if (p->count == MAX_USERS)
	{
		printf("用户已满\n");
		return;
	}
	char a[20] = { 0 };
	char b[20] = { 0 };
	printf("请输入新账号和密码\n");
	printf("账号：");
	scanf("%s", a);
	printf("密码：");
	scanf("%s", b);
	if (Find_user(p, a) != -1)
	{
		printf("该账号已存在\n");
		return;
	}
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
	p->count++;
	printf("添加用户成功\n");
}

void Delete_User(Users_info* p)
{
	if (p->count == 0)
	{
		printf("无用户可删除\n");
		return;
	}
	char a[20] = { 0 };
	printf("请输入要删除的账号\n");
	printf("账号：");
	scanf("%s", a);
	int pos = Find_user(p, a);
	if (pos == -1)
	{
		printf("无此用户\n");
		return;
	}
	for (int i = pos; i < p->count - 1; i++)
	{
		p->users[i] = p->users[i + 1];
	}
	p->count--;
	printf("删除用户成功\n");
}

void Show_User(Users_info* p)
{
	if (p->count == 0)
	{
		printf("暂无用户\n");
		return;
	}
	printf("%20s\t%20s\t%20s\n", "账号", "密码", "编号");
	for (int i = 0; i < p->count; i++)
	{
		printf("%20s\t%20s\t%20d\n",
			p->users[i].admin,
			p->users[i].paswd,
			p->users[i].user_num);
	}
}

void Updata_user(Users_info* p)
{
	if (p->count == 0)
	{
		printf("暂无用户\n");
		return;
	}
	int num = 0;
	int input = 0;
	char a[20] = { 0 };
	char b[20] = { 0 };
	int f = 0;
	printf("请输入要更改的用户编号\n");
	printf("编号：");
	scanf("%d", &num);
	int pos = Find_user_by_num(p, num);
	if (pos == -1)
	{
		printf("无此用户\n");
		return;
	}
	do {
		printf("请选择要更改的信息：\n");
		printf("1.账号\n2.密码\n3.编号\n0:退出\n");
		scanf("%d", &input);
		switch (input)
		{
		case 0:printf("退出\n"); break;
		case 1:
			printf("请输入账号\n");
			scanf("%s", a);
			strcpy(p->users[pos].admin, a); break;
		case 2:
			printf("请输入密码\n");
			scanf("%s", b);
			strcpy(p->users[pos].paswd, b); break;
		case 3:
			printf("请输入编号\n");
			scanf("%d", &f);
			p->users[pos].user_num = f; break;
		default:printf("输入错误\n"); continue;
		}
	} while (input);
}
