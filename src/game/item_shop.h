#pragma once

class Item_Shop
{
public:
	void open();
	void close();

	void update_ui();

	bool is_open = false;
};
extern Item_Shop* item_shop;