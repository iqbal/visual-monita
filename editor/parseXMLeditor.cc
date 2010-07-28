#include "parseXMLeditor.hh"

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

int isi_prop_header(xmlDocPtr doc, obj_header hdr[])
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

int isi_prop_body(xmlDocPtr doc, obj_body bdy[])
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

int isi_prop_garis(xmlDocPtr doc, obj_garis grs[])
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
	
	return 0;
}

int isi_prop_kotak(xmlDocPtr doc, obj_kotak ktk[])
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
	
	return 0;
}

int isi_prop_gambar(xmlDocPtr doc, obj_gambar gbr[])
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
	
	return 0;
}

int isi_prop_animasi(xmlDocPtr doc, obj_animasi anm[])
{
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
	
	return 0;
}

int isi_prop_teks(xmlDocPtr doc, obj_teks teks[])
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
	
	return 0;
}

int isi_prop_teks_aktif(xmlDocPtr doc, obj_teks_aktif teks_aktif[])
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
						}if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_enable") == 0)
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
	
	return 0;
}

int isi_prop_gauge_bar(xmlDocPtr doc, obj_gauge_bar gauge_bar[])
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
	
	return 0;
}

int isi_prop_gauge_dial(xmlDocPtr doc, obj_gauge_dial gauge_dial[])
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
	
	return 0;
}

int isi_prop_grafik(xmlDocPtr doc, obj_grafik grafik[])
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
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_enable") == 0)
						{
							grafik[n].alarm_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
						}
						if(strcmp((const char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"), "alarm_min_min_enable") == 0)
						{
							grafik[n].alarm_min_min_enable = atoi(xml_str_trim((char*)nodeLevel4->content));
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


int isi_prop_gambar_aktif(xmlDocPtr doc, obj_gambar_aktif gambar_aktif[])
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
