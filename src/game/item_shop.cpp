#include "item_shop.h"
#include "core/input.h"
#include "core/context.h"
#include "ui/ui.h"
#include "ui/container/wcanvas.h"
#include "ui/container/wbox.h"
#include "ui/container/wvertical_box.h"
#include "ui/container/whorizontal_box.h"
#include "ui/input/wbutton.h"
#include "ui/visual/wtext.h"
#include "player/item_bank.h"

Item_Shop* item_shop = nullptr;

void Item_Shop::open()
{
	context.unlock_cursor();
	input->set_active_group(Input_Group::Game_UI);
	is_open = true;
}

void Item_Shop::close()
{
	context.lock_cursor();
	input->set_active_group(Input_Group::Game);
	is_open = false;
}

void Item_Shop::update_ui()
{
	INPUT_SCOPE(Input_Group::Game_UI);

	Canvas_Style::anchor(Vec2(0.5f));
	Canvas_Style::alignment(Vec2(0.5f));

	ui->begin<WBox>(Vec2(450.f, 300.f), Color(0.f, 0.f, 0.f, 0.4f));
	{
		Default_Style::padding(Vec2(8.f));
		ui->begin<WVertical_Box>();
		{
			// Header
			Vertical_Box_Style::halign(Horizontal_Align::Fill);
			ui->begin<WBox>(Color(0.8f, 0.6f, 0.2f, 0.2f));
			{
				Default_Style::padding(8.f);
				ui->add<WText>("This is the item shop");
			}
			ui->end();

			// Main shop area
			Vertical_Box_Style::valign(Vertical_Align::Fill);
			ui->begin<WBox>(Color(0.f, 0.f, 0.f, 0.4f));
			{
				ui->begin<WVertical_Box>();
				Vertical_Box_Style::padding(Vec2(4.f));
				for(const auto& wpn_info : weapon_bank)
				{
					auto* buy_btn = ui->begin<WButton>();
					if (buy_btn->was_pressed())
					{
						debug->print(TString::printf("Bought '%s'", wpn_info.name.data()), 4.f);
						close();
					}

					ui->add<WText>(TString::printf("Buy '%s'", wpn_info.name.data()));
					ui->end();
				}
				ui->end();
			}
			ui->end();
		}
		ui->end();
	}
	ui->end();
}