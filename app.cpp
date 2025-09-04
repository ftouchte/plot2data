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
#include <ctime>
#include <iomanip>

/** Constructor */
Window::Window() :
	// Initialisation // Take of the order of declaration
	VBox_main(Gtk::Orientation::VERTICAL),
	VBox_header(Gtk::Orientation::VERTICAL,10),
	VBox_body(Gtk::Orientation::VERTICAL,10),
	VBox_footer(Gtk::Orientation::VERTICAL,10),
	VBox_sidebar(Gtk::Orientation::VERTICAL,0),
	VBox_tools(Gtk::Orientation::VERTICAL,10),
	VBox_settings(Gtk::Orientation::VERTICAL,5),
	HPaned(Gtk::Orientation::HORIZONTAL),
	VPaned_sidebar(Gtk::Orientation::VERTICAL),
	VPaned(Gtk::Orientation::VERTICAL),
	HBox_xmin(Gtk::Orientation::HORIZONTAL,20),
	HBox_xmax(Gtk::Orientation::HORIZONTAL,20),
	HBox_ymin(Gtk::Orientation::HORIZONTAL,20),
	HBox_ymax(Gtk::Orientation::HORIZONTAL,20),
	HBox_X(Gtk::Orientation::HORIZONTAL,20),
	HBox_Y(Gtk::Orientation::HORIZONTAL,20)
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
	VPaned_sidebar.set_position(550); 
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
	VPaned.set_position(700);
	VPaned.set_start_child(Frame_area);
	Frame_area.set_label("Drawing Area");
	Frame_area.set_label_align(Gtk::Align::START);
	Frame_area.set_name("frame-area");
	Frame_area.set_child(DrawingArea_plot);
	DrawingArea_plot.set_expand();
    DrawingArea_plot.set_draw_func(sigc::mem_fun(*this, &Window::on_draw_plot) );
	// Create a new object: Terminal or Log that inherits from ScrolledWindow
	VPaned.set_end_child(Frame_terminal);
    Frame_terminal.set_child(ScrolledWindow_terminal);
    ScrolledWindow_terminal.set_child(TextView_terminal);
    TextBuffer_terminal = Gtk::TextBuffer::create();
    TextView_terminal.set_buffer(TextBuffer_terminal);
	ScrolledWindow_terminal.set_child(TextView_terminal);
	TextView_terminal.set_name("text-terminal");
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
	VBox_tools.append(Button_set_wmin);
	Button_set_wmin.set_child(*Gtk::make_managed<Gtk::Label>("Set wmin", Gtk::Align::CENTER));
	Button_set_wmin.add_css_class("button-layout");
	VBox_tools.append(Button_set_wmax);
	Button_set_wmax.set_child(*Gtk::make_managed<Gtk::Label>("Set wmax", Gtk::Align::CENTER));
	Button_set_wmax.add_css_class("button-layout");
	VBox_tools.append(Button_set_hmin);
	Button_set_hmin.set_child(*Gtk::make_managed<Gtk::Label>("Set hmin", Gtk::Align::CENTER));
	Button_set_hmin.add_css_class("button-layout");
	VBox_tools.append(Button_set_hmax);
	Button_set_hmax.set_child(*Gtk::make_managed<Gtk::Label>("Set hmax", Gtk::Align::CENTER));
	Button_set_hmax.add_css_class("button-layout");
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

    // Button signals
    Button_select_file.signal_clicked().connect( sigc::mem_fun(*this, &Window::on_button_select_file_clicked));
    Button_set_wmin.signal_clicked().connect( sigc::mem_fun(*this, &Window::on_button_set_wmin_clicked));
    Button_set_wmax.signal_clicked().connect( sigc::mem_fun(*this, &Window::on_button_set_wmax_clicked));
    Button_set_hmin.signal_clicked().connect( sigc::mem_fun(*this, &Window::on_button_set_hmin_clicked));
    Button_set_hmax.signal_clicked().connect( sigc::mem_fun(*this, &Window::on_button_set_hmax_clicked));
    Button_get_single_coord.signal_clicked().connect( sigc::mem_fun(*this, &Window::on_button_get_single_coord_clicked));
    Button_start_recording.signal_clicked().connect( sigc::mem_fun(*this, &Window::on_button_start_recording_clicked));
    Button_end_recording.signal_clicked().connect( sigc::mem_fun(*this, &Window::on_button_end_recording_clicked));
    Button_save_data_as.signal_clicked().connect( sigc::mem_fun(*this, &Window::on_button_save_data_as_clicked));
    Button_measure_distance.signal_clicked().connect( sigc::mem_fun(*this, &Window::on_button_measure_distance_clicked));
	// Sidebar/Settings
	VBox_settings.set_name("sidebar-settings");
	VBox_settings.set_expand();
        // xmin
	VBox_settings.append(HBox_xmin);
    HBox_xmin.append(Label_xmin);
    Label_xmin.set_text("  xmin");
    Label_xmin.add_css_class("label-current-settings");
    HBox_xmin.append(Frame_xmin);
    Frame_xmin.set_child(Entry_xmin);
    Entry_xmin.set_text("nan");
    Frame_xmin.add_css_class("current-settings");
            // wmin
    HBox_xmin.append(Label_wmin);
    Label_wmin.set_text("  wmin");
    Label_wmin.add_css_class("label-current-settings");
    HBox_xmin.append(Frame_wmin);
    Frame_wmin.set_child(Value_wmin);
    Value_wmin.set_text("nan");
    Frame_wmin.add_css_class("current-settings");
        // xmax
	VBox_settings.append(HBox_xmax);
    HBox_xmax.append(Label_xmax);
    Label_xmax.set_text("  xmax");
    Label_xmax.add_css_class("label-current-settings");
    HBox_xmax.append(Frame_xmax);
    Frame_xmax.set_child(Entry_xmax);
    Entry_xmax.set_text("nan");
    Frame_xmax.add_css_class("current-settings");
            // wmax
    HBox_xmax.append(Label_wmax);
    Label_wmax.set_text("  wmax");
    Label_wmax.add_css_class("label-current-settings");
    HBox_xmax.append(Frame_wmax);
    Frame_wmax.set_child(Value_wmax);
    Value_wmax.set_text("nan");
    Frame_wmax.add_css_class("current-settings");
        // ymin
	VBox_settings.append(HBox_ymin);
    HBox_ymin.append(Label_ymin);
    Label_ymin.set_text("  ymin");
    Label_ymin.add_css_class("label-current-settings");
    HBox_ymin.append(Frame_ymin);
    Frame_ymin.set_child(Entry_ymin);
    Entry_ymin.set_text("nan");
    Frame_ymin.add_css_class("current-settings");
            // hmin
    HBox_ymin.append(Label_hmin);
    Label_hmin.set_text("  hmin");
    Label_hmin.add_css_class("label-current-settings");
    HBox_ymin.append(Frame_hmin);
    Frame_hmin.set_child(Value_hmin);
    Value_hmin.set_text("nan");
    Frame_hmin.add_css_class("current-settings");
        // ymax
	VBox_settings.append(HBox_ymax);
    HBox_ymax.append(Label_ymax);
    Label_ymax.set_text("  ymax");
    Label_ymax.add_css_class("label-current-settings");
    HBox_ymax.append(Frame_ymax);
    Frame_ymax.set_child(Entry_ymax);
    Entry_ymax.set_text("nan");
    Frame_ymax.add_css_class("current-settings");
            // hmax
    HBox_ymax.append(Label_hmax);
    Label_hmax.set_text("  hmax");
    Label_hmax.add_css_class("label-current-settings");
    HBox_ymax.append(Frame_hmax);
    Frame_hmax.set_child(Value_hmax);
    Value_hmax.set_text("nan");
    Frame_hmax.add_css_class("current-settings");
        // X
	VBox_settings.append(HBox_X);
    HBox_X.append(Label_X);
    Label_X.set_text("current X ");
    Label_X.add_css_class("label-current-settings");
    HBox_X.append(Frame_X);
    Frame_X.set_child(Value_X);
    Value_X.set_text("nan");
    Frame_X.add_css_class("current-settings");
            // W
    HBox_X.append(Label_W);
    Label_W.set_text("  W");
    Label_W.add_css_class("label-current-settings");
    HBox_X.append(Frame_W);
    Frame_W.set_child(Value_W);
    Value_W.set_text("nan");
    Frame_W.add_css_class("current-settings");
        // Y
	VBox_settings.append(HBox_Y);
    HBox_Y.append(Label_Y);
    Label_Y.set_text("current Y ");
    Label_Y.add_css_class("label-current-settings");
    HBox_Y.append(Frame_Y);
    Frame_Y.set_child(Value_Y);
    Value_Y.set_text("nan");
    Frame_Y.add_css_class("current-settings");
            // H
    HBox_Y.append(Label_H);
    Label_H.set_text("  H");
    Label_H.add_css_class("label-current-settings");
    HBox_Y.append(Frame_H);
    Frame_H.set_child(Value_H);
    Value_H.set_text("nan");
    Frame_H.add_css_class("current-settings");

    //////////////////////////////////
    /// Event controller
    //////////////////////////////////
    auto gesture_track = Gtk::EventControllerMotion::create();
    auto gesture_click = Gtk::GestureClick::create();
    DrawingArea_plot.add_controller(gesture_track);
    DrawingArea_plot.add_controller(gesture_click);
    gesture_track->signal_enter().connect(sigc::mem_fun(*this, &Window::on_mouse_tracked_enter));
    gesture_track->signal_leave().connect(sigc::mem_fun(*this, &Window::on_mouse_tracked_leave));
    gesture_track->signal_motion().connect(sigc::mem_fun(*this, &Window::on_mouse_tracked_move));
    gesture_click->signal_pressed().connect(sigc::mem_fun(*this, &Window::on_mouse_clicked));

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

	auto app = Gtk::Application::create("org.gtkmm.example.plot2data");

	return app->make_window_and_run<Window>(argc, argv);
}

void Window::on_button_select_file_clicked(){
    log_event("Select file...");
    log_event(std::string("  filename : ") + filename);
    ///////////////
    int width = DrawingArea_plot.get_width();
	int height = DrawingArea_plot.get_height();
    pixels =  Gdk::Pixbuf::create_from_file(filename.c_str(), width, height, true);
    DrawingArea_plot.queue_draw();
}

void Window::on_button_set_wmin_clicked(){
    wmin = wrec;
    Value_wmin.set_text(std::to_string(wmin));
    log_event("Set wmin...");
    log_event(std::string("  wmin : ") + std::to_string(wmin));
}

void Window::on_button_set_wmax_clicked(){
    wmax = wrec;
    Value_wmax.set_text(std::to_string(wmax));
    log_event("Set wmax...");
    log_event(std::string("  wmax : ") + std::to_string(wmax));
}

void Window::on_button_set_hmin_clicked(){
    hmin = hrec;
    Value_hmin.set_text(std::to_string(hmin));
    log_event("Set hmin...");
    log_event(std::string("  hmin : ") + std::to_string(hmin));
}

void Window::on_button_set_hmax_clicked(){
    hmax = hrec;
    Value_hmax.set_text(std::to_string(hmax));
    log_event("Set hmax...");
    log_event(std::string("  hmax : ") + std::to_string(hmax));
}

void Window::on_button_get_single_coord_clicked(){
    log_event("Get coord...");
    log_event(std::string("  X : ") + std::to_string(X) + std::string(";  Y : ") + std::to_string(Y));
}

void Window::on_button_start_recording_clicked(){
    log_event("Start recording...");
}

void Window::on_button_end_recording_clicked(){
    log_event("End recording...");
}

void Window::on_button_save_data_as_clicked(){
    log_event("Save data as...");
}

void Window::on_button_measure_distance_clicked(){
    log_event("Measure distance...");
}

std::string Window::time_t2string(std::time_t t, std::string format) {
	std::tm tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, format.c_str());
	return oss.str();
}

void Window::log_event(std::string event_description) {
    std::time_t now = std::time(nullptr);
    Gtk::TextBuffer::iterator iter = TextBuffer_terminal->end();
    iter = TextBuffer_terminal->insert_markup(iter, (std::string("<span foreground='red'>") + time_t2string(now, "[%H:%M:%S]  ") + std::string("</span>")).c_str());
    iter = TextBuffer_terminal->insert(iter, event_description);
    iter = TextBuffer_terminal->insert(iter, "\n");
    String_terminal = TextBuffer_terminal->get_text();
    TextView_terminal.scroll_to(iter);
}

void Window::on_mouse_tracked_enter(double x, double y){
    //log_event("Enter plot area...");
    flagInPlotArea = true;
    //int width = DrawingArea_plot.get_width();
	//int height = DrawingArea_plot.get_height();
    //log_event(std::string("  width : ") + std::to_string(width) + "  ; height : " + std::to_string(height));
}

void Window::on_mouse_tracked_move(double x, double y){
    W = x;
    H = y;
    Value_W.set_text(std::to_string(H));
    Value_H.set_text(std::to_string(W));
    Value_X.set_text("???");
    Value_Y.set_text("???");
    //log_event(std::string("  x : ") + std::to_string(x) + std::string(";  y : ") + std::to_string(y));
}

void Window::on_mouse_tracked_leave(){
    //log_event("Leave plot area...");
    flagInPlotArea = false;
}

void Window::on_mouse_clicked(int n_press, double x, double y){
    //log_event("Point selected...");
    wrec = x;
    hrec = y;
    log_event(std::string("Point selected... (nb. press = ") + std::to_string(n_press) + std::string(")"));
    log_event(std::string("  x : ") + std::to_string(x) + std::string(";  y : ") + std::to_string(y));
}

void Window::on_draw_plot(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    if (!pixels) return;
    Gdk::Cairo::set_source_pixbuf(cr, pixels, (width - pixels->get_width())/2, (height - pixels->get_height())/2);
    cr->paint();
}
