#include <stdlib.h>
#include "SPWindow.h"

void destroyWindow(SPWindow* src){
	if(!src){
		return;
	}
	src->destroyWindow(src);
}
