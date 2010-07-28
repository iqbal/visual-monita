#include "parseXMLmonita.hh"



char* xml_str_trim(char* string)
{
    size_t size;
    char *end;

    size = strlen(string);

    if (!size)
        return string;

    end = string + size - 1;
    while (end >= string && isspace(*end))
        end--;
    *(end + 1) = '\0';

    while (*string && isspace(*string))
        string++;

    return string;
}

int hitung_jml_obj(xmlDocPtr doc, const char* jenis_obj)
{
	int jml_obj = 0;
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if(strcmp((char*)nodeLevel2->name, "object") == 0)
			{
				if(strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), jenis_obj) == 0)
				{
					jml_obj = jml_obj + 1;
				}
			}
    	}
	}
	return jml_obj;
}

int isi_prop_header(xmlDocPtr doc, obj_header *hdr)
{	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{        	
        	for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{					
					if(strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_header") == 0)
					{						
						if(strcmp((const char*)nodeLevel3->name, "property") == 0)
						{
							if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "version") == 0)
							{								
								strcpy(hdr[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
								strcpy(hdr[n].version, xml_str_trim((char*)nodeLevel4->content));
							}
							if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "title") == 0)
							{
								strcpy(hdr[n].title, xml_str_trim((char*)nodeLevel4->content));
							}
							if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "date") == 0)
							{
								strcpy(hdr[n].date, xml_str_trim((char*)nodeLevel4->content));
							}
							if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "author") == 0)
							{
								strcpy(hdr[n].author, xml_str_trim((char*)nodeLevel4->content));
							}
							if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "approval") == 0)
							{
								strcpy(hdr[n].approval, xml_str_trim((char*)nodeLevel4->content));
							}
							if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "note") == 0)
							{
								strcpy(hdr[n].note, xml_str_trim((char*)nodeLevel4->content));
							}
						}
					}
				}
    		}
    	}
	}

	return 0;
}

int isi_prop_body(xmlDocPtr doc, obj_body *bdy)
{	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{ 
        	for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{					
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_body") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{				
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "bg_width") == 0)
						{	
							strcpy(bdy[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							bdy[n].bg_width = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "bg_height") == 0)
						{
							bdy[n].bg_height = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "bg_image") == 0)
						{
							strcpy(bdy[n].bg_image, xml_str_trim((char*)nodeLevel4->content));
						}						
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{
							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_body") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "bg_color") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									bdy[n].bg_color.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									bdy[n].bg_color.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									bdy[n].bg_color.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									bdy[n].bg_color.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
						}						
					}
				}
    		}
    	}
	}

	return 0;
}

int isi_prop_garis(xmlDocPtr doc, obj_garis *m_grs, int jumlah_garis)
{
	obj_garis *grs;
	grs = new obj_garis[jumlah_garis];
	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{		
			for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{		
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_garis") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{						
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "caption") == 0)
						{
							strcpy(grs[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							strcpy(grs[n].caption, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "width") == 0)
						{
							grs[n].width = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "points_count") == 0)
						{
							grs[n].points_count = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_garis") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									grs[n].warna.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									grs[n].warna.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									grs[n].warna.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									grs[n].warna.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_garis") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "titik#1") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									grs[n].titik[0].x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									grs[n].titik[0].y = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_garis") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "titik#2") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									grs[n].titik[1].x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									grs[n].titik[1].y = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_garis") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "titik#3") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									grs[n].titik[2].x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									grs[n].titik[2].y = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_garis") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "titik#4") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									grs[n].titik[3].x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									grs[n].titik[3].y = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_garis") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "titik#5") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									grs[n].titik[4].x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									grs[n].titik[4].y = atoi(xml_str_trim((char*)nodeLevel6->content));
									n = n+1;
								}
							}
						}						
					}
				}
    		}
    	}
	}
	
	for(int j = 0; j < jumlah_garis; j++)
	{
		strcpy(m_grs[j].name, grs[j].name);
		strcpy(m_grs[j].caption, grs[j].caption);
		m_grs[j].points_count = grs[j].points_count;
		m_grs[j].width = grs[j].width;
		
		m_grs[j].warna.r = grs[j].warna.r;
		m_grs[j].warna.g = grs[j].warna.g;
		m_grs[j].warna.b = grs[j].warna.b;
		m_grs[j].warna.a = grs[j].warna.a;
		
		for(int t = 0; t < 5; t++)
		{
			m_grs[j].titik[t].x = grs[j].titik[t].x;
			m_grs[j].titik[t].y = grs[j].titik[t].y;
		}
	}
	
	delete [] grs;
	
	return 0;
}

int isi_prop_kotak(xmlDocPtr doc, obj_kotak *m_ktk, int jumlah_kotak)
{
	obj_kotak *ktk;
	ktk = new obj_kotak[jumlah_kotak];
	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{		
			for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{		
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_kotak") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{						
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "caption") == 0)
						{
							strcpy(ktk[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							strcpy(ktk[n].caption, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "width") == 0)
						{
							ktk[n].width = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "height") == 0)
						{
							ktk[n].height = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "link") == 0)
						{
							strcpy(ktk[n].link, xml_str_trim((char*)nodeLevel4->content));
						}
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_kotak") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "posisi") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									ktk[n].posisi.x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									ktk[n].posisi.y = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_kotak") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_outline") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									ktk[n].warna_outline.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									ktk[n].warna_outline.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									ktk[n].warna_outline.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									ktk[n].warna_outline.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_kotak") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_fill") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									ktk[n].warna_fill.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									ktk[n].warna_fill.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									ktk[n].warna_fill.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									ktk[n].warna_fill.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_kotak") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_hover") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									ktk[n].warna_hover.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									ktk[n].warna_hover.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									ktk[n].warna_hover.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									ktk[n].warna_hover.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_kotak") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_select") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									ktk[n].warna_select.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									ktk[n].warna_select.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									ktk[n].warna_select.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									ktk[n].warna_select.a = atof(xml_str_trim((char*)nodeLevel6->content));
									n = n+1;
								}
							}
						}						
					}
				}
    		}
    	}
	}
	
	for(int j = 0; j < jumlah_kotak; j++)
	{
		strcpy(m_ktk[j].name, ktk[j].name);
		strcpy(m_ktk[j].caption, ktk[j].caption);
		m_ktk[j].width = ktk[j].width;
		m_ktk[j].height = ktk[j].height;
		strcpy(m_ktk[j].link, ktk[j].link);
		strcpy(m_ktk[j].link_caption, ktk[j].link_caption);
		m_ktk[j].show_caption = ktk[j].show_caption;
		m_ktk[j].text_size = ktk[j].text_size;
		
		m_ktk[j].posisi.x = ktk[j].posisi.x;
		m_ktk[j].posisi.y = ktk[j].posisi.y;
		
		m_ktk[j].warna_outline.r = ktk[j].warna_outline.r;
		m_ktk[j].warna_outline.g = ktk[j].warna_outline.g;
		m_ktk[j].warna_outline.b = ktk[j].warna_outline.b;
		m_ktk[j].warna_outline.a = ktk[j].warna_outline.a;
		
		m_ktk[j].warna_fill.r = ktk[j].warna_fill.r;
		m_ktk[j].warna_fill.g = ktk[j].warna_fill.g;
		m_ktk[j].warna_fill.b = ktk[j].warna_fill.b;
		m_ktk[j].warna_fill.a = ktk[j].warna_fill.a;
		
		m_ktk[j].warna_hover.r = ktk[j].warna_hover.r;
		m_ktk[j].warna_hover.g = ktk[j].warna_hover.g;
		m_ktk[j].warna_hover.b = ktk[j].warna_hover.b;
		m_ktk[j].warna_hover.a = ktk[j].warna_hover.a;
		
		m_ktk[j].warna_select.r = ktk[j].warna_select.r;
		m_ktk[j].warna_select.g = ktk[j].warna_select.g;
		m_ktk[j].warna_select.b = ktk[j].warna_select.b;
		m_ktk[j].warna_select.a = ktk[j].warna_select.a;
	}
	
	delete [] ktk;
	
	return 0;
}

int isi_prop_gambar(xmlDocPtr doc, obj_gambar *m_gbr, int jumlah_gambar)
{
	obj_gambar *gbr;
	gbr = new obj_gambar[jumlah_gambar];
	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{		
			for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{		
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gambar") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{						
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "caption") == 0)
						{
							strcpy(gbr[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							strcpy(gbr[n].caption, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "source") == 0)
						{
							strcpy(gbr[n].source, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "width") == 0)
						{
							gbr[n].width = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "height") == 0)
						{
							gbr[n].height = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "skala") == 0)
						{
							gbr[n].skala = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "keterangan") == 0)
						{
							strcpy(gbr[n].keterangan, xml_str_trim((char*)nodeLevel4->content));
						}
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gambar") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "posisi") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									gbr[n].posisi.x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									gbr[n].posisi.y = atoi(xml_str_trim((char*)nodeLevel6->content));
									n = n+1;
								}
							}
						}						
					}
				}
    		}
    	}
	}
	
	for(int j = 0; j < jumlah_gambar; j++)
	{	
		strcpy(m_gbr[j].name, gbr[j].name);
		strcpy(m_gbr[j].caption, gbr[j].caption);
		strcpy(m_gbr[j].source, gbr[j].source);
		m_gbr[j].width = gbr[j].width;
		m_gbr[j].height = gbr[j].height;
		m_gbr[j].skala = gbr[j].skala;
		strcpy(m_gbr[j].keterangan, gbr[j].keterangan);
		
		m_gbr[j].posisi.x = gbr[j].posisi.x;
		m_gbr[j].posisi.y = gbr[j].posisi.y;
	}
	
	delete [] gbr;
	return 0;
}

int isi_prop_animasi(xmlDocPtr doc, obj_animasi *m_anm, int jumlah_animasi)
{
	obj_animasi * anm;
	anm = new obj_animasi[jumlah_animasi];
	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	xmlNodePtr nodeLevel7;
	xmlNodePtr nodeLevel8;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{			
        	for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{		
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_animasi") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{					
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "caption") == 0)
						{
							strcpy(anm[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							strcpy(anm[n].caption, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "frame_counts") == 0)
						{
							anm[n].frame_counts = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "frame_rate") == 0)
						{
							anm[n].frame_rate = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{
							if(strcmp((const char*)nodeLevel5->name, "property") == 0)
							{
								if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_animasi") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "gambar1") == 0))
								{
										if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "caption") == 0)
										{
											strcpy(anm[n].gambar[0].caption, xml_str_trim((char*)nodeLevel6->content));
										}
										if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "source") == 0)
										{
											strcpy(anm[n].gambar[0].source, xml_str_trim((char*)nodeLevel6->content));
										}
										if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "width") == 0)
										{
											anm[n].gambar[0].width = atoi(xml_str_trim((char*)nodeLevel6->content));
										}
										if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "height") == 0)
										{
											anm[n].gambar[0].height = atoi(xml_str_trim((char*)nodeLevel6->content));
										}
										if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "skala") == 0)
										{
											anm[n].gambar[0].skala = atof(xml_str_trim((char*)nodeLevel6->content));
										}
										if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "keterangan") == 0)
										{
											strcpy(anm[n].gambar[0].keterangan, xml_str_trim((char*)nodeLevel6->content));
										}
								}
								
								if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_animasi") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "gambar2") == 0))
								{
									if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "caption") == 0)
									{
										strcpy(anm[n].gambar[1].caption, xml_str_trim((char*)nodeLevel6->content));								
									}
									if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "source") == 0)
									{
										strcpy(anm[n].gambar[1].source, xml_str_trim((char*)nodeLevel6->content));
									}
									if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "width") == 0)
									{
										anm[n].gambar[1].width = atoi(xml_str_trim((char*)nodeLevel6->content));
									}
									if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "height") == 0)
									{
										anm[n].gambar[1].height = atoi(xml_str_trim((char*)nodeLevel6->content));
									}
									if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "skala") == 0)
									{
										anm[n].gambar[1].skala = atof(xml_str_trim((char*)nodeLevel6->content));
									}
									if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "keterangan") == 0)
									{
										strcpy(anm[n].gambar[1].keterangan, xml_str_trim((char*)nodeLevel6->content));
									}
								}
								
							}
							
							for(nodeLevel7 = nodeLevel6->children; nodeLevel7 != NULL; nodeLevel7 = nodeLevel7->next)
							{
								for(nodeLevel8 = nodeLevel7->children; nodeLevel8 != NULL; nodeLevel8 = nodeLevel8->next)
								{									
									if((strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "gambar1") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel6, (const xmlChar*)"id"), "posisi") == 0))
									{
										if(strcmp((const char*)xmlGetProp(nodeLevel7, (const xmlChar*)"name"), "x") == 0)
										{
											anm[n].gambar[0].posisi.x = atoi(xml_str_trim((char*)nodeLevel8->content));
										}
										if(strcmp((const char*)xmlGetProp(nodeLevel7, (const xmlChar*)"name"), "y") == 0)
										{
											anm[n].gambar[0].posisi.y = atoi(xml_str_trim((char*)nodeLevel8->content));
										}
									}
									if((strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "gambar2") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel6, (const xmlChar*)"id"), "posisi") == 0))
									{
										if(strcmp((const char*)xmlGetProp(nodeLevel7, (const xmlChar*)"name"), "x") == 0)
										{
											anm[n].gambar[1].posisi.x = atoi(xml_str_trim((char*)nodeLevel8->content));
										}
										if(strcmp((const char*)xmlGetProp(nodeLevel7, (const xmlChar*)"name"), "y") == 0)
										{
											anm[n].gambar[1].posisi.y = atoi(xml_str_trim((char*)nodeLevel8->content));
											n = n+ 1;
										}
									}
								}
							}
						}						
					}
				}
    		}
    	}
	}
	
	for(int j = 0; j < jumlah_animasi; j++)
	{	
		strcpy(m_anm[j].name, anm[j].name);
		strcpy(m_anm[j].caption, anm[j].caption);
		m_anm[j].frame_counts = anm[j].frame_counts;
		m_anm[j].frame_rate = anm[j].frame_rate;
		
		strcpy(m_anm[j].gambar[0].name, anm[j].gambar[0].name);
		strcpy(m_anm[j].gambar[0].caption, anm[j].gambar[0].caption);
		strcpy(m_anm[j].gambar[0].source, anm[j].gambar[0].source);
		m_anm[j].gambar[0].width = anm[j].gambar[0].width;
		m_anm[j].gambar[0].height = anm[j].gambar[0].height;
		m_anm[j].gambar[0].skala = anm[j].gambar[0].skala;
		strcpy(m_anm[j].gambar[0].keterangan, anm[j].gambar[0].keterangan);
		
		m_anm[j].gambar[0].posisi.x = anm[j].gambar[0].posisi.x;
		m_anm[j].gambar[0].posisi.y = anm[j].gambar[0].posisi.y;
		
		strcpy(m_anm[j].gambar[1].name, anm[j].gambar[1].name);
		strcpy(m_anm[j].gambar[1].caption, anm[j].gambar[1].caption);
		strcpy(m_anm[j].gambar[1].source, anm[j].gambar[1].source);
		m_anm[j].gambar[1].width = anm[j].gambar[1].width;
		m_anm[j].gambar[1].height = anm[j].gambar[1].height;
		m_anm[j].gambar[1].skala = anm[j].gambar[1].skala;
		strcpy(m_anm[j].gambar[1].keterangan, anm[j].gambar[1].keterangan);
		
		m_anm[j].gambar[1].posisi.x = anm[j].gambar[1].posisi.x;
		m_anm[j].gambar[1].posisi.y = anm[j].gambar[1].posisi.y;		
	}
	
	delete [] anm;
	
	return 0;
}

int isi_prop_teks(xmlDocPtr doc, obj_teks *m_teks, int jumlah_teks)
{
	obj_teks *teks;
	teks = new obj_teks[jumlah_teks];
	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{		
			for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{		
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{						
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "isi_teks") == 0)
						{
							strcpy(teks[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							strcpy(teks[n].isi_teks, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "font") == 0)
						{
							strcpy(teks[n].font, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "size") == 0)
						{
							teks[n].size = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "posisi") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									teks[n].posisi.x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									teks[n].posisi.y = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_fill") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									teks[n].warna_fill.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									teks[n].warna_fill.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									teks[n].warna_fill.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									teks[n].warna_fill.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_outline") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									teks[n].warna_outline.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									teks[n].warna_outline.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									teks[n].warna_outline.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									teks[n].warna_outline.a = atof(xml_str_trim((char*)nodeLevel6->content));
									n = n + 1;
								}
							}
						}						
					}
				}
    		}
    	}
	}
	
	for(int j = 0; j < jumlah_teks; j++)
	{	 
		strcpy(m_teks[j].name, teks[j].name);
		strcpy(m_teks[j].font, teks[j].font);
		strcpy(m_teks[j].isi_teks, teks[j].isi_teks);
		m_teks[j].size = teks[j].size;
		
		m_teks[j].posisi.x = teks[j].posisi.x;
		m_teks[j].posisi.y = teks[j].posisi.y;
		
		m_teks[j].warna_fill.r = teks[j].warna_fill.r;
		m_teks[j].warna_fill.g = teks[j].warna_fill.g;
		m_teks[j].warna_fill.b = teks[j].warna_fill.b;
		m_teks[j].warna_fill.a = teks[j].warna_fill.a;
		
		m_teks[j].warna_outline.r = teks[j].warna_outline.r;
		m_teks[j].warna_outline.g = teks[j].warna_outline.g;
		m_teks[j].warna_outline.b = teks[j].warna_outline.b;
		m_teks[j].warna_outline.a = teks[j].warna_outline.a;		
	}
	
	delete [] teks;
	
	return 0;
}

int isi_prop_teks_aktif(xmlDocPtr doc, obj_teks_aktif *m_teks_aktif, int jumlah_teks_aktif)
{
	obj_teks_aktif *teks_aktif;
	teks_aktif = new obj_teks_aktif[jumlah_teks_aktif];
	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{		
			for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{		
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{						
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "id_titik_data") == 0)
						{
							strcpy(teks_aktif[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							teks_aktif[n].id_titik_data = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "caption") == 0)
						{
							strcpy(teks_aktif[n].caption, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "size") == 0)
						{
							teks_aktif[n].size = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "range_min") == 0)
						{
							teks_aktif[n].range_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "range_max") == 0)
						{
							teks_aktif[n].range_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min") == 0)
						{
							teks_aktif[n].alarm_min_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min") == 0)
						{
							teks_aktif[n].alarm_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max") == 0)
						{
							teks_aktif[n].alarm_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_max") == 0)
						{
							teks_aktif[n].alarm_max_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min_enable") == 0)
						{
							teks_aktif[n].alarm_min_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_enable") == 0)
						{
							teks_aktif[n].alarm_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_enable") == 0)
						{
							teks_aktif[n].alarm_max_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_max_enable") == 0)
						{
							teks_aktif[n].alarm_max_max_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "posisi") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									teks_aktif[n].posisi.x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									teks_aktif[n].posisi.y = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_fill") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									teks_aktif[n].warna_fill.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									teks_aktif[n].warna_fill.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									teks_aktif[n].warna_fill.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									teks_aktif[n].warna_fill.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_outline") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									teks_aktif[n].warna_outline.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									teks_aktif[n].warna_outline.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									teks_aktif[n].warna_outline.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									teks_aktif[n].warna_outline.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									teks_aktif[n].warna.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									teks_aktif[n].warna.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									teks_aktif[n].warna.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									teks_aktif[n].warna.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_min_min") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									teks_aktif[n].warna_min_min.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									teks_aktif[n].warna_min_min.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									teks_aktif[n].warna_min_min.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									teks_aktif[n].warna_min_min.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_min") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									teks_aktif[n].warna_min.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									teks_aktif[n].warna_min.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									teks_aktif[n].warna_min.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									teks_aktif[n].warna_min.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_max") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									teks_aktif[n].warna_max.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									teks_aktif[n].warna_max.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									teks_aktif[n].warna_max.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									teks_aktif[n].warna_max.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_max_max") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									teks_aktif[n].warna_max_max.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									teks_aktif[n].warna_max_max.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									teks_aktif[n].warna_max_max.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									teks_aktif[n].warna_max_max.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_oor") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									teks_aktif[n].warna_oor.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									teks_aktif[n].warna_oor.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									teks_aktif[n].warna_oor.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									teks_aktif[n].warna_oor.a = atof(xml_str_trim((char*)nodeLevel6->content));
									n = n + 1;
								}
							}
						}						
					}
				}
    		}
    	}
	}
	
	for(int j = 0; j < jumlah_teks_aktif; j++)
	{		
		strcpy(m_teks_aktif[j].name, teks_aktif[j].name);
		m_teks_aktif[j].id_titik_data = teks_aktif[j].id_titik_data;
		strcpy(m_teks_aktif[j].caption, teks_aktif[j].caption);
		
		m_teks_aktif[j].posisi.x = teks_aktif[j].posisi.x;
		m_teks_aktif[j].posisi.y = teks_aktif[j].posisi.y;
		
		m_teks_aktif[j].warna_fill.r = teks_aktif[j].warna_fill.r;
		m_teks_aktif[j].warna_fill.g = teks_aktif[j].warna_fill.g;
		m_teks_aktif[j].warna_fill.b = teks_aktif[j].warna_fill.b;
		m_teks_aktif[j].warna_fill.a = teks_aktif[j].warna_fill.a;
		
		m_teks_aktif[j].warna_outline.r = teks_aktif[j].warna_outline.r;
		m_teks_aktif[j].warna_outline.g = teks_aktif[j].warna_outline.g;
		m_teks_aktif[j].warna_outline.b = teks_aktif[j].warna_outline.b;
		m_teks_aktif[j].warna_outline.a = teks_aktif[j].warna_outline.a;
		
		m_teks_aktif[j].size = teks_aktif[j].size;	
		
		m_teks_aktif[j].range_min = teks_aktif[j].range_min;
		m_teks_aktif[j].range_max = teks_aktif[j].range_max;
		
		m_teks_aktif[j].alarm_min_min = teks_aktif[j].alarm_min_min;
		m_teks_aktif[j].alarm_min = teks_aktif[j].alarm_min;
		m_teks_aktif[j].alarm_max = teks_aktif[j].alarm_max;
		m_teks_aktif[j].alarm_max_max = teks_aktif[j].alarm_max_max;
		
		m_teks_aktif[j].alarm_min_min_enable = teks_aktif[j].alarm_min_min_enable;
		m_teks_aktif[j].alarm_min_enable = teks_aktif[j].alarm_min_enable;
		m_teks_aktif[j].alarm_max_enable = teks_aktif[j].alarm_max_enable;
		m_teks_aktif[j].alarm_max_max_enable = teks_aktif[j].alarm_max_max_enable;
		
		m_teks_aktif[j].warna_min_min.r = teks_aktif[j].warna_min_min.r;
		m_teks_aktif[j].warna_min_min.g = teks_aktif[j].warna_min_min.g;
		m_teks_aktif[j].warna_min_min.b = teks_aktif[j].warna_min_min.b;
		m_teks_aktif[j].warna_min_min.a = teks_aktif[j].warna_min_min.a;
		
		m_teks_aktif[j].warna_min.r = teks_aktif[j].warna_min.r;
		m_teks_aktif[j].warna_min.g = teks_aktif[j].warna_min.g;
		m_teks_aktif[j].warna_min.b = teks_aktif[j].warna_min.b;
		m_teks_aktif[j].warna_min.a = teks_aktif[j].warna_min.a;		
		
		m_teks_aktif[j].warna_max.r = teks_aktif[j].warna_max.r;
		m_teks_aktif[j].warna_max.g = teks_aktif[j].warna_max.g;
		m_teks_aktif[j].warna_max.b = teks_aktif[j].warna_max.b;
		m_teks_aktif[j].warna_max.a = teks_aktif[j].warna_max.a;
		
		m_teks_aktif[j].warna_max_max.r = teks_aktif[j].warna_max_max.r;
		m_teks_aktif[j].warna_max_max.g = teks_aktif[j].warna_max_max.g;
		m_teks_aktif[j].warna_max_max.b = teks_aktif[j].warna_max_max.b;
		m_teks_aktif[j].warna_max_max.a = teks_aktif[j].warna_max_max.a;
		
		m_teks_aktif[j].warna_oor.r = teks_aktif[j].warna_oor.r;
		m_teks_aktif[j].warna_oor.g = teks_aktif[j].warna_oor.g;
		m_teks_aktif[j].warna_oor.b = teks_aktif[j].warna_oor.b;
		m_teks_aktif[j].warna_oor.a = teks_aktif[j].warna_oor.a;
	}
	
	delete [] teks_aktif;
	
	return 0;
}

int isi_prop_gauge_bar(xmlDocPtr doc, obj_gauge_bar *m_gauge_bar, int jumlah_gauge_bar)
{
	obj_gauge_bar *gauge_bar;
	gauge_bar = new obj_gauge_bar[jumlah_gauge_bar];
	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{		
			for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{		
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_bar") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{						
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "id_titik_data") == 0)
						{
							strcpy(gauge_bar[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							gauge_bar[n].id_titik_data = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "caption") == 0)
						{
							strcpy(gauge_bar[n].caption, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "width") == 0)
						{
							gauge_bar[n].width = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "height") == 0)
						{
							gauge_bar[n].height = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "show_text") == 0)
						{
							gauge_bar[n].show_text = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "show_caption") == 0)
						{
							gauge_bar[n].show_caption = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "auto_alarm_range") == 0)
						{
							gauge_bar[n].auto_alarm_range = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "range_min") == 0)
						{
							gauge_bar[n].range_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "range_max") == 0)
						{
							gauge_bar[n].range_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min") == 0)
						{
							gauge_bar[n].alarm_min_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min") == 0)
						{
							gauge_bar[n].alarm_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max") == 0)
						{
							gauge_bar[n].alarm_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_max") == 0)
						{
							gauge_bar[n].alarm_max_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min_enable") == 0)
						{
							gauge_bar[n].alarm_min_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_enable") == 0)
						{
							gauge_bar[n].alarm_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_enable") == 0)
						{
							gauge_bar[n].alarm_max_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_max_enable") == 0)
						{
							gauge_bar[n].alarm_max_max_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_bar") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "posisi") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									gauge_bar[n].posisi.x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									gauge_bar[n].posisi.y = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_bar") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_min_min") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_bar[n].warna_min_min.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_bar[n].warna_min_min.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_bar[n].warna_min_min.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_bar[n].warna_min_min.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_bar") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_min") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_bar[n].warna_min.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_bar[n].warna_min.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_bar[n].warna_min.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_bar[n].warna_min.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_bar") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_max") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_bar[n].warna_max.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_bar[n].warna_max.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_bar[n].warna_max.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_bar[n].warna_max.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_bar") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_max_max") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_bar[n].warna_max_max.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_bar[n].warna_max_max.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_bar[n].warna_max_max.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_bar[n].warna_max_max.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_bar") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_oor") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_bar[n].warna_oor.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_bar[n].warna_oor.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_bar[n].warna_oor.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_bar[n].warna_oor.a = atof(xml_str_trim((char*)nodeLevel6->content));
									n = n + 1;
								}
							}
						}						
					}
				}
    		}
    	}
	}
	
	for(int j = 0; j < jumlah_gauge_bar; j++)
	{
		strcpy(m_gauge_bar[j].name, gauge_bar[j].name);
		m_gauge_bar[j].id_titik_data = gauge_bar[j].id_titik_data;
		strcpy(m_gauge_bar[j].caption, gauge_bar[j].caption);
		
		m_gauge_bar[j].posisi.x = gauge_bar[j].posisi.x;
		m_gauge_bar[j].posisi.y = gauge_bar[j].posisi.y;
		
		m_gauge_bar[j].width = gauge_bar[j].width;
		m_gauge_bar[j].height = gauge_bar[j].height;		
		
		m_gauge_bar[j].show_text = gauge_bar[j].show_text;
		m_gauge_bar[j].show_caption = gauge_bar[j].show_caption;	
		m_gauge_bar[j].auto_alarm_range = gauge_bar[j].auto_alarm_range;
		
		m_gauge_bar[j].range_min = gauge_bar[j].range_min;
		m_gauge_bar[j].range_max = gauge_bar[j].range_max;
		
		m_gauge_bar[j].alarm_min_min = gauge_bar[j].alarm_min_min;
		m_gauge_bar[j].alarm_min = gauge_bar[j].alarm_min;
		m_gauge_bar[j].alarm_max = gauge_bar[j].alarm_max;
		m_gauge_bar[j].alarm_max_max = gauge_bar[j].alarm_max_max;
		
		m_gauge_bar[j].alarm_min_min_enable = gauge_bar[j].alarm_min_min_enable;
		m_gauge_bar[j].alarm_min_enable = gauge_bar[j].alarm_min_enable;
		m_gauge_bar[j].alarm_max_enable = gauge_bar[j].alarm_max_enable;
		m_gauge_bar[j].alarm_max_max_enable = gauge_bar[j].alarm_max_max_enable;
		
		m_gauge_bar[j].warna_min_min.r = gauge_bar[j].warna_min_min.r;
		m_gauge_bar[j].warna_min_min.g = gauge_bar[j].warna_min_min.g;
		m_gauge_bar[j].warna_min_min.b = gauge_bar[j].warna_min_min.b;
		m_gauge_bar[j].warna_min_min.a = gauge_bar[j].warna_min_min.a;
		
		m_gauge_bar[j].warna_min.r = gauge_bar[j].warna_min.r;
		m_gauge_bar[j].warna_min.g = gauge_bar[j].warna_min.g;
		m_gauge_bar[j].warna_min.b = gauge_bar[j].warna_min.b;
		m_gauge_bar[j].warna_min.a = gauge_bar[j].warna_min.a;		
		
		m_gauge_bar[j].warna_max.r = gauge_bar[j].warna_max.r;
		m_gauge_bar[j].warna_max.g = gauge_bar[j].warna_max.g;
		m_gauge_bar[j].warna_max.b = gauge_bar[j].warna_max.b;
		m_gauge_bar[j].warna_max.a = gauge_bar[j].warna_max.a;
		
		m_gauge_bar[j].warna_max_max.r = gauge_bar[j].warna_max_max.r;
		m_gauge_bar[j].warna_max_max.g = gauge_bar[j].warna_max_max.g;
		m_gauge_bar[j].warna_max_max.b = gauge_bar[j].warna_max_max.b;
		m_gauge_bar[j].warna_max_max.a = gauge_bar[j].warna_max_max.a;
		
		m_gauge_bar[j].warna_oor.r = gauge_bar[j].warna_oor.r;
		m_gauge_bar[j].warna_oor.g = gauge_bar[j].warna_oor.g;
		m_gauge_bar[j].warna_oor.b = gauge_bar[j].warna_oor.b;
		m_gauge_bar[j].warna_oor.a = gauge_bar[j].warna_oor.a;
	}
	
	delete [] gauge_bar;
	
	return 0;
}

int isi_prop_gauge_dial(xmlDocPtr doc, obj_gauge_dial *m_gauge_dial, int jumlah_gauge_dial)
{
	obj_gauge_dial *gauge_dial;
	gauge_dial = new obj_gauge_dial[jumlah_gauge_dial];
	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{		
			for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{		
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_dial") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{						
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "id_titik_data") == 0)
						{
							strcpy(gauge_dial[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							gauge_dial[n].id_titik_data = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "caption") == 0)
						{
							strcpy(gauge_dial[n].caption, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "width") == 0)
						{
							gauge_dial[n].width = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "height") == 0)
						{
							gauge_dial[n].height = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "show_text") == 0)
						{
							gauge_dial[n].show_text = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "show_caption") == 0)
						{
							gauge_dial[n].show_caption = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "auto_alarm_range") == 0)
						{
							gauge_dial[n].auto_alarm_range = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "range_min") == 0)
						{
							gauge_dial[n].range_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "range_max") == 0)
						{
							gauge_dial[n].range_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min") == 0)
						{
							gauge_dial[n].alarm_min_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min") == 0)
						{
							gauge_dial[n].alarm_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max") == 0)
						{
							gauge_dial[n].alarm_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_max") == 0)
						{
							gauge_dial[n].alarm_max_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min_enable") == 0)
						{
							gauge_dial[n].alarm_min_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_enable") == 0)
						{
							gauge_dial[n].alarm_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_enable") == 0)
						{
							gauge_dial[n].alarm_max_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_max_enable") == 0)
						{
							gauge_dial[n].alarm_max_max_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_dial") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "posisi") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									gauge_dial[n].posisi.x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									gauge_dial[n].posisi.y = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
							}							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_dial") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_dial[n].warna.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_dial[n].warna.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_dial[n].warna.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_dial[n].warna.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_dial") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_min_min") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_dial[n].warna_min_min.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_dial[n].warna_min_min.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_dial[n].warna_min_min.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_dial[n].warna_min_min.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_dial") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_min") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_dial[n].warna_min.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_dial[n].warna_min.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_dial[n].warna_min.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_dial[n].warna_min.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_dial") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_max") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_dial[n].warna_max.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_dial[n].warna_max.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_dial[n].warna_max.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_dial[n].warna_max.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_dial") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_max_max") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_dial[n].warna_max_max.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_dial[n].warna_max_max.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_dial[n].warna_max_max.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_dial[n].warna_max_max.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_dial") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_oor") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									gauge_dial[n].warna_oor.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									gauge_dial[n].warna_oor.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									gauge_dial[n].warna_oor.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									gauge_dial[n].warna_oor.a = atof(xml_str_trim((char*)nodeLevel6->content));
									n = n + 1;
								}
							}
						}						
					}
				}
    		}
    	}
	}
	
	for(int j = 0; j < jumlah_gauge_dial; j++)
	{
		strcpy(m_gauge_dial[j].name, gauge_dial[j].name);
		m_gauge_dial[j].id_titik_data = gauge_dial[j].id_titik_data;
		strcpy(m_gauge_dial[j].caption, gauge_dial[j].caption);
		
		m_gauge_dial[j].posisi.x = gauge_dial[j].posisi.x;
		m_gauge_dial[j].posisi.y = gauge_dial[j].posisi.y;
		
		m_gauge_dial[j].width = gauge_dial[j].width;
		m_gauge_dial[j].height = gauge_dial[j].height;		
		
		m_gauge_dial[j].show_text = gauge_dial[j].show_text;
		m_gauge_dial[j].show_caption = gauge_dial[j].show_caption;	
		m_gauge_dial[j].auto_alarm_range = gauge_dial[j].auto_alarm_range;
		
		m_gauge_dial[j].range_min = gauge_dial[j].range_min;
		m_gauge_dial[j].range_max = gauge_dial[j].range_max;
		
		m_gauge_dial[j].alarm_min_min = gauge_dial[j].alarm_min_min;
		m_gauge_dial[j].alarm_min = gauge_dial[j].alarm_min;
		m_gauge_dial[j].alarm_max = gauge_dial[j].alarm_max;
		m_gauge_dial[j].alarm_max_max = gauge_dial[j].alarm_max_max;
		
		m_gauge_dial[j].alarm_min_min_enable = gauge_dial[j].alarm_min_min_enable;
		m_gauge_dial[j].alarm_min_enable = gauge_dial[j].alarm_min_enable;
		m_gauge_dial[j].alarm_max_enable = gauge_dial[j].alarm_max_enable;
		m_gauge_dial[j].alarm_max_max_enable = gauge_dial[j].alarm_max_max_enable;
		
		m_gauge_dial[j].warna_min_min.r = gauge_dial[j].warna_min_min.r;
		m_gauge_dial[j].warna_min_min.g = gauge_dial[j].warna_min_min.g;
		m_gauge_dial[j].warna_min_min.b = gauge_dial[j].warna_min_min.b;
		m_gauge_dial[j].warna_min_min.a = gauge_dial[j].warna_min_min.a;
		
		m_gauge_dial[j].warna_min.r = gauge_dial[j].warna_min.r;
		m_gauge_dial[j].warna_min.g = gauge_dial[j].warna_min.g;
		m_gauge_dial[j].warna_min.b = gauge_dial[j].warna_min.b;
		m_gauge_dial[j].warna_min.a = gauge_dial[j].warna_min.a;		
		
		m_gauge_dial[j].warna_max.r = gauge_dial[j].warna_max.r;
		m_gauge_dial[j].warna_max.g = gauge_dial[j].warna_max.g;
		m_gauge_dial[j].warna_max.b = gauge_dial[j].warna_max.b;
		m_gauge_dial[j].warna_max.a = gauge_dial[j].warna_max.a;
		
		m_gauge_dial[j].warna_max_max.r = gauge_dial[j].warna_max_max.r;
		m_gauge_dial[j].warna_max_max.g = gauge_dial[j].warna_max_max.g;
		m_gauge_dial[j].warna_max_max.b = gauge_dial[j].warna_max_max.b;
		m_gauge_dial[j].warna_max_max.a = gauge_dial[j].warna_max_max.a;
		
		m_gauge_dial[j].warna_oor.r = gauge_dial[j].warna_oor.r;
		m_gauge_dial[j].warna_oor.g = gauge_dial[j].warna_oor.g;
		m_gauge_dial[j].warna_oor.b = gauge_dial[j].warna_oor.b;
		m_gauge_dial[j].warna_oor.a = gauge_dial[j].warna_oor.a;
	}
	
	delete [] gauge_dial;
	
	return 0;
}

int isi_prop_gambar_aktif(xmlDocPtr doc, obj_gambar_aktif *m_gambar_aktif, int jumlah_gambar_aktif)
{
	obj_gambar_aktif *gambar_aktif;
	gambar_aktif = new obj_gambar_aktif[jumlah_gambar_aktif];
	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{		
			for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{		
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gambar_aktif") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{						
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "id_titik_data") == 0)
						{
							strcpy(gambar_aktif[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							gambar_aktif[n].id_titik_data = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "caption") == 0)
						{
							strcpy(gambar_aktif[n].caption, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "width") == 0)
						{
							gambar_aktif[n].width = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "height") == 0)
						{
							gambar_aktif[n].height = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "skala") == 0)
						{
							gambar_aktif[n].skala = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "show_text") == 0)
						{
							gambar_aktif[n].show_text = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "show_caption") == 0)
						{
							gambar_aktif[n].show_caption = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "auto_alarm_range") == 0)
						{
							gambar_aktif[n].auto_alarm_range = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "range_min") == 0)
						{
							gambar_aktif[n].range_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "range_max") == 0)
						{
							gambar_aktif[n].range_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min") == 0)
						{
							gambar_aktif[n].alarm_min_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min") == 0)
						{
							gambar_aktif[n].alarm_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max") == 0)
						{
							gambar_aktif[n].alarm_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_max") == 0)
						{
							gambar_aktif[n].alarm_max_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min_enable") == 0)
						{
							gambar_aktif[n].alarm_min_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_enable") == 0)
						{
							gambar_aktif[n].alarm_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_enable") == 0)
						{
							gambar_aktif[n].alarm_max_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_max_enable") == 0)
						{
							gambar_aktif[n].alarm_max_max_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "source") == 0)
						{
							strcpy(gambar_aktif[n].source, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "source_min") == 0)
						{
							strcpy(gambar_aktif[n].source_min, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "source_min_min") == 0)
						{
							strcpy(gambar_aktif[n].source_min_min, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "source_max") == 0)
						{
							strcpy(gambar_aktif[n].source_max, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "source_max_max") == 0)
						{
							strcpy(gambar_aktif[n].source_max_max, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "source_oor") == 0)
						{
							strcpy(gambar_aktif[n].source_oor, xml_str_trim((char*)nodeLevel4->content));
						}
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gambar_aktif") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "posisi") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									gambar_aktif[n].posisi.x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									gambar_aktif[n].posisi.y = atoi(xml_str_trim((char*)nodeLevel6->content));
									n = n + 1;
								}
							}
						}						
					}
				}
    		}
    	}
	}
	
	return 0;
}

int isi_prop_grafik(xmlDocPtr doc, obj_grafik *m_grafik, int jumlah_grafik)
{
	obj_grafik *grafik;
	grafik = new obj_grafik[jumlah_grafik];
	
	xmlNodePtr nodeLevel1;
	xmlNodePtr nodeLevel2;
	xmlNodePtr nodeLevel3;
	xmlNodePtr nodeLevel4;
	xmlNodePtr nodeLevel5;
	xmlNodePtr nodeLevel6;
	int n = 0;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{		
			for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
			{
				for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
				{		
					if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_grafik") == 0) && (strcmp((const char*)nodeLevel3->name, "property") == 0))
					{						
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "id_titik_data") == 0)
						{
							strcpy(grafik[n].name, xml_str_trim((char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")));
							grafik[n].id_titik_data = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "caption") == 0)
						{
							strcpy(grafik[n].caption, xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "width") == 0)
						{
							grafik[n].width = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "height") == 0)
						{
							grafik[n].height = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "show_text") == 0)
						{
							grafik[n].show_text = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "show_caption") == 0)
						{
							grafik[n].show_caption = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "auto_alarm_range") == 0)
						{
							grafik[n].auto_alarm_range = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "range_min") == 0)
						{
							grafik[n].range_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "range_max") == 0)
						{
							grafik[n].range_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min") == 0)
						{
							grafik[n].alarm_min_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min") == 0)
						{
							grafik[n].alarm_min = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max") == 0)
						{
							grafik[n].alarm_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_max") == 0)
						{
							grafik[n].alarm_max_max = atof(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min_enable") == 0)
						{
							grafik[n].alarm_min_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_enable") == 0)
						{
							grafik[n].alarm_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_enable") == 0)
						{
							grafik[n].alarm_max_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_max_max_enable") == 0)
						{
							grafik[n].alarm_max_max_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
					}
					
					for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
					{
						for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
						{							
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_grafik") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "posisi") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "x") == 0)
								{
									grafik[n].posisi.x = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "y") == 0)
								{
									grafik[n].posisi.y = atoi(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_grafik") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_min_min") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									grafik[n].warna_min_min.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									grafik[n].warna_min_min.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									grafik[n].warna_min_min.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									grafik[n].warna_min_min.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_grafik") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_min") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									grafik[n].warna_min.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									grafik[n].warna_min.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									grafik[n].warna_min.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									grafik[n].warna_min.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_grafik") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_max") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									grafik[n].warna_max.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									grafik[n].warna_max.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									grafik[n].warna_max.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									grafik[n].warna_max.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_grafik") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_max_max") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									grafik[n].warna_max_max.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									grafik[n].warna_max_max.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									grafik[n].warna_max_max.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									grafik[n].warna_max_max.a = atof(xml_str_trim((char*)nodeLevel6->content));
								}
							}
							if((strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_grafik") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id"), "warna_oor") == 0))
							{
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "r") == 0)
								{
									grafik[n].warna_oor.r = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "g") == 0)
								{
									grafik[n].warna_oor.g = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "b") == 0)
								{
									grafik[n].warna_oor.b = atof(xml_str_trim((char*)nodeLevel6->content));
								}
								if(strcmp((const char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"), "a") == 0)
								{
									grafik[n].warna_oor.a = atof(xml_str_trim((char*)nodeLevel6->content));
									n = n + 1;
								}
							}
						}						
					}
				}
    		}
    	}
	}
	
	return 0;
}

