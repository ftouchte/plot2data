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
#include <iostream>

/** Constructor */
Window::Window() :
	// Initialisation // Take of the order of declaration
	VBox_main(Gtk::Orientation::VERTICAL),
	VBox_header(Gtk::Orientation::VERTICAL,10),
	VBox_body(Gtk::Orientation::VERTICAL,10),
	VBox_footer(Gtk::Orientation::VERTICAL,10),
	VBox_sidebar(Gtk::Orientation::VERTICAL,0),
	VBox_tools(Gtk::Orientation::VERTICAL,10),
	VBox_settings(Gtk::Orientation::VERTICAL,10),
	HPaned(Gtk::Orientation::HORIZONTAL),
	VPaned_sidebar(Gtk::Orientation::VERTICAL),
	VPaned(Gtk::Orientation::VERTICAL)
	//Separator_sidebar(Gtk::Orientation::HORIZONTAL)
{
	set_title("plot2data");
	set_default_size(1600,990);
	set_child(VBox_main);
	VBox_main.append(VBox_header);
	//VBox_header.append(*Gtk::make_managed<Gtk::Label>("Header area"));
	VBox_main.append(VBox_body);
	VBox_main.append(VBox_footer);
	//VBox_footer.append(*Gtk::make_managed<Gtk::Label>("Footer area"));

	// Header
	VBox_header.add_css_class("bar");
	// Footer
	VBox_footer.add_css_class("bar");
	// Body overview
	VBox_body.append(HPaned);
	HPaned.set_start_child(VBox_sidebar);
	HPaned.set_position(400); // requested size for the first child
	VBox_sidebar.append(VPaned_sidebar);
	VPaned_sidebar.set_start_child(ScrolledWindow_tools);
	VPaned_sidebar.set_position(600); 
	ScrolledWindow_tools.set_child(Frame_tools);
	//VBox_sidebar.append(Frame_tools);
	Frame_tools.set_label("Tools");
	Frame_tools.set_name("frame-tools");
	Frame_tools.set_child(VBox_tools);
	VPaned_sidebar.set_end_child(Frame_settings);
	Frame_settings.set_label("Settings");
	Frame_settings.set_name("frame-settings");
	Frame_settings.set_child(VBox_settings);
	HPaned.set_end_child(VPaned);
	VPaned.set_position(800);
	VPaned.set_start_child(Frame_area);
	Frame_area.set_label("Drawing Area");
	Frame_area.set_label_align(Gtk::Align::START);
	Frame_area.set_name("frame-area");
	Frame_area.set_child(DrawingArea_plot);
	DrawingArea_plot.set_expand();
	// Create a new object: Terminal or Log that inherits from ScrolledWindow
	VPaned.set_end_child(Frame_terminal);
	Frame_terminal.set_label("Terminal");
	Frame_terminal.set_label_align(Gtk::Align::START);
	Frame_terminal.set_name("frame-terminal");

	// Sidebar/Tools
	VBox_sidebar.set_name("sidebar");
	VBox_tools.set_name("sidebar-tools");
	//VBox_sidebar.add_css_class("bar");
	VBox_tools.append(Button_select_file);
	Button_select_file.set_margin_top(20);
	Button_select_file.set_child(*Gtk::make_managed<Gtk::Label>("Select file", Gtk::Align::CENTER));
	Button_select_file.add_css_class("button-layout");
	VBox_tools.append(Button_set_xmin);
	Button_set_xmin.set_child(*Gtk::make_managed<Gtk::Label>("Set xmin", Gtk::Align::CENTER));
	Button_set_xmin.add_css_class("button-layout");
	VBox_tools.append(Button_set_xmax);
	Button_set_xmax.set_child(*Gtk::make_managed<Gtk::Label>("Set xmax", Gtk::Align::CENTER));
	Button_set_xmax.add_css_class("button-layout");
	VBox_tools.append(Button_set_ymin);
	Button_set_ymin.set_child(*Gtk::make_managed<Gtk::Label>("Set ymin", Gtk::Align::CENTER));
	Button_set_ymin.add_css_class("button-layout");
	VBox_tools.append(Button_set_ymax);
	Button_set_ymax.set_child(*Gtk::make_managed<Gtk::Label>("Set ymax", Gtk::Align::CENTER));
	Button_set_ymax.add_css_class("button-layout");
	VBox_tools.append(Button_get_single_coord);
	Button_get_single_coord.set_child(*Gtk::make_managed<Gtk::Label>("Select a point", Gtk::Align::CENTER));
	Button_get_single_coord.add_css_class("button-layout");
	VBox_tools.append(Button_start_recording);
	Button_start_recording.set_child(*Gtk::make_managed<Gtk::Label>("Start recording", Gtk::Align::CENTER));
	Button_start_recording.add_css_class("button-layout");
	VBox_tools.append(Button_end_recording);
	Button_end_recording.set_child(*Gtk::make_managed<Gtk::Label>("End recording", Gtk::Align::CENTER));
	Button_end_recording.add_css_class("button-layout");
	VBox_tools.append(Button_save_data_as);
	Button_save_data_as.set_child(*Gtk::make_managed<Gtk::Label>("Save data As", Gtk::Align::CENTER));
	Button_save_data_as.add_css_class("button-layout");
	VBox_tools.append(Button_measure_distance);
	Button_measure_distance.set_child(*Gtk::make_managed<Gtk::Label>("Measure a distance", Gtk::Align::CENTER));
	Button_measure_distance.add_css_class("button-layout");
	
	// Sidebar/Settings
	VBox_settings.set_name("sidebar-settings");
	VBox_settings.set_expand();
	VBox_settings.append(Label_settings);
	//Label_settings.set_wrap();
	//Label_settings.set_justify(Gtk::Justification::FILL);
	Label_settings.set_text("xmin : \n"
				"xmax : \n"
				"ymin : \n"
				"ymax : \n"
				" X   : \n"
				" Y   : ");	

	// etc...
	//VBox_tools.append(Separator_sidebar);
	//Separator_sidebar.set_name("separator-sidebar");


	// Load extra CSS file (code copied from https://gnome.pages.gitlab.gnome.org/gtkmm-documentation/sec-custom-css-names.html)
	m_refCssProvider = Gtk::CssProvider::create();
#if HAS_STYLE_PROVIDER_ADD_PROVIDER_FOR_DISPLAY
	Gtk::StyleProvider::add_provider_for_display(get_display(), m_refCssProvider,
	GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#else
	Gtk::StyleContext::add_provider_for_display(get_display(), m_refCssProvider,
	GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#endif
	m_refCssProvider->signal_parsing_error().connect(
    		[](const auto& section, const auto& error)
    		{ on_parsing_error(section, error); }
  	);
	m_refCssProvider->load_from_path("custom_gtkmm.css");
	// end load extra CSS file



}


Window::~Window() {
	// do nothing
}

void Window::on_parsing_error(const Glib::RefPtr<const Gtk::CssSection>& section, const Glib::Error& error)
{ // gtkmm documention, load css file
	std::cerr << "on_parsing_error(): " << error.what() << std::endl;
	if (section)
	{
		const auto file = section->get_file();
		if (file)
		{
			std::cerr << "  URI = " << file->get_uri() << std::endl;
		}

	auto start_location = section->get_start_location();
	auto end_location = section->get_end_location();
	std::cerr << "  start_line = " << start_location.get_lines()+1
	      << ", end_line = " << end_location.get_lines()+1 << std::endl;
	std::cerr << "  start_position = " << start_location.get_line_chars()
	      << ", end_position = " << end_location.get_line_chars() << std::endl;
	}
}

/** Main function */
int main (int argc, char * argv[]) {
	printf("Start plot2data...\n");

	auto app = Gtk::Application::create("org.gtkmm.example");

	return app->make_window_and_run<Window>(argc, argv);
}



