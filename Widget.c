#include <stdlib.h>
#include "Widget.h"

void destroyWidget(Widget* src) {//Making NULL Safe Destroy
	if (src == NULL ) {
		return;
	}
	src->destroyWidget(src);
}
