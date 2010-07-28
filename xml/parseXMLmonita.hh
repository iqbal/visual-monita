#ifndef __PARSEXMLMONITA_HH
#define __PARSEXMLMONITA_HH

#include <iostream>
#include <string.h>
#include <libxml/xmlreader.h>
#include <cstdlib>
#include <new>
using namespace std;

class obj_header 
{
public:
	char name[32];
	char version[11];
	char title[64];
	char date[15]; //20091026232159
	char author[32];
	char approval[64];
	char note[128];
};

class color_rgba
{
public:
	double r;
	double g;
	double b;
	double a;
};

class obj_titik
{
public:
	int x;
	int y;
};

class obj_body
{
public:
	char name[32];
	int bg_width;
	int bg_height;
	color_rgba bg_color;
	char bg_image[128]; //path ke gambar
};

class visual_object
{
public:
	int jenis; 	//0 - Titik/Lingkaran
			   	//1x - Garis  
			   	//10 - Garis 2 titik
				//11 - Garis 3 titik
				
				//20 - Bidang rectangular
	obj_titik posisi;
	double skala;
	char name[32];
};

class obj_garis
{
public:
	char name[32];
	char caption[64];
	int points_count;		
	int width;
	
	color_rgba warna;
	obj_titik titik[5];
};

class obj_kotak
{
public:
	char name[32];
	char caption[32];
	int width;
	int height;
	char link[64];
	char link_caption[64];
	bool show_caption;
	int text_size;
	
	obj_titik posisi;	
	color_rgba warna_outline; 
	color_rgba warna_fill;
	color_rgba warna_hover;
	color_rgba warna_select;	
	
	//run time
	int current_event;
	int last_event;
};

class obj_teks  //maks 64 karakter
{
public:
	char name[32];
	char font[32];
	char isi_teks[64];
	
	obj_titik posisi;
	color_rgba warna_fill;
	color_rgba warna_outline;

	int size; 
};

class obj_teks_aktif 
{
public:
	char name[32];
	int id_titik_data;
	char caption[32];
	
	obj_titik posisi;
	color_rgba warna_fill;
	color_rgba warna_outline;

	int size; 
	
	//Range dan alarm
	float range_min;
	float range_max;

	float alarm_min;
	float alarm_min_min;
	float alarm_max;
	float alarm_max_max;

	bool alarm_min_enable;
	bool alarm_min_min_enable;
	bool alarm_max_enable;
	bool alarm_max_max_enable;

	//warna
	color_rgba warna; //normal 
	color_rgba warna_min;
	color_rgba warna_min_min;
	color_rgba warna_max;
	color_rgba warna_max_max;
	color_rgba warna_oor;//out of range
	
	//Runtime
	char teks[64];
	float data;
}; 

class obj_gambar 
{
public:
	char name[32];
	char caption[64];
	char source[32];
	int width;
	int height;
	double skala;
	char keterangan[64];
	
	obj_titik posisi;
	
	//run time
	int current_event;
	int last_event;
};

class obj_gambar_aktif
{
public:
	char name[32];
	int id_titik_data;
	char caption[32];

	//Posisi
	obj_titik posisi;
	int width;
	int height;
	float skala;
	
	bool show_text;
	bool show_caption;
	bool auto_alarm_range;  
	
	//Range dan alarm
	float range_min;
	float range_max;

	float alarm_min;
	float alarm_min_min;
	float alarm_max;
	float alarm_max_max;

	bool alarm_min_enable;
	bool alarm_min_min_enable;
	bool alarm_max_enable;
	bool alarm_max_max_enable;

	// warna
	char source[32];  //normal 
	char source_min[32];
	char source_min_min[32];
	char source_max[32];
	char source_max_max[32];
	char source_oor[32]; //out of range
	
	// Runtime
	float data;
	char teks[32];
};

class obj_animasi 
{
public:
	char name[32];
	char caption[64];
	int frame_counts;
	int frame_rate;
	obj_gambar gambar[2];
};

class obj_gauge_bar
{
public:
	char name[32];
	int id_titik_data;
	char caption[32];

	//Posisi
	obj_titik posisi;
	int width;
	int height;

	bool show_text;
	bool show_caption;
	bool auto_alarm_range;  
	
	//Range dan alarm
	float range_min;
	float range_max;

	float alarm_min;
	float alarm_min_min;
	float alarm_max;
	float alarm_max_max;

	bool alarm_min_enable;
	bool alarm_min_min_enable;
	bool alarm_max_enable;
	bool alarm_max_max_enable;

	//warna
	color_rgba warna; //normal 
	color_rgba warna_min;
	color_rgba warna_min_min;
	color_rgba warna_max;
	color_rgba warna_max_max;
	color_rgba warna_oor; //out of range
	
	//Runtime
	float data;
	char teks[32];
};

class obj_gauge_dial
{
public:
	char name[32];
	int id_titik_data;
	char caption[32];

	//Posisi
	obj_titik posisi;
	int width;
	int height;

	bool show_text;
	bool show_caption;
	bool auto_alarm_range;
	
	//Range dan alarm
	float range_min;
	float range_max;

	float alarm_min;
	float alarm_min_min;
	float alarm_max;
	float alarm_max_max;

	bool alarm_min_enable;
	bool alarm_min_min_enable;
	bool alarm_max_enable;
	bool alarm_max_max_enable;

	//warna
	color_rgba warna; //normal
	color_rgba warna_min;
	color_rgba warna_min_min;
	color_rgba warna_max;
	color_rgba warna_max_max;
	color_rgba warna_oor; //out of range
	
	//Runtime
	float data;
	char teks[32];
};

class obj_grafik
{
public:
	char name[32];
	int id_titik_data;
	char caption[32];

	//Posisi
	obj_titik posisi;
	int width;
	int height;

	bool show_text;
	bool show_caption;
	bool auto_alarm_range;
	
	//Range dan alarm
	float range_min;
	float range_max;

	float alarm_min;
	float alarm_min_min;
	float alarm_max;
	float alarm_max_max;

	bool alarm_min_enable;
	bool alarm_min_min_enable;
	bool alarm_max_enable;
	bool alarm_max_max_enable;

	//warna
	color_rgba warna; //normal 
	color_rgba warna_min;
	color_rgba warna_min_min;
	color_rgba warna_max;
	color_rgba warna_max_max;
	color_rgba warna_oor;//out of range
	
	//Runtime
	int y_length; // = height - 10px
	int x_length; // = width - 10px
	float value[61];
	float temp[61];
	float konversi_value; // = (((range_max) - (range_min)) / y_length)
};

char* xml_str_trim(char* string);

int hitung_jml_obj(xmlDocPtr doc, const char* jenis_obj);

int isi_prop_header(xmlDocPtr doc, obj_header *m_hdr);
int isi_prop_body(xmlDocPtr doc, obj_body *m_bdy);
int isi_prop_garis(xmlDocPtr doc, obj_garis *m_grs, int jumlah_garis);
int isi_prop_kotak(xmlDocPtr doc, obj_kotak *m_ktk, int jumlah_kotak);
int isi_prop_gambar(xmlDocPtr doc, obj_gambar *m_gbr, int jumlah_gambar);
int isi_prop_gambar_aktif(xmlDocPtr doc, obj_gambar_aktif *m_gambar_aktif, int jumlah_gambar_aktif);
int isi_prop_animasi(xmlDocPtr doc, obj_animasi *m_anm, int jumlah_animasi);
int isi_prop_teks(xmlDocPtr doc, obj_teks *m_teks, int jumlah_teks);
int isi_prop_teks_aktif(xmlDocPtr doc, obj_teks_aktif *m_teks_aktif, int jumlah_teks_aktif);
int isi_prop_gauge_bar(xmlDocPtr doc, obj_gauge_bar *m_gauge_bar, int jumlah_gauge_bar);
int isi_prop_gauge_dial(xmlDocPtr doc, obj_gauge_dial *m_gauge_dial, int jumlah_gauge_dial);
int isi_prop_grafik(xmlDocPtr doc, obj_grafik *m_grafik, int jumlah_grafik);

#endif //__PARSEXML_HH
