#ifndef _SM_SIM_USER_
#define _SM_SIM_USER_

#include "alisttpl.h"

/*------- userlist --------------------------*/
typedef int user;

alisttpl_struct(user)
/*
typedef struct userlist	{
	int* list;
	int size;
	int capacity;
}userlist;
*/


/*------- userlist ops --------------------------*/
/*
userlist* userlist_create();
int userlist_add(userlist* ul, int uid);
int userlist_expand(userlist* ul);
*/
int userlist_remove_by_id(user_alist* ul, int uid);

#endif
