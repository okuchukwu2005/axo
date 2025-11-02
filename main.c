// main.c
#include <stdio.h>
#include <stdbool.h>
#include "forms/form.h"
int main(void) {
    App app = init_app();
    app.window = new_window("My Window", 1000, 700);
    Parent container = new_container(&app.window, 10, 10, 360, 500);

    // Enable moving, title bar, close button,
    set_container_properties(&container, true, "My Container", true, true);
    register_container(&container);

    Parent container2 = new_container(&app.window, 400, 10, 400, 650);

    // Enable moving, title bar, close button,
    set_container_properties(&container2, true, "My Second Container", true, true);
    register_container(&container2);


 	 Entry entry =  new_entry(&container, 300, 70, 300, 2048);
 	 set_entry_placeholder(&entry, "enter text");
	register_entry(&entry);
	
Entry entry1 =  new_entry(&container2, 20, 70, 300, 2048);
 	 set_entry_placeholder(&entry, "enter text");
	register_entry(&entry1);
	
Entry entry2 =  new_entry(&app.window, 20, 70, 300, 2048);
 	 set_entry_placeholder(&entry, "enter text");
	register_entry(&entry2);
	


// 
     TextBox texty = new_textbox(&app.window, 20, 150, 300, 1024);
    register_textbox(&texty);

   TextBox texty1 = new_textbox(&container, 20, 300, 300, 1024);
    register_textbox(&texty1);

   TextBox texty2 = new_textbox(&container2, 20, 150, 300, 1024);
    register_textbox(&texty2);




    app_run_(&app.window);


    return 0;
}
