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
	Gtk::Box VBox_main, VBox_header, VBox_body, VBox_footer, VBox_sidebar;
	Gtk::Paned HPaned, VPaned;
	Gtk::DrawingArea DrawingArea_plot;

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

	// data
	std::string filename;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	// for CSS file	
	Glib::RefPtr<Gtk::CssProvider> m_refCssProvider; // copied from gtkmm documention
	static void on_parsing_error(const Glib::RefPtr<const Gtk::CssSection>& section, const Glib::Error& error);
public :
	Window();
	~Window();

	// Signals
	//void on_button_prev_clicked();
	//void on_mouse_clicked(int n_press, double x, double y);
};

#endif


