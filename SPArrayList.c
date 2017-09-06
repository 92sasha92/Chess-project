
#include "SPArrayList.h"


SPArrayList* spArrayListCreate(int maxSize){
	SPArrayList *list = (SPArrayList *)malloc(sizeof(SPArrayList));
	if(list == NULL || maxSize < 0){
		return NULL;
	}
	list->maxSize = maxSize;
	list->actualSize = 0;
	list->elements = (int *)malloc(sizeof(int)*list->maxSize);
	if(!(list->elements)){
		return NULL;
	}
	return list;
}


SPArrayList* spArrayListCopy(SPArrayList* src){
	if(src == NULL){
		return NULL;
	}
	SPArrayList *copyList = (SPArrayList *)malloc(sizeof(SPArrayList));
	if(!copyList){
		return NULL;
	}
	copyList->actualSize = src->actualSize;
	copyList->maxSize = src->maxSize;
	copyList->elements = (int *)malloc(sizeof(int)*src->maxSize);
	if(!(copyList->elements)){
		return NULL;
	}
	for(int i = 0;i < src->actualSize;i++){
		copyList->elements[i] = src->elements[i];
	}
	return copyList;

}


void spArrayListDestroy(SPArrayList* src){
	if(src != NULL){
		if(src->elements != NULL){
			free(src->elements);
		}
		free(src);
	}
}


SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}


SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index){
	if(src == NULL || index < 0 || index > src->actualSize){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	else if(src->actualSize == src->maxSize){
		return SP_ARRAY_LIST_FULL;
	}
	else{
		for(int i = src->actualSize;i > index;i--){
			src->elements[i] = src->elements[i-1];
		}
		src->elements[index] = elem;
		src->actualSize +=1;
		return SP_ARRAY_LIST_SUCCESS;
	}
}


 SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem){
	 return spArrayListAddAt(src,elem,0);
 }


SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem){
	if(src==NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	return spArrayListAddAt(src,elem,src->actualSize);
}


SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	if(src == NULL || index < 0 || index >= src->actualSize){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	else if(src->actualSize == 0){
		return SP_ARRAY_LIST_EMPTY;
	}
	else{
		for(int i = index;i < src->actualSize - 1;i++){
			src->elements[i] = src->elements[i+1];
		}
		src->actualSize -=1;
		return SP_ARRAY_LIST_SUCCESS;
	}
}



SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src, 0);
}


SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	if(src==NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	return spArrayListRemoveAt(src, src->actualSize-1);
}


int spArrayListGetAt(SPArrayList* src, int index){
	if(src == NULL || index >= src->actualSize || index < 0){
		return -1;
	}
	return src->elements[index];
}


int spArrayListGetFirst(SPArrayList* src){
	if(src == NULL || src->actualSize == 0){
		return -1;
	}
	return src->elements[0];
}


int spArrayListGetLast(SPArrayList* src){
	if(src == NULL || src->actualSize == 0){
		return -1;
	}
	return src->elements[src->actualSize-1];
}


int spArrayListMaxCapacity(SPArrayList* src){
	if(src == NULL || src->elements == NULL){
		return -1;
	}
	return src->maxSize;
}


int spArrayListSize(SPArrayList* src){
	if(src == NULL || src->elements == NULL){
		return -1;
	}
	return src->actualSize;
}


bool spArrayListIsFull(SPArrayList* src){
	if(src == NULL || src->elements == NULL || src->actualSize < src->maxSize){
		return false;
	}
	return true;
}


bool spArrayListIsEmpty(SPArrayList* src){
	if(src == NULL || src->actualSize!=0){
		return false;
	}
	return true;
}
