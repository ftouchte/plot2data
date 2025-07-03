/**************************************
 * plot2data, GUI app
 * 
 * @author ftouchte
 * @date July 3, 2025
 * **********************************/

#include "app.h"

#include <string>
#include <cmath>
#include <vector>
#include <functional>
#include <cstdio>
#include <cstdlib>

/** Constructor */
Window::Window() :
	// Initialisation // Take of the order of declaration
	VBox_main(Gtk::Orientation::VERTICAL),
	VBox_header(Gtk::Orientation::VERTICAL,10),
	VBox_body(Gtk::Orientation::VERTICAL,10),
	VBox_footer(Gtk::Orientation::VERTICAL,10),
	VBox_sidebar(Gtk::Orientation::VERTICAL,10),
	HPaned(Gtk::Orientation::HORIZONTAL),
	VPaned(Gtk::Orientation::VERTICAL)
{
	set_title("plot2data");
	set_default_size(1600,990);
	set_child(VBox_main);
	VBox_main.append(VBox_header);
	VBox_header.append(*Gtk::make_managed<Gtk::Label>("Header area"));
	VBox_main.append(VBox_body);
	VBox_main.append(VBox_footer);
	VBox_footer.append(*Gtk::make_managed<Gtk::Label>("Footer area"));
	
	// Body
	VBox_body.append(HPaned);
	HPaned.set_start_child(VBox_sidebar);
	VBox_sidebar.append(*Gtk::make_managed<Gtk::Label>("Sidebar area"));
	HPaned.set_end_child(VPaned);
	VPaned.set_start_child(DrawingArea_plot);
	DrawingArea_plot.set_expand();
	// Create a new object: Terminal or Log that inherits from ScrolledWindow
	VPaned.set_end_child(*Gtk::make_managed<Gtk::Label>("Terminal area"));

	// Sidebar
	VBox_sidebar.append(Button_select_file);
	Button_select_file.set_child(*Gtk::make_managed<Gtk::Label>("Select file"));
	VBox_sidebar.append(Button_set_xmin);
	Button_set_xmin.set_child(*Gtk::make_managed<Gtk::Label>("Set xmin"));
	VBox_sidebar.append(Button_set_xmax);
	Button_set_xmax.set_child(*Gtk::make_managed<Gtk::Label>("Set xmax"));
	VBox_sidebar.append(Button_set_xmin);
	Button_set_ymin.set_child(*Gtk::make_managed<Gtk::Label>("Set ymin"));
	VBox_sidebar.append(Button_set_ymax);
	Button_set_ymax.set_child(*Gtk::make_managed<Gtk::Label>("Set ymax"));
	// etc...

}


Window::~Window() {
	// do nothing
}

/** Main function */
int main (int argc, char * argv[]) {
	printf("Start plot2data...\n");

	auto app = Gtk::Application::create("org.gtkmm.example");

	return app->make_window_and_run<Window>(argc, argv);
}



