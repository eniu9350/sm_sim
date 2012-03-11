#ifndef _SM_SIM_USER_
#define _SM_SIM_USER_

/*------- userlist --------------------------*/
typedef struct userlist	{
	int* list;
	int size;
	int capacity;
}userlist;


/*------- userlist ops --------------------------*/
userlist* userlist_create();
int userlist_add(userlist* ul, int uid);
int userlist_remove_by_id(userlist* ul, int uid);
int userlist_expand(userlist* ul);

#endif
