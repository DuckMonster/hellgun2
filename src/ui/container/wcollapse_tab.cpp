#include "wcollapse_tab.h"
#include "ui/container/wbox.h"
#include "ui/container/wchild_widget.h"
#include "ui/container/whorizontal_box.h"
#include "ui/container/wvertical_box.h"
#include "ui/input/wbutton.h"
#include "ui/visual/wimage.h"
#include "ui/visual/wtext.h"

void WCollapse_Tab::on_begin()
{
	ui->begin<WVertical_Box>();
	{
		Vertical_Box_Style::halign(Horizontal_Align::Fill);
		ui->begin<WBox>(Vec2(350.f, 0.f), Color::red);
		{
			ui->begin<WButton>([this] { collapsed = !collapsed; });
			{
				Default_Style::padding(Vec2(4.f));
				ui->begin<WHorizontal_Box>();
				{
					TString arrow_tex = "ui/arrow_right.tga";
					if (collapsed)
						arrow_tex = "ui/arrow_down.tga";

					Horizontal_Box_Style::valign(Vertical_Align::Center);
					ui->add<WImage>(arrow_tex, Vec2(8.f));

					//Horizontal_Box_Style::halign(Horizontal_Align::Center);
					Horizontal_Box_Style::padding(Vec2(8.f, 0.f));
					Horizontal_Box_Style::valign(Vertical_Align::Center);
					Horizontal_Box_Style::halign(Horizontal_Align::Center);
					ui->add<WText>("Collapse Tab");
				}
				ui->end();
			}
			ui->end();
		}
		ui->end();

		// Where the child content goes!
		ui->begin<WBox>(Color::black)->visibility = collapsed ? Widget_Visibility::Visible : Widget_Visibility::Collapsed;
		{
			ui->begin<WChild_Widget>(this, &WCollapse_Tab::on_child_end);

			// SNIP!
			// Rest of all the end's are in on_child_end()...
			// Very ugly but whatevah
		}
	}
}

void WCollapse_Tab::on_child_end()
{
	ui->end(); // WBox
	ui->end(); // WVertical_Box
	ui->end(); // end ourself :)
}