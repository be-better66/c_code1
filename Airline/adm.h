#pragma once
#ifndef _ADM_H_
#define _ADM_H_
#include "user.h"

void Adminior_panel();

void Add_Book(Air_col* p);

void Delete_Book(Air_col* p);

void Updata_Book(Air_col* p);

void Show_Book(Air_col* p);

int Find_user(const Users_info* p, char a[]);

int Find_user_by_num(const Users_info* p, int num);

void Add_User(Users_info* p);

void Delete_User(Users_info* p);

void Show_User(Users_info* p);

void Updata_user(Users_info* p);
























#endif 
