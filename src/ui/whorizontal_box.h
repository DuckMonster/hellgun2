#pragma once
#include "widget.h"

class WHorizontal_Box : public Widget
{
	WIDGET_CLASS(WHorizontal_Box)

public:
	void init() {}
	void build() override;
};