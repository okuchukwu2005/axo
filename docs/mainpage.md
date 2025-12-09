# Welcome
this is the official documentation of the axo project:
* [API BY CATEGORY]
* [ALL FUNCTIONS]

example main.c :
```c
#include <axo/axo.h>

int main(void)
{
	axInit();

	axParent *win = axCreateWindow("My Window", 1000, 700, WIN_RESIZABLE);

	axButton *button = axCreateButton(container2, 100, 400, 120, 40, "Click Me", OVERRIDE);
	axRegisterButton(button);

	axEvent event;
int running = 1;
while (running)
{
	while (poll_event(&event))
	{
		if (event.type == EVENT_QUIT)
		{
			running = 0;
		}

		// === UPDATE ALL WIDGETS ===
		axUpdateAllRegisteredButtons(&event);

		// === RENDER ===
		clear_screen(&parent->base, parent->color);
		axRenderAllRegisteredButtons();
		present_screen(&parent->base);
	}

	// === CLEANUP ===
	axFreeAllRegisteredButtons();
}


	return 0;
}

```