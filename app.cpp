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

	// Header
	VBox_header.add_css_class("bar");
	// Footer
	VBox_footer.add_css_class("bar");
	// Body overview
		// left side
	VBox_body.append(HPaned);
	HPaned.set_start_child(Frame_sidebar);
	HPaned.set_position(300); // requested size for the first child
	Frame_sidebar.set_label("Tools");
	Frame_sidebar.set_name("frame-sidebar");
	Frame_sidebar.set_child(VBox_sidebar);
		// right side
	HPaned.set_end_child(VPaned);
		// top of right side
	VPaned.set_position(800);
	VPaned.set_start_child(Frame_area);
	Frame_area.set_label("Drawing Area");
	Frame_area.set_label_align(Gtk::Align::CENTER);
	Frame_area.set_name("frame-area");
	Frame_area.set_child(DrawingArea_plot);
	DrawingArea_plot.set_expand();
		// bottom of right side
	// Create a new object: Terminal or Log that inherits from ScrolledWindow
	VPaned.set_end_child(Frame_terminal);
	Frame_terminal.set_label("Terminal");
	Frame_terminal.set_label_align(Gtk::Align::CENTER);
	Frame_terminal.set_name("frame-terminal");

	// Sidebar widgets
	VBox_sidebar.set_name("sidebar");
	//VBox_sidebar.add_css_class("bar");
	VBox_sidebar.append(Button_select_file);
	Button_select_file.set_margin_top(20);
	Button_select_file.set_child(*Gtk::make_managed<Gtk::Label>("Select file", Gtk::Align::CENTER));
	Button_select_file.add_css_class("button-layout");
	VBox_sidebar.append(Button_set_xmin);
	Button_set_xmin.set_child(*Gtk::make_managed<Gtk::Label>("Set xmin", Gtk::Align::CENTER));
	Button_set_xmin.add_css_class("button-layout");
	VBox_sidebar.append(Button_set_xmax);
	Button_set_xmax.set_child(*Gtk::make_managed<Gtk::Label>("Set xmax", Gtk::Align::CENTER));
	Button_set_xmax.add_css_class("button-layout");
	VBox_sidebar.append(Button_set_ymin);
	Button_set_ymin.set_child(*Gtk::make_managed<Gtk::Label>("Set ymin", Gtk::Align::CENTER));
	Button_set_ymin.add_css_class("button-layout");
	VBox_sidebar.append(Button_set_ymax);
	Button_set_ymax.set_child(*Gtk::make_managed<Gtk::Label>("Set ymax", Gtk::Align::CENTER));
	Button_set_ymax.add_css_class("button-layout");
	// etc...
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



