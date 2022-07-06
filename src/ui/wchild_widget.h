#pragma once
#include "widget.h"
#include "core/delegate.h"

class WChild_Widget : public Slotted_Widget<Default_Style>
{
	WIDGET_CLASS(WChild_Widget)

public:
	void init(TDelegate<void> end_callback) { this->end_callback = end_callback; }
	template<typename TObject>
	void init(TObject* obj, void (TObject::*func)()) { end_callback.bind_method(obj, func); }

	void on_end() { end_callback.execute(); }

private:
	TDelegate<void> end_callback;
};
