#include "SPArrayList.h"


SPArrayList* spArrayListCreate(int maxSize) {
	SPArrayList *list = (SPArrayList *)malloc(sizeof(SPArrayList));
	if (list == NULL || maxSize < 0) {
		return NULL;
	}
	list->maxSize = maxSize;
	list->actualSize = 0;
	list->elements = (CHMoveNode *)malloc(sizeof(CHMoveNode)*(list->maxSize));
	if (!(list->elements)) {
		return NULL;
	}
	return list;
}


SPArrayList* spArrayListCopy(SPArrayList* src) {
	if(src == NULL) {
		return NULL;
	}
	int i;
	SPArrayList *copyList = (SPArrayList *)malloc(sizeof(SPArrayList));
	if (!copyList) {
		return NULL;
	}
	copyList->actualSize = src->actualSize;
	copyList->maxSize = src->maxSize;
	copyList->elements = (CHMoveNode *)malloc(sizeof(CHMoveNode)*src->maxSize);
	if (!(copyList->elements)) {
		return NULL;
	}
	for(i = 0; i < src->actualSize; i++) {
		copyList->elements[i] = src->elements[i];
	}
	return copyList;
}


void spArrayListDestroy(SPArrayList* src) {
	if (src != NULL) {
		if(src->elements != NULL) {
			free(src->elements);
		}
		free(src);
	}
}


bool spArrayListIsEmpty(SPArrayList* src) {
	if ((src == NULL) || (src->actualSize!=0)) {
		return false;
	}
	return true;
}


SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, CHMoveNode elem, int index) {
	int i;
	if (src == NULL || index < 0 || index > src->actualSize) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	else {
		if (src->actualSize == src->maxSize) {
			for (i = src->actualSize - 1; i > index; i--) {
				src->elements[i] = src->elements[i-1];
			}
		} else {
			for(i = src->actualSize; i > index; i--) {
				src->elements[i] = src->elements[i-1];
			}
		}
		src->elements[index] = elem;
		if (src->actualSize < src->maxSize) {
			src->actualSize +=1;
		}
		return SP_ARRAY_LIST_SUCCESS;
	}
}


 SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, CHMoveNode elem) {
	 return spArrayListAddAt(src,elem,0);
 }


CHMoveNode spArrayListGetFirst(SPArrayList* src) {
//	if(src == NULL || src->actualSize == 0){
//		return NULL;
//	}
	return src->elements[0];
}


SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index) {
	int i;
	if (src == NULL || index < 0 || index >= src->actualSize) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	} else if (src->actualSize == 0){
		return SP_ARRAY_LIST_EMPTY;
	} else {
		for(i = index; i < src->actualSize - 1; i++) {
			src->elements[i] = src->elements[i+1];
		}
		src->actualSize -=1;
		return SP_ARRAY_LIST_SUCCESS;
	}
}


SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src) {
	return spArrayListRemoveAt(src, 0);
}
