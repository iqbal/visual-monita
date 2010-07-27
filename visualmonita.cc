#include "visualmonita.hh"
#include "parseXMLmonita.hh"

#define FRAME_RATE 100
#define FRAME_RATE_DATA 500
#define FRAME_RATE_GRAFIK 60000
#define _DEBUG_PRINTF_ 1 //0 gak didebug, 1, debug sebagian, 2 debug semuanya

int update_counter;

VisualMonita::VisualMonita(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
    : Gtk::Window(cobject), m_refGlade(refGlade)
{	
    isOpened = false;

    m_refGlade->get_widget("menu_open", m_menu_open);
    m_refGlade->get_widget("menu_quit", m_menu_quit);
    m_refGlade->get_widget("menu_setting", m_menu_setting);
    m_refGlade->get_widget("drawingarea_preview", m_preview_area);

    m_menu_open->signal_activate().connect(sigc::mem_fun(*this, &VisualMonita::on_menu_file_open));
    m_menu_quit->signal_activate().connect(sigc::mem_fun(*this, &VisualMonita::on_menu_file_quit));
    m_menu_setting->signal_activate().connect(sigc::mem_fun(*this, &VisualMonita::on_menu_file_setting));
    m_preview_area->signal_expose_event().connect(sigc::mem_fun(*this, &VisualMonita::on_drawingarea_preview_expose_event));
    m_preview_area->signal_button_release_event().connect(sigc::mem_fun(*this, &VisualMonita::on_drawingarea_preview_button_release_event));
    m_preview_area->signal_motion_notify_event().connect(sigc::mem_fun(*this, &VisualMonita::on_drawingarea_preview_motion_notify_event));
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &VisualMonita::timer_callback),FRAME_RATE);
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &VisualMonita::timer_data_update),FRAME_RATE_DATA);
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &VisualMonita::timer_grafik_update),FRAME_RATE_GRAFIK);

    if(BacaKonfig ())
    {
        printf("Gagal membaca file konfigurasi. Set ke default\n");
        return;
    }

    //applikasikan konfigurasi ke database
    strcpy(MYQ.server,kfg.db_host);
    strcpy(MYQ.user,kfg.db_user);
    strcpy(MYQ.psw,kfg.db_pass);
    strcpy(MYQ.port,kfg.db_port);
    strcpy(MYQ.db,kfg.nama_db);
    //memcpy(&MYQ2,(void*)&MYQ,sizeof(MyQuery));

    if(!MYQ.init())//return 1 jika sukses
    {
        printf("Gagal koneksi ke database\n");
        on_menu_file_setting();
        return;
    }
    
    strcpy(currentpath, "xml/");
    strcpy(filename, "xml/index.xml");
    load_objects();
    if(doc != NULL)
    	xmlFreeDoc(doc);

    show_all_children();
}

VisualMonita::~VisualMonita()
{
    delete_objects();
    SimpanKonfig();
}

void VisualMonita::on_menu_file_quit()
{
    hide();
}

void VisualMonita::on_menu_file_open()
{
    Gtk::FileChooserDialog dialogop("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialogop.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialogop.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialogop.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:

    Gtk::FileFilter filter_xml;
    filter_xml.set_name("xml files");
    filter_xml.add_mime_type("text/xml");
    dialogop.add_filter(filter_xml);

    Gtk::FileFilter filter_any;
    filter_any.set_name("Any files");
    filter_any.add_pattern("*");
    dialogop.add_filter(filter_any);

    //Show the dialog and wait for a user response:
    int result = dialogop.run();

    //Handle the response:
    switch(result)
    {
    case(Gtk::RESPONSE_OK):
        {
            strcpy(filename, (dialogop.get_filename()).c_str());
            strcpy(currentpath, (dialogop.get_current_folder()).c_str());
            printf("%s\n", currentpath);
            printf("%s\n", filename);
            load_objects();
            if(doc != NULL)
    			xmlFreeDoc(doc);
    		m_preview_area->queue_draw();
            
            break;
        }
    case(Gtk::RESPONSE_CANCEL):
        {
            break;
        }
    default:
        {
            break;
        }
    }   
}

void VisualMonita::load_objects()
{
	doc = xmlParseFile(filename);
	isOpened = true;
    int hasil;
    int x;

    //HEADER
    m_hdr = new obj_header[1];
    hasil = isi_prop_header(doc, m_hdr);

    //BODY
    m_bg = new obj_body[1];
    hasil = isi_prop_body(doc, m_bg);

    //GARIS
    jumlah_garis = hitung_jml_obj(doc, "obj_garis");
    m_obj_garis = new obj_garis[jumlah_garis];
    hasil = isi_prop_garis(doc, m_obj_garis);

    //KOTAK
    jumlah_kotak = hitung_jml_obj(doc, "obj_kotak");
    m_obj_kotak = new obj_kotak[jumlah_kotak];
    hasil = isi_prop_kotak(doc, m_obj_kotak);

    //GAMBAR
    jumlah_gambar = hitung_jml_obj(doc, "obj_gambar");
    m_obj_gambar = new obj_gambar[jumlah_gambar];
    hasil = isi_prop_gambar(doc, m_obj_gambar);

    //GAMBAR AKTIF
    jumlah_gambar_aktif = hitung_jml_obj(doc, "obj_gambar_aktif");
    m_obj_gambar_aktif = new obj_gambar_aktif[jumlah_gambar_aktif];
    hasil = isi_prop_gambar_aktif(doc, m_obj_gambar_aktif);

    //ANIMASI
    jumlah_animasi = hitung_jml_obj(doc, "obj_animasi");
    m_obj_animasi = new obj_animasi[jumlah_animasi];
    hasil = isi_prop_animasi(doc, m_obj_animasi);

    //TEKS
    jumlah_teks = hitung_jml_obj(doc, "obj_teks");
    m_obj_teks = new obj_teks[jumlah_teks];
    hasil=isi_prop_teks(doc, m_obj_teks);

    //TEKS AKTIF
    jumlah_teks_aktif = hitung_jml_obj(doc, "obj_teks_aktif");
    m_obj_teks_aktif = new obj_teks_aktif[jumlah_teks_aktif];
    hasil = isi_prop_teks_aktif(doc, m_obj_teks_aktif);

    //GAUGE BAR
    jumlah_gauge_bar = hitung_jml_obj(doc, "obj_gauge_bar");
    m_obj_gauge_bar = new obj_gauge_bar[jumlah_gauge_bar];
    hasil = isi_prop_gauge_bar(doc, m_obj_gauge_bar);

    //GAUGE DIAL
    jumlah_gauge_dial = hitung_jml_obj(doc, "obj_gauge_dial");
    m_obj_gauge_dial = new obj_gauge_dial[jumlah_gauge_dial];
    hasil = isi_prop_gauge_dial(doc, m_obj_gauge_dial);

    //GRAFIK
    jumlah_grafik = hitung_jml_obj(doc, "obj_grafik");
    m_obj_grafik = new obj_grafik[jumlah_grafik];
    hasil = isi_prop_grafik(doc, m_obj_grafik);

	
	
    if(jumlah_grafik != 0)
    {
    	float fTemp;
        for(int g = 0; g < jumlah_grafik; g++)
        {
        	sprintf(MYQ.Text, "%s%i%s", "SELECT * FROM data_harian WHERE id_titik_ukur=" , m_obj_grafik[g].id_titik_data, " order by id_waktu DESC");

			if(MYQ.Query())
			{
				printf("error saat query\n");
				Gtk::MessageDialog dialog(*this, "Gagal melakukan query ke database");
				dialog.set_secondary_text("Periksa parameter koneksi database");

				dialog.run();
			}
			
			printf("jum row = %i\n", MYQ.jum_row);
			
			int n_max = 61;
			if(n_max >= MYQ.jum_row)
				n_max = MYQ.jum_row;
			for(int i = 0; i < n_max; i++)
            {
				sTemp = MYQ.ambil_string ("data_tunggal");
		        fTemp = atof(sTemp.c_str());
		        m_obj_grafik[g].value[i] = fTemp;
		        MYQ.Prev();
            }
            
            MYQ.Buang();
        }
    }
    
    if(jumlah_gauge_dial != 0)
    {
    	for(int g = 0; g < jumlah_grafik; g++)
        {
        	m_obj_gauge_dial[g].data = 0;
        }
    }    
    
}

void VisualMonita::delete_objects()
{
    delete [] m_hdr;
    m_hdr = NULL;
    delete [] m_bg;
    m_bg = NULL;
    delete [] m_obj_garis;
    m_obj_garis = NULL;
    delete [] m_obj_kotak;
    m_obj_kotak = NULL;
    delete [] m_obj_gambar;
    m_obj_gambar = NULL;
    delete [] m_obj_gambar_aktif;
    m_obj_gambar_aktif = NULL;
    delete [] m_obj_animasi;
    m_obj_animasi = NULL;
    delete [] m_obj_teks;
    m_obj_teks = NULL;
    delete [] m_obj_teks_aktif;
    m_obj_teks_aktif = NULL;
    delete [] m_obj_gauge_bar;
    m_obj_gauge_bar = NULL;
    delete [] m_obj_gauge_dial;
    m_obj_gauge_dial = NULL;
    delete [] m_obj_grafik;
    m_obj_grafik = NULL;
}

void VisualMonita::view_garis()
{
    for(int g = 0; g < jumlah_garis; g++)
    {
        cr->set_line_width(m_obj_garis[g].width);
        cr->move_to(m_obj_garis[g].titik[0].x,m_obj_garis[g].titik[0].y);
        cr->set_source_rgba(m_obj_garis[g].warna.r/255,
                            m_obj_garis[g].warna.g/255,
                            m_obj_garis[g].warna.b/255,
                            m_obj_garis[g].warna.a/100);
        for(int i = 0; i < m_obj_garis[g].points_count; i++)
        {
            cr->line_to(m_obj_garis[g].titik[i].x,m_obj_garis[g].titik[i].y);
        }
        cr->set_line_join(Cairo::LINE_JOIN_ROUND);
        cr->stroke();
    }
}

void VisualMonita::view_kotak()
{
    for(int k = 0; k < jumlah_kotak; k++)
    {
        cr->set_line_width(1);
        cr->set_source_rgba(m_obj_kotak[k].warna_outline.r/255,m_obj_kotak[k].warna_outline.g/255,m_obj_kotak[k].warna_outline.b/255,m_obj_kotak[k].warna_outline.a/100);
        cr->rectangle(m_obj_kotak[k].posisi.x, m_obj_kotak[k].posisi.y, m_obj_kotak[k].width, m_obj_kotak[k].height);
        cr->save();
        cr->set_source_rgba(m_obj_kotak[k].warna_fill.r/255, m_obj_kotak[k].warna_fill.g/255, m_obj_kotak[k].warna_fill.b/255, m_obj_kotak[k].warna_fill.a/100);
        cr->fill_preserve();
        cr->restore();
        cr->stroke();
    }
}

void VisualMonita::view_gambar()
{
    for(int g = 0; g < jumlah_gambar; g++)
    {
        image = Gdk::Pixbuf::create_from_file(m_obj_gambar[g].source);
        Cairo::Format format = Cairo::FORMAT_RGB24;
        if (image->get_has_alpha())
        {
            format = Cairo::FORMAT_ARGB32;
        }

        image_surface = Cairo::ImageSurface::create(format, image->get_width(), image->get_height());
        Gdk::Cairo::set_source_pixbuf (cr, image, m_obj_gambar[g].posisi.x, m_obj_gambar[g].posisi.y);

        cr->paint();
    }
}

void VisualMonita::view_gambar_aktif()
{
    for(int g = 0; g < jumlah_gambar_aktif; g++)
    {
        m_obj_gambar_aktif[g].data = (double)((rand() % 100) + 0);
        if(m_obj_gambar_aktif[g].data < m_obj_gambar_aktif[g].alarm_min_min)
            image = Gdk::Pixbuf::create_from_file(m_obj_gambar_aktif[g].source_min_min);
        if(m_obj_gambar_aktif[g].data >= m_obj_gambar_aktif[g].alarm_min_min && m_obj_gambar_aktif[g].data < m_obj_gambar_aktif[g].alarm_min)
            image = Gdk::Pixbuf::create_from_file(m_obj_gambar_aktif[g].source_min);
        if(m_obj_gambar_aktif[g].data >= m_obj_gambar_aktif[g].alarm_min && m_obj_gambar_aktif[g].data < m_obj_gambar_aktif[g].alarm_max)
            image = Gdk::Pixbuf::create_from_file(m_obj_gambar_aktif[g].source);
        if(m_obj_gambar_aktif[g].data >= m_obj_gambar_aktif[g].alarm_max && m_obj_gambar_aktif[g].data < m_obj_gambar_aktif[g].alarm_max_max)
            image = Gdk::Pixbuf::create_from_file(m_obj_gambar_aktif[g].source_max);
        if(m_obj_gambar_aktif[g].data >= m_obj_gambar_aktif[g].alarm_max_max)
            image = Gdk::Pixbuf::create_from_file(m_obj_gambar_aktif[g].source_max_max);

        Cairo::Format format = Cairo::FORMAT_RGB24;
        if (image->get_has_alpha())
        {
            format = Cairo::FORMAT_ARGB32;
        }

        image_surface = Cairo::ImageSurface::create(format, image->get_width(), image->get_height());
        Gdk::Cairo::set_source_pixbuf (cr, image, m_obj_gambar_aktif[g].posisi.x, m_obj_gambar_aktif[g].posisi.y);

        cr->paint();
    }
}

void VisualMonita::view_animasi()
{
    for(int a = 0; a < jumlah_animasi; a++)
    {
        image = Gdk::Pixbuf::create_from_file(m_obj_animasi[a].gambar[anima_state].source);

        Cairo::Format format = Cairo::FORMAT_RGB24;
        if (image->get_has_alpha())
        {
            format = Cairo::FORMAT_ARGB32;
        }

        image_surface = Cairo::ImageSurface::create(format, image->get_width(), image->get_height());
        Gdk::Cairo::set_source_pixbuf (cr, image, m_obj_animasi[a].gambar[anima_state].posisi.x, m_obj_animasi[a].gambar[anima_state].posisi.y);

        cr->paint();
    }
}

void VisualMonita::view_teks()
{
    for(int t = 0; t < jumlah_teks; t++)
    {
        cr->set_source_rgba(m_obj_teks[t].warna_fill.r/255, m_obj_teks[t].warna_fill.g/255, m_obj_teks[t].warna_fill.b/255, m_obj_teks[t].warna_fill.a/100);
        cr->select_font_face(m_obj_teks[t].font, Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
        cr->set_font_size(m_obj_teks[t].size);
        cr->move_to(m_obj_teks[t].posisi.x,m_obj_teks[t].posisi.y);
        cr->show_text(m_obj_teks[t].isi_teks);
        cr->stroke();
    }
}

void VisualMonita::view_teks_aktif()
{
    for(int t = 0; t < jumlah_teks_aktif; t++)
    {
        /* setting data teks aktif */
        //m_obj_teks_aktif[t].data = (double)((rand() % 100) + 0);
        sprintf(m_obj_teks_aktif[t].teks, "%0.2f", m_obj_teks_aktif[t].data);
        cr->select_font_face("Arial", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
        if(m_obj_teks_aktif[t].data < m_obj_teks_aktif[t].alarm_min_min)
            cr->set_source_rgba(m_obj_teks_aktif[t].warna_min_min.r/255, m_obj_teks_aktif[t].warna_min_min.g/255, m_obj_teks_aktif[t].warna_min_min.b/255, m_obj_teks_aktif[t].warna_min_min.a/100);
        if(m_obj_teks_aktif[t].data >= m_obj_teks_aktif[t].alarm_min_min && m_obj_teks_aktif[t].data < m_obj_teks_aktif[t].alarm_min)
            cr->set_source_rgba(m_obj_teks_aktif[t].warna_min.r/255, m_obj_teks_aktif[t].warna_min.g/255, m_obj_teks_aktif[t].warna_min.b/255, m_obj_teks_aktif[t].warna_min.a/100);
        if(m_obj_teks_aktif[t].data >= m_obj_teks_aktif[t].alarm_min && m_obj_teks_aktif[t].data < m_obj_teks_aktif[t].alarm_max)
            cr->set_source_rgba(m_obj_teks_aktif[t].warna.r/255, m_obj_teks_aktif[t].warna.g/255, m_obj_teks_aktif[t].warna.b/255, m_obj_teks_aktif[t].warna.a/100);
        if(m_obj_teks_aktif[t].data >= m_obj_teks_aktif[t].alarm_max && m_obj_teks_aktif[t].data < m_obj_teks_aktif[t].alarm_max_max)
            cr->set_source_rgba(m_obj_teks_aktif[t].warna_max.r/255, m_obj_teks_aktif[t].warna_max.g/255, m_obj_teks_aktif[t].warna_max.b/255, m_obj_teks_aktif[t].warna_max.a/100);
        if(m_obj_teks_aktif[t].data >= m_obj_teks_aktif[t].alarm_max_max)
            cr->set_source_rgba(m_obj_teks_aktif[t].warna_max_max.r/255, m_obj_teks_aktif[t].warna_max_max.g/255, m_obj_teks_aktif[t].warna_max_max.b/255, m_obj_teks_aktif[t].warna_max_max.a/100);
        cr->set_font_size(m_obj_teks_aktif[t].size);
        cr->move_to(m_obj_teks_aktif[t].posisi.x,m_obj_teks_aktif[t].posisi.y);
        cr->show_text(m_obj_teks_aktif[t].teks);
        cr->stroke();
    }
}

void VisualMonita::view_gauge_bar()
{
}

void VisualMonita::view_gauge_dial()
{
    char cDataTemp[32];
	for(int k=0; k < jumlah_gauge_dial; k++)
    {
        int i=k;

        cr->set_source_rgba(255.0/255,255.0/255,255.0/255,100.0/100);
        cr->arc(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2,
                m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height/2,
                m_obj_gauge_dial[k].width/2, 0 , 2 * M_PI);
        cr->fill();
        
        cr->set_line_width(m_obj_gauge_dial[k].width / 40);
        cr->set_source_rgba(150.0/255, 150.0/255, 150.0/255, 60.0/100);
        cr->arc(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2,
                m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height/2,
                m_obj_gauge_dial[k].width/1.90, 0 , 2 * M_PI);        
        cr->stroke();
        cr->set_source_rgba(200.0/255, 200.0/255, 200.0/255, 100.0/100);
        cr->arc(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2,
                m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height/2,
                m_obj_gauge_dial[k].width/1.95, 0 , 2 * M_PI);        
        cr->stroke();

		float batas_warna1;
		float batas_warna2;
		
		cr->set_line_width(m_obj_gauge_dial[k].width / 10);
		
		batas_warna1 = m_obj_gauge_dial[k].range_min;
		batas_warna2 = ((m_obj_gauge_dial[k].alarm_min_min - m_obj_gauge_dial[k].range_min) / (m_obj_gauge_dial[k].range_max - m_obj_gauge_dial[k].range_min)) * (390 - 150);
		cr->set_source_rgba(m_obj_gauge_dial[k].warna_min_min.r/255, m_obj_gauge_dial[k].warna_min_min.g/255, m_obj_gauge_dial[k].warna_min_min.b/255, m_obj_gauge_dial[k].warna_min_min.a/100);
        cr->arc(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2,
                m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height/2,
                m_obj_gauge_dial[k].width/2.5, ((150 + batas_warna1) * M_PI / 180) , ((150 + batas_warna2) * M_PI / 180));
        cr->stroke();
        
        batas_warna1 = batas_warna2;
		batas_warna2 = ((m_obj_gauge_dial[k].alarm_min - m_obj_gauge_dial[k].range_min) / (m_obj_gauge_dial[k].range_max - m_obj_gauge_dial[k].range_min)) * (390 - 150);
		cr->set_source_rgba(m_obj_gauge_dial[k].warna_min.r/255, m_obj_gauge_dial[k].warna_min.g/255, m_obj_gauge_dial[k].warna_min.b/255, m_obj_gauge_dial[k].warna_min.a/100);
        cr->arc(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2,
                m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height/2,
                m_obj_gauge_dial[k].width/2.5, ((150 + batas_warna1) * M_PI / 180) , ((150 + batas_warna2) * M_PI / 180));
        cr->stroke();
        
        batas_warna1 = batas_warna2;
		batas_warna2 = ((m_obj_gauge_dial[k].alarm_max - m_obj_gauge_dial[k].range_min) / (m_obj_gauge_dial[k].range_max - m_obj_gauge_dial[k].range_min)) * (390 - 150);
		cr->set_source_rgba(m_obj_gauge_dial[k].warna.r/255, m_obj_gauge_dial[k].warna.g/255, m_obj_gauge_dial[k].warna.b/255, m_obj_gauge_dial[k].warna.a/100);
        cr->arc(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2,
                m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height/2,
                m_obj_gauge_dial[k].width/2.5, ((150 + batas_warna1) * M_PI / 180) , ((150 + batas_warna2) * M_PI / 180));
        cr->stroke();
        
        batas_warna1 = batas_warna2;
		batas_warna2 = ((m_obj_gauge_dial[k].alarm_max_max - m_obj_gauge_dial[k].range_min) / (m_obj_gauge_dial[k].range_max - m_obj_gauge_dial[k].range_min)) * (390 - 150);
		cr->set_source_rgba(m_obj_gauge_dial[k].warna_max.r/255, m_obj_gauge_dial[k].warna_max.g/255, m_obj_gauge_dial[k].warna_max.b/255, m_obj_gauge_dial[k].warna_max.a/100);
        cr->arc(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2,
                m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height/2,
                m_obj_gauge_dial[k].width/2.5, ((150 + batas_warna1) * M_PI / 180) , ((150 + batas_warna2) * M_PI / 180));
        cr->stroke();
        
        batas_warna1 = batas_warna2;
		batas_warna2 = ((m_obj_gauge_dial[k].range_max - m_obj_gauge_dial[k].range_min) / (m_obj_gauge_dial[k].range_max - m_obj_gauge_dial[k].range_min)) * (390 - 150);
		cr->set_source_rgba(m_obj_gauge_dial[k].warna_max_max.r/255, m_obj_gauge_dial[k].warna_max_max.g/255, m_obj_gauge_dial[k].warna_max_max.b/255, m_obj_gauge_dial[k].warna_max_max.a/100);
        cr->arc(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2,
                m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height/2,
                m_obj_gauge_dial[k].width/2.5, ((150 + batas_warna1) * M_PI / 180) , ((150 + batas_warna2) * M_PI / 180));
        cr->stroke();
        
        cr->set_line_width(1);
        //Gambar skala
        //Lingkaran dimulai dari sudut 0 Pi sampai 2Pi
        // 0 ada di kanan (seperti kartesian X-Y biasa)
        // Arah ARC searah jarum jam
        //- Dikonversi jadi sudut 0-360
        //- yang dipakai adalah dari 150 - 360+30 jadi range-nya adalah 260 derajat

        float radius = m_obj_gauge_dial[k].width/2;
        float x = m_obj_gauge_dial[k].posisi.x + m_obj_gauge_dial[k].width/2;
        float y = m_obj_gauge_dial[k].posisi.y + m_obj_gauge_dial[k].height/2;
        float inset = 0.1;
        float sudut_min = 150;
        float sudut_max = 390;
        float sudut_plot;
        float nilai;
        cr->save(); /* save pen size to stack */
        cr->select_font_face("Courier",Cairo::FONT_SLANT_NORMAL ,Cairo::FONT_WEIGHT_BOLD);
        cr->set_font_size(10);

        //printf("Gauge dial- Marker dan Label. nomer gauge: %d\n", i);
        for (int j = 0; j <= 10; j++)
        {

            nilai = j * (m_obj_gauge_dial[k].range_max - m_obj_gauge_dial[k].range_min) / 10;
            sudut_plot = sudut_min + (nilai * 240 / (m_obj_gauge_dial[k].range_max - m_obj_gauge_dial[k].range_min));


            if (j % 5 == 0)
            {
                inset = 0.15 * radius;
            }
            else
            {
                inset = 0.1 * radius;

            }
            cr->set_source_rgba(0.0/255,0.0/255,0.0/255,100.0/100);
            cr->move_to (x + (radius - inset) * cos (sudut_plot * M_PI / 180),
                         y + (radius - inset) * sin (sudut_plot * M_PI / 180));
            cr->line_to (x + radius * cos (sudut_plot * M_PI / 180),
                         y + radius * sin (sudut_plot * M_PI / 180));
            cr->stroke();

            sprintf(cDataTemp,"%g",m_obj_gauge_dial[k].range_min+nilai);
            
            cr->move_to(x + (0.8 *radius * cos (sudut_plot * M_PI / 180)) - (strlen(cDataTemp)*5),
                        y + 0.8 *radius * sin (sudut_plot* M_PI / 180)+ (6));

            cr->set_source_rgba(0.0/255,0.0/255,0.0/255,80.0/100);


            cr->show_text(cDataTemp);
            cr->stroke();

        }
        //cr->restore(); /* recover pen size from stack */

        //Plot garis-garis range dan alarm
        //Gambar jarum
        radius=0.8*radius; //panjang jarum
        sudut_plot=sudut_min+(m_obj_gauge_dial[k].data*240/(m_obj_gauge_dial[k].range_max-m_obj_gauge_dial[k].range_min));

        //Jarum besar
        cr->set_line_width(4);
        cr->set_source_rgba(0.0/255,100.0/255,200.0/255,100.0/100);
        cr->move_to (x,y);
        cr->line_to (x + radius * cos (sudut_plot * M_PI / 180),
                     y + radius * sin (sudut_plot * M_PI / 180));
        cr->stroke();

        //Jarum tengah
        cr->set_line_width(1);
        cr->set_source_rgba(240.0/255,0.0/255,0.0/255,100.0/100);
        cr->move_to (x,y);
        cr->line_to (x + radius * cos (sudut_plot * M_PI / 180),
                     y + radius * sin (sudut_plot * M_PI / 180));
        cr->stroke();

        //Penutup Jarum
        int besar_tutup=m_obj_gauge_dial[k].width*0.05;
        cr->set_source_rgba(0.0/255,0.0/255,00.0/255,100.0/100);
        cr->arc(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2,
                m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height/2,
                besar_tutup, 0, 2* M_PI);
        cr->fill();

        cr->set_source_rgba(0.0/255,0.0/255,00.0/255,20.0/100);
        cr->arc(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2+2,
                m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height/2+2,
                besar_tutup, 0, 2* M_PI);
        cr->fill();


        int x_dasar=m_obj_gauge_dial[k].posisi.x;
        int y_dasar=m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height;
        int width_raksa=m_obj_gauge_dial[k].width;

        //Tinggi raksa= data/(max-min) * height beneran
        float height_raksa=m_obj_gauge_dial[k].height*(m_obj_gauge_dial[k].data/(m_obj_gauge_dial[k].range_max-m_obj_gauge_dial[k].range_min));

        int y_atas=y_dasar-height_raksa;

        int font_size;
        //Teks nilai
        if(m_obj_gauge_dial[k].show_text)
        {
            font_size=m_obj_gauge_dial[k].width*0.15;
            cr->set_font_size(font_size);
            cr->select_font_face("Courier",Cairo::FONT_SLANT_NORMAL ,Cairo::FONT_WEIGHT_BOLD);

            if(abs(m_obj_gauge_dial[k].data)<=99)
                sprintf(cDataTemp,"%0.2f",m_obj_gauge_dial[k].data);
            else
                sprintf(cDataTemp,"%0.5g",m_obj_gauge_dial[k].data);

            cr->move_to(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2 - (strlen(cDataTemp)*font_size/3),
                        m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height*3/4);
            cr->set_source_rgba(0.0/255,0.0/255,0.0/255,100.0/100);


            cr->show_text(cDataTemp);
        }
        //Caption
        if(m_obj_gauge_dial[k].show_caption)
        {
            font_size=m_obj_gauge_dial[k].width*0.10;
            cr->set_font_size(font_size);
            sprintf(cDataTemp,"%s",m_obj_gauge_dial[k].caption);
            cr->move_to(m_obj_gauge_dial[k].posisi.x+m_obj_gauge_dial[k].width/2 - (strlen(cDataTemp)*font_size/3),
                        m_obj_gauge_dial[k].posisi.y+m_obj_gauge_dial[k].height*2/5);
            cr->set_source_rgba(0.0/255,0.0/255,0.0/255,100.0/100);

            cr->show_text(cDataTemp);
        }
        cr->restore();
    }
    /*EOF GAUGE DIAL*/
}

void VisualMonita::view_grafik()
{
    for(int g = 0; g < jumlah_grafik; g++)
	{	
		int x_off1 = 50;
		int x_off2 = 20;
		int y_off1 = 30;
		int y_off2 = 35;
		
		m_obj_grafik[g].x_length = m_obj_grafik[g].width - (x_off1 + x_off2);
		m_obj_grafik[g].y_length = m_obj_grafik[g].height - (y_off1 + y_off2);
		m_obj_grafik[g].konversi_value = (((m_obj_grafik[g].range_max) - (m_obj_grafik[g].range_min)) / m_obj_grafik[g].y_length);
		
		//gambar kontainer
		
		cr->rectangle(m_obj_grafik[g].posisi.x, m_obj_grafik[g].posisi.y, m_obj_grafik[g].width, m_obj_grafik[g].height);
		cr->set_source_rgba(255, 255 , 255, 100);
		cr->fill();
		cr->set_line_width(5);
        cr->set_source_rgba(150.0/255, 150.0/255, 150.0/255, 60.0/100);
        cr->rectangle(m_obj_grafik[g].posisi.x - 2, m_obj_grafik[g].posisi.y - 2, m_obj_grafik[g].width + 4, m_obj_grafik[g].height + 4);
        cr->stroke();
        cr->set_line_width(3);
        cr->set_source_rgba(200.0/255, 200.0/255, 200.0/255, 100.0/100);
        cr->rectangle(m_obj_grafik[g].posisi.x, m_obj_grafik[g].posisi.y, m_obj_grafik[g].width, m_obj_grafik[g].height);
        cr->stroke();
		
		//gambar sumbu
		cr->set_line_width(2);
		cr->set_source_rgba(0, 0, 0, 100);		
		cr->move_to((m_obj_grafik[g].posisi.x + x_off1) , (m_obj_grafik[g].posisi.y + y_off1));
		cr->line_to((m_obj_grafik[g].posisi.x + x_off1) , (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length));
		cr->line_to((m_obj_grafik[g].posisi.x + x_off1 + m_obj_grafik[g].x_length) , (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length));
		cr->stroke();
		
		int second = 60;
		char cDataTemp[32];
		cr->set_font_size(10);
		for(int i = 0; i <= 6; i++)
		{
			cr->move_to((m_obj_grafik[g].posisi.x + x_off1 + (m_obj_grafik[g].x_length * i/6)), (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length));
			cr->line_to((m_obj_grafik[g].posisi.x + x_off1 + (m_obj_grafik[g].x_length * i/6)), (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length + 4));
			cr->stroke();
			cr->move_to((m_obj_grafik[g].posisi.x + x_off1 + (m_obj_grafik[g].x_length * i/6) - 6), (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length + 15));
			sprintf(cDataTemp,"%i", second);
			cr->show_text(cDataTemp);
			second = second - 10;
		}
		
		cr->move_to((m_obj_grafik[g].posisi.x + m_obj_grafik[g].x_length + 10), (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length + 25));
		cr->show_text("Minutes");
		
		float val_label;
		for(int i = 0; i <= 4; i++)
		{
			cr->move_to((m_obj_grafik[g].posisi.x + x_off1), (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length * (i)/4));
			cr->line_to((m_obj_grafik[g].posisi.x + x_off1 - 4), (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length * (i)/4));
			cr->stroke();
			cr->move_to((m_obj_grafik[g].posisi.x + x_off1 - 30), (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length * (i)/4 + 3));
			val_label = (m_obj_grafik[g].range_max) - (((m_obj_grafik[g].range_max) - (m_obj_grafik[g].range_min)) * i/4);
			sprintf(cDataTemp,"%0.1f", val_label);
			cr->show_text(cDataTemp);
		}
		
		cr->move_to((m_obj_grafik[g].posisi.x + x_off1 - 30), (m_obj_grafik[g].posisi.y + y_off1 - 15));
		sprintf(cDataTemp,"%s", m_obj_grafik[g].caption);
		cr->show_text(cDataTemp);
		
		cr->save();
		
		cr->set_line_width(2);
		std::valarray< double > dashes(2);
    	dashes[0] = 2.0;
    	dashes[1] = 2.0;    
    	cr->set_dash (dashes, 0.0);
    		
		cr->set_source_rgba(m_obj_grafik[g].warna_min_min.r/255, m_obj_grafik[g].warna_min_min.g/255, m_obj_grafik[g].warna_min_min.b/255, m_obj_grafik[g].warna_min_min.a/100);
        cr->move_to((m_obj_grafik[g].posisi.x + x_off1) , (m_obj_grafik[g].posisi.y + y_off1 + (m_obj_grafik[g].y_length - ((m_obj_grafik[g].alarm_min_min - m_obj_grafik[g].range_min)  / m_obj_grafik[g].konversi_value))));
		cr->line_to((m_obj_grafik[g].posisi.x + x_off1 + m_obj_grafik[g].x_length) , (m_obj_grafik[g].posisi.y + y_off1 + (m_obj_grafik[g].y_length - ((m_obj_grafik[g].alarm_min_min - m_obj_grafik[g].range_min) / m_obj_grafik[g].konversi_value))));
		cr->stroke();
		
		cr->set_source_rgba(m_obj_grafik[g].warna_min.r/255, m_obj_grafik[g].warna_min.g/255, m_obj_grafik[g].warna_min.b/255, m_obj_grafik[g].warna_min.a/100);
        cr->move_to((m_obj_grafik[g].posisi.x + x_off1) , (m_obj_grafik[g].posisi.y + y_off1 + (m_obj_grafik[g].y_length - ((m_obj_grafik[g].alarm_min - m_obj_grafik[g].range_min) / m_obj_grafik[g].konversi_value))));
		cr->line_to((m_obj_grafik[g].posisi.x + x_off1 + m_obj_grafik[g].x_length) , (m_obj_grafik[g].posisi.y + y_off1 + (m_obj_grafik[g].y_length - ((m_obj_grafik[g].alarm_min - m_obj_grafik[g].range_min) / m_obj_grafik[g].konversi_value))));
		cr->stroke();
		
		cr->set_source_rgba(m_obj_grafik[g].warna_max.r/255, m_obj_grafik[g].warna_max.g/255, m_obj_grafik[g].warna_max.b/255, m_obj_grafik[g].warna_max.a/100);
        cr->move_to((m_obj_grafik[g].posisi.x + x_off1) , (m_obj_grafik[g].posisi.y + y_off1 + (m_obj_grafik[g].y_length - ((m_obj_grafik[g].alarm_max - m_obj_grafik[g].range_min) / m_obj_grafik[g].konversi_value))));
		cr->line_to((m_obj_grafik[g].posisi.x + x_off1 + m_obj_grafik[g].x_length) , (m_obj_grafik[g].posisi.y + y_off1 + (m_obj_grafik[g].y_length - ((m_obj_grafik[g].alarm_max - m_obj_grafik[g].range_min) / m_obj_grafik[g].konversi_value))));
		cr->stroke();
		
		cr->set_source_rgba(m_obj_grafik[g].warna_max_max.r/255, m_obj_grafik[g].warna_max_max.g/255, m_obj_grafik[g].warna_max_max.b/255, m_obj_grafik[g].warna_max_max.a/100);
        cr->move_to((m_obj_grafik[g].posisi.x + x_off1) , (m_obj_grafik[g].posisi.y + y_off1 + (m_obj_grafik[g].y_length - ((m_obj_grafik[g].alarm_max_max - m_obj_grafik[g].range_min) / m_obj_grafik[g].konversi_value))));
		cr->line_to((m_obj_grafik[g].posisi.x + x_off1 + m_obj_grafik[g].x_length) , (m_obj_grafik[g].posisi.y + y_off1 + (m_obj_grafik[g].y_length - ((m_obj_grafik[g].alarm_max_max - m_obj_grafik[g].range_min) / m_obj_grafik[g].konversi_value))));
		cr->stroke();
		
		cr->restore();
		//gambar data
		cr->set_line_width(2);
		cr->set_source_rgba(0, 0, 255, 100);
		cr->move_to((m_obj_grafik[g].posisi.x + x_off1 + m_obj_grafik[g].x_length), (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length - ((m_obj_grafik[g].value[0] - m_obj_grafik[g].range_min) / m_obj_grafik[g].konversi_value)));
		cr->set_line_join(Cairo::LINE_JOIN_ROUND);
		for(int i = 1; i < 61; i++)
		{
			cr->line_to((m_obj_grafik[g].posisi.x + x_off1 + m_obj_grafik[g].x_length - (m_obj_grafik[g].x_length * i/60)), (m_obj_grafik[g].posisi.y + y_off1 + m_obj_grafik[g].y_length - ((m_obj_grafik[g].value[i] - m_obj_grafik[g].range_min) / m_obj_grafik[g].konversi_value)));
		}
			
		cr->stroke();
		
		// gambar batas alarm
				
	}
}

bool VisualMonita::on_drawingarea_preview_expose_event(GdkEventExpose* event)
{
    if(isOpened == true)
    {
        if(m_preview_area)
        {
            cr = m_preview_area->get_window()->create_cairo_context();

            cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
            cr->clip();
            cr->scale(1, 1);

            view_garis();
            view_kotak();
            view_gambar();
            view_gambar_aktif();
            view_animasi();
            view_teks();
            view_teks_aktif();
            view_gauge_bar();
            view_gauge_dial();
            view_grafik();
            
            image = Gdk::Pixbuf::create_from_file("png/logo20px.png");
		    Cairo::Format format = Cairo::FORMAT_RGB24;
		    if (image->get_has_alpha())
		    {
		        format = Cairo::FORMAT_ARGB32;
		    }

		    //image_surface = Cairo::ImageSurface::create(format, 930, 625);
		    Gdk::Cairo::set_source_pixbuf (cr, image, (event->area.width - 265), (event->area.height - 20));

		    cr->paint();
        }
    }
    return true;
}

bool VisualMonita::on_drawingarea_preview_motion_notify_event(GdkEventMotion *event)
{
    if(isOpened == true)
    {
        x_mouse = event->x;
        y_mouse = event->y;
	
        for(int k = 0; k < jumlah_kotak; k++)
        {
            if((event->x >= m_obj_kotak[k].posisi.x) & (event->x <= (m_obj_kotak[k].posisi.x + m_obj_kotak[k].width)) & (event->y > m_obj_kotak[k].posisi.y) & (event->y < (m_obj_kotak[k].posisi.y + m_obj_kotak[k].height)))
            {
                m_obj_kotak[k].current_event = EVENT_CURSOR_INSIDE;
            }
            else
            {
                m_obj_kotak[k].current_event = NO_EVENT;
            }
        }

        for(int k = 0; k < jumlah_kotak; k++)
        {
            if(m_obj_kotak[k].current_event == EVENT_CURSOR_INSIDE)
            {
                m_preview_area->get_window()->set_cursor(Gdk::Cursor(Gdk::HAND2));
                break;
            }
            else
            {
                m_preview_area->get_window()->set_cursor(Gdk::Cursor(Gdk::ARROW));
            }
        }
    }
}

bool VisualMonita::on_drawingarea_preview_button_release_event(GdkEventButton *event)
{
    if(isOpened == true)
    {
        for(int k=0; k < jumlah_kotak; k++)
        {
            if(m_obj_kotak[k].current_event == EVENT_CURSOR_INSIDE)
            {
                printf("Kotak#%d->link: %s\n", k, m_obj_kotak[k].link);
                currentpath[strlen(currentpath)+1] = NULL;
                sprintf(filename, "%s/%s", currentpath, m_obj_kotak[k].link);
                delete_objects();
                load_objects();
                if(doc != NULL)
    				xmlFreeDoc(doc);
                m_preview_area->queue_draw();
            }
        }
        m_preview_area->get_window()->set_cursor(Gdk::Cursor(Gdk::ARROW));
    }
}


bool VisualMonita::timer_callback()
{
    time_counter++;
    if(time_counter%5 == 0)
    {
        if(anima_state)
            anima_state=0;
        else
            anima_state=1;

        update_gambar=true;
    }

    if(update_gambar)
    {
        m_preview_area->queue_draw();
        update_gambar=false;
    }
    
    return true;
}

bool VisualMonita::timer_data_update()
{	
    float fTemp;

    
    if(isOpened == true)
    {
    	//obj_teks_aktif
        for(int i=0; i < jumlah_teks_aktif; i++)
        {
            sprintf(MYQ.Text, "%s%i%s", "SELECT * FROM data_harian WHERE id_titik_ukur=" , m_obj_teks_aktif[i].id_titik_data, " order by id_waktu DESC");

            if(MYQ.Query())
            {
                printf("error saat query\n");
                Gtk::MessageDialog dialog(*this, "Gagal melakukan query ke database");
                dialog.set_secondary_text("Periksa parameter koneksi database");

                dialog.run();
                return false;
            }

            if(MYQ.jum_row>=1)
            {
                sTemp = MYQ.ambil_string ("data_tunggal");
                fTemp = atof(sTemp.c_str());
                m_obj_teks_aktif[i].data = fTemp;
            }
            else
            {
                printf("		|-- Data Not available\n");
                printf("			> Query: %s\n			>jumlah_row: %d\n",usQuery.c_str(),MYQ.jum_row);
                strcpy(m_obj_teks_aktif[i].teks,"N/A");
            }
            MYQ.Buang();
        }
        
        //obj_gauge_dial
        for(int i=0; i < jumlah_gauge_dial; i++)
        {
            sprintf(MYQ.Text, "%s%i%s", "SELECT * FROM data_harian WHERE id_titik_ukur=" , m_obj_gauge_dial[i].id_titik_data, " order by id_waktu DESC");

            if(MYQ.Query())
            {
                printf("error saat query\n");
                Gtk::MessageDialog dialog(*this, "Gagal melakukan query ke database");
                dialog.set_secondary_text("Periksa parameter koneksi database");

                dialog.run();
                return false;
            }

            if(MYQ.jum_row>=1)
            {
                sTemp = MYQ.ambil_string ("data_tunggal");
                fTemp = atof(sTemp.c_str());
                m_obj_gauge_dial[i].data = fTemp;
            }
            else
            {
            	m_obj_gauge_dial[i].data = 0;
                printf("		|-- Data Not available\n");
                printf("			> Query: %s\n			>jumlah_row: %d\n",usQuery.c_str(),MYQ.jum_row);
                strcpy(m_obj_gauge_dial[i].teks,"N/A");
            }
            MYQ.Buang();
        }
    }

    return true;
    
}

bool VisualMonita::timer_grafik_update()
{
	float fTemp;
	
    //obj_grafik
    if(isOpened == true)
    {
        for(int i = 0; i < jumlah_grafik; i++)
        {
        	m_obj_grafik[i].temp[0] = m_obj_grafik[i].value[0];
        	
            sprintf(MYQ.Text, "%s%i%s", "SELECT * FROM data_harian WHERE id_titik_ukur=" , m_obj_grafik[i].id_titik_data, " order by id_waktu DESC");

            if(MYQ.Query())
            {
                printf("error saat query\n");
                Gtk::MessageDialog dialog(*this, "Gagal melakukan query ke database");
                dialog.set_secondary_text("Periksa parameter koneksi database");

                dialog.run();
                return false;
            }
            
            if(MYQ.jum_row >= 1)
            {
                sTemp = MYQ.ambil_string ("data_tunggal");
                fTemp = atof(sTemp.c_str());
                m_obj_grafik[i].value[0] = fTemp;
            }
            else
            {
                printf("Data Grafik : Not available\n");
            }
            MYQ.Buang();

			for(int n = 1; n < 61; n++)
			{			
				m_obj_grafik[i].temp[n] = m_obj_grafik[i].value[n];
				m_obj_grafik[i].value[n] = m_obj_grafik[i].temp[n - 1];
			}
        }
    }
    
    return true;
    
}

int VisualMonita::BacaKonfig()
{
    printf("[BacaKonfig]\n");
    FILE *stream;

    Glib::ustring asFilePath;
    char cTemp[32];

    asFilePath="konfigurasi.kfg";

    if ((stream = fopen(asFilePath.c_str(), "rb"))== NULL)
    {
        fprintf(stderr, "Cannot open input file.\n");
        return 1;
    }

    int h = fread(&kfg, sizeof(konfigurasi_vismon), 1, stream);

    fclose(stream);

    //periksa file konfigurasi
    printf("-db_host:%s\n",kfg.db_host);

    if(kfg.cek_konf != 2857740885UL)
    {
        printf("File konfigurasi rusak. Set ke default\n");

        strcpy(kfg.versi,APP_VERSION);

        //Layout options (v 0.1) 22 Feb.2010
        strcpy(kfg.default_layout,"index.xml");
	
        //database
        strcpy(kfg.db_host,"localhost");
        strcpy(kfg.db_port,"3306");
        strcpy(kfg.db_user,"root");
        strcpy(kfg.db_pass,"tentakel");
        strcpy(kfg.nama_db,"monita_db");

        kfg.cek_konf= 2857740885UL;
        return ERROR_GAGAL_BUKA_FILE_KONFIG;
    } //0xAA55AA55
    else
        printf("OK\n");

    return 0;
}

int VisualMonita::ApplyKonfig()
{
    printf("[ApplyKonfig]\n");
    //Aplikasikan setting database

    //applikasikan konfigurasi ke database
    strcpy(MYQ.server,kfg.db_host);
    strcpy(MYQ.user,kfg.db_user);
    strcpy(MYQ.psw,kfg.db_pass);
    strcpy(MYQ.port,kfg.db_port);
    strcpy(MYQ.db,kfg.nama_db);


    if(!MYQ.init())//return 1 jika sukses
    {
        printf("Gagal koneksi ke database\n");
        //on_button_setting_clicked();
        return ERROR_GAGAL_KONEKSI_DATABASE;
    }
}

int VisualMonita::SimpanKonfig()
{
    printf("[SimpanKonfig]\n");
    FILE *stream;
    Glib::ustring asFilePath;

    printf("Simpan konfigurasi...");
    asFilePath="konfigurasi.kfg";

    if ((stream = fopen(asFilePath.c_str(), "wb"))== NULL)
    {
        fprintf(stderr, "Cannot open output file.\n");
        printf("Gagal membuka file: %s\n", asFilePath.c_str());
        return ERROR_GAGAL_SIMPAN_FILE_KONFIG;
    }

    fwrite(&kfg, sizeof(konfigurasi_vismon), 1, stream);

    fclose(stream);

    //kfg.cek_konf=2857740885UL; //0xAA55AA55

    if(_DEBUG_PRINTF_>=2)printf("OK. Simpan (%d)bytes\n", sizeof(konfigurasi_vismon));
    return 0;
}

void VisualMonita::on_menu_file_setting()
{
    char ctemp[32];
    Glib::ustring usTemp;

    Gtk::Dialog* dlg_db;
    Gtk::Entry*	m_entry_db_host;
    Gtk::Entry*	m_entry_db_port;
    Gtk::Entry*	m_entry_db_username;
    Gtk::Entry*	m_entry_db_password;
    Gtk::Entry*	m_entry_db_name;

    m_refGlade->get_widget("dlg_db", dlg_db);

    m_refGlade->get_widget("entry_host", m_entry_db_host);
    m_refGlade->get_widget("entry_port", m_entry_db_port);
    m_refGlade->get_widget("entry_username", m_entry_db_username);
    m_refGlade->get_widget("entry_password", m_entry_db_password);
    m_refGlade->get_widget("entry_db_name", m_entry_db_name);

    if(BacaKonfig())
    {
        printf("Gagal membuka file konfigurasi. Set ke default\n");

    }

    strcpy(ctemp,kfg.db_host);
    usTemp.assign(ctemp);
    m_entry_db_host->set_text(usTemp);

    //port
    strcpy(ctemp,kfg.db_port);
    usTemp.assign(ctemp);
    m_entry_db_port->set_text(usTemp);

    //username
    strcpy(ctemp,kfg.db_user);
    usTemp.assign(ctemp);
    m_entry_db_username->set_text(usTemp);

    //pass
    strcpy(ctemp,kfg.db_pass);
    usTemp.assign(ctemp);
    m_entry_db_password->set_text(usTemp);
    m_entry_db_password->set_visibility(false);

    //name db
    strcpy(ctemp,kfg.nama_db);
    usTemp.assign(ctemp);
    m_entry_db_name->set_text(usTemp);

    int result = dlg_db->run();

    switch(result)
    {
    case(1):
        {
            usTemp = m_entry_db_host->get_text();
            strcpy(kfg.db_host,usTemp.c_str());

            usTemp = m_entry_db_port->get_text();
            strcpy(kfg.db_port,usTemp.c_str());

            usTemp = m_entry_db_username->get_text();
            strcpy(kfg.db_user,usTemp.c_str());

            usTemp = m_entry_db_password->get_text();
            strcpy(kfg.db_pass,usTemp.c_str());

            usTemp = m_entry_db_name->get_text();
            strcpy(kfg.nama_db,usTemp.c_str());

            dlg_db->hide();
            SimpanKonfig();
            ApplyKonfig();

            break;
        }
    case(2):
        {
            dlg_db->hide();
            break;
        }
    default:
        {
            break;
        }
    }
}

