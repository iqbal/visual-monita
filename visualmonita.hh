#ifndef GTKMM_VISUALMONITA_HH
#define GTKMM_VISUALMONITA_HH

#include <gtkmm.h>
#include <libxml/tree.h>
#include "parseXMLmonita.hh"
#include <gtkmm/drawingarea.h>
#include "sql_routine.hh"
#include <new>
using namespace std;


#define APP_VERSION "0.2"

#define MAKS_MESIN 5

//DEFINISI ERROR
#define ERROR_GAGAL_BUKA_FILE_KONFIG 	-1
#define ERROR_GAGAL_SIMPAN_FILE_KONFIG	-2

#define ERROR_GAGAL_KONEKSI_DATABASE 	-10
#define ERROR_GAGAL_SAAT_QUERY			-11

#define NO_EVENT 0
#define EVENT_ENTER_NOTIFY 100
#define EVENT_LEAVE_NOTIFY 101
#define EVENT_PRESS_NOTIFY 102
#define EVENT_CURSOR_INSIDE 103

class konfigurasi_vismon
{
public:
	char versi[7];

	//Layout options (v 0.1) 22 Feb.2010
	char default_layout[32];
	
	//database
	char db_host[25];
	char db_port[25];
	char db_user[25];
	char db_pass[25];
	char nama_db[25];


	//konfigurasi setting tampilan
	bool garis_display_caption;
	bool gambar_display_caption;
	bool akteks_display_caption;
	bool kotak_display_caption;
	bool gauge_bar_display_caption;
	bool gauge_dial_display_caption;
	
	unsigned long cek_konf;
};

class VisualMonita : public Gtk::Window
{
public:
	VisualMonita(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	virtual ~VisualMonita();

protected:
	void on_menu_file_open();
	void on_menu_file_quit();
	void on_menu_file_setting();
	
	Glib::RefPtr<Gtk::Builder> m_refGlade;
	
	Gtk::MenuItem* m_menu_open;
	Gtk::MenuItem* m_menu_quit;
	Gtk::MenuItem* m_menu_setting;
	
	char filename[64];
	char currentpath[64];
	bool isOpened;

	xmlDocPtr doc;
	xmlNodePtr 
		nodeLevel1,
		nodeLevel2,
		nodeLevel3,
		nodeLevel4,
		nodeLevel5,
		nodeLevel6,
		nodeLevel7,
		nodeLevel8;
	
	obj_header *m_hdr;
	obj_body *m_bg;
	obj_garis *m_obj_garis;
	obj_kotak *m_obj_kotak;
	obj_gambar *m_obj_gambar;
	obj_gambar_aktif *m_obj_gambar_aktif;
	obj_animasi *m_obj_animasi;
	obj_teks *m_obj_teks;
	obj_teks_aktif *m_obj_teks_aktif;
	obj_gauge_bar *m_obj_gauge_bar;
	obj_gauge_dial *m_obj_gauge_dial;
	obj_grafik *m_obj_grafik;
	
	int jumlah_garis;
	int jumlah_kotak;	
	int jumlah_gambar;
	int jumlah_gambar_aktif;
	int jumlah_animasi;
	int jumlah_teks;
	int jumlah_teks_aktif;
	int jumlah_gauge_bar;
	int jumlah_gauge_dial;
	int jumlah_grafik;	
	
	void load_objects();
	void delete_objects();
	
	void view_garis();
	void view_kotak();
	void view_gambar();
	void view_gambar_aktif();
	void view_animasi();
	void view_teks();
	void view_teks_aktif();
	void view_gauge_bar();
	void view_gauge_dial();
	void view_grafik();
	
	double x_mouse, y_mouse, state_mouse;
	
	virtual bool on_drawingarea_preview_expose_event(GdkEventExpose* event);
	virtual bool on_drawingarea_preview_button_release_event(GdkEventButton* event);
	virtual bool on_drawingarea_preview_motion_notify_event(GdkEventMotion *event);
	
	MyQuery MYQ;
	konfigurasi_vismon kfg;
	
	int BacaKonfig();
	int ApplyKonfig();
	int SimpanKonfig();
	
	Gtk::DrawingArea* m_preview_area;
	Cairo::RefPtr<Cairo::Context> cr;
	Glib::RefPtr<Gdk::Pixbuf> image;
	Cairo::RefPtr<Cairo::ImageSurface> image_surface;
	
	int time_counter;
	int anima_state;
	bool update_gambar;
	
	bool timer_callback();
	bool timer_data_update();
	bool timer_grafik_update();
	
	Glib::ustring usQuery, usTemp;
	Glib::ustring sTemp;
};

#endif //GTKMM_EDITOR_HH

