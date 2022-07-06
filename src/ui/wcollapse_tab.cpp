#include "wcollapse_tab.h"
#include "wchild_widget.h"
#include "whorizontal_box.h"
#include "wvertical_box.h"
#include "wbutton.h"
#include "wimage.h"
#include "wtext.h"

void WCollapse_Tab::on_begin()
{
	ui->begin<WVertical_Box>();
	{
		ui->begin<WButton>();
		{
			ui->begin<WHorizontal_Box>();
			{
				ui->add<WImage>("texture/skull.tga", Vec2(10, 10));
				ui->add<WText>("Collapse Tab");
			}
			ui->end();
		}
		ui->end();

		// Where the child content goes!
		ui->begin<WChild_Widget>(this, &WCollapse_Tab::on_child_end);
	}
}

void WCollapse_Tab::on_child_end()
{
	ui->end(); // WVertical_Box
	ui->end(); // end ourself :)
}