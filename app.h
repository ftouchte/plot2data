/**************************************
 * plot2data, GUI app
 *
 * check the layout below
 * 
 * @author ftouchte
 * @date July 3, 2025
 * **********************************/
 
/* -------------------------------------------------------------------
 * 				Header
 * -------------------------------------------------------------------  
 * Options/Tools |							⇑			
 *               |							⇑
 *               |							⇑
 *               |							⇑
 *				        Graph area			⇑
 *               |   							⇑
 *               |							⇑
 *               |							  body
 *									⇓
 *               | ---------------------------------------------------	⇓
 *               | 			Terminal/Log			⇓
 *               |							⇓
 *               |							⇓
 *               |							⇓
 * -------------------------------------------------------------------		
 *				Footer
 * -------------------------------------------------------------------*/

#ifndef PLOT_2_DATA_APP
#define PLOT_2_DATA_APP

#include <gtkmm.h>
#include <string>
#include <vector>
#include <cmath>

class Window : public Gtk::Window {
protected :
	// Widgets
	Gtk::Box VBox_main, VBox_header, VBox_body, VBox_footer, VBox_sidebar, VBox_tools, VBox_settings;
	Gtk::Paned HPaned, VPaned_sidebar, VPaned;
	Gtk::ScrolledWindow ScrolledWindow_tools, ScrolledWindow_terminal;
	Gtk::DrawingArea DrawingArea_plot;
	Gtk::Frame Frame_tools, Frame_settings, Frame_terminal, Frame_area;
	Gtk::Box HBox_xmin, HBox_xmax, HBox_ymin, HBox_ymax, HBox_X, HBox_Y;
	Gtk::Label Label_xmin, Label_xmax, Label_ymin, Label_ymax, Label_X, Label_Y;
	Gtk::Label Value_xmin, Value_xmax, Value_ymin, Value_ymax, Value_X, Value_Y;
    Gtk::Frame Frame_xmin, Frame_xmax, Frame_ymin, Frame_ymax, Frame_X, Frame_Y;
	//Gtk::Separator Separator_sidebar;

	// Peut-être devrais-je customiser les bouttons
	Gtk::Button Button_select_file;
	Gtk::Button Button_set_xmin;
	Gtk::Button Button_set_xmax;
	Gtk::Button Button_set_ymin;
	Gtk::Button Button_set_ymax;
	Gtk::Button Button_get_single_coord;
	Gtk::Button Button_start_recording;
	Gtk::Button Button_end_recording;
	Gtk::Button Button_save_data_as;
	Gtk::Button Button_measure_distance;

    // 
    Gtk::TextView TextView_terminal;
	Glib::RefPtr<Gtk::TextBuffer> TextBuffer_terminal;
    std::string String_terminal;

	// data
	std::string filename;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
    double X, Y;
	// for CSS file	
	Glib::RefPtr<Gtk::CssProvider> m_refCssProvider; // copied from gtkmm documention
	static void on_parsing_error(const Glib::RefPtr<const Gtk::CssSection>& section, const Glib::Error& error);
public :
	Window();
	~Window();

	// Signals
	void on_button_select_file_clicked();
	void on_button_set_xmin_clicked();
	void on_button_set_xmax_clicked();
	void on_button_set_ymin_clicked();
	void on_button_set_ymax_clicked();
	void on_button_get_single_coord_clicked();
	void on_button_start_recording_clicked();
	void on_button_end_recording_clicked();
	void on_button_save_data_as_clicked();
	void on_button_measure_distance_clicked();
    void update_terminal();
    void log_event(std::string event_description);
	//void on_mouse_clicked(int n_press, double x, double y);
    std::string time_t2string(std::time_t t, std::string format = "%Y/%m/%d %H:%M:%S");
};

#endif


