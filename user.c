#include "user.h"

#include <stdio.h>
#include <stdlib.h>


/*------- userlist ops --------------------------*/
userlist* userlist_create()
{
	userlist* ul;
	ul = (userlist*)malloc(sizeof(userlist));
	ul->size = 0;
	ul->capacity = 100;
	ul->list = (int*)malloc(100*sizeof(int));
	return ul;
}


int userlist_add(userlist* ul, int uid)
{
	int* old;
	old = ul->list;
	if(ul->size==ul->capacity)	{
		if(userlist_expand(ul) != 0)	{
			return -1;
		}
	}

	ul->list[ul->size] = uid;
	ul->size = ul->size+1;

	return 0;
}


int userlist_remove_by_id(userlist* ul, int uid)
{
	int i;
	for(i=0;i<ul->size;i++)	{
		if(ul->list[i] == uid)	{
			break;
		}
	}

	if(i==ul->size)	{	//not found
		perror("userlist_remove_by_id fail --- uid not found\n");
		return -1;
	}

	for(--i;i<ul->size-1;i++)	{
		ul->list[i] = ul->list[i+1];
	}
	return 0;
}

int userlist_expand(userlist* ul)
{
	ul->list = (int*)realloc(ul->list, ul->capacity * 2);
	if(ul->list==NULL)	{
		perror("realloc failed when expanding userlist\n");
		return -1;
	}
	ul->capacity = ul->capacity*2;
	return 0;
}
