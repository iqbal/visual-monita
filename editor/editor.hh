#ifndef GTKMM_EDITOR_HH
#define GTKMM_EDITOR_HH

#include <gtkmm.h>
#include <libxml/tree.h>
#include "parseXMLeditor.hh"
#include <gtkmm/drawingarea.h>

class EditorWindow : public Gtk::Window
{
public:
	EditorWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	virtual ~EditorWindow();

protected:

	void isi_tree();
	void isi_tree_per_jenis(const char* jenis, const char* jenis_obj);
	void load_tree();
	void on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
	
	void on_menu_file_new();
	void on_menu_file_open();
	void on_menu_file_save();
	void on_menu_file_quit();
	
	void on_file_save();	
	
	void on_button_add();
	void on_button_delete();
	void on_button_save();
	void on_button_grid();
		
	void update_title();
		
	void new_xml_garis();
	void new_xml_kotak();
	void new_xml_gambar();
	void new_xml_gambar_aktif();
	void new_xml_animasi();
	void new_xml_teks();
	void new_xml_teks_aktif();
	void new_xml_gauge_bar();
	void new_xml_gauge_dial();
	void new_xml_grafik();
	
	void new_file_xml();
	
	void delete_xml_object();
	void format_xml();
	 
	class XMLColumns : public Gtk::TreeModel::ColumnRecord
	{
	public:

		XMLColumns();		

		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		Gtk::TreeModelColumn<Glib::ustring> m_obj_name;
	};
	
	class PropColumns : public Gtk::TreeModel::ColumnRecord
	{
	public:

		PropColumns();

		Gtk::TreeModelColumn<Glib::ustring> m_prop_name;
		Gtk::TreeModelColumn<Glib::ustring> m_prop_value;
	};
	
	Glib::RefPtr<Gtk::Builder> m_refGlade;

	typedef Gtk::TreeModel::Children ambil_all_tree;

	XMLColumns m_XMLColumns;
	PropColumns m_PropColumns;
	
	Gtk::TreeModel::Row jenis_row;
	Gtk::TreeModel::Row row;
	Gtk::TreeModel::Row prop_row;
	Gtk::TreeModel::Row prop_row_child;
	Gtk::TreeModel::Row prop_row_child_prop;
	Gtk::TreeModel::Row prop_row_grand_child;
	Gtk::TreeModel::Row prop_row_grand_child_prop;
	
	Gtk::Button* m_Button_Save;
	Gtk::Button* m_Button_Add;
	Gtk::Button* m_Button_Delete;
	Gtk::Button* m_Button_Grid;
	
	Gtk::MenuItem* m_menu_new;
	Gtk::MenuItem* m_menu_open;
	Gtk::MenuItem* m_menu_save;
	Gtk::MenuItem* m_menu_quit;
	
	Gtk::Entry*	m_obj_entry_jenis;
	Gtk::Entry*	m_obj_entry_name;
	char tmp_entry_name[32];
	char filename[128];

	Gtk::ScrolledWindow* m_ScrolledWindow;
	Gtk::TreeView m_TreeView;
	Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;
	
	Gtk::ScrolledWindow* m_ScrolledWindow_prop;
	Gtk::TreeView m_TreeView_prop;
	Glib::RefPtr<Gtk::TreeStore> m_refTreeModel_prop;	
	
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
	void reload_objects();
	void delete_objects();
	
	bool isOpened;
	
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
	void view_grid();
	
	bool showGrid;
	double x_mouse, y_mouse;
	Gtk::Label*	m_status_posisi;
	
	virtual bool on_drawingarea_preview_expose_event(GdkEventExpose* event);
	virtual bool on_drawingarea_preview_motion_notify_event(GdkEventMotion *event);
	
	Gtk::DrawingArea* m_preview_area;
	Cairo::RefPtr<Cairo::Context> cr;
	Glib::RefPtr<Gdk::Pixbuf> image;
	Cairo::RefPtr<Cairo::ImageSurface> image_surface;
	
	int time_counter;
	int anima_state;
	bool update_gambar;
	
	bool timer_callback();
};

#endif //GTKMM_EDITOR_HH

