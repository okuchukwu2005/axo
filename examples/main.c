// main.c
#include <stdio.h>
#include <stdbool.h>
#include "../axo/axo.h"
#include "../axo/asset/bin/fira_code.h"

int main(void) {
    axInit();

    axParent win = axCreateWindow("My Window", 1000, 700);
    // Get DPI scaling for primary display
     float dpi = win.base.dpi_scale = get_display_dpi(0);

      // Scale font size based on DPI
      int scaled_font_size = (int)(current_theme->default_font_size * dpi); // round to nearest int

	RWops* bin = open_font_rw(fira_code_ttf,fira_code_ttf_len);
    // global_font = load_font_ttf("FiraCode-Regular.ttf", scaled_font_size);
    global_font = load_font_rw(bin, scaled_font_size);

    axParent container = axCreateContainer(&win, 10, 10, 360, 500);

    // Enable moving, title bar, close button,
    axSetContainerProperties(&container, true, "My Container", true, true);
     axRegisterContainer(&container);
 
     axParent container2 = axCreateContainer(&win, 400, 10, 400, 650);
// 
//     // Enable moving, title bar, close button,
     axSetContainerProperties(&container2, true, "My Second Container", true, true);
     axRegisterContainer(&container2);
// 
     	const char* content = "This is a Demo of c forms";
     axText text = axCreateText(&container, 0,0, content, 16, ALIGN_LEFT);
     // Register widget
     axRegisterText(&text);
 
         // ---------- RADIO BUTTON DEMO ----------
     // Group 1 (only one of these three can be selected at a time)
     axRadioButton radio[3];
     radio[0] = axCreateRadioButton(&container, 15, 100, 20, 20, "Option 1", 1);
     radio[1] = axCreateRadioButton(&container, 15, 130, 20, 20, "Option 2", 1);
     radio[2] = axCreateRadioButton(&container, 15, 160, 20, 20, "Option 3", 1);
 	for(int i = 0; i<3; i++){
 		axRegisterRadioButton(&radio[i]);
 	}
     // Group 2 (independent from group 1)
 	axRadioButton radio2[2];
 	radio2[0] = axCreateRadioButton(&container, 200, 100, 20, 20, "Choice A", 2);
 	radio2[1] = axCreateRadioButton(&container, 200, 130, 20, 20, "Choice B", 2);
 
 	for(int i = 0; i<2; i++){
 		axRegisterRadioButton(&radio2[i]);
 	}
//        // Create dropdown options
          char* options[] = {"Option 1", "Option 2", "Option 3", "Option 4"};
          int option_count = 4;
          axDropDown drop_down = axCreateDropDown(&container, 15, 200, 170, 30, options, option_count);
          axRegisterDropDown(&drop_down);
 // ---------- SLIDER DEMO ----------
     // Create a slider in container (horizontal, 100px wide, 20px high, range 0-100, starting at 50)
     axSlider slider = axCreateSlider(&container, 10, 370, 250, 15, 0, 100, 50, "Volume");
     axRegisterSlider(&slider);
// 
  	 axEntry entry =  axCreateEntry(&container2, 20, 70, 300, 2048);
  	 axSetEntryPlaceHolder(&entry, "enter text");
 	 axRegisterEntry(&entry);


      axTextBox texty = axCreateTextBox(&container2, 20, 150, 300, 1024);

    axRegisterTextBox(&texty);

    	axButton button = axCreateButton(&container2, 100, 400, 120, 40, "Click Me", OVERRIDE);
 	axRegisterButton(&button);
// 
 	axProgressBar prox = axCreateProgressBar(&container2, 20, 500, 250, 15, 0, 100, 50, true);
 
    axRegisterProgressBar(&prox);

//    Image  image = new_image(&app.window, 10, 0, "img.jpg", 0, 0 );
// register_image(&image);
    axRun(&win);


    return 0;
}
