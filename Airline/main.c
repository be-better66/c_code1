#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include<time.h>
#include<math.h>
#include<climits>
#include "user.h"
#include "adm.h"
Air_col airline = { { {"1-100-98","巴黎","合肥","20:00",30,1},
	{"1-102-80","北京","阜阳","20:30",40,2},
	{"1-100-91","东京","蚌埠","21:00",50,3},
	{"1-90-101","纽约","六安","21:30",35,4},
	{"1-100-92","上海","石家庄","22:00",40,5}},0};
char admin[] = "123";
char paswd[] = "123";
Users_info user_list = { {"123","123",1},0 };

int main()
{

	int input = 0;
	int input1 = 0;
	int input2 = 0;
	int op = 0;
	int op1 = 0;
	int ret;
	/*system("chcp 65001 >nul");*/
	Init_book(&airline);
	Init_user(&user_list);
	do {
		printf("欢迎进入劳大航空，请选择\n");
		printf("0.退出\n");
		printf("1.用户模式\n");
		printf("2.管理员模式\n");

		scanf("%d", &input);
		switch (input)
		{
		case 0:printf("退出成功，欢迎下次使用\n"); break;
		case 1:
			printf("请选择：\n1.登录\n2.注册\n0.退出\n");
			scanf("%d", &input2);
			switch (input2)
			{
			case 0:printf("退出\n"); break;
			case 1:
				ret = Login(&user_list);
				if (ret == 0)
				{
					continue;
				}
				do {
					uerpanel();
					scanf("%d", &input1);
					switch (input1)
					{
					case 1:
						do {
							Searchbook();
							scanf("%d", &op);

							switch (op)
							{
							case 0:printf("退出\n"); break;
							case 1: Search_Engine(&airline); break;
							case 2: Search_Engine1(&airline); break;
							case 3: Search_Engine2(&airline); break;
							case 4: Search_Engine3(&airline); break;
							case 5: Search_Engine4(&airline); break;
							case 6: Search_Engine5(&airline); break;
							default:printf("输入错误\n"); continue;
							}
						} while (op); break;
					case 2: Borrow_Book(&airline); break;
					case 3: Revert_Book(&airline); break;
					case 4: Borrow_History(&airline); break;
					case 0:printf("退出\n"); break;
					default:printf("输入错误\n"); continue;
					}
				} while (input1);
				break;
			case 2:Init_user(&user_list); Rigs(&user_list); break;
			default:printf("输入错误\n"); continue;
			}break;





		case 2:
			do
			{
				printf("请输出管理员账户与密码\n");
				char a[20] = { 0 };
				char b[20] = { 0 };
				scanf("%s", a);
				scanf("%s", b);
				if (0 == strcmp(admin, a))
				{
					if (0 == strcmp(paswd, b))
					{
						printf("登陆成功\n");
						break;
					}
					else {
						printf("密码错误，请重新输入\n");
					}
				}
				else {
					printf("账户不存在，请重新输入\n");
				}
			} while (1);
			do
			{
				Adminior_panel();
				scanf("%d", &op1);
				switch (op1)
				{
				case 0:printf("退出\n"); break;
				case 1: Add_Book(&airline); break;
				case 2: Delete_Book(&airline); break;
				case 3: Updata_Book(&airline); break;
				case 4: Show_Book(&airline); break;
				case 5: Add_User(&user_list); break;
				case 6: Delete_User(&user_list); break;
				case 7: Show_User(&user_list); break;
				case 8: Updata_user(&user_list); break;
				default:printf("输入错误\n"); continue;
				}

			} while (op1);
			break;
		default:printf("输入错误\n"); break;

		}
	} while (input);

	return 0;
}