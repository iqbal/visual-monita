#include "editor.hh"
#include "parseXMLeditor.hh"

#define FRAME_RATE 200

EditorWindow::EditorWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Window(cobject), m_refGlade(refGlade)
{
	isOpened = false;
	showGrid = false;
	
	m_refGlade->get_widget("m_ScrolledWindow", m_ScrolledWindow);
	m_refGlade->get_widget("m_ScrolledWindow_prop", m_ScrolledWindow_prop);
	
	m_refGlade->get_widget("m_obj_entry_name", m_obj_entry_name);
	m_refGlade->get_widget("m_obj_entry_jenis", m_obj_entry_jenis);
	
	m_refGlade->get_widget("m_Button_Save", m_Button_Save);
	m_refGlade->get_widget("m_Button_Add", m_Button_Add);
	m_refGlade->get_widget("m_Button_Delete", m_Button_Delete);
	m_refGlade->get_widget("m_Button_Grid", m_Button_Grid);

	m_refGlade->get_widget("menu_new", m_menu_new);
	m_refGlade->get_widget("menu_open", m_menu_open);
	m_refGlade->get_widget("menu_save", m_menu_save);
	m_refGlade->get_widget("menu_quit", m_menu_quit);
	
	m_refGlade->get_widget("m_status_posisi", m_status_posisi);
	
	m_refGlade->get_widget("drawingarea_preview", m_preview_area);	
	
	m_ScrolledWindow->add(m_TreeView);
	m_ScrolledWindow_prop->add(m_TreeView_prop);

	m_menu_new->signal_activate().connect(sigc::mem_fun(*this, &EditorWindow::on_menu_file_new));
	m_menu_open->signal_activate().connect(sigc::mem_fun(*this, &EditorWindow::on_menu_file_open));
	m_menu_save->signal_activate().connect(sigc::mem_fun(*this, &EditorWindow::on_menu_file_save));
	m_menu_quit->signal_activate().connect(sigc::mem_fun(*this, &EditorWindow::on_menu_file_quit));
	m_Button_Save->signal_clicked().connect(sigc::mem_fun(*this, &EditorWindow::on_button_save));
	m_Button_Add->signal_clicked().connect(sigc::mem_fun(*this, &EditorWindow::on_button_add));
	m_Button_Delete->signal_clicked().connect(sigc::mem_fun(*this, &EditorWindow::on_button_delete));
	m_Button_Grid->signal_clicked().connect(sigc::mem_fun(*this, &EditorWindow::on_button_grid));
	m_TreeView.signal_row_activated().connect(sigc::mem_fun(*this, &EditorWindow::on_treeview_row_activated));
	m_preview_area->signal_expose_event().connect(sigc::mem_fun(*this, &EditorWindow::on_drawingarea_preview_expose_event));
	m_preview_area->signal_motion_notify_event().connect(sigc::mem_fun(*this, &EditorWindow::on_drawingarea_preview_motion_notify_event));
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &EditorWindow::timer_callback),FRAME_RATE);
	
	show_all_children();
}

EditorWindow::~EditorWindow()
{
	delete_objects();
	xmlFreeDoc(doc);
}

void EditorWindow::update_title()
{
	char title_baru[128];
	sprintf(title_baru, "%s : %s", "visual monita XML editor", filename);
	this->set_title(title_baru);
}

void EditorWindow::isi_tree_per_jenis(const char* jenis, const char* jenis_obj)
{
	jenis_row = *(m_refTreeModel->append());
	jenis_row[m_XMLColumns.m_col_name] = jenis;
	
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{			
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), jenis_obj) == 0))
			{
				row = *(m_refTreeModel->append(jenis_row.children()));
				row[m_XMLColumns.m_col_name] = xml_str_trim((char*)xmlGetProp(nodeLevel2,(const xmlChar*)"struct"));
				row[m_XMLColumns.m_obj_name] = xml_str_trim((char*)xmlGetProp(nodeLevel2,(const xmlChar*)"id"));
			}
		}
	}
}

EditorWindow::XMLColumns::XMLColumns()
{
	add(m_col_name);
	add(m_obj_name);
}

EditorWindow::PropColumns::PropColumns()
{
	add(m_prop_name);
	add(m_prop_value);
}

void EditorWindow::isi_tree()
{
	isi_tree_per_jenis("HEADER", "obj_header");
	isi_tree_per_jenis("BODY", "obj_body");
	isi_tree_per_jenis("GARIS", "obj_garis");
	isi_tree_per_jenis("KOTAK", "obj_kotak");
	isi_tree_per_jenis("GAMBAR", "obj_gambar");
	isi_tree_per_jenis("GAMBAR AKTIF", "obj_gambar_aktif");
	isi_tree_per_jenis("ANIMASI", "obj_animasi");
	isi_tree_per_jenis("TEKS", "obj_teks");
	isi_tree_per_jenis("TEKS AKTIF", "obj_teks_aktif");
	isi_tree_per_jenis("GAUGE BAR", "obj_gauge_bar");
	isi_tree_per_jenis("GAUGE DIAL", "obj_gauge_dial");
	isi_tree_per_jenis("GRAFIK", "obj_grafik");
}

void EditorWindow::on_menu_file_save()
{
	on_file_save();
}

void EditorWindow::on_menu_file_quit()
{
	hide();
}

void EditorWindow::on_button_save()
{
	on_file_save();
}

void EditorWindow::on_button_grid()
{
	if(showGrid == false)
		showGrid = true;
	else
		showGrid = false;
}

void EditorWindow::on_file_save()
{
	if(m_obj_entry_name->get_text() == "")
	{
		xmlSaveFile(filename,doc);
		format_xml();
	}
	else
	{
		Glib::ustring str;
		const char *val = NULL;	
		
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id"), tmp_entry_name) == 0))
				{
					str = m_obj_entry_name->get_text();
					locale_from_utf8 (str);
					val = str.c_str();
					xmlSetProp(nodeLevel2, (const xmlChar*)"id", (const xmlChar*)val);
				}
			}
		}
		
		ambil_all_tree row1 = m_refTreeModel_prop->children();
		
		for(ambil_all_tree::iterator iter = row1.begin(); iter != row1.end(); ++iter)
		{
			Gtk::TreeModel::Row prop1 = *iter;
			
			if(!(strcmp((locale_from_utf8 (prop1[m_PropColumns.m_prop_name]).c_str()), "child object") == 0))
			{
				for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
				{		
					for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
					{									
						for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
						{				
							for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
							{
								if(((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id") == m_obj_entry_name->get_text()) && (strcmp((char*)nodeLevel3->name, "property") == 0) && (xml_str_trim((char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name")) == prop1[m_PropColumns.m_prop_name]))
								{
									str = prop1[m_PropColumns.m_prop_name];
									locale_from_utf8 (str);
									val = str.c_str();
			
									str = prop1[m_PropColumns.m_prop_value];
									locale_from_utf8 (str);
									val = str.c_str();
									
									xmlNodeSetContent(nodeLevel4, (const xmlChar*)val);
								}							
							}
						}
					}
				}
			}
				
			Gtk::TreeModel::Children row2 =  prop1.children(); 
			for (ambil_all_tree::iterator iter2 = row2.begin(); iter2 != row2.end(); ++iter2) 
			{ 
				Gtk::TreeModel::Row prop2 = *iter2;
				
				str = prop2[m_PropColumns.m_prop_name];
				locale_from_utf8 (str);
				val = str.c_str();
				
				Gtk::TreeModel::Children row3 =  prop2.children(); 
				for (ambil_all_tree::iterator iter3 = row3.begin(); iter3 != row3.end(); ++iter3) 
				{ 
					Gtk::TreeModel::Row prop3 = *iter3;
					
					for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
					{		
						for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
						{									
							for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
							{				
								for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
								{	
									for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
									{
										for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
										{
											if(((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id") == m_obj_entry_name->get_text()) && (strcmp((char*)nodeLevel5->name, "property") == 0) && (xml_str_trim((char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name")) == prop3[m_PropColumns.m_prop_name]) && (xml_str_trim((char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id")) == prop2[m_PropColumns.m_prop_name]))
											{
												str = prop3[m_PropColumns.m_prop_name];
												locale_from_utf8 (str);
												val = str.c_str();
			
												str = prop3[m_PropColumns.m_prop_value];
												locale_from_utf8 (str);
												val = str.c_str();
												
												xmlNodeSetContent(nodeLevel6, (const xmlChar*)val);												
											}
										}
									}
								}
							}
						}
					}
					
					Gtk::TreeModel::Children row4 =  prop3.children(); 
					for (ambil_all_tree::iterator iter4 = row4.begin(); iter4 != row4.end(); ++iter4) 
					{ 
						Gtk::TreeModel::Row prop4 = *iter4;
				
						str = prop4[m_PropColumns.m_prop_name];
						locale_from_utf8 (str);
						val = str.c_str();
				
						Gtk::TreeModel::Children row5 =  prop4.children(); 
						for (ambil_all_tree::iterator iter5 = row5.begin(); iter5 != row5.end(); ++iter5) 
						{ 
							Gtk::TreeModel::Row prop5 = *iter5;
							
							for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
							{		
								for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
								{									
									for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
									{				
										for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
										{	
											for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
											{
												for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
												{
													for(nodeLevel7 = nodeLevel6->children; nodeLevel7 != NULL; nodeLevel7 = nodeLevel7->next)
													{
														for(nodeLevel8 = nodeLevel7->children; nodeLevel8 != NULL; nodeLevel8 = nodeLevel8->next)
														{
															if(((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id") == m_obj_entry_name->get_text()) && (strcmp((char*)nodeLevel7->name, "property") == 0) && (xml_str_trim((char*)xmlGetProp(nodeLevel7, (const xmlChar*)"name")) == prop5[m_PropColumns.m_prop_name]) && (xml_str_trim((char*)xmlGetProp(nodeLevel6, (const xmlChar*)"id")) == prop4[m_PropColumns.m_prop_name]) && (xml_str_trim((char*)xmlGetProp(nodeLevel4, (const xmlChar*)"id")) == prop2[m_PropColumns.m_prop_name]))
															{
																str = prop5[m_PropColumns.m_prop_name];
																locale_from_utf8 (str);
																val = str.c_str();
			
																str = prop5[m_PropColumns.m_prop_value];
																locale_from_utf8 (str);
																val = str.c_str();											
												
																xmlNodeSetContent(nodeLevel8, (const xmlChar*)val);											
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
					}
				}
			}
		}
		
		xmlSaveFile(filename,doc);
		format_xml();
			
		Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = m_TreeView.get_selection();
		Gtk::TreeModel::iterator iterzz = refTreeSelection->get_selected();
		if(iterzz)
		{
			Gtk::TreeModel::Row row = *iterzz;
			row[m_XMLColumns.m_obj_name] = m_obj_entry_name->get_text();
		}
	}
	
	reload_objects();
}

void EditorWindow::on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */)
{
	Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(path);
	
	m_TreeView_prop.remove_all_columns();
	m_refTreeModel_prop = Gtk::TreeStore::create(m_PropColumns);
	m_TreeView_prop.set_model(m_refTreeModel_prop);
	m_TreeView_prop.append_column("property name", m_PropColumns.m_prop_name);
	m_TreeView_prop.append_column_editable("value", m_PropColumns.m_prop_value);
	
	if(iter)
	{
		Gtk::TreeModel::Row row = *iter;		
		
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{			
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && ((row[m_XMLColumns.m_obj_name]) == (const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")) && ((row[m_XMLColumns.m_col_name]) == (const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct")))
				{
					strcpy(tmp_entry_name, xml_str_trim((char*)xmlGetProp(nodeLevel2,(const xmlChar*)"id")));
					m_obj_entry_name->set_text(xml_str_trim((char*)xmlGetProp(nodeLevel2,(const xmlChar*)"id")));
					m_obj_entry_jenis->set_text(xml_str_trim((char*)xmlGetProp(nodeLevel2,(const xmlChar*)"struct")));
					m_refTreeModel_prop->clear();
					
					for(nodeLevel3 = nodeLevel2->children; nodeLevel3 != NULL; nodeLevel3 = nodeLevel3->next)
					{	
						if(strcmp((char*)nodeLevel3->name, "child") == 0)
						{
							prop_row = *(m_refTreeModel_prop->append());
							prop_row[m_PropColumns.m_prop_name] = "child object";								
						}
											
						for(nodeLevel4 = nodeLevel3->children; nodeLevel4 != NULL; nodeLevel4 = nodeLevel4->next)
						{
							if((strcmp((char*)nodeLevel2->name, "object") == 0) && ((row[m_XMLColumns.m_obj_name]) == (const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")) && (strcmp((char*)nodeLevel3->name, "property") == 0))
							{
								prop_row = *(m_refTreeModel_prop->append());
								prop_row[m_PropColumns.m_prop_name] = xml_str_trim((char*)xmlGetProp(nodeLevel3, (const xmlChar*)"name"));
								prop_row[m_PropColumns.m_prop_value] = xml_str_trim((char*)nodeLevel4->content);
							}							
							
							if((strcmp((char*)nodeLevel3->name, "child") == 0) && (strcmp((char*)nodeLevel4->name, "object") == 0))
							{	
								prop_row_child = *(m_refTreeModel_prop->append(prop_row.children()));
								prop_row_child[m_PropColumns.m_prop_name] = xml_str_trim((char*)xmlGetProp(nodeLevel4,(const xmlChar*)"id"));
								
								for(nodeLevel5 = nodeLevel4->children; nodeLevel5 != NULL; nodeLevel5 = nodeLevel5->next)
								{
									if(strcmp((char*)nodeLevel5->name, "child") == 0)
									{
										prop_row_child_prop = *(m_refTreeModel_prop->append(prop_row_child.children()));
										prop_row_child_prop[m_PropColumns.m_prop_name] = "grand child object";								
									}
									
									for(nodeLevel6 = nodeLevel5->children; nodeLevel6 != NULL; nodeLevel6 = nodeLevel6->next)
									{
										if((strcmp((char*)nodeLevel4->name, "object") == 0) && ((row[m_XMLColumns.m_obj_name]) == (const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")) && (strcmp((char*)nodeLevel5->name, "property") == 0))
										{
											prop_row_child_prop = *(m_refTreeModel_prop->append(prop_row_child.children()));
											prop_row_child_prop[m_PropColumns.m_prop_name] = xml_str_trim((char*)xmlGetProp(nodeLevel5, (const xmlChar*)"name"));
											prop_row_child_prop[m_PropColumns.m_prop_value] = xml_str_trim((char*)nodeLevel6->content);
										}
										
										if((strcmp((char*)nodeLevel5->name, "child") == 0) && (strcmp((char*)nodeLevel6->name, "object") == 0))
										{	
											prop_row_grand_child = *(m_refTreeModel_prop->append(prop_row_child_prop.children()));
											prop_row_grand_child[m_PropColumns.m_prop_name] = xml_str_trim((char*)xmlGetProp(nodeLevel6,(const xmlChar*)"id"));
											
											for(nodeLevel7 = nodeLevel6->children; nodeLevel7 != NULL; nodeLevel7 = nodeLevel7->next)
											{
												for(nodeLevel8 = nodeLevel7->children; nodeLevel8 != NULL; nodeLevel8 = nodeLevel8->next)
												{
													if((strcmp((char*)nodeLevel6->name, "object") == 0) && ((row[m_XMLColumns.m_obj_name]) == (const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id")) && (strcmp((char*)nodeLevel7->name, "property") == 0))
													{
														prop_row_grand_child_prop = *(m_refTreeModel_prop->append(prop_row_grand_child.children()));
														prop_row_grand_child_prop[m_PropColumns.m_prop_name] = xml_str_trim((char*)xmlGetProp(nodeLevel7, (const xmlChar*)"name"));
														prop_row_grand_child_prop[m_PropColumns.m_prop_value] = xml_str_trim((char*)nodeLevel8->content);
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
			}
		}
	}
}

void EditorWindow::on_menu_file_open()
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
			doc = xmlParseFile(filename);
			update_title();
			load_tree();
			isOpened = true;
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
	
	load_objects();
	m_preview_area->queue_draw();
}

void EditorWindow::on_menu_file_new()
{
	Gtk::FileChooserDialog dialogsv("Please choose a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialogsv.set_transient_for(*this);

	//Add response buttons the the dialog:
	dialogsv.add_button(Gtk::Stock::CANCEL, 6);
	dialogsv.add_button(Gtk::Stock::SAVE, 5);

	//Add filters, so that only certain file types can be selected:

	Gtk::FileFilter filter_xml;
	filter_xml.set_name("xml files");
	filter_xml.add_mime_type("text/xml");
	dialogsv.add_filter(filter_xml);

	Gtk::FileFilter filter_any;
	filter_any.set_name("Any files");
	filter_any.add_pattern("*");
	dialogsv.add_filter(filter_any);

	//Show the dialog and wait for a user response:
	int result = dialogsv.run();

	//Handle the response:
	switch(result)
	{
		case(5):
		{
			strcpy(filename, (dialogsv.get_filename()).c_str());
			update_title();
			break;
		}
		case(6):
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void EditorWindow::load_tree()
{
	m_TreeView.remove_all_columns();
	m_TreeView_prop.remove_all_columns();	
	m_obj_entry_name->set_text("");
	m_obj_entry_jenis->set_text("");
	
	m_refTreeModel = Gtk::TreeStore::create(m_XMLColumns);
	m_TreeView.set_model(m_refTreeModel);
	
	m_TreeView.append_column("jenis", m_XMLColumns.m_col_name);
	m_TreeView.append_column("nama", m_XMLColumns.m_obj_name);			
	
	isi_tree();	
}

void EditorWindow::on_button_add()
{	
	Gtk::Dialog* dlg_select;
	Gtk::RadioButton* rb_garis;
	Gtk::RadioButton* rb_kotak;
	Gtk::RadioButton* rb_gambar;
	Gtk::RadioButton* rb_gambar_aktif;
	Gtk::RadioButton* rb_animasi;
	Gtk::RadioButton* rb_teks;
	Gtk::RadioButton* rb_teks_aktif;
	Gtk::RadioButton* rb_gauge_bar;
	Gtk::RadioButton* rb_gauge_dial;
	Gtk::RadioButton* rb_grafik;
	
	Gtk::RadioButton::Group group_obj_select;
	
	m_refGlade->get_widget("dlg_select", dlg_select);
	
	m_refGlade->get_widget("rb_garis", rb_garis);
	m_refGlade->get_widget("rb_kotak", rb_kotak);
	m_refGlade->get_widget("rb_gambar", rb_gambar);
	m_refGlade->get_widget("rb_gambar_aktif", rb_gambar_aktif);
	m_refGlade->get_widget("rb_animasi", rb_animasi);
	m_refGlade->get_widget("rb_teks", rb_teks);
	m_refGlade->get_widget("rb_teks_aktif", rb_teks_aktif);
	m_refGlade->get_widget("rb_gauge_bar", rb_gauge_bar);
	m_refGlade->get_widget("rb_gauge_dial", rb_gauge_dial);
	m_refGlade->get_widget("rb_grafik", rb_grafik);
	
	group_obj_select = rb_garis->get_group();
	rb_kotak->set_group(group_obj_select);
	rb_gambar->set_group(group_obj_select);
	rb_gambar_aktif->set_group(group_obj_select);
	rb_animasi->set_group(group_obj_select);
	rb_teks->set_group(group_obj_select);
	rb_teks_aktif->set_group(group_obj_select);
	rb_gauge_bar->set_group(group_obj_select);
	rb_gauge_dial->set_group(group_obj_select);
	rb_grafik->set_group(group_obj_select);
	rb_garis->set_active();

	int result = dlg_select->run();
	
	switch(result)
	{
		case(3):
		{
			if(rb_garis->property_active() == true)
			{
				new_xml_garis();
				load_tree();
			}
			if(rb_kotak->property_active() == true)
			{
				new_xml_kotak();
				load_tree();
			}
			if(rb_gambar->property_active() == true)
			{
				new_xml_gambar();
				load_tree();
			}
			if(rb_gambar_aktif->property_active() == true)
			{
				new_xml_gambar_aktif();
				load_tree();
			}
			if(rb_animasi->property_active() == true)
			{
				new_xml_animasi();
				load_tree();
			}
			if(rb_teks->property_active() == true)
			{
				new_xml_teks();
				load_tree();
			}
			if(rb_teks_aktif->property_active() == true)
			{
				new_xml_teks_aktif();
				load_tree();
			}
			if(rb_gauge_bar->property_active() == true)
			{
				new_xml_gauge_bar();
				load_tree();
			}
			if(rb_gauge_dial->property_active() == true)
			{
				new_xml_gauge_dial();
				load_tree();
			}
			if(rb_grafik->property_active() == true)
			{
				new_xml_grafik();
				load_tree();
			}
			dlg_select->hide();
			break;
		}
		case(4):
		{
			dlg_select->hide();
			break;
		}
		default:
		{
			break;
		}
	}	
	
}

void EditorWindow::new_xml_garis()
{
	char gr_baru[32];
	
	int x = 0;
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_garis") == 0))
			{
				x++;
			}
		}
	}
	
	if(x == 0)
		strcpy(gr_baru, "new_garis#1");
	else
	{
		int n = 2;
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_garis") == 0))
				{
					char gr_name[32];
					sprintf(gr_name, "%s%d", "new_garis#", n);
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), gr_name) == 0)
						n++;
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), gr_name) != 0)
					{	
						strcpy(gr_baru, gr_name);
						n++;
					}
				}
			}
		}
	}
	
	nodeLevel1 = doc->children;	
	
	xmlNodePtr node_garis = nodeLevel1->children;
	node_garis = xmlNewNode(NULL, (const xmlChar*)"object");	
	node_garis = xmlDocCopyNode(node_garis, doc, 1);
	xmlAddChild(nodeLevel1, node_garis);
	xmlSetProp(node_garis, (const xmlChar*) "struct", (const xmlChar*) "obj_garis" );
	xmlSetProp(node_garis, (const xmlChar*) "id", (const xmlChar*) gr_baru );
	
	xmlNodePtr garis_caption = node_garis->children;	
	garis_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_caption = xmlDocCopyNode(garis_caption, doc, 1);
	xmlAddChild(node_garis, garis_caption);
	xmlSetProp(garis_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(garis_caption, (const xmlChar*) gr_baru);
		
	xmlNodePtr garis_width = node_garis->children;	
	garis_width = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_width = xmlDocCopyNode(garis_width, doc, 1);
	xmlAddChild(node_garis, garis_width);
	xmlSetProp(garis_width, (const xmlChar*) "name", (const xmlChar*) "width");
	xmlNodeSetContent(garis_width, (const xmlChar*) "2");
	
	xmlNodePtr garis_points_count = node_garis->children;	
	garis_points_count = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_points_count = xmlDocCopyNode(garis_points_count, doc, 1);
	xmlAddChild(node_garis, garis_points_count);
	xmlSetProp(garis_points_count, (const xmlChar*) "name", (const xmlChar*) "points_count");
	xmlNodeSetContent(garis_points_count, (const xmlChar*) "2");
	
	//child
	
	xmlNodePtr garis_child = node_garis->children;	
	garis_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	garis_child = xmlDocCopyNode(garis_child, doc, 1);
	xmlAddChild(node_garis, garis_child);
	
	// warna 
	
	xmlNodePtr garis_warna = garis_child->children;
	garis_warna = xmlNewNode(NULL, (const xmlChar*)"object");	
	garis_warna = xmlDocCopyNode(garis_warna, doc, 1);
	xmlAddChild(garis_child, garis_warna);
	xmlSetProp(garis_warna, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(garis_warna, (const xmlChar*) "id", (const xmlChar*) "warna" );

	xmlNodePtr garis_warna_r = garis_warna->children;	
	garis_warna_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_warna_r = xmlDocCopyNode(garis_warna_r, doc, 1);
	xmlAddChild(garis_warna, garis_warna_r);
	xmlSetProp(garis_warna_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(garis_warna_r, (const xmlChar*) "100");

	xmlNodePtr garis_warna_g = garis_warna->children;	
	garis_warna_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_warna_g = xmlDocCopyNode(garis_warna_g, doc, 1);
	xmlAddChild(garis_warna, garis_warna_g);
	xmlSetProp(garis_warna_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(garis_warna_g, (const xmlChar*) "100");
	
	xmlNodePtr garis_warna_b = garis_warna->children;	
	garis_warna_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_warna_b = xmlDocCopyNode(garis_warna_b, doc, 1);
	xmlAddChild(garis_warna, garis_warna_b);
	xmlSetProp(garis_warna_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(garis_warna_b, (const xmlChar*) "100");

	xmlNodePtr garis_warna_a = garis_warna->children;	
	garis_warna_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_warna_a = xmlDocCopyNode(garis_warna_a, doc, 1);
	xmlAddChild(garis_warna, garis_warna_a);
	xmlSetProp(garis_warna_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(garis_warna_a, (const xmlChar*) "100");
	
	//titik#1
	
	xmlNodePtr garis_titik1 = garis_child->children;
	garis_titik1 = xmlNewNode(NULL, (const xmlChar*)"object");	
	garis_titik1 = xmlDocCopyNode(garis_titik1, doc, 1);
	xmlAddChild(garis_child, garis_titik1);
	xmlSetProp(garis_titik1, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(garis_titik1, (const xmlChar*) "id", (const xmlChar*) "titik#1" );
	
	xmlNodePtr garis_titik1_x = garis_titik1->children;	
	garis_titik1_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_titik1_x = xmlDocCopyNode(garis_titik1_x, doc, 1);
	xmlAddChild(garis_titik1, garis_titik1_x);
	xmlSetProp(garis_titik1_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(garis_titik1_x, (const xmlChar*) "0");
	
	xmlNodePtr garis_titik1_y = garis_titik1->children;	
	garis_titik1_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_titik1_y = xmlDocCopyNode(garis_titik1_y, doc, 1);
	xmlAddChild(garis_titik1, garis_titik1_y);
	xmlSetProp(garis_titik1_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(garis_titik1_y, (const xmlChar*) "0");
	
	//titik#2
	
	xmlNodePtr garis_titik2 = garis_child->children;
	garis_titik2 = xmlNewNode(NULL, (const xmlChar*)"object");	
	garis_titik2 = xmlDocCopyNode(garis_titik2, doc, 1);
	xmlAddChild(garis_child, garis_titik2);
	xmlSetProp(garis_titik2, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(garis_titik2, (const xmlChar*) "id", (const xmlChar*) "titik#2" );
	
	xmlNodePtr garis_titik2_x = garis_titik2->children;	
	garis_titik2_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_titik2_x = xmlDocCopyNode(garis_titik2_x, doc, 1);
	xmlAddChild(garis_titik2, garis_titik2_x);
	xmlSetProp(garis_titik2_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(garis_titik2_x, (const xmlChar*) "0");
	
	xmlNodePtr garis_titik2_y = garis_titik2->children;	
	garis_titik2_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_titik2_y = xmlDocCopyNode(garis_titik2_y, doc, 1);
	xmlAddChild(garis_titik2, garis_titik2_y);
	xmlSetProp(garis_titik2_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(garis_titik2_y, (const xmlChar*) "0");
	
	//titik#3
	
	xmlNodePtr garis_titik3 = garis_child->children;
	garis_titik3 = xmlNewNode(NULL, (const xmlChar*)"object");	
	garis_titik3 = xmlDocCopyNode(garis_titik3, doc, 1);
	xmlAddChild(garis_child, garis_titik3);
	xmlSetProp(garis_titik3, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(garis_titik3, (const xmlChar*) "id", (const xmlChar*) "titik#3" );
	
	xmlNodePtr garis_titik3_x = garis_titik3->children;	
	garis_titik3_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_titik3_x = xmlDocCopyNode(garis_titik3_x, doc, 1);
	xmlAddChild(garis_titik3, garis_titik3_x);
	xmlSetProp(garis_titik3_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(garis_titik3_x, (const xmlChar*) "0");
	
	xmlNodePtr garis_titik3_y = garis_titik3->children;	
	garis_titik3_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_titik3_y = xmlDocCopyNode(garis_titik3_y, doc, 1);
	xmlAddChild(garis_titik3, garis_titik3_y);
	xmlSetProp(garis_titik3_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(garis_titik3_y, (const xmlChar*) "0");
	
	//titik#4

	xmlNodePtr garis_titik4 = garis_child->children;
	garis_titik4 = xmlNewNode(NULL, (const xmlChar*)"object");	
	garis_titik4 = xmlDocCopyNode(garis_titik4, doc, 1);
	xmlAddChild(garis_child, garis_titik4);
	xmlSetProp(garis_titik4, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(garis_titik4, (const xmlChar*) "id", (const xmlChar*) "titik#4" );
	
	xmlNodePtr garis_titik4_x = garis_titik4->children;	
	garis_titik4_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_titik4_x = xmlDocCopyNode(garis_titik4_x, doc, 1);
	xmlAddChild(garis_titik4, garis_titik4_x);
	xmlSetProp(garis_titik4_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(garis_titik4_x, (const xmlChar*) "0");
	
	xmlNodePtr garis_titik4_y = garis_titik4->children;	
	garis_titik4_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_titik4_y = xmlDocCopyNode(garis_titik4_y, doc, 1);
	xmlAddChild(garis_titik4, garis_titik4_y);
	xmlSetProp(garis_titik4_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(garis_titik4_y, (const xmlChar*) "0");
	
	//titik#5
	
	xmlNodePtr garis_titik5 = garis_child->children;
	garis_titik5 = xmlNewNode(NULL, (const xmlChar*)"object");	
	garis_titik5 = xmlDocCopyNode(garis_titik5, doc, 1);
	xmlAddChild(garis_child, garis_titik5);
	xmlSetProp(garis_titik5, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(garis_titik5, (const xmlChar*) "id", (const xmlChar*) "titik#5" );
	
	xmlNodePtr garis_titik5_x = garis_titik5->children;	
	garis_titik5_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_titik5_x = xmlDocCopyNode(garis_titik5_x, doc, 1);
	xmlAddChild(garis_titik5, garis_titik5_x);
	xmlSetProp(garis_titik5_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(garis_titik5_x, (const xmlChar*) "0");
	
	xmlNodePtr garis_titik5_y = garis_titik5->children;	
	garis_titik5_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	garis_titik5_y = xmlDocCopyNode(garis_titik5_y, doc, 1);
	xmlAddChild(garis_titik5, garis_titik5_y);
	xmlSetProp(garis_titik5_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(garis_titik5_y, (const xmlChar*) "0");
	
}

void EditorWindow::new_xml_kotak()
{
	char ktk_baru[32];
	
	int x = 0;
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_kotak") == 0))
			{
				x++;
			}
		}
	}
	
	if(x == 0)
		strcpy(ktk_baru, "new_kotak#1");
	else
	{
		int n = 2;
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_kotak") == 0))
				{
					char ktk_name[32];
					sprintf(ktk_name, "%s%d", "new_kotak#", n);
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), ktk_name) == 0)
						n++;
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), ktk_name) != 0)
					{	
						strcpy(ktk_baru, ktk_name);
						n++;
					}
				}
			}
		}
	}
	nodeLevel1 = doc->children;	
	
	xmlNodePtr node_kotak = nodeLevel1->children;
	node_kotak = xmlNewNode(NULL, (const xmlChar*)"object");	
	node_kotak = xmlDocCopyNode(node_kotak, doc, 1);
	xmlAddChild(nodeLevel1, node_kotak);
	xmlSetProp(node_kotak, (const xmlChar*) "struct", (const xmlChar*) "obj_kotak" );
	xmlSetProp(node_kotak, (const xmlChar*) "id", (const xmlChar*)  ktk_baru);
	
	xmlNodePtr kotak_caption = node_kotak->children;	
	kotak_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_caption = xmlDocCopyNode(kotak_caption, doc, 1);
	xmlAddChild(node_kotak, kotak_caption);
	xmlSetProp(kotak_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(kotak_caption, (const xmlChar*) ktk_baru);
	
	xmlNodePtr kotak_width = node_kotak->children;	
	kotak_width = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_width = xmlDocCopyNode(kotak_width, doc, 1);
	xmlAddChild(node_kotak, kotak_width);
	xmlSetProp(kotak_width, (const xmlChar*) "name", (const xmlChar*) "width");
	xmlNodeSetContent(kotak_width, (const xmlChar*) "100");
	
	xmlNodePtr kotak_height = node_kotak->children;	
	kotak_height = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_height = xmlDocCopyNode(kotak_height, doc, 1);
	xmlAddChild(node_kotak, kotak_height);
	xmlSetProp(kotak_height, (const xmlChar*) "name", (const xmlChar*) "height");
	xmlNodeSetContent(kotak_height, (const xmlChar*) "100");
	
	xmlNodePtr kotak_link = node_kotak->children;	
	kotak_link = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_link = xmlDocCopyNode(kotak_link, doc, 1);
	xmlAddChild(node_kotak, kotak_link);
	xmlSetProp(kotak_link, (const xmlChar*) "name", (const xmlChar*) "link");
	xmlNodeSetContent(kotak_link, (const xmlChar*) "index.xml");
	
	xmlNodePtr kotak_link_caption = node_kotak->children;	
	kotak_link_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_link_caption = xmlDocCopyNode(kotak_link_caption, doc, 1);
	xmlAddChild(node_kotak, kotak_link_caption);
	xmlSetProp(kotak_link_caption, (const xmlChar*) "name", (const xmlChar*) "link_caption");
	xmlNodeSetContent(kotak_link_caption, (const xmlChar*) "home");

	//child
	
	xmlNodePtr kotak_child = node_kotak->children;	
	kotak_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	kotak_child = xmlDocCopyNode(kotak_child, doc, 1);
	xmlAddChild(node_kotak, kotak_child);
	
	//posisi
	
	xmlNodePtr kotak_posisi = kotak_child->children;
	kotak_posisi = xmlNewNode(NULL, (const xmlChar*)"object");	
	kotak_posisi = xmlDocCopyNode(kotak_posisi, doc, 1);
	xmlAddChild(kotak_child, kotak_posisi);
	xmlSetProp(kotak_posisi, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(kotak_posisi, (const xmlChar*) "id", (const xmlChar*) "posisi" );
	
	xmlNodePtr kotak_posisi_x = kotak_posisi->children;	
	kotak_posisi_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_posisi_x = xmlDocCopyNode(kotak_posisi_x, doc, 1);
	xmlAddChild(kotak_posisi, kotak_posisi_x);
	xmlSetProp(kotak_posisi_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(kotak_posisi_x, (const xmlChar*) "0");
	
	xmlNodePtr kotak_posisi_y = kotak_posisi->children;	
	kotak_posisi_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_posisi_y = xmlDocCopyNode(kotak_posisi_y, doc, 1);
	xmlAddChild(kotak_posisi, kotak_posisi_y);
	xmlSetProp(kotak_posisi_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(kotak_posisi_y, (const xmlChar*) "0");
	
	//warna outline
	
	xmlNodePtr kotak_warna_outline = kotak_child->children;
	kotak_warna_outline = xmlNewNode(NULL, (const xmlChar*)"object");	
	kotak_warna_outline = xmlDocCopyNode(kotak_warna_outline, doc, 1);
	xmlAddChild(kotak_child, kotak_warna_outline);
	xmlSetProp(kotak_warna_outline, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(kotak_warna_outline, (const xmlChar*) "id", (const xmlChar*) "warna_outline" );
	
	xmlNodePtr kotak_warna_outline_r = kotak_warna_outline->children;	
	kotak_warna_outline_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_outline_r = xmlDocCopyNode(kotak_warna_outline_r, doc, 1);
	xmlAddChild(kotak_warna_outline, kotak_warna_outline_r);
	xmlSetProp(kotak_warna_outline_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(kotak_warna_outline_r, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_outline_g = kotak_warna_outline->children;	
	kotak_warna_outline_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_outline_g = xmlDocCopyNode(kotak_warna_outline_g, doc, 1);
	xmlAddChild(kotak_warna_outline, kotak_warna_outline_g);
	xmlSetProp(kotak_warna_outline_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(kotak_warna_outline_g, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_outline_b = kotak_warna_outline->children;	
	kotak_warna_outline_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_outline_b = xmlDocCopyNode(kotak_warna_outline_b, doc, 1);
	xmlAddChild(kotak_warna_outline, kotak_warna_outline_b);
	xmlSetProp(kotak_warna_outline_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(kotak_warna_outline_b, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_outline_a = kotak_warna_outline->children;	
	kotak_warna_outline_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_outline_a = xmlDocCopyNode(kotak_warna_outline_a, doc, 1);
	xmlAddChild(kotak_warna_outline, kotak_warna_outline_a);
	xmlSetProp(kotak_warna_outline_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(kotak_warna_outline_a, (const xmlChar*) "100");
	
	//warna fill
	
	xmlNodePtr kotak_warna_fill = kotak_child->children;
	kotak_warna_fill = xmlNewNode(NULL, (const xmlChar*)"object");	
	kotak_warna_fill = xmlDocCopyNode(kotak_warna_fill, doc, 1);
	xmlAddChild(kotak_child, kotak_warna_fill);
	xmlSetProp(kotak_warna_fill, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(kotak_warna_fill, (const xmlChar*) "id", (const xmlChar*) "warna_fill" );
	
	xmlNodePtr kotak_warna_fill_r = kotak_warna_fill->children;	
	kotak_warna_fill_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_fill_r = xmlDocCopyNode(kotak_warna_fill_r, doc, 1);
	xmlAddChild(kotak_warna_fill, kotak_warna_fill_r);
	xmlSetProp(kotak_warna_fill_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(kotak_warna_fill_r, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_fill_g = kotak_warna_fill->children;	
	kotak_warna_fill_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_fill_g = xmlDocCopyNode(kotak_warna_fill_g, doc, 1);
	xmlAddChild(kotak_warna_fill, kotak_warna_fill_g);
	xmlSetProp(kotak_warna_fill_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(kotak_warna_fill_g, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_fill_b = kotak_warna_fill->children;	
	kotak_warna_fill_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_fill_b = xmlDocCopyNode(kotak_warna_fill_b, doc, 1);
	xmlAddChild(kotak_warna_fill, kotak_warna_fill_b);
	xmlSetProp(kotak_warna_fill_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(kotak_warna_fill_b, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_fill_a = kotak_warna_fill->children;	
	kotak_warna_fill_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_fill_a = xmlDocCopyNode(kotak_warna_fill_a, doc, 1);
	xmlAddChild(kotak_warna_fill, kotak_warna_fill_a);
	xmlSetProp(kotak_warna_fill_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(kotak_warna_fill_a, (const xmlChar*) "100");
	
	//warna hover
	
	xmlNodePtr kotak_warna_hover = kotak_child->children;
	kotak_warna_hover = xmlNewNode(NULL, (const xmlChar*)"object");	
	kotak_warna_hover = xmlDocCopyNode(kotak_warna_hover, doc, 1);
	xmlAddChild(kotak_child, kotak_warna_hover);
	xmlSetProp(kotak_warna_hover, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(kotak_warna_hover, (const xmlChar*) "id", (const xmlChar*) "warna_hover" );
	
	xmlNodePtr kotak_warna_hover_r = kotak_warna_hover->children;	
	kotak_warna_hover_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_hover_r = xmlDocCopyNode(kotak_warna_hover_r, doc, 1);
	xmlAddChild(kotak_warna_hover, kotak_warna_hover_r);
	xmlSetProp(kotak_warna_hover_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(kotak_warna_hover_r, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_hover_g = kotak_warna_hover->children;	
	kotak_warna_hover_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_hover_g = xmlDocCopyNode(kotak_warna_hover_g, doc, 1);
	xmlAddChild(kotak_warna_hover, kotak_warna_hover_g);
	xmlSetProp(kotak_warna_hover_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(kotak_warna_hover_g, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_hover_b = kotak_warna_hover->children;	
	kotak_warna_hover_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_hover_b = xmlDocCopyNode(kotak_warna_hover_b, doc, 1);
	xmlAddChild(kotak_warna_hover, kotak_warna_hover_b);
	xmlSetProp(kotak_warna_hover_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(kotak_warna_hover_b, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_hover_a = kotak_warna_hover->children;	
	kotak_warna_hover_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_hover_a = xmlDocCopyNode(kotak_warna_hover_a, doc, 1);
	xmlAddChild(kotak_warna_hover, kotak_warna_hover_a);
	xmlSetProp(kotak_warna_hover_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(kotak_warna_hover_a, (const xmlChar*) "100");
	
	//warna select
	
	xmlNodePtr kotak_warna_select = kotak_child->children;
	kotak_warna_select = xmlNewNode(NULL, (const xmlChar*)"object");	
	kotak_warna_select = xmlDocCopyNode(kotak_warna_select, doc, 1);
	xmlAddChild(kotak_child, kotak_warna_select);
	xmlSetProp(kotak_warna_select, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(kotak_warna_select, (const xmlChar*) "id", (const xmlChar*) "warna_select" );
	
	xmlNodePtr kotak_warna_select_r = kotak_warna_select->children;	
	kotak_warna_select_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_select_r = xmlDocCopyNode(kotak_warna_select_r, doc, 1);
	xmlAddChild(kotak_warna_select, kotak_warna_select_r);
	xmlSetProp(kotak_warna_select_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(kotak_warna_select_r, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_select_g = kotak_warna_select->children;	
	kotak_warna_select_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_select_g = xmlDocCopyNode(kotak_warna_select_g, doc, 1);
	xmlAddChild(kotak_warna_select, kotak_warna_select_g);
	xmlSetProp(kotak_warna_select_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(kotak_warna_select_g, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_select_b = kotak_warna_select->children;	
	kotak_warna_select_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_select_b = xmlDocCopyNode(kotak_warna_select_b, doc, 1);
	xmlAddChild(kotak_warna_select, kotak_warna_select_b);
	xmlSetProp(kotak_warna_select_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(kotak_warna_select_b, (const xmlChar*) "100");
	
	xmlNodePtr kotak_warna_select_a = kotak_warna_select->children;	
	kotak_warna_select_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	kotak_warna_select_a = xmlDocCopyNode(kotak_warna_select_a, doc, 1);
	xmlAddChild(kotak_warna_select, kotak_warna_select_a);
	xmlSetProp(kotak_warna_select_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(kotak_warna_select_a, (const xmlChar*) "100");
}

void EditorWindow::new_xml_gambar()
{
	char gbr_baru[32];
	
	int x = 0;
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gambar") == 0))
			{
				x++;
			}
		}
	}
	
	if(x == 0)
		strcpy(gbr_baru, "new_gambar#1");
	else
	{
		int n = 2;
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gambar") == 0))
				{
					char gbr_name[32];
					sprintf(gbr_name, "%s%d", "new_gambar#", n);
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), gbr_name) == 0)
						n++;
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), gbr_name) != 0)
					{	
						strcpy(gbr_baru, gbr_name);
						n++;
					}
				}
			}
		}
	}
	nodeLevel1 = doc->children;	
	
	xmlNodePtr node_gambar = nodeLevel1->children;
	node_gambar = xmlNewNode(NULL, (const xmlChar*)"object");	
	node_gambar = xmlDocCopyNode(node_gambar, doc, 1);
	xmlAddChild(nodeLevel1, node_gambar);
	xmlSetProp(node_gambar, (const xmlChar*) "struct", (const xmlChar*) "obj_gambar" );
	xmlSetProp(node_gambar, (const xmlChar*) "id", (const xmlChar*)  gbr_baru);
	
	xmlNodePtr gambar_caption = node_gambar->children;	
	gambar_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_caption = xmlDocCopyNode(gambar_caption, doc, 1);
	xmlAddChild(node_gambar, gambar_caption);
	xmlSetProp(gambar_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(gambar_caption, (const xmlChar*) gbr_baru);
	
	xmlNodePtr gambar_source = node_gambar->children;	
	gambar_source = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_source = xmlDocCopyNode(gambar_source, doc, 1);
	xmlAddChild(node_gambar, gambar_source);
	xmlSetProp(gambar_source, (const xmlChar*) "name", (const xmlChar*) "source");
	xmlNodeSetContent(gambar_source, (const xmlChar*) "valve.png");
	
	xmlNodePtr gambar_width = node_gambar->children;	
	gambar_width = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_width = xmlDocCopyNode(gambar_width, doc, 1);
	xmlAddChild(node_gambar, gambar_width);
	xmlSetProp(gambar_width, (const xmlChar*) "name", (const xmlChar*) "width");
	xmlNodeSetContent(gambar_width, (const xmlChar*) "100");
	
	xmlNodePtr gambar_height = node_gambar->children;	
	gambar_height = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_height = xmlDocCopyNode(gambar_height, doc, 1);
	xmlAddChild(node_gambar, gambar_height);
	xmlSetProp(gambar_height, (const xmlChar*) "name", (const xmlChar*) "height");
	xmlNodeSetContent(gambar_height, (const xmlChar*) "100");
	
	xmlNodePtr gambar_skala = node_gambar->children;	
	gambar_skala = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_skala = xmlDocCopyNode(gambar_skala, doc, 1);
	xmlAddChild(node_gambar, gambar_skala);
	xmlSetProp(gambar_skala, (const xmlChar*) "name", (const xmlChar*) "skala");
	xmlNodeSetContent(gambar_skala, (const xmlChar*) "1.0");
	
	xmlNodePtr gambar_keterangan = node_gambar->children;	
	gambar_keterangan = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_keterangan = xmlDocCopyNode(gambar_keterangan, doc, 1);
	xmlAddChild(node_gambar, gambar_keterangan);
	xmlSetProp(gambar_keterangan, (const xmlChar*) "name", (const xmlChar*) "keterangan");
	xmlNodeSetContent(gambar_keterangan, (const xmlChar*) "keterangan gambar");

	//child
	
	xmlNodePtr gambar_child = node_gambar->children;	
	gambar_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	gambar_child = xmlDocCopyNode(gambar_child, doc, 1);
	xmlAddChild(node_gambar, gambar_child);
	
	//posisi
	
	xmlNodePtr gambar_posisi = gambar_child->children;
	gambar_posisi = xmlNewNode(NULL, (const xmlChar*)"object");	
	gambar_posisi = xmlDocCopyNode(gambar_posisi, doc, 1);
	xmlAddChild(gambar_child, gambar_posisi);
	xmlSetProp(gambar_posisi, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(gambar_posisi, (const xmlChar*) "id", (const xmlChar*) "posisi" );
	
	xmlNodePtr gambar_posisi_x = gambar_posisi->children;	
	gambar_posisi_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_posisi_x = xmlDocCopyNode(gambar_posisi_x, doc, 1);
	xmlAddChild(gambar_posisi, gambar_posisi_x);
	xmlSetProp(gambar_posisi_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(gambar_posisi_x, (const xmlChar*) "0");
	
	xmlNodePtr gambar_posisi_y = gambar_posisi->children;	
	gambar_posisi_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_posisi_y = xmlDocCopyNode(gambar_posisi_y, doc, 1);
	xmlAddChild(gambar_posisi, gambar_posisi_y);
	xmlSetProp(gambar_posisi_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(gambar_posisi_y, (const xmlChar*) "0");
}

void EditorWindow::new_xml_gambar_aktif()
{
	char gbr_aktif_baru[32];
	
	int x = 0;
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gambar_aktif") == 0))
			{
				x++;
			}
		}
	}
	
	if(x == 0)
		strcpy(gbr_aktif_baru, "new_gambar_aktif#1");
	else
	{
		int n = 2;
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gambar_aktif") == 0))
				{
					char gbr_aktif_name[32];
					sprintf(gbr_aktif_name, "%s%d", "new_gambar_aktif#", n);
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), gbr_aktif_name) == 0)
						n++;
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), gbr_aktif_name) != 0)
					{	
						strcpy(gbr_aktif_baru, gbr_aktif_name);
						n++;
					}
				}
			}
		}
	}
	nodeLevel1 = doc->children;	
		
	xmlNodePtr node_gambar_aktif = nodeLevel1->children;
	node_gambar_aktif = xmlNewNode(NULL, (const xmlChar*)"object");	
	node_gambar_aktif = xmlDocCopyNode(node_gambar_aktif, doc, 1);
	xmlAddChild(nodeLevel1, node_gambar_aktif);
	xmlSetProp(node_gambar_aktif, (const xmlChar*) "struct", (const xmlChar*) "obj_gambar_aktif" );
	xmlSetProp(node_gambar_aktif, (const xmlChar*) "id", (const xmlChar*)  gbr_aktif_baru);
	
	xmlNodePtr gambar_aktif_id_titik_data = node_gambar_aktif->children;	
	gambar_aktif_id_titik_data = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_id_titik_data = xmlDocCopyNode(gambar_aktif_id_titik_data, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_id_titik_data);
	xmlSetProp(gambar_aktif_id_titik_data, (const xmlChar*) "name", (const xmlChar*) "id_titik_data");
	xmlNodeSetContent(gambar_aktif_id_titik_data, (const xmlChar*) "10");
	
	xmlNodePtr gambar_aktif_caption = node_gambar_aktif->children;	
	gambar_aktif_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_caption = xmlDocCopyNode(gambar_aktif_caption, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_caption);
	xmlSetProp(gambar_aktif_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(gambar_aktif_caption, (const xmlChar*) gbr_aktif_baru);
		
	xmlNodePtr gambar_aktif_width = node_gambar_aktif->children;	
	gambar_aktif_width = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_width = xmlDocCopyNode(gambar_aktif_width, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_width);
	xmlSetProp(gambar_aktif_width, (const xmlChar*) "name", (const xmlChar*) "width");
	xmlNodeSetContent(gambar_aktif_width, (const xmlChar*) "100");
	
	xmlNodePtr gambar_aktif_height = node_gambar_aktif->children;	
	gambar_aktif_height = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_height = xmlDocCopyNode(gambar_aktif_height, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_height);
	xmlSetProp(gambar_aktif_height, (const xmlChar*) "name", (const xmlChar*) "height");
	xmlNodeSetContent(gambar_aktif_height, (const xmlChar*) "100");
	
	xmlNodePtr gambar_aktif_skala = node_gambar_aktif->children;	
	gambar_aktif_skala = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_skala = xmlDocCopyNode(gambar_aktif_skala, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_skala);
	xmlSetProp(gambar_aktif_skala, (const xmlChar*) "name", (const xmlChar*) "skala");
	xmlNodeSetContent(gambar_aktif_skala, (const xmlChar*) "1.0");
	
	xmlNodePtr gambar_aktif_show_text = node_gambar_aktif->children;	
	gambar_aktif_show_text = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_show_text = xmlDocCopyNode(gambar_aktif_show_text, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_show_text);
	xmlSetProp(gambar_aktif_show_text, (const xmlChar*) "name", (const xmlChar*) "show_text");
	xmlNodeSetContent(gambar_aktif_show_text, (const xmlChar*) "1");
	
	xmlNodePtr gambar_aktif_show_caption = node_gambar_aktif->children;	
	gambar_aktif_show_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_show_caption = xmlDocCopyNode(gambar_aktif_show_caption, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_show_caption);
	xmlSetProp(gambar_aktif_show_caption, (const xmlChar*) "name", (const xmlChar*) "show_caption");
	xmlNodeSetContent(gambar_aktif_show_caption, (const xmlChar*) "1");
	
	xmlNodePtr gambar_aktif_auto_alarm_range = node_gambar_aktif->children;	
	gambar_aktif_auto_alarm_range = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_auto_alarm_range = xmlDocCopyNode(gambar_aktif_auto_alarm_range, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_auto_alarm_range);
	xmlSetProp(gambar_aktif_auto_alarm_range, (const xmlChar*) "name", (const xmlChar*) "auto_alarm_range");
	xmlNodeSetContent(gambar_aktif_auto_alarm_range, (const xmlChar*) "1");
	
	xmlNodePtr gambar_aktif_range_min = node_gambar_aktif->children;	
	gambar_aktif_range_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_range_min = xmlDocCopyNode(gambar_aktif_range_min, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_range_min);
	xmlSetProp(gambar_aktif_range_min, (const xmlChar*) "name", (const xmlChar*) "range_min");
	xmlNodeSetContent(gambar_aktif_range_min, (const xmlChar*) "0");
	
	xmlNodePtr gambar_aktif_range_max = node_gambar_aktif->children;	
	gambar_aktif_range_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_range_max = xmlDocCopyNode(gambar_aktif_range_max, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_range_max);
	xmlSetProp(gambar_aktif_range_max, (const xmlChar*) "name", (const xmlChar*) "range_max");
	xmlNodeSetContent(gambar_aktif_range_max, (const xmlChar*) "100");
	
	xmlNodePtr gambar_aktif_alarm_min_min = node_gambar_aktif->children;	
	gambar_aktif_alarm_min_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_alarm_min_min = xmlDocCopyNode(gambar_aktif_alarm_min_min, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_alarm_min_min);
	xmlSetProp(gambar_aktif_alarm_min_min, (const xmlChar*) "name", (const xmlChar*) "alarm_min_min");
	xmlNodeSetContent(gambar_aktif_alarm_min_min, (const xmlChar*) "10");
	
	xmlNodePtr gambar_aktif_alarm_min = node_gambar_aktif->children;	
	gambar_aktif_alarm_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_alarm_min = xmlDocCopyNode(gambar_aktif_alarm_min, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_alarm_min);
	xmlSetProp(gambar_aktif_alarm_min, (const xmlChar*) "name", (const xmlChar*) "alarm_min");
	xmlNodeSetContent(gambar_aktif_alarm_min, (const xmlChar*) "20");
	
	xmlNodePtr gambar_aktif_alarm_max = node_gambar_aktif->children;	
	gambar_aktif_alarm_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_alarm_max = xmlDocCopyNode(gambar_aktif_alarm_max, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_alarm_max);
	xmlSetProp(gambar_aktif_alarm_max, (const xmlChar*) "name", (const xmlChar*) "alarm_max");
	xmlNodeSetContent(gambar_aktif_alarm_max, (const xmlChar*) "80");
	
	xmlNodePtr gambar_aktif_alarm_max_max = node_gambar_aktif->children;	
	gambar_aktif_alarm_max_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_alarm_max_max = xmlDocCopyNode(gambar_aktif_alarm_max_max, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_alarm_max_max);
	xmlSetProp(gambar_aktif_alarm_max_max, (const xmlChar*) "name", (const xmlChar*) "alarm_max_max");
	xmlNodeSetContent(gambar_aktif_alarm_max_max, (const xmlChar*) "90");
	
	xmlNodePtr gambar_aktif_alarm_min_enable = node_gambar_aktif->children;	
	gambar_aktif_alarm_min_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_alarm_min_enable = xmlDocCopyNode(gambar_aktif_alarm_min_enable, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_alarm_min_enable);
	xmlSetProp(gambar_aktif_alarm_min_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_min_enable");
	xmlNodeSetContent(gambar_aktif_alarm_min_enable, (const xmlChar*) "1");
	
	xmlNodePtr gambar_aktif_alarm_min_min_enable = node_gambar_aktif->children;	
	gambar_aktif_alarm_min_min_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_alarm_min_min_enable = xmlDocCopyNode(gambar_aktif_alarm_min_min_enable, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_alarm_min_min_enable);
	xmlSetProp(gambar_aktif_alarm_min_min_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_min_min_enable");
	xmlNodeSetContent(gambar_aktif_alarm_min_min_enable, (const xmlChar*) "1");
	
	xmlNodePtr gambar_aktif_alarm_max_enable = node_gambar_aktif->children;	
	gambar_aktif_alarm_max_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_alarm_max_enable = xmlDocCopyNode(gambar_aktif_alarm_max_enable, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_alarm_max_enable);
	xmlSetProp(gambar_aktif_alarm_max_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_max_enable");
	xmlNodeSetContent(gambar_aktif_alarm_max_enable, (const xmlChar*) "1");
	
	xmlNodePtr gambar_aktif_alarm_max_max_enable = node_gambar_aktif->children;	
	gambar_aktif_alarm_max_max_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_alarm_max_max_enable = xmlDocCopyNode(gambar_aktif_alarm_max_max_enable, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_alarm_max_max_enable);
	xmlSetProp(gambar_aktif_alarm_max_max_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_max_max_enable");
	xmlNodeSetContent(gambar_aktif_alarm_max_max_enable, (const xmlChar*) "1");
	
	xmlNodePtr gambar_aktif_source = node_gambar_aktif->children;	
	gambar_aktif_source = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_source = xmlDocCopyNode(gambar_aktif_source, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_source);
	xmlSetProp(gambar_aktif_source, (const xmlChar*) "name", (const xmlChar*) "source");
	xmlNodeSetContent(gambar_aktif_source, (const xmlChar*) "valve.png");
	
	xmlNodePtr gambar_aktif_source_min_min = node_gambar_aktif->children;	
	gambar_aktif_source_min_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_source_min_min = xmlDocCopyNode(gambar_aktif_source_min_min, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_source_min_min);
	xmlSetProp(gambar_aktif_source_min_min, (const xmlChar*) "name", (const xmlChar*) "source_min_min");
	xmlNodeSetContent(gambar_aktif_source_min_min, (const xmlChar*) "valve.png");
	
	xmlNodePtr gambar_aktif_source_min = node_gambar_aktif->children;	
	gambar_aktif_source_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_source_min = xmlDocCopyNode(gambar_aktif_source_min, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_source_min);
	xmlSetProp(gambar_aktif_source_min, (const xmlChar*) "name", (const xmlChar*) "source_min");
	xmlNodeSetContent(gambar_aktif_source_min, (const xmlChar*) "valve.png");
	
	xmlNodePtr gambar_aktif_source_max = node_gambar_aktif->children;	
	gambar_aktif_source_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_source_max = xmlDocCopyNode(gambar_aktif_source_max, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_source_max);
	xmlSetProp(gambar_aktif_source_max, (const xmlChar*) "name", (const xmlChar*) "source_max");
	xmlNodeSetContent(gambar_aktif_source_max, (const xmlChar*) "valve.png");
	
	xmlNodePtr gambar_aktif_source_max_max = node_gambar_aktif->children;	
	gambar_aktif_source_max_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_source_max_max = xmlDocCopyNode(gambar_aktif_source_max_max, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_source_max_max);
	xmlSetProp(gambar_aktif_source_max_max, (const xmlChar*) "name", (const xmlChar*) "source_max_max");
	xmlNodeSetContent(gambar_aktif_source_max_max, (const xmlChar*) "valve.png");
	
	xmlNodePtr gambar_aktif_source_oor = node_gambar_aktif->children;	
	gambar_aktif_source_oor = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_source_oor = xmlDocCopyNode(gambar_aktif_source_oor, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_source_oor);
	xmlSetProp(gambar_aktif_source_oor, (const xmlChar*) "name", (const xmlChar*) "source_oor");
	xmlNodeSetContent(gambar_aktif_source_oor, (const xmlChar*) "valve.png");

	//child
	
	xmlNodePtr gambar_aktif_child = node_gambar_aktif->children;	
	gambar_aktif_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	gambar_aktif_child = xmlDocCopyNode(gambar_aktif_child, doc, 1);
	xmlAddChild(node_gambar_aktif, gambar_aktif_child);
	
	//posisi
	
	xmlNodePtr gambar_aktif_posisi = gambar_aktif_child->children;
	gambar_aktif_posisi = xmlNewNode(NULL, (const xmlChar*)"object");	
	gambar_aktif_posisi = xmlDocCopyNode(gambar_aktif_posisi, doc, 1);
	xmlAddChild(gambar_aktif_child, gambar_aktif_posisi);
	xmlSetProp(gambar_aktif_posisi, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(gambar_aktif_posisi, (const xmlChar*) "id", (const xmlChar*) "posisi" );
	
	xmlNodePtr gambar_aktif_posisi_x = gambar_aktif_posisi->children;	
	gambar_aktif_posisi_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_posisi_x = xmlDocCopyNode(gambar_aktif_posisi_x, doc, 1);
	xmlAddChild(gambar_aktif_posisi, gambar_aktif_posisi_x);
	xmlSetProp(gambar_aktif_posisi_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(gambar_aktif_posisi_x, (const xmlChar*) "0");
	
	xmlNodePtr gambar_aktif_posisi_y = gambar_aktif_posisi->children;	
	gambar_aktif_posisi_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	gambar_aktif_posisi_y = xmlDocCopyNode(gambar_aktif_posisi_y, doc, 1);
	xmlAddChild(gambar_aktif_posisi, gambar_aktif_posisi_y);
	xmlSetProp(gambar_aktif_posisi_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(gambar_aktif_posisi_y, (const xmlChar*) "0");
}

void EditorWindow::new_xml_teks()
{
	char tks_baru[32];
	
	int x = 0;
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks") == 0))
			{
				x++;
			}
		}
	}
	
	if(x == 0)
		strcpy(tks_baru, "new_teks#1");
	else
	{
		int n = 2;
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks") == 0))
				{
					char tks_name[32];
					sprintf(tks_name, "%s%d", "new_teks#", n);
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), tks_name) == 0)
						n++;
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), tks_name) != 0)
					{	
						strcpy(tks_baru, tks_name);
						n++;
					}
				}
			}
		}
	}
	nodeLevel1 = doc->children;	
	
	xmlNodePtr node_teks = nodeLevel1->children;
	node_teks = xmlNewNode(NULL, (const xmlChar*)"object");	
	node_teks = xmlDocCopyNode(node_teks, doc, 1);
	xmlAddChild(nodeLevel1, node_teks);
	xmlSetProp(node_teks, (const xmlChar*) "struct", (const xmlChar*) "obj_teks" );
	xmlSetProp(node_teks, (const xmlChar*) "id", (const xmlChar*)  tks_baru);
		
	xmlNodePtr teks_isi_teks = node_teks->children;	
	teks_isi_teks = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_isi_teks = xmlDocCopyNode(teks_isi_teks, doc, 1);
	xmlAddChild(node_teks, teks_isi_teks);
	xmlSetProp(teks_isi_teks, (const xmlChar*) "name", (const xmlChar*) "isi_teks");
	xmlNodeSetContent(teks_isi_teks, (const xmlChar*) "teks baru");
	
	xmlNodePtr teks_size = node_teks->children;	
	teks_size = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_size = xmlDocCopyNode(teks_size, doc, 1);
	xmlAddChild(node_teks, teks_size);
	xmlSetProp(teks_size, (const xmlChar*) "name", (const xmlChar*) "size");
	xmlNodeSetContent(teks_size, (const xmlChar*) "12");
	
	xmlNodePtr teks_font = node_teks->children;	
	teks_font = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_font = xmlDocCopyNode(teks_font, doc, 1);
	xmlAddChild(node_teks, teks_font);
	xmlSetProp(teks_font, (const xmlChar*) "name", (const xmlChar*) "font");
	xmlNodeSetContent(teks_font, (const xmlChar*) "Arial");

	//child
	
	xmlNodePtr teks_child = node_teks->children;	
	teks_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	teks_child = xmlDocCopyNode(teks_child, doc, 1);
	xmlAddChild(node_teks, teks_child);
	
	//posisi
	
	xmlNodePtr teks_posisi = teks_child->children;
	teks_posisi = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_posisi = xmlDocCopyNode(teks_posisi, doc, 1);
	xmlAddChild(teks_child, teks_posisi);
	xmlSetProp(teks_posisi, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(teks_posisi, (const xmlChar*) "id", (const xmlChar*) "posisi" );
	
	xmlNodePtr teks_posisi_x = teks_posisi->children;	
	teks_posisi_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_posisi_x = xmlDocCopyNode(teks_posisi_x, doc, 1);
	xmlAddChild(teks_posisi, teks_posisi_x);
	xmlSetProp(teks_posisi_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(teks_posisi_x, (const xmlChar*) "0");
	
	xmlNodePtr teks_posisi_y = teks_posisi->children;	
	teks_posisi_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_posisi_y = xmlDocCopyNode(teks_posisi_y, doc, 1);
	xmlAddChild(teks_posisi, teks_posisi_y);
	xmlSetProp(teks_posisi_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(teks_posisi_y, (const xmlChar*) "0");
	
	//warna fill
	
	xmlNodePtr teks_warna_fill = teks_child->children;
	teks_warna_fill = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_warna_fill = xmlDocCopyNode(teks_warna_fill, doc, 1);
	xmlAddChild(teks_child, teks_warna_fill);
	xmlSetProp(teks_warna_fill, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(teks_warna_fill, (const xmlChar*) "id", (const xmlChar*) "warna_fill" );
	
	xmlNodePtr teks_warna_fill_r = teks_warna_fill->children;	
	teks_warna_fill_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_warna_fill_r = xmlDocCopyNode(teks_warna_fill_r, doc, 1);
	xmlAddChild(teks_warna_fill, teks_warna_fill_r);
	xmlSetProp(teks_warna_fill_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(teks_warna_fill_r, (const xmlChar*) "100");
	
	xmlNodePtr teks_warna_fill_g = teks_warna_fill->children;	
	teks_warna_fill_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_warna_fill_g = xmlDocCopyNode(teks_warna_fill_g, doc, 1);
	xmlAddChild(teks_warna_fill, teks_warna_fill_g);
	xmlSetProp(teks_warna_fill_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(teks_warna_fill_g, (const xmlChar*) "100");
	
	xmlNodePtr teks_warna_fill_b = teks_warna_fill->children;	
	teks_warna_fill_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_warna_fill_b = xmlDocCopyNode(teks_warna_fill_b, doc, 1);
	xmlAddChild(teks_warna_fill, teks_warna_fill_b);
	xmlSetProp(teks_warna_fill_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(teks_warna_fill_b, (const xmlChar*) "100");
	
	xmlNodePtr teks_warna_fill_a = teks_warna_fill->children;	
	teks_warna_fill_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_warna_fill_a = xmlDocCopyNode(teks_warna_fill_a, doc, 1);
	xmlAddChild(teks_warna_fill, teks_warna_fill_a);
	xmlSetProp(teks_warna_fill_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(teks_warna_fill_a, (const xmlChar*) "100");
	
	//warna outline
	
	xmlNodePtr teks_warna_outline = teks_child->children;
	teks_warna_outline = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_warna_outline = xmlDocCopyNode(teks_warna_outline, doc, 1);
	xmlAddChild(teks_child, teks_warna_outline);
	xmlSetProp(teks_warna_outline, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(teks_warna_outline, (const xmlChar*) "id", (const xmlChar*) "warna_outline" );
	
	xmlNodePtr teks_warna_outline_r = teks_warna_outline->children;	
	teks_warna_outline_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_warna_outline_r = xmlDocCopyNode(teks_warna_outline_r, doc, 1);
	xmlAddChild(teks_warna_outline, teks_warna_outline_r);
	xmlSetProp(teks_warna_outline_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(teks_warna_outline_r, (const xmlChar*) "100");
	
	xmlNodePtr teks_warna_outline_g = teks_warna_outline->children;	
	teks_warna_outline_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_warna_outline_g = xmlDocCopyNode(teks_warna_outline_g, doc, 1);
	xmlAddChild(teks_warna_outline, teks_warna_outline_g);
	xmlSetProp(teks_warna_outline_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(teks_warna_outline_g, (const xmlChar*) "100");
	
	xmlNodePtr teks_warna_outline_b = teks_warna_outline->children;	
	teks_warna_outline_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_warna_outline_b = xmlDocCopyNode(teks_warna_outline_b, doc, 1);
	xmlAddChild(teks_warna_outline, teks_warna_outline_b);
	xmlSetProp(teks_warna_outline_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(teks_warna_outline_b, (const xmlChar*) "100");
	
	xmlNodePtr teks_warna_outline_a = teks_warna_outline->children;	
	teks_warna_outline_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_warna_outline_a = xmlDocCopyNode(teks_warna_outline_a, doc, 1);
	xmlAddChild(teks_warna_outline, teks_warna_outline_a);
	xmlSetProp(teks_warna_outline_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(teks_warna_outline_a, (const xmlChar*) "100");
}

void EditorWindow::new_xml_teks_aktif()
{
	char tks_aktif_baru[32];
	
	int x = 0;
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0))
			{
				x++;
			}
		}
	}
	
	if(x == 0)
		strcpy(tks_aktif_baru, "new_teks_aktif#1");
	else
	{
		int n = 2;
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_teks_aktif") == 0))
				{
					char tks_aktif_name[32];
					sprintf(tks_aktif_name, "%s%d", "new_teks_aktif#", n);
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), tks_aktif_name) == 0)
						n++;
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), tks_aktif_name) != 0)
					{	
						strcpy(tks_aktif_baru, tks_aktif_name);
						n++;
					}
				}
			}
		}
	}
	nodeLevel1 = doc->children;	
	
	xmlNodePtr node_teks_aktif = nodeLevel1->children;
	node_teks_aktif = xmlNewNode(NULL, (const xmlChar*)"object");	
	node_teks_aktif = xmlDocCopyNode(node_teks_aktif, doc, 1);
	xmlAddChild(nodeLevel1, node_teks_aktif);
	xmlSetProp(node_teks_aktif, (const xmlChar*) "struct", (const xmlChar*) "obj_teks_aktif" );
	xmlSetProp(node_teks_aktif, (const xmlChar*) "id", (const xmlChar*)  tks_aktif_baru);
		
	xmlNodePtr teks_aktif_id_titik_data = node_teks_aktif->children;	
	teks_aktif_id_titik_data = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_id_titik_data = xmlDocCopyNode(teks_aktif_id_titik_data, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_id_titik_data);
	xmlSetProp(teks_aktif_id_titik_data, (const xmlChar*) "name", (const xmlChar*) "id_titik_data");
	xmlNodeSetContent(teks_aktif_id_titik_data, (const xmlChar*) "10");
	
	xmlNodePtr teks_aktif_size = node_teks_aktif->children;	
	teks_aktif_size = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_size = xmlDocCopyNode(teks_aktif_size, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_size);
	xmlSetProp(teks_aktif_size, (const xmlChar*) "name", (const xmlChar*) "size");
	xmlNodeSetContent(teks_aktif_size, (const xmlChar*) "12");
	
	xmlNodePtr teks_aktif_font = node_teks_aktif->children;	
	teks_aktif_font = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_font = xmlDocCopyNode(teks_aktif_font, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_font);
	xmlSetProp(teks_aktif_font, (const xmlChar*) "name", (const xmlChar*) "font");
	xmlNodeSetContent(teks_aktif_font, (const xmlChar*) "Arial");
	
	xmlNodePtr teks_aktif_caption = node_teks_aktif->children;	
	teks_aktif_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_caption = xmlDocCopyNode(teks_aktif_caption, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_caption);
	xmlSetProp(teks_aktif_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(teks_aktif_caption, (const xmlChar*) "teks aktif");
		
	xmlNodePtr teks_aktif_range_min = node_teks_aktif->children;	
	teks_aktif_range_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_range_min = xmlDocCopyNode(teks_aktif_range_min, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_range_min);
	xmlSetProp(teks_aktif_range_min, (const xmlChar*) "name", (const xmlChar*) "range_min");
	xmlNodeSetContent(teks_aktif_range_min, (const xmlChar*) "0");
	
	xmlNodePtr teks_aktif_range_max = node_teks_aktif->children;	
	teks_aktif_range_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_range_max = xmlDocCopyNode(teks_aktif_range_max, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_range_max);
	xmlSetProp(teks_aktif_range_max, (const xmlChar*) "name", (const xmlChar*) "range_max");
	xmlNodeSetContent(teks_aktif_range_max, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_alarm_min_min = node_teks_aktif->children;	
	teks_aktif_alarm_min_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_alarm_min_min = xmlDocCopyNode(teks_aktif_alarm_min_min, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_alarm_min_min);
	xmlSetProp(teks_aktif_alarm_min_min, (const xmlChar*) "name", (const xmlChar*) "alarm_min_min");
	xmlNodeSetContent(teks_aktif_alarm_min_min, (const xmlChar*) "10");
	
	xmlNodePtr teks_aktif_alarm_min = node_teks_aktif->children;	
	teks_aktif_alarm_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_alarm_min = xmlDocCopyNode(teks_aktif_alarm_min, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_alarm_min);
	xmlSetProp(teks_aktif_alarm_min, (const xmlChar*) "name", (const xmlChar*) "alarm_min");
	xmlNodeSetContent(teks_aktif_alarm_min, (const xmlChar*) "20");
	
	xmlNodePtr teks_aktif_alarm_max = node_teks_aktif->children;	
	teks_aktif_alarm_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_alarm_max = xmlDocCopyNode(teks_aktif_alarm_max, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_alarm_max);
	xmlSetProp(teks_aktif_alarm_max, (const xmlChar*) "name", (const xmlChar*) "alarm_max");
	xmlNodeSetContent(teks_aktif_alarm_max, (const xmlChar*) "80");
	
	xmlNodePtr teks_aktif_alarm_max_max = node_teks_aktif->children;	
	teks_aktif_alarm_max_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_alarm_max_max = xmlDocCopyNode(teks_aktif_alarm_max_max, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_alarm_max_max);
	xmlSetProp(teks_aktif_alarm_max_max, (const xmlChar*) "name", (const xmlChar*) "alarm_max_max");
	xmlNodeSetContent(teks_aktif_alarm_max_max, (const xmlChar*) "90");
	
	xmlNodePtr teks_aktif_alarm_min_min_enable = node_teks_aktif->children;	
	teks_aktif_alarm_min_min_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_alarm_min_min_enable = xmlDocCopyNode(teks_aktif_alarm_min_min_enable, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_alarm_min_min_enable);
	xmlSetProp(teks_aktif_alarm_min_min_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_min_min_enable");
	xmlNodeSetContent(teks_aktif_alarm_min_min_enable, (const xmlChar*) "1");
	
	xmlNodePtr teks_aktif_alarm_min_enable = node_teks_aktif->children;	
	teks_aktif_alarm_min_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_alarm_min_enable = xmlDocCopyNode(teks_aktif_alarm_min_enable, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_alarm_min_enable);
	xmlSetProp(teks_aktif_alarm_min_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_min_enable");
	xmlNodeSetContent(teks_aktif_alarm_min_enable, (const xmlChar*) "1");
	
	xmlNodePtr teks_aktif_alarm_max_enable = node_teks_aktif->children;	
	teks_aktif_alarm_max_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_alarm_max_enable = xmlDocCopyNode(teks_aktif_alarm_max_enable, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_alarm_max_enable);
	xmlSetProp(teks_aktif_alarm_max_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_max_enable");
	xmlNodeSetContent(teks_aktif_alarm_max_enable, (const xmlChar*) "1");
	
	xmlNodePtr teks_aktif_alarm_max_max_enable = node_teks_aktif->children;	
	teks_aktif_alarm_max_max_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_alarm_max_max_enable = xmlDocCopyNode(teks_aktif_alarm_max_max_enable, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_alarm_max_max_enable);
	xmlSetProp(teks_aktif_alarm_max_max_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_max_max_enable");
	xmlNodeSetContent(teks_aktif_alarm_max_max_enable, (const xmlChar*) "1");
	
	//child
	
	xmlNodePtr teks_aktif_child = node_teks_aktif->children;	
	teks_aktif_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	teks_aktif_child = xmlDocCopyNode(teks_aktif_child, doc, 1);
	xmlAddChild(node_teks_aktif, teks_aktif_child);
	
	//posisi
	
	xmlNodePtr teks_aktif_posisi = teks_aktif_child->children;
	teks_aktif_posisi = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_aktif_posisi = xmlDocCopyNode(teks_aktif_posisi, doc, 1);
	xmlAddChild(teks_aktif_child, teks_aktif_posisi);
	xmlSetProp(teks_aktif_posisi, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(teks_aktif_posisi, (const xmlChar*) "id", (const xmlChar*) "posisi" );
	
	xmlNodePtr teks_aktif_posisi_x = teks_aktif_posisi->children;	
	teks_aktif_posisi_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_posisi_x = xmlDocCopyNode(teks_aktif_posisi_x, doc, 1);
	xmlAddChild(teks_aktif_posisi, teks_aktif_posisi_x);
	xmlSetProp(teks_aktif_posisi_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(teks_aktif_posisi_x, (const xmlChar*) "0");
	
	xmlNodePtr teks_aktif_posisi_y = teks_aktif_posisi->children;	
	teks_aktif_posisi_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_posisi_y = xmlDocCopyNode(teks_aktif_posisi_y, doc, 1);
	xmlAddChild(teks_aktif_posisi, teks_aktif_posisi_y);
	xmlSetProp(teks_aktif_posisi_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(teks_aktif_posisi_y, (const xmlChar*) "0");
	
	//warna fill
	
	xmlNodePtr teks_aktif_warna_fill = teks_aktif_child->children;
	teks_aktif_warna_fill = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_aktif_warna_fill = xmlDocCopyNode(teks_aktif_warna_fill, doc, 1);
	xmlAddChild(teks_aktif_child, teks_aktif_warna_fill);
	xmlSetProp(teks_aktif_warna_fill, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(teks_aktif_warna_fill, (const xmlChar*) "id", (const xmlChar*) "warna_fill" );
	
	xmlNodePtr teks_aktif_warna_fill_r = teks_aktif_warna_fill->children;	
	teks_aktif_warna_fill_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_fill_r = xmlDocCopyNode(teks_aktif_warna_fill_r, doc, 1);
	xmlAddChild(teks_aktif_warna_fill, teks_aktif_warna_fill_r);
	xmlSetProp(teks_aktif_warna_fill_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(teks_aktif_warna_fill_r, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_fill_g = teks_aktif_warna_fill->children;	
	teks_aktif_warna_fill_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_fill_g = xmlDocCopyNode(teks_aktif_warna_fill_g, doc, 1);
	xmlAddChild(teks_aktif_warna_fill, teks_aktif_warna_fill_g);
	xmlSetProp(teks_aktif_warna_fill_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(teks_aktif_warna_fill_g, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_fill_b = teks_aktif_warna_fill->children;	
	teks_aktif_warna_fill_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_fill_b = xmlDocCopyNode(teks_aktif_warna_fill_b, doc, 1);
	xmlAddChild(teks_aktif_warna_fill, teks_aktif_warna_fill_b);
	xmlSetProp(teks_aktif_warna_fill_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(teks_aktif_warna_fill_b, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_fill_a = teks_aktif_warna_fill->children;	
	teks_aktif_warna_fill_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_fill_a = xmlDocCopyNode(teks_aktif_warna_fill_a, doc, 1);
	xmlAddChild(teks_aktif_warna_fill, teks_aktif_warna_fill_a);
	xmlSetProp(teks_aktif_warna_fill_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(teks_aktif_warna_fill_a, (const xmlChar*) "100");
	
	//warna outline
	
	xmlNodePtr teks_aktif_warna_outline = teks_aktif_child->children;
	teks_aktif_warna_outline = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_aktif_warna_outline = xmlDocCopyNode(teks_aktif_warna_outline, doc, 1);
	xmlAddChild(teks_aktif_child, teks_aktif_warna_outline);
	xmlSetProp(teks_aktif_warna_outline, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(teks_aktif_warna_outline, (const xmlChar*) "id", (const xmlChar*) "warna_outline" );
	
	xmlNodePtr teks_aktif_warna_outline_r = teks_aktif_warna_outline->children;	
	teks_aktif_warna_outline_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_outline_r = xmlDocCopyNode(teks_aktif_warna_outline_r, doc, 1);
	xmlAddChild(teks_aktif_warna_outline, teks_aktif_warna_outline_r);
	xmlSetProp(teks_aktif_warna_outline_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(teks_aktif_warna_outline_r, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_outline_g = teks_aktif_warna_outline->children;	
	teks_aktif_warna_outline_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_outline_g = xmlDocCopyNode(teks_aktif_warna_outline_g, doc, 1);
	xmlAddChild(teks_aktif_warna_outline, teks_aktif_warna_outline_g);
	xmlSetProp(teks_aktif_warna_outline_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(teks_aktif_warna_outline_g, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_outline_b = teks_aktif_warna_outline->children;	
	teks_aktif_warna_outline_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_outline_b = xmlDocCopyNode(teks_aktif_warna_outline_b, doc, 1);
	xmlAddChild(teks_aktif_warna_outline, teks_aktif_warna_outline_b);
	xmlSetProp(teks_aktif_warna_outline_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(teks_aktif_warna_outline_b, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_outline_a = teks_aktif_warna_outline->children;	
	teks_aktif_warna_outline_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_outline_a = xmlDocCopyNode(teks_aktif_warna_outline_a, doc, 1);
	xmlAddChild(teks_aktif_warna_outline, teks_aktif_warna_outline_a);
	xmlSetProp(teks_aktif_warna_outline_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(teks_aktif_warna_outline_a, (const xmlChar*) "100");
	
	//warna
	
	xmlNodePtr teks_aktif_warna = teks_aktif_child->children;
	teks_aktif_warna = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_aktif_warna = xmlDocCopyNode(teks_aktif_warna, doc, 1);
	xmlAddChild(teks_aktif_child, teks_aktif_warna);
	xmlSetProp(teks_aktif_warna, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(teks_aktif_warna, (const xmlChar*) "id", (const xmlChar*) "warna" );
	
	xmlNodePtr teks_aktif_warna_r = teks_aktif_warna->children;	
	teks_aktif_warna_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_r = xmlDocCopyNode(teks_aktif_warna_r, doc, 1);
	xmlAddChild(teks_aktif_warna, teks_aktif_warna_r);
	xmlSetProp(teks_aktif_warna_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(teks_aktif_warna_r, (const xmlChar*) "0");
	
	xmlNodePtr teks_aktif_warna_g = teks_aktif_warna->children;	
	teks_aktif_warna_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_g = xmlDocCopyNode(teks_aktif_warna_g, doc, 1);
	xmlAddChild(teks_aktif_warna, teks_aktif_warna_g);
	xmlSetProp(teks_aktif_warna_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(teks_aktif_warna_g, (const xmlChar*) "250");
	
	xmlNodePtr teks_aktif_warna_b = teks_aktif_warna->children;	
	teks_aktif_warna_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_b = xmlDocCopyNode(teks_aktif_warna_b, doc, 1);
	xmlAddChild(teks_aktif_warna, teks_aktif_warna_b);
	xmlSetProp(teks_aktif_warna_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(teks_aktif_warna_b, (const xmlChar*) "0");
	
	xmlNodePtr teks_aktif_warna_a = teks_aktif_warna->children;	
	teks_aktif_warna_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_a = xmlDocCopyNode(teks_aktif_warna_a, doc, 1);
	xmlAddChild(teks_aktif_warna, teks_aktif_warna_a);
	xmlSetProp(teks_aktif_warna_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(teks_aktif_warna_a, (const xmlChar*) "100");
	
	//warna_min_min
	
	xmlNodePtr teks_aktif_warna_min_min = teks_aktif_child->children;
	teks_aktif_warna_min_min = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_aktif_warna_min_min = xmlDocCopyNode(teks_aktif_warna_min_min, doc, 1);
	xmlAddChild(teks_aktif_child, teks_aktif_warna_min_min);
	xmlSetProp(teks_aktif_warna_min_min, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(teks_aktif_warna_min_min, (const xmlChar*) "id", (const xmlChar*) "warna_min_min" );
	
	xmlNodePtr teks_aktif_warna_min_min_r = teks_aktif_warna_min_min->children;	
	teks_aktif_warna_min_min_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_min_min_r = xmlDocCopyNode(teks_aktif_warna_min_min_r, doc, 1);
	xmlAddChild(teks_aktif_warna_min_min, teks_aktif_warna_min_min_r);
	xmlSetProp(teks_aktif_warna_min_min_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(teks_aktif_warna_min_min_r, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_min_min_g = teks_aktif_warna_min_min->children;	
	teks_aktif_warna_min_min_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_min_min_g = xmlDocCopyNode(teks_aktif_warna_min_min_g, doc, 1);
	xmlAddChild(teks_aktif_warna_min_min, teks_aktif_warna_min_min_g);
	xmlSetProp(teks_aktif_warna_min_min_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(teks_aktif_warna_min_min_g, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_min_min_b = teks_aktif_warna_min_min->children;	
	teks_aktif_warna_min_min_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_min_min_b = xmlDocCopyNode(teks_aktif_warna_min_min_b, doc, 1);
	xmlAddChild(teks_aktif_warna_min_min, teks_aktif_warna_min_min_b);
	xmlSetProp(teks_aktif_warna_min_min_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(teks_aktif_warna_min_min_b, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_min_min_a = teks_aktif_warna_min_min->children;	
	teks_aktif_warna_min_min_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_min_min_a = xmlDocCopyNode(teks_aktif_warna_min_min_a, doc, 1);
	xmlAddChild(teks_aktif_warna_min_min, teks_aktif_warna_min_min_a);
	xmlSetProp(teks_aktif_warna_min_min_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(teks_aktif_warna_min_min_a, (const xmlChar*) "100");
	
	//warna_min
	
	xmlNodePtr teks_aktif_warna_min = teks_aktif_child->children;
	teks_aktif_warna_min = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_aktif_warna_min = xmlDocCopyNode(teks_aktif_warna_min, doc, 1);
	xmlAddChild(teks_aktif_child, teks_aktif_warna_min);
	xmlSetProp(teks_aktif_warna_min, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(teks_aktif_warna_min, (const xmlChar*) "id", (const xmlChar*) "warna_min" );
	
	xmlNodePtr teks_aktif_warna_min_r = teks_aktif_warna_min->children;	
	teks_aktif_warna_min_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_min_r = xmlDocCopyNode(teks_aktif_warna_min_r, doc, 1);
	xmlAddChild(teks_aktif_warna_min, teks_aktif_warna_min_r);
	xmlSetProp(teks_aktif_warna_min_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(teks_aktif_warna_min_r, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_min_g = teks_aktif_warna_min->children;	
	teks_aktif_warna_min_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_min_g = xmlDocCopyNode(teks_aktif_warna_min_g, doc, 1);
	xmlAddChild(teks_aktif_warna_min, teks_aktif_warna_min_g);
	xmlSetProp(teks_aktif_warna_min_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(teks_aktif_warna_min_g, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_min_b = teks_aktif_warna_min->children;	
	teks_aktif_warna_min_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_min_b = xmlDocCopyNode(teks_aktif_warna_min_b, doc, 1);
	xmlAddChild(teks_aktif_warna_min, teks_aktif_warna_min_b);
	xmlSetProp(teks_aktif_warna_min_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(teks_aktif_warna_min_b, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_min_a = teks_aktif_warna_min->children;	
	teks_aktif_warna_min_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_min_a = xmlDocCopyNode(teks_aktif_warna_min_a, doc, 1);
	xmlAddChild(teks_aktif_warna_min, teks_aktif_warna_min_a);
	xmlSetProp(teks_aktif_warna_min_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(teks_aktif_warna_min_a, (const xmlChar*) "100");
	
	//warna_max
	
	xmlNodePtr teks_aktif_warna_max = teks_aktif_child->children;
	teks_aktif_warna_max = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_aktif_warna_max = xmlDocCopyNode(teks_aktif_warna_max, doc, 1);
	xmlAddChild(teks_aktif_child, teks_aktif_warna_max);
	xmlSetProp(teks_aktif_warna_max, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(teks_aktif_warna_max, (const xmlChar*) "id", (const xmlChar*) "warna_max" );
	
	xmlNodePtr teks_aktif_warna_max_r = teks_aktif_warna_max->children;	
	teks_aktif_warna_max_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_max_r = xmlDocCopyNode(teks_aktif_warna_max_r, doc, 1);
	xmlAddChild(teks_aktif_warna_max, teks_aktif_warna_max_r);
	xmlSetProp(teks_aktif_warna_max_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(teks_aktif_warna_max_r, (const xmlChar*) "255");
	
	xmlNodePtr teks_aktif_warna_max_g = teks_aktif_warna_max->children;	
	teks_aktif_warna_max_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_max_g = xmlDocCopyNode(teks_aktif_warna_max_g, doc, 1);
	xmlAddChild(teks_aktif_warna_max, teks_aktif_warna_max_g);
	xmlSetProp(teks_aktif_warna_max_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(teks_aktif_warna_max_g, (const xmlChar*) "255");
	
	xmlNodePtr teks_aktif_warna_max_b = teks_aktif_warna_max->children;	
	teks_aktif_warna_max_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_max_b = xmlDocCopyNode(teks_aktif_warna_max_b, doc, 1);
	xmlAddChild(teks_aktif_warna_max, teks_aktif_warna_max_b);
	xmlSetProp(teks_aktif_warna_max_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(teks_aktif_warna_max_b, (const xmlChar*) "0");
	
	xmlNodePtr teks_aktif_warna_max_a = teks_aktif_warna_max->children;	
	teks_aktif_warna_max_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_max_a = xmlDocCopyNode(teks_aktif_warna_max_a, doc, 1);
	xmlAddChild(teks_aktif_warna_max, teks_aktif_warna_max_a);
	xmlSetProp(teks_aktif_warna_max_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(teks_aktif_warna_max_a, (const xmlChar*) "100");
	
	//warna_max_max
	
	xmlNodePtr teks_aktif_warna_max_max = teks_aktif_child->children;
	teks_aktif_warna_max_max = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_aktif_warna_max_max = xmlDocCopyNode(teks_aktif_warna_max_max, doc, 1);
	xmlAddChild(teks_aktif_child, teks_aktif_warna_max_max);
	xmlSetProp(teks_aktif_warna_max_max, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(teks_aktif_warna_max_max, (const xmlChar*) "id", (const xmlChar*) "warna_max_max" );
	
	xmlNodePtr teks_aktif_warna_max_max_r = teks_aktif_warna_max_max->children;	
	teks_aktif_warna_max_max_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_max_max_r = xmlDocCopyNode(teks_aktif_warna_max_max_r, doc, 1);
	xmlAddChild(teks_aktif_warna_max_max, teks_aktif_warna_max_max_r);
	xmlSetProp(teks_aktif_warna_max_max_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(teks_aktif_warna_max_max_r, (const xmlChar*) "250");
	
	xmlNodePtr teks_aktif_warna_max_max_g = teks_aktif_warna_max_max->children;	
	teks_aktif_warna_max_max_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_max_max_g = xmlDocCopyNode(teks_aktif_warna_max_max_g, doc, 1);
	xmlAddChild(teks_aktif_warna_max_max, teks_aktif_warna_max_max_g);
	xmlSetProp(teks_aktif_warna_max_max_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(teks_aktif_warna_max_max_g, (const xmlChar*) "0");
	
	xmlNodePtr teks_aktif_warna_max_max_b = teks_aktif_warna_max_max->children;	
	teks_aktif_warna_max_max_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_max_max_b = xmlDocCopyNode(teks_aktif_warna_max_max_b, doc, 1);
	xmlAddChild(teks_aktif_warna_max_max, teks_aktif_warna_max_max_b);
	xmlSetProp(teks_aktif_warna_max_max_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(teks_aktif_warna_max_max_b, (const xmlChar*) "0");
	
	xmlNodePtr teks_aktif_warna_max_max_a = teks_aktif_warna_max_max->children;	
	teks_aktif_warna_max_max_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_max_max_a = xmlDocCopyNode(teks_aktif_warna_max_max_a, doc, 1);
	xmlAddChild(teks_aktif_warna_max_max, teks_aktif_warna_max_max_a);
	xmlSetProp(teks_aktif_warna_max_max_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(teks_aktif_warna_max_max_a, (const xmlChar*) "100");
	
	//warna_oor
	
	xmlNodePtr teks_aktif_warna_oor = teks_aktif_child->children;
	teks_aktif_warna_oor = xmlNewNode(NULL, (const xmlChar*)"object");	
	teks_aktif_warna_oor = xmlDocCopyNode(teks_aktif_warna_oor, doc, 1);
	xmlAddChild(teks_aktif_child, teks_aktif_warna_oor);
	xmlSetProp(teks_aktif_warna_oor, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(teks_aktif_warna_oor, (const xmlChar*) "id", (const xmlChar*) "warna_oor" );
	
	xmlNodePtr teks_aktif_warna_oor_r = teks_aktif_warna_oor->children;	
	teks_aktif_warna_oor_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_oor_r = xmlDocCopyNode(teks_aktif_warna_oor_r, doc, 1);
	xmlAddChild(teks_aktif_warna_oor, teks_aktif_warna_oor_r);
	xmlSetProp(teks_aktif_warna_oor_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(teks_aktif_warna_oor_r, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_oor_g = teks_aktif_warna_oor->children;	
	teks_aktif_warna_oor_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_oor_g = xmlDocCopyNode(teks_aktif_warna_oor_g, doc, 1);
	xmlAddChild(teks_aktif_warna_oor, teks_aktif_warna_oor_g);
	xmlSetProp(teks_aktif_warna_oor_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(teks_aktif_warna_oor_g, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_oor_b = teks_aktif_warna_oor->children;	
	teks_aktif_warna_oor_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_oor_b = xmlDocCopyNode(teks_aktif_warna_oor_b, doc, 1);
	xmlAddChild(teks_aktif_warna_oor, teks_aktif_warna_oor_b);
	xmlSetProp(teks_aktif_warna_oor_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(teks_aktif_warna_oor_b, (const xmlChar*) "100");
	
	xmlNodePtr teks_aktif_warna_oor_a = teks_aktif_warna_oor->children;	
	teks_aktif_warna_oor_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	teks_aktif_warna_oor_a = xmlDocCopyNode(teks_aktif_warna_oor_a, doc, 1);
	xmlAddChild(teks_aktif_warna_oor, teks_aktif_warna_oor_a);
	xmlSetProp(teks_aktif_warna_oor_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(teks_aktif_warna_oor_a, (const xmlChar*) "100");
}

void EditorWindow::new_xml_animasi()
{
	char anm_baru[32];
	
	int x = 0;
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_animasi") == 0))
			{
				x++;
			}
		}
	}
	
	if(x == 0)
		strcpy(anm_baru, "new_animasi#1");
	else
	{
		int n = 2;
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_animasi") == 0))
				{
					char anm_name[32];
					sprintf(anm_name, "%s%d", "new_animasi#", n);
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), anm_name) == 0)
						n++;
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), anm_name) != 0)
					{	
						strcpy(anm_baru, anm_name);
						n++;
					}
				}
			}
		}
	}
	nodeLevel1 = doc->children;	
	
	xmlNodePtr node_animasi = nodeLevel1->children;
	node_animasi = xmlNewNode(NULL, (const xmlChar*)"object");	
	node_animasi = xmlDocCopyNode(node_animasi, doc, 1);
	xmlAddChild(nodeLevel1, node_animasi);
	xmlSetProp(node_animasi, (const xmlChar*) "struct", (const xmlChar*) "obj_animasi" );
	xmlSetProp(node_animasi, (const xmlChar*) "id", (const xmlChar*)  anm_baru);
	
	xmlNodePtr animasi_caption = node_animasi->children;	
	animasi_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_caption = xmlDocCopyNode(animasi_caption, doc, 1);
	xmlAddChild(node_animasi, animasi_caption);
	xmlSetProp(animasi_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(animasi_caption, (const xmlChar*) "animasi");
	
	xmlNodePtr animasi_frame_counts = node_animasi->children;	
	animasi_frame_counts = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_frame_counts = xmlDocCopyNode(animasi_frame_counts, doc, 1);
	xmlAddChild(node_animasi, animasi_frame_counts);
	xmlSetProp(animasi_frame_counts, (const xmlChar*) "name", (const xmlChar*) "frame_counts");
	xmlNodeSetContent(animasi_frame_counts, (const xmlChar*) "12");
	
	xmlNodePtr animasi_frame_rate = node_animasi->children;	
	animasi_frame_rate = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_frame_rate = xmlDocCopyNode(animasi_frame_rate, doc, 1);
	xmlAddChild(node_animasi, animasi_frame_rate);
	xmlSetProp(animasi_frame_rate, (const xmlChar*) "name", (const xmlChar*) "frame_rate");
	xmlNodeSetContent(animasi_frame_rate, (const xmlChar*) "12");

	//child
	
	xmlNodePtr animasi_child = node_animasi->children;	
	animasi_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	animasi_child = xmlDocCopyNode(animasi_child, doc, 1);
	xmlAddChild(node_animasi, animasi_child);
	
	//gambar1
	
	xmlNodePtr animasi_gambar1 = animasi_child->children;
	animasi_gambar1 = xmlNewNode(NULL, (const xmlChar*)"object");	
	animasi_gambar1 = xmlDocCopyNode(animasi_gambar1, doc, 1);
	xmlAddChild(animasi_child, animasi_gambar1);
	xmlSetProp(animasi_gambar1, (const xmlChar*) "struct", (const xmlChar*) "obj_gambar" );
	xmlSetProp(animasi_gambar1, (const xmlChar*) "id", (const xmlChar*) "gambar1" );
	
	xmlNodePtr animasi_gambar1_caption = animasi_gambar1->children;	
	animasi_gambar1_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar1_caption = xmlDocCopyNode(animasi_gambar1_caption, doc, 1);
	xmlAddChild(animasi_gambar1, animasi_gambar1_caption);
	xmlSetProp(animasi_gambar1_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(animasi_gambar1_caption, (const xmlChar*) "gambar#1");
	
	xmlNodePtr animasi_gambar1_source = animasi_gambar1->children;	
	animasi_gambar1_source = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar1_source = xmlDocCopyNode(animasi_gambar1_source, doc, 1);
	xmlAddChild(animasi_gambar1, animasi_gambar1_source);
	xmlSetProp(animasi_gambar1_source, (const xmlChar*) "name", (const xmlChar*) "source");
	xmlNodeSetContent(animasi_gambar1_source, (const xmlChar*) "valve.png");
	
	xmlNodePtr animasi_gambar1_width = animasi_gambar1->children;	
	animasi_gambar1_width = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar1_width = xmlDocCopyNode(animasi_gambar1_width, doc, 1);
	xmlAddChild(animasi_gambar1, animasi_gambar1_width);
	xmlSetProp(animasi_gambar1_width, (const xmlChar*) "name", (const xmlChar*) "width");
	xmlNodeSetContent(animasi_gambar1_width, (const xmlChar*) "100");
	
	xmlNodePtr animasi_gambar1_height = animasi_gambar1->children;	
	animasi_gambar1_height = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar1_height = xmlDocCopyNode(animasi_gambar1_height, doc, 1);
	xmlAddChild(animasi_gambar1, animasi_gambar1_height);
	xmlSetProp(animasi_gambar1_height, (const xmlChar*) "name", (const xmlChar*) "height");
	xmlNodeSetContent(animasi_gambar1_height, (const xmlChar*) "100");
	
	xmlNodePtr animasi_gambar1_skala = animasi_gambar1->children;	
	animasi_gambar1_skala = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar1_skala = xmlDocCopyNode(animasi_gambar1_skala, doc, 1);
	xmlAddChild(animasi_gambar1, animasi_gambar1_skala);
	xmlSetProp(animasi_gambar1_skala, (const xmlChar*) "name", (const xmlChar*) "skala");
	xmlNodeSetContent(animasi_gambar1_skala, (const xmlChar*) "1.0");
	
	xmlNodePtr animasi_gambar1_keterangan = animasi_gambar1->children;	
	animasi_gambar1_keterangan = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar1_keterangan = xmlDocCopyNode(animasi_gambar1_keterangan, doc, 1);
	xmlAddChild(animasi_gambar1, animasi_gambar1_keterangan);
	xmlSetProp(animasi_gambar1_keterangan, (const xmlChar*) "name", (const xmlChar*) "keterangan");
	xmlNodeSetContent(animasi_gambar1_keterangan, (const xmlChar*) "keterangan animasi_gambar1");

	//child
	
	xmlNodePtr animasi_gambar1_child = animasi_gambar1->children;	
	animasi_gambar1_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	animasi_gambar1_child = xmlDocCopyNode(animasi_gambar1_child, doc, 1);
	xmlAddChild(animasi_gambar1, animasi_gambar1_child);
	
	//posisi
	
	xmlNodePtr animasi_gambar1_posisi = animasi_gambar1_child->children;
	animasi_gambar1_posisi = xmlNewNode(NULL, (const xmlChar*)"object");	
	animasi_gambar1_posisi = xmlDocCopyNode(animasi_gambar1_posisi, doc, 1);
	xmlAddChild(animasi_gambar1_child, animasi_gambar1_posisi);
	xmlSetProp(animasi_gambar1_posisi, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(animasi_gambar1_posisi, (const xmlChar*) "id", (const xmlChar*) "posisi" );
	
	xmlNodePtr animasi_gambar1_posisi_x = animasi_gambar1_posisi->children;	
	animasi_gambar1_posisi_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar1_posisi_x = xmlDocCopyNode(animasi_gambar1_posisi_x, doc, 1);
	xmlAddChild(animasi_gambar1_posisi, animasi_gambar1_posisi_x);
	xmlSetProp(animasi_gambar1_posisi_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(animasi_gambar1_posisi_x, (const xmlChar*) "0");
	
	xmlNodePtr animasi_gambar1_posisi_y = animasi_gambar1_posisi->children;	
	animasi_gambar1_posisi_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar1_posisi_y = xmlDocCopyNode(animasi_gambar1_posisi_y, doc, 1);
	xmlAddChild(animasi_gambar1_posisi, animasi_gambar1_posisi_y);
	xmlSetProp(animasi_gambar1_posisi_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(animasi_gambar1_posisi_y, (const xmlChar*) "0");
	
	//gambar2
	
	xmlNodePtr animasi_gambar2 = animasi_child->children;
	animasi_gambar2 = xmlNewNode(NULL, (const xmlChar*)"object");	
	animasi_gambar2 = xmlDocCopyNode(animasi_gambar2, doc, 1);
	xmlAddChild(animasi_child, animasi_gambar2);
	xmlSetProp(animasi_gambar2, (const xmlChar*) "struct", (const xmlChar*) "obj_gambar" );
	xmlSetProp(animasi_gambar2, (const xmlChar*) "id", (const xmlChar*) "gambar2" );
	
	xmlNodePtr animasi_gambar2_caption = animasi_gambar2->children;	
	animasi_gambar2_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar2_caption = xmlDocCopyNode(animasi_gambar2_caption, doc, 1);
	xmlAddChild(animasi_gambar2, animasi_gambar2_caption);
	xmlSetProp(animasi_gambar2_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(animasi_gambar2_caption, (const xmlChar*) "gambar#2");
	
	xmlNodePtr animasi_gambar2_source = animasi_gambar2->children;	
	animasi_gambar2_source = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar2_source = xmlDocCopyNode(animasi_gambar2_source, doc, 1);
	xmlAddChild(animasi_gambar2, animasi_gambar2_source);
	xmlSetProp(animasi_gambar2_source, (const xmlChar*) "name", (const xmlChar*) "source");
	xmlNodeSetContent(animasi_gambar2_source, (const xmlChar*) "valve.png");
	
	xmlNodePtr animasi_gambar2_width = animasi_gambar2->children;	
	animasi_gambar2_width = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar2_width = xmlDocCopyNode(animasi_gambar2_width, doc, 1);
	xmlAddChild(animasi_gambar2, animasi_gambar2_width);
	xmlSetProp(animasi_gambar2_width, (const xmlChar*) "name", (const xmlChar*) "width");
	xmlNodeSetContent(animasi_gambar2_width, (const xmlChar*) "100");
	
	xmlNodePtr animasi_gambar2_height = animasi_gambar2->children;	
	animasi_gambar2_height = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar2_height = xmlDocCopyNode(animasi_gambar2_height, doc, 1);
	xmlAddChild(animasi_gambar2, animasi_gambar2_height);
	xmlSetProp(animasi_gambar2_height, (const xmlChar*) "name", (const xmlChar*) "height");
	xmlNodeSetContent(animasi_gambar2_height, (const xmlChar*) "100");
	
	xmlNodePtr animasi_gambar2_skala = animasi_gambar2->children;	
	animasi_gambar2_skala = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar2_skala = xmlDocCopyNode(animasi_gambar2_skala, doc, 1);
	xmlAddChild(animasi_gambar2, animasi_gambar2_skala);
	xmlSetProp(animasi_gambar2_skala, (const xmlChar*) "name", (const xmlChar*) "skala");
	xmlNodeSetContent(animasi_gambar2_skala, (const xmlChar*) "1.0");
	
	xmlNodePtr animasi_gambar2_keterangan = animasi_gambar2->children;	
	animasi_gambar2_keterangan = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar2_keterangan = xmlDocCopyNode(animasi_gambar2_keterangan, doc, 1);
	xmlAddChild(animasi_gambar2, animasi_gambar2_keterangan);
	xmlSetProp(animasi_gambar2_keterangan, (const xmlChar*) "name", (const xmlChar*) "keterangan");
	xmlNodeSetContent(animasi_gambar2_keterangan, (const xmlChar*) "keterangan animasi_gambar2");

	//child
	
	xmlNodePtr animasi_gambar2_child = animasi_gambar2->children;	
	animasi_gambar2_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	animasi_gambar2_child = xmlDocCopyNode(animasi_gambar2_child, doc, 1);
	xmlAddChild(animasi_gambar2, animasi_gambar2_child);
	
	//posisi
	
	xmlNodePtr animasi_gambar2_posisi = animasi_gambar2_child->children;
	animasi_gambar2_posisi = xmlNewNode(NULL, (const xmlChar*)"object");	
	animasi_gambar2_posisi = xmlDocCopyNode(animasi_gambar2_posisi, doc, 1);
	xmlAddChild(animasi_gambar2_child, animasi_gambar2_posisi);
	xmlSetProp(animasi_gambar2_posisi, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(animasi_gambar2_posisi, (const xmlChar*) "id", (const xmlChar*) "posisi" );
	
	xmlNodePtr animasi_gambar2_posisi_x = animasi_gambar2_posisi->children;	
	animasi_gambar2_posisi_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar2_posisi_x = xmlDocCopyNode(animasi_gambar2_posisi_x, doc, 1);
	xmlAddChild(animasi_gambar2_posisi, animasi_gambar2_posisi_x);
	xmlSetProp(animasi_gambar2_posisi_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(animasi_gambar2_posisi_x, (const xmlChar*) "0");
	
	xmlNodePtr animasi_gambar2_posisi_y = animasi_gambar2_posisi->children;	
	animasi_gambar2_posisi_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	animasi_gambar2_posisi_y = xmlDocCopyNode(animasi_gambar2_posisi_y, doc, 1);
	xmlAddChild(animasi_gambar2_posisi, animasi_gambar2_posisi_y);
	xmlSetProp(animasi_gambar2_posisi_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(animasi_gambar2_posisi_y, (const xmlChar*) "0");
}

void EditorWindow::new_xml_gauge_bar()
{
	char gauge_bar_baru[32];
	
	int x = 0;
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_bar") == 0))
			{
				x++;
			}
		}
	}
	
	if(x == 0)
		strcpy(gauge_bar_baru, "new_gauge_bar#1");
	else
	{
		int n = 2;
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_bar") == 0))
				{
					char gauge_bar_name[32];
					sprintf(gauge_bar_name, "%s%d", "new_gauge_bar#", n);
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), gauge_bar_name) == 0)
						n++;
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), gauge_bar_name) != 0)
					{	
						strcpy(gauge_bar_baru, gauge_bar_name);
						n++;
					}
				}
			}
		}
	}
	nodeLevel1 = doc->children;	
	
	xmlNodePtr node_gauge_bar = nodeLevel1->children;
	node_gauge_bar = xmlNewNode(NULL, (const xmlChar*)"object");	
	node_gauge_bar = xmlDocCopyNode(node_gauge_bar, doc, 1);
	xmlAddChild(nodeLevel1, node_gauge_bar);
	xmlSetProp(node_gauge_bar, (const xmlChar*) "struct", (const xmlChar*) "obj_gauge_bar" );
	xmlSetProp(node_gauge_bar, (const xmlChar*) "id", (const xmlChar*)  gauge_bar_baru);
	
	xmlNodePtr gauge_bar_id_titik_data = node_gauge_bar->children;	
	gauge_bar_id_titik_data = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_id_titik_data = xmlDocCopyNode(gauge_bar_id_titik_data, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_id_titik_data);
	xmlSetProp(gauge_bar_id_titik_data, (const xmlChar*) "name", (const xmlChar*) "id_titik_data");
	xmlNodeSetContent(gauge_bar_id_titik_data, (const xmlChar*) "10");
	
	xmlNodePtr gauge_bar_caption = node_gauge_bar->children;	
	gauge_bar_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_caption = xmlDocCopyNode(gauge_bar_caption, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_caption);
	xmlSetProp(gauge_bar_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(gauge_bar_caption, (const xmlChar*) gauge_bar_baru);
	
	xmlNodePtr gauge_bar_width = node_gauge_bar->children;	
	gauge_bar_width = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_width = xmlDocCopyNode(gauge_bar_width, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_width);
	xmlSetProp(gauge_bar_width, (const xmlChar*) "name", (const xmlChar*) "width");
	xmlNodeSetContent(gauge_bar_width, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_height = node_gauge_bar->children;	
	gauge_bar_height = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_height = xmlDocCopyNode(gauge_bar_height, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_height);
	xmlSetProp(gauge_bar_height, (const xmlChar*) "name", (const xmlChar*) "height");
	xmlNodeSetContent(gauge_bar_height, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_show_text = node_gauge_bar->children;	
	gauge_bar_show_text = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_show_text = xmlDocCopyNode(gauge_bar_show_text, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_show_text);
	xmlSetProp(gauge_bar_show_text, (const xmlChar*) "name", (const xmlChar*) "show_text");
	xmlNodeSetContent(gauge_bar_show_text, (const xmlChar*) "1");
	
	xmlNodePtr gauge_bar_show_caption = node_gauge_bar->children;	
	gauge_bar_show_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_show_caption = xmlDocCopyNode(gauge_bar_show_caption, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_show_caption);
	xmlSetProp(gauge_bar_show_caption, (const xmlChar*) "name", (const xmlChar*) "show_caption");
	xmlNodeSetContent(gauge_bar_show_caption, (const xmlChar*) "1");
	
	xmlNodePtr gauge_bar_auto_alarm_range = node_gauge_bar->children;	
	gauge_bar_auto_alarm_range = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_auto_alarm_range = xmlDocCopyNode(gauge_bar_auto_alarm_range, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_auto_alarm_range);
	xmlSetProp(gauge_bar_auto_alarm_range, (const xmlChar*) "name", (const xmlChar*) "auto_alarm_range");
	xmlNodeSetContent(gauge_bar_auto_alarm_range, (const xmlChar*) "1");
	
	xmlNodePtr gauge_bar_range_min = node_gauge_bar->children;	
	gauge_bar_range_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_range_min = xmlDocCopyNode(gauge_bar_range_min, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_range_min);
	xmlSetProp(gauge_bar_range_min, (const xmlChar*) "name", (const xmlChar*) "range_min");
	xmlNodeSetContent(gauge_bar_range_min, (const xmlChar*) "0");
	
	xmlNodePtr gauge_bar_range_max = node_gauge_bar->children;	
	gauge_bar_range_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_range_max = xmlDocCopyNode(gauge_bar_range_max, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_range_max);
	xmlSetProp(gauge_bar_range_max, (const xmlChar*) "name", (const xmlChar*) "range_max");
	xmlNodeSetContent(gauge_bar_range_max, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_alarm_min_min = node_gauge_bar->children;	
	gauge_bar_alarm_min_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_alarm_min_min = xmlDocCopyNode(gauge_bar_alarm_min_min, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_alarm_min_min);
	xmlSetProp(gauge_bar_alarm_min_min, (const xmlChar*) "name", (const xmlChar*) "alarm_min_min");
	xmlNodeSetContent(gauge_bar_alarm_min_min, (const xmlChar*) "10");
	
	xmlNodePtr gauge_bar_alarm_min = node_gauge_bar->children;	
	gauge_bar_alarm_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_alarm_min = xmlDocCopyNode(gauge_bar_alarm_min, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_alarm_min);
	xmlSetProp(gauge_bar_alarm_min, (const xmlChar*) "name", (const xmlChar*) "alarm_min");
	xmlNodeSetContent(gauge_bar_alarm_min, (const xmlChar*) "20");
		
	xmlNodePtr gauge_bar_alarm_max = node_gauge_bar->children;	
	gauge_bar_alarm_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_alarm_max = xmlDocCopyNode(gauge_bar_alarm_max, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_alarm_max);
	xmlSetProp(gauge_bar_alarm_max, (const xmlChar*) "name", (const xmlChar*) "alarm_max");
	xmlNodeSetContent(gauge_bar_alarm_max, (const xmlChar*) "80");
	
	xmlNodePtr gauge_bar_alarm_max_max = node_gauge_bar->children;	
	gauge_bar_alarm_max_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_alarm_max_max = xmlDocCopyNode(gauge_bar_alarm_max_max, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_alarm_max_max);
	xmlSetProp(gauge_bar_alarm_max_max, (const xmlChar*) "name", (const xmlChar*) "alarm_max_max");
	xmlNodeSetContent(gauge_bar_alarm_max_max, (const xmlChar*) "90");
	
	xmlNodePtr gauge_bar_alarm_min_min_enable = node_gauge_bar->children;	
	gauge_bar_alarm_min_min_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_alarm_min_min_enable = xmlDocCopyNode(gauge_bar_alarm_min_min_enable, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_alarm_min_min_enable);
	xmlSetProp(gauge_bar_alarm_min_min_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_min_min_enable");
	xmlNodeSetContent(gauge_bar_alarm_min_min_enable, (const xmlChar*) "1");
	
	xmlNodePtr gauge_bar_alarm_min_enable = node_gauge_bar->children;	
	gauge_bar_alarm_min_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_alarm_min_enable = xmlDocCopyNode(gauge_bar_alarm_min_enable, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_alarm_min_enable);
	xmlSetProp(gauge_bar_alarm_min_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_min_enable");
	xmlNodeSetContent(gauge_bar_alarm_min_enable, (const xmlChar*) "1");
	
	xmlNodePtr gauge_bar_alarm_max_enable = node_gauge_bar->children;	
	gauge_bar_alarm_max_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_alarm_max_enable = xmlDocCopyNode(gauge_bar_alarm_max_enable, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_alarm_max_enable);
	xmlSetProp(gauge_bar_alarm_max_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_max_enable");
	xmlNodeSetContent(gauge_bar_alarm_max_enable, (const xmlChar*) "1");
	
	xmlNodePtr gauge_bar_alarm_max_max_enable = node_gauge_bar->children;	
	gauge_bar_alarm_max_max_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_alarm_max_max_enable = xmlDocCopyNode(gauge_bar_alarm_max_max_enable, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_alarm_max_max_enable);
	xmlSetProp(gauge_bar_alarm_max_max_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_max_max_enable");
	xmlNodeSetContent(gauge_bar_alarm_max_max_enable, (const xmlChar*) "1");
	
	//child
	
	xmlNodePtr gauge_bar_child = node_gauge_bar->children;	
	gauge_bar_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	gauge_bar_child = xmlDocCopyNode(gauge_bar_child, doc, 1);
	xmlAddChild(node_gauge_bar, gauge_bar_child);
	
	//posisi
	
	xmlNodePtr gauge_bar_posisi = gauge_bar_child->children;
	gauge_bar_posisi = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_bar_posisi = xmlDocCopyNode(gauge_bar_posisi, doc, 1);
	xmlAddChild(gauge_bar_child, gauge_bar_posisi);
	xmlSetProp(gauge_bar_posisi, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(gauge_bar_posisi, (const xmlChar*) "id", (const xmlChar*) "posisi" );
	
	xmlNodePtr gauge_bar_posisi_x = gauge_bar_posisi->children;	
	gauge_bar_posisi_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_posisi_x = xmlDocCopyNode(gauge_bar_posisi_x, doc, 1);
	xmlAddChild(gauge_bar_posisi, gauge_bar_posisi_x);
	xmlSetProp(gauge_bar_posisi_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(gauge_bar_posisi_x, (const xmlChar*) "0");
	
	xmlNodePtr gauge_bar_posisi_y = gauge_bar_posisi->children;	
	gauge_bar_posisi_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_posisi_y = xmlDocCopyNode(gauge_bar_posisi_y, doc, 1);
	xmlAddChild(gauge_bar_posisi, gauge_bar_posisi_y);
	xmlSetProp(gauge_bar_posisi_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(gauge_bar_posisi_y, (const xmlChar*) "0");
	
	//warna
	
	xmlNodePtr gauge_bar_warna = gauge_bar_child->children;
	gauge_bar_warna = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_bar_warna = xmlDocCopyNode(gauge_bar_warna, doc, 1);
	xmlAddChild(gauge_bar_child, gauge_bar_warna);
	xmlSetProp(gauge_bar_warna, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_bar_warna, (const xmlChar*) "id", (const xmlChar*) "warna" );
	
	xmlNodePtr gauge_bar_warna_r = gauge_bar_warna->children;	
	gauge_bar_warna_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_r = xmlDocCopyNode(gauge_bar_warna_r, doc, 1);
	xmlAddChild(gauge_bar_warna, gauge_bar_warna_r);
	xmlSetProp(gauge_bar_warna_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_bar_warna_r, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_g = gauge_bar_warna->children;	
	gauge_bar_warna_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_g = xmlDocCopyNode(gauge_bar_warna_g, doc, 1);
	xmlAddChild(gauge_bar_warna, gauge_bar_warna_g);
	xmlSetProp(gauge_bar_warna_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_bar_warna_g, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_b = gauge_bar_warna->children;	
	gauge_bar_warna_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_b = xmlDocCopyNode(gauge_bar_warna_b, doc, 1);
	xmlAddChild(gauge_bar_warna, gauge_bar_warna_b);
	xmlSetProp(gauge_bar_warna_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_bar_warna_b, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_a = gauge_bar_warna->children;	
	gauge_bar_warna_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_a = xmlDocCopyNode(gauge_bar_warna_a, doc, 1);
	xmlAddChild(gauge_bar_warna, gauge_bar_warna_a);
	xmlSetProp(gauge_bar_warna_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_bar_warna_a, (const xmlChar*) "100");
	
	//warna_min_min
	
	xmlNodePtr gauge_bar_warna_min_min = gauge_bar_child->children;
	gauge_bar_warna_min_min = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_bar_warna_min_min = xmlDocCopyNode(gauge_bar_warna_min_min, doc, 1);
	xmlAddChild(gauge_bar_child, gauge_bar_warna_min_min);
	xmlSetProp(gauge_bar_warna_min_min, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_bar_warna_min_min, (const xmlChar*) "id", (const xmlChar*) "warna_min_min" );
	
	xmlNodePtr gauge_bar_warna_min_min_r = gauge_bar_warna_min_min->children;	
	gauge_bar_warna_min_min_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_min_min_r = xmlDocCopyNode(gauge_bar_warna_min_min_r, doc, 1);
	xmlAddChild(gauge_bar_warna_min_min, gauge_bar_warna_min_min_r);
	xmlSetProp(gauge_bar_warna_min_min_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_bar_warna_min_min_r, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_min_min_g = gauge_bar_warna_min_min->children;	
	gauge_bar_warna_min_min_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_min_min_g = xmlDocCopyNode(gauge_bar_warna_min_min_g, doc, 1);
	xmlAddChild(gauge_bar_warna_min_min, gauge_bar_warna_min_min_g);
	xmlSetProp(gauge_bar_warna_min_min_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_bar_warna_min_min_g, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_min_min_b = gauge_bar_warna_min_min->children;	
	gauge_bar_warna_min_min_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_min_min_b = xmlDocCopyNode(gauge_bar_warna_min_min_b, doc, 1);
	xmlAddChild(gauge_bar_warna_min_min, gauge_bar_warna_min_min_b);
	xmlSetProp(gauge_bar_warna_min_min_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_bar_warna_min_min_b, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_min_min_a = gauge_bar_warna_min_min->children;	
	gauge_bar_warna_min_min_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_min_min_a = xmlDocCopyNode(gauge_bar_warna_min_min_a, doc, 1);
	xmlAddChild(gauge_bar_warna_min_min, gauge_bar_warna_min_min_a);
	xmlSetProp(gauge_bar_warna_min_min_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_bar_warna_min_min_a, (const xmlChar*) "100");
	
	//warna_min
	
	xmlNodePtr gauge_bar_warna_min = gauge_bar_child->children;
	gauge_bar_warna_min = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_bar_warna_min = xmlDocCopyNode(gauge_bar_warna_min, doc, 1);
	xmlAddChild(gauge_bar_child, gauge_bar_warna_min);
	xmlSetProp(gauge_bar_warna_min, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_bar_warna_min, (const xmlChar*) "id", (const xmlChar*) "warna_min" );
	
	xmlNodePtr gauge_bar_warna_min_r = gauge_bar_warna_min->children;	
	gauge_bar_warna_min_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_min_r = xmlDocCopyNode(gauge_bar_warna_min_r, doc, 1);
	xmlAddChild(gauge_bar_warna_min, gauge_bar_warna_min_r);
	xmlSetProp(gauge_bar_warna_min_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_bar_warna_min_r, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_min_g = gauge_bar_warna_min->children;	
	gauge_bar_warna_min_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_min_g = xmlDocCopyNode(gauge_bar_warna_min_g, doc, 1);
	xmlAddChild(gauge_bar_warna_min, gauge_bar_warna_min_g);
	xmlSetProp(gauge_bar_warna_min_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_bar_warna_min_g, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_min_b = gauge_bar_warna_min->children;	
	gauge_bar_warna_min_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_min_b = xmlDocCopyNode(gauge_bar_warna_min_b, doc, 1);
	xmlAddChild(gauge_bar_warna_min, gauge_bar_warna_min_b);
	xmlSetProp(gauge_bar_warna_min_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_bar_warna_min_b, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_min_a = gauge_bar_warna_min->children;	
	gauge_bar_warna_min_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_min_a = xmlDocCopyNode(gauge_bar_warna_min_a, doc, 1);
	xmlAddChild(gauge_bar_warna_min, gauge_bar_warna_min_a);
	xmlSetProp(gauge_bar_warna_min_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_bar_warna_min_a, (const xmlChar*) "100");
	
	//warna_max
	
	xmlNodePtr gauge_bar_warna_max = gauge_bar_child->children;
	gauge_bar_warna_max = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_bar_warna_max = xmlDocCopyNode(gauge_bar_warna_max, doc, 1);
	xmlAddChild(gauge_bar_child, gauge_bar_warna_max);
	xmlSetProp(gauge_bar_warna_max, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_bar_warna_max, (const xmlChar*) "id", (const xmlChar*) "warna_max" );
	
	xmlNodePtr gauge_bar_warna_max_r = gauge_bar_warna_max->children;	
	gauge_bar_warna_max_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_max_r = xmlDocCopyNode(gauge_bar_warna_max_r, doc, 1);
	xmlAddChild(gauge_bar_warna_max, gauge_bar_warna_max_r);
	xmlSetProp(gauge_bar_warna_max_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_bar_warna_max_r, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_max_g = gauge_bar_warna_max->children;	
	gauge_bar_warna_max_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_max_g = xmlDocCopyNode(gauge_bar_warna_max_g, doc, 1);
	xmlAddChild(gauge_bar_warna_max, gauge_bar_warna_max_g);
	xmlSetProp(gauge_bar_warna_max_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_bar_warna_max_g, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_max_b = gauge_bar_warna_max->children;	
	gauge_bar_warna_max_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_max_b = xmlDocCopyNode(gauge_bar_warna_max_b, doc, 1);
	xmlAddChild(gauge_bar_warna_max, gauge_bar_warna_max_b);
	xmlSetProp(gauge_bar_warna_max_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_bar_warna_max_b, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_max_a = gauge_bar_warna_max->children;	
	gauge_bar_warna_max_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_max_a = xmlDocCopyNode(gauge_bar_warna_max_a, doc, 1);
	xmlAddChild(gauge_bar_warna_max, gauge_bar_warna_max_a);
	xmlSetProp(gauge_bar_warna_max_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_bar_warna_max_a, (const xmlChar*) "100");
	
	//warna_max_max
	
	xmlNodePtr gauge_bar_warna_max_max = gauge_bar_child->children;
	gauge_bar_warna_max_max = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_bar_warna_max_max = xmlDocCopyNode(gauge_bar_warna_max_max, doc, 1);
	xmlAddChild(gauge_bar_child, gauge_bar_warna_max_max);
	xmlSetProp(gauge_bar_warna_max_max, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_bar_warna_max_max, (const xmlChar*) "id", (const xmlChar*) "warna_max_max" );
	
	xmlNodePtr gauge_bar_warna_max_max_r = gauge_bar_warna_max_max->children;	
	gauge_bar_warna_max_max_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_max_max_r = xmlDocCopyNode(gauge_bar_warna_max_max_r, doc, 1);
	xmlAddChild(gauge_bar_warna_max_max, gauge_bar_warna_max_max_r);
	xmlSetProp(gauge_bar_warna_max_max_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_bar_warna_max_max_r, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_max_max_g = gauge_bar_warna_max_max->children;	
	gauge_bar_warna_max_max_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_max_max_g = xmlDocCopyNode(gauge_bar_warna_max_max_g, doc, 1);
	xmlAddChild(gauge_bar_warna_max_max, gauge_bar_warna_max_max_g);
	xmlSetProp(gauge_bar_warna_max_max_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_bar_warna_max_max_g, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_max_max_b = gauge_bar_warna_max_max->children;	
	gauge_bar_warna_max_max_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_max_max_b = xmlDocCopyNode(gauge_bar_warna_max_max_b, doc, 1);
	xmlAddChild(gauge_bar_warna_max_max, gauge_bar_warna_max_max_b);
	xmlSetProp(gauge_bar_warna_max_max_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_bar_warna_max_max_b, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_max_max_a = gauge_bar_warna_max_max->children;	
	gauge_bar_warna_max_max_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_max_max_a = xmlDocCopyNode(gauge_bar_warna_max_max_a, doc, 1);
	xmlAddChild(gauge_bar_warna_max_max, gauge_bar_warna_max_max_a);
	xmlSetProp(gauge_bar_warna_max_max_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_bar_warna_max_max_a, (const xmlChar*) "100");
	
	//warna_oor
	
	xmlNodePtr gauge_bar_warna_oor = gauge_bar_child->children;
	gauge_bar_warna_oor = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_bar_warna_oor = xmlDocCopyNode(gauge_bar_warna_oor, doc, 1);
	xmlAddChild(gauge_bar_child, gauge_bar_warna_oor);
	xmlSetProp(gauge_bar_warna_oor, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_bar_warna_oor, (const xmlChar*) "id", (const xmlChar*) "warna_oor" );
	
	xmlNodePtr gauge_bar_warna_oor_r = gauge_bar_warna_oor->children;	
	gauge_bar_warna_oor_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_oor_r = xmlDocCopyNode(gauge_bar_warna_oor_r, doc, 1);
	xmlAddChild(gauge_bar_warna_oor, gauge_bar_warna_oor_r);
	xmlSetProp(gauge_bar_warna_oor_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_bar_warna_oor_r, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_oor_g = gauge_bar_warna_oor->children;	
	gauge_bar_warna_oor_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_oor_g = xmlDocCopyNode(gauge_bar_warna_oor_g, doc, 1);
	xmlAddChild(gauge_bar_warna_oor, gauge_bar_warna_oor_g);
	xmlSetProp(gauge_bar_warna_oor_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_bar_warna_oor_g, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_oor_b = gauge_bar_warna_oor->children;	
	gauge_bar_warna_oor_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_oor_b = xmlDocCopyNode(gauge_bar_warna_oor_b, doc, 1);
	xmlAddChild(gauge_bar_warna_oor, gauge_bar_warna_oor_b);
	xmlSetProp(gauge_bar_warna_oor_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_bar_warna_oor_b, (const xmlChar*) "100");
	
	xmlNodePtr gauge_bar_warna_oor_a = gauge_bar_warna_oor->children;	
	gauge_bar_warna_oor_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_bar_warna_oor_a = xmlDocCopyNode(gauge_bar_warna_oor_a, doc, 1);
	xmlAddChild(gauge_bar_warna_oor, gauge_bar_warna_oor_a);
	xmlSetProp(gauge_bar_warna_oor_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_bar_warna_oor_a, (const xmlChar*) "100");
}

void EditorWindow::new_xml_gauge_dial()
{
	char gauge_dial_baru[32];
	
	int x = 0;
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_dial") == 0))
			{
				x++;
			}
		}
	}
	
	if(x == 0)
		strcpy(gauge_dial_baru, "new_gauge_dial#1");
	else
	{
		int n = 2;
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_gauge_dial") == 0))
				{
					char gauge_dial_name[32];
					sprintf(gauge_dial_name, "%s%d", "new_gauge_dial#", n);
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), gauge_dial_name) == 0)
						n++;
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), gauge_dial_name) != 0)
					{	
						strcpy(gauge_dial_baru, gauge_dial_name);
						n++;
					}
				}
			}
		}
	}
	nodeLevel1 = doc->children;	
	
	xmlNodePtr node_gauge_dial = nodeLevel1->children;
	node_gauge_dial = xmlNewNode(NULL, (const xmlChar*)"object");	
	node_gauge_dial = xmlDocCopyNode(node_gauge_dial, doc, 1);
	xmlAddChild(nodeLevel1, node_gauge_dial);
	xmlSetProp(node_gauge_dial, (const xmlChar*) "struct", (const xmlChar*) "obj_gauge_dial" );
	xmlSetProp(node_gauge_dial, (const xmlChar*) "id", (const xmlChar*)  gauge_dial_baru);
	
	xmlNodePtr gauge_dial_id_titik_data = node_gauge_dial->children;	
	gauge_dial_id_titik_data = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_id_titik_data = xmlDocCopyNode(gauge_dial_id_titik_data, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_id_titik_data);
	xmlSetProp(gauge_dial_id_titik_data, (const xmlChar*) "name", (const xmlChar*) "id_titik_data");
	xmlNodeSetContent(gauge_dial_id_titik_data, (const xmlChar*) "10");
	
	xmlNodePtr gauge_dial_caption = node_gauge_dial->children;	
	gauge_dial_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_caption = xmlDocCopyNode(gauge_dial_caption, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_caption);
	xmlSetProp(gauge_dial_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(gauge_dial_caption, (const xmlChar*) gauge_dial_baru);
	
	xmlNodePtr gauge_dial_width = node_gauge_dial->children;	
	gauge_dial_width = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_width = xmlDocCopyNode(gauge_dial_width, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_width);
	xmlSetProp(gauge_dial_width, (const xmlChar*) "name", (const xmlChar*) "width");
	xmlNodeSetContent(gauge_dial_width, (const xmlChar*) "100");
	
	xmlNodePtr gauge_dial_height = node_gauge_dial->children;	
	gauge_dial_height = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_height = xmlDocCopyNode(gauge_dial_height, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_height);
	xmlSetProp(gauge_dial_height, (const xmlChar*) "name", (const xmlChar*) "height");
	xmlNodeSetContent(gauge_dial_height, (const xmlChar*) "100");
	
	xmlNodePtr gauge_dial_show_text = node_gauge_dial->children;	
	gauge_dial_show_text = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_show_text = xmlDocCopyNode(gauge_dial_show_text, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_show_text);
	xmlSetProp(gauge_dial_show_text, (const xmlChar*) "name", (const xmlChar*) "show_text");
	xmlNodeSetContent(gauge_dial_show_text, (const xmlChar*) "1");
	
	xmlNodePtr gauge_dial_show_caption = node_gauge_dial->children;	
	gauge_dial_show_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_show_caption = xmlDocCopyNode(gauge_dial_show_caption, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_show_caption);
	xmlSetProp(gauge_dial_show_caption, (const xmlChar*) "name", (const xmlChar*) "show_caption");
	xmlNodeSetContent(gauge_dial_show_caption, (const xmlChar*) "1");
	
	xmlNodePtr gauge_dial_auto_alarm_range = node_gauge_dial->children;	
	gauge_dial_auto_alarm_range = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_auto_alarm_range = xmlDocCopyNode(gauge_dial_auto_alarm_range, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_auto_alarm_range);
	xmlSetProp(gauge_dial_auto_alarm_range, (const xmlChar*) "name", (const xmlChar*) "auto_alarm_range");
	xmlNodeSetContent(gauge_dial_auto_alarm_range, (const xmlChar*) "1");
	
	xmlNodePtr gauge_dial_range_min = node_gauge_dial->children;	
	gauge_dial_range_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_range_min = xmlDocCopyNode(gauge_dial_range_min, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_range_min);
	xmlSetProp(gauge_dial_range_min, (const xmlChar*) "name", (const xmlChar*) "range_min");
	xmlNodeSetContent(gauge_dial_range_min, (const xmlChar*) "0");
	
	xmlNodePtr gauge_dial_range_max = node_gauge_dial->children;	
	gauge_dial_range_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_range_max = xmlDocCopyNode(gauge_dial_range_max, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_range_max);
	xmlSetProp(gauge_dial_range_max, (const xmlChar*) "name", (const xmlChar*) "range_max");
	xmlNodeSetContent(gauge_dial_range_max, (const xmlChar*) "100");
	
	xmlNodePtr gauge_dial_alarm_min_min = node_gauge_dial->children;	
	gauge_dial_alarm_min_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_alarm_min_min = xmlDocCopyNode(gauge_dial_alarm_min_min, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_alarm_min_min);
	xmlSetProp(gauge_dial_alarm_min_min, (const xmlChar*) "name", (const xmlChar*) "alarm_min_min");
	xmlNodeSetContent(gauge_dial_alarm_min_min, (const xmlChar*) "10");
	
	xmlNodePtr gauge_dial_alarm_min = node_gauge_dial->children;	
	gauge_dial_alarm_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_alarm_min = xmlDocCopyNode(gauge_dial_alarm_min, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_alarm_min);
	xmlSetProp(gauge_dial_alarm_min, (const xmlChar*) "name", (const xmlChar*) "alarm_min");
	xmlNodeSetContent(gauge_dial_alarm_min, (const xmlChar*) "20");
	
	xmlNodePtr gauge_dial_alarm_max = node_gauge_dial->children;	
	gauge_dial_alarm_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_alarm_max = xmlDocCopyNode(gauge_dial_alarm_max, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_alarm_max);
	xmlSetProp(gauge_dial_alarm_max, (const xmlChar*) "name", (const xmlChar*) "alarm_max");
	xmlNodeSetContent(gauge_dial_alarm_max, (const xmlChar*) "80");
	
	xmlNodePtr gauge_dial_alarm_max_max = node_gauge_dial->children;	
	gauge_dial_alarm_max_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_alarm_max_max = xmlDocCopyNode(gauge_dial_alarm_max_max, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_alarm_max_max);
	xmlSetProp(gauge_dial_alarm_max_max, (const xmlChar*) "name", (const xmlChar*) "alarm_max_max");
	xmlNodeSetContent(gauge_dial_alarm_max_max, (const xmlChar*) "90");
	
	xmlNodePtr gauge_dial_alarm_min_min_enable = node_gauge_dial->children;	
	gauge_dial_alarm_min_min_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_alarm_min_min_enable = xmlDocCopyNode(gauge_dial_alarm_min_min_enable, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_alarm_min_min_enable);
	xmlSetProp(gauge_dial_alarm_min_min_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_min_min_enable");
	xmlNodeSetContent(gauge_dial_alarm_min_min_enable, (const xmlChar*) "1");
	
	xmlNodePtr gauge_dial_alarm_min_enable = node_gauge_dial->children;	
	gauge_dial_alarm_min_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_alarm_min_enable = xmlDocCopyNode(gauge_dial_alarm_min_enable, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_alarm_min_enable);
	xmlSetProp(gauge_dial_alarm_min_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_min_enable");
	xmlNodeSetContent(gauge_dial_alarm_min_enable, (const xmlChar*) "1");
	
	xmlNodePtr gauge_dial_alarm_max_enable = node_gauge_dial->children;	
	gauge_dial_alarm_max_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_alarm_max_enable = xmlDocCopyNode(gauge_dial_alarm_max_enable, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_alarm_max_enable);
	xmlSetProp(gauge_dial_alarm_max_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_max_enable");
	xmlNodeSetContent(gauge_dial_alarm_max_enable, (const xmlChar*) "1");
	
	xmlNodePtr gauge_dial_alarm_max_max_enable = node_gauge_dial->children;	
	gauge_dial_alarm_max_max_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_alarm_max_max_enable = xmlDocCopyNode(gauge_dial_alarm_max_max_enable, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_alarm_max_max_enable);
	xmlSetProp(gauge_dial_alarm_max_max_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_max_max_enable");
	xmlNodeSetContent(gauge_dial_alarm_max_max_enable, (const xmlChar*) "1");
		
	//child
	
	xmlNodePtr gauge_dial_child = node_gauge_dial->children;	
	gauge_dial_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	gauge_dial_child = xmlDocCopyNode(gauge_dial_child, doc, 1);
	xmlAddChild(node_gauge_dial, gauge_dial_child);
	
	//posisi
	
	xmlNodePtr gauge_dial_posisi = gauge_dial_child->children;
	gauge_dial_posisi = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_dial_posisi = xmlDocCopyNode(gauge_dial_posisi, doc, 1);
	xmlAddChild(gauge_dial_child, gauge_dial_posisi);
	xmlSetProp(gauge_dial_posisi, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(gauge_dial_posisi, (const xmlChar*) "id", (const xmlChar*) "posisi" );
	
	xmlNodePtr gauge_dial_posisi_x = gauge_dial_posisi->children;	
	gauge_dial_posisi_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_posisi_x = xmlDocCopyNode(gauge_dial_posisi_x, doc, 1);
	xmlAddChild(gauge_dial_posisi, gauge_dial_posisi_x);
	xmlSetProp(gauge_dial_posisi_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(gauge_dial_posisi_x, (const xmlChar*) "0");
	
	xmlNodePtr gauge_dial_posisi_y = gauge_dial_posisi->children;	
	gauge_dial_posisi_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_posisi_y = xmlDocCopyNode(gauge_dial_posisi_y, doc, 1);
	xmlAddChild(gauge_dial_posisi, gauge_dial_posisi_y);
	xmlSetProp(gauge_dial_posisi_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(gauge_dial_posisi_y, (const xmlChar*) "0");
	
	//warna
	
	xmlNodePtr gauge_dial_warna = gauge_dial_child->children;
	gauge_dial_warna = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_dial_warna = xmlDocCopyNode(gauge_dial_warna, doc, 1);
	xmlAddChild(gauge_dial_child, gauge_dial_warna);
	xmlSetProp(gauge_dial_warna, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_dial_warna, (const xmlChar*) "id", (const xmlChar*) "warna" );
	
	xmlNodePtr gauge_dial_warna_r = gauge_dial_warna->children;	
	gauge_dial_warna_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_r = xmlDocCopyNode(gauge_dial_warna_r, doc, 1);
	xmlAddChild(gauge_dial_warna, gauge_dial_warna_r);
	xmlSetProp(gauge_dial_warna_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_dial_warna_r, (const xmlChar*) "100");
	
	xmlNodePtr gauge_dial_warna_g = gauge_dial_warna->children;	
	gauge_dial_warna_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_g = xmlDocCopyNode(gauge_dial_warna_g, doc, 1);
	xmlAddChild(gauge_dial_warna, gauge_dial_warna_g);
	xmlSetProp(gauge_dial_warna_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_dial_warna_g, (const xmlChar*) "200");
	
	xmlNodePtr gauge_dial_warna_b = gauge_dial_warna->children;	
	gauge_dial_warna_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_b = xmlDocCopyNode(gauge_dial_warna_b, doc, 1);
	xmlAddChild(gauge_dial_warna, gauge_dial_warna_b);
	xmlSetProp(gauge_dial_warna_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_dial_warna_b, (const xmlChar*) "0");
	
	xmlNodePtr gauge_dial_warna_a = gauge_dial_warna->children;	
	gauge_dial_warna_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_a = xmlDocCopyNode(gauge_dial_warna_a, doc, 1);
	xmlAddChild(gauge_dial_warna, gauge_dial_warna_a);
	xmlSetProp(gauge_dial_warna_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_dial_warna_a, (const xmlChar*) "100");
	
	//warna_min_min
	
	xmlNodePtr gauge_dial_warna_min_min = gauge_dial_child->children;
	gauge_dial_warna_min_min = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_dial_warna_min_min = xmlDocCopyNode(gauge_dial_warna_min_min, doc, 1);
	xmlAddChild(gauge_dial_child, gauge_dial_warna_min_min);
	xmlSetProp(gauge_dial_warna_min_min, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_dial_warna_min_min, (const xmlChar*) "id", (const xmlChar*) "warna_min_min" );
	
	xmlNodePtr gauge_dial_warna_min_min_r = gauge_dial_warna_min_min->children;	
	gauge_dial_warna_min_min_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_min_min_r = xmlDocCopyNode(gauge_dial_warna_min_min_r, doc, 1);
	xmlAddChild(gauge_dial_warna_min_min, gauge_dial_warna_min_min_r);
	xmlSetProp(gauge_dial_warna_min_min_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_dial_warna_min_min_r, (const xmlChar*) "100");
	
	xmlNodePtr gauge_dial_warna_min_min_g = gauge_dial_warna_min_min->children;	
	gauge_dial_warna_min_min_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_min_min_g = xmlDocCopyNode(gauge_dial_warna_min_min_g, doc, 1);
	xmlAddChild(gauge_dial_warna_min_min, gauge_dial_warna_min_min_g);
	xmlSetProp(gauge_dial_warna_min_min_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_dial_warna_min_min_g, (const xmlChar*) "100");
	
	xmlNodePtr gauge_dial_warna_min_min_b = gauge_dial_warna_min_min->children;	
	gauge_dial_warna_min_min_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_min_min_b = xmlDocCopyNode(gauge_dial_warna_min_min_b, doc, 1);
	xmlAddChild(gauge_dial_warna_min_min, gauge_dial_warna_min_min_b);
	xmlSetProp(gauge_dial_warna_min_min_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_dial_warna_min_min_b, (const xmlChar*) "100");
	
	xmlNodePtr gauge_dial_warna_min_min_a = gauge_dial_warna_min_min->children;	
	gauge_dial_warna_min_min_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_min_min_a = xmlDocCopyNode(gauge_dial_warna_min_min_a, doc, 1);
	xmlAddChild(gauge_dial_warna_min_min, gauge_dial_warna_min_min_a);
	xmlSetProp(gauge_dial_warna_min_min_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_dial_warna_min_min_a, (const xmlChar*) "100");
	
	//warna_min
	
	xmlNodePtr gauge_dial_warna_min = gauge_dial_child->children;
	gauge_dial_warna_min = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_dial_warna_min = xmlDocCopyNode(gauge_dial_warna_min, doc, 1);
	xmlAddChild(gauge_dial_child, gauge_dial_warna_min);
	xmlSetProp(gauge_dial_warna_min, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_dial_warna_min, (const xmlChar*) "id", (const xmlChar*) "warna_min" );
	
	xmlNodePtr gauge_dial_warna_min_r = gauge_dial_warna_min->children;	
	gauge_dial_warna_min_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_min_r = xmlDocCopyNode(gauge_dial_warna_min_r, doc, 1);
	xmlAddChild(gauge_dial_warna_min, gauge_dial_warna_min_r);
	xmlSetProp(gauge_dial_warna_min_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_dial_warna_min_r, (const xmlChar*) "150");
	
	xmlNodePtr gauge_dial_warna_min_g = gauge_dial_warna_min->children;	
	gauge_dial_warna_min_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_min_g = xmlDocCopyNode(gauge_dial_warna_min_g, doc, 1);
	xmlAddChild(gauge_dial_warna_min, gauge_dial_warna_min_g);
	xmlSetProp(gauge_dial_warna_min_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_dial_warna_min_g, (const xmlChar*) "150");
	
	xmlNodePtr gauge_dial_warna_min_b = gauge_dial_warna_min->children;	
	gauge_dial_warna_min_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_min_b = xmlDocCopyNode(gauge_dial_warna_min_b, doc, 1);
	xmlAddChild(gauge_dial_warna_min, gauge_dial_warna_min_b);
	xmlSetProp(gauge_dial_warna_min_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_dial_warna_min_b, (const xmlChar*) "150");
	
	xmlNodePtr gauge_dial_warna_min_a = gauge_dial_warna_min->children;	
	gauge_dial_warna_min_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_min_a = xmlDocCopyNode(gauge_dial_warna_min_a, doc, 1);
	xmlAddChild(gauge_dial_warna_min, gauge_dial_warna_min_a);
	xmlSetProp(gauge_dial_warna_min_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_dial_warna_min_a, (const xmlChar*) "100");
	
	//warna_max
	
	xmlNodePtr gauge_dial_warna_max = gauge_dial_child->children;
	gauge_dial_warna_max = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_dial_warna_max = xmlDocCopyNode(gauge_dial_warna_max, doc, 1);
	xmlAddChild(gauge_dial_child, gauge_dial_warna_max);
	xmlSetProp(gauge_dial_warna_max, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_dial_warna_max, (const xmlChar*) "id", (const xmlChar*) "warna_max" );
	
	xmlNodePtr gauge_dial_warna_max_r = gauge_dial_warna_max->children;	
	gauge_dial_warna_max_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_max_r = xmlDocCopyNode(gauge_dial_warna_max_r, doc, 1);
	xmlAddChild(gauge_dial_warna_max, gauge_dial_warna_max_r);
	xmlSetProp(gauge_dial_warna_max_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_dial_warna_max_r, (const xmlChar*) "255");
	
	xmlNodePtr gauge_dial_warna_max_g = gauge_dial_warna_max->children;	
	gauge_dial_warna_max_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_max_g = xmlDocCopyNode(gauge_dial_warna_max_g, doc, 1);
	xmlAddChild(gauge_dial_warna_max, gauge_dial_warna_max_g);
	xmlSetProp(gauge_dial_warna_max_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_dial_warna_max_g, (const xmlChar*) "200");
	
	xmlNodePtr gauge_dial_warna_max_b = gauge_dial_warna_max->children;	
	gauge_dial_warna_max_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_max_b = xmlDocCopyNode(gauge_dial_warna_max_b, doc, 1);
	xmlAddChild(gauge_dial_warna_max, gauge_dial_warna_max_b);
	xmlSetProp(gauge_dial_warna_max_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_dial_warna_max_b, (const xmlChar*) "0");
	
	xmlNodePtr gauge_dial_warna_max_a = gauge_dial_warna_max->children;	
	gauge_dial_warna_max_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_max_a = xmlDocCopyNode(gauge_dial_warna_max_a, doc, 1);
	xmlAddChild(gauge_dial_warna_max, gauge_dial_warna_max_a);
	xmlSetProp(gauge_dial_warna_max_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_dial_warna_max_a, (const xmlChar*) "100");
	
	//warna_max_max
	
	xmlNodePtr gauge_dial_warna_max_max = gauge_dial_child->children;
	gauge_dial_warna_max_max = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_dial_warna_max_max = xmlDocCopyNode(gauge_dial_warna_max_max, doc, 1);
	xmlAddChild(gauge_dial_child, gauge_dial_warna_max_max);
	xmlSetProp(gauge_dial_warna_max_max, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_dial_warna_max_max, (const xmlChar*) "id", (const xmlChar*) "warna_max_max" );
	
	xmlNodePtr gauge_dial_warna_max_max_r = gauge_dial_warna_max_max->children;	
	gauge_dial_warna_max_max_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_max_max_r = xmlDocCopyNode(gauge_dial_warna_max_max_r, doc, 1);
	xmlAddChild(gauge_dial_warna_max_max, gauge_dial_warna_max_max_r);
	xmlSetProp(gauge_dial_warna_max_max_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_dial_warna_max_max_r, (const xmlChar*) "250");
	
	xmlNodePtr gauge_dial_warna_max_max_g = gauge_dial_warna_max_max->children;	
	gauge_dial_warna_max_max_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_max_max_g = xmlDocCopyNode(gauge_dial_warna_max_max_g, doc, 1);
	xmlAddChild(gauge_dial_warna_max_max, gauge_dial_warna_max_max_g);
	xmlSetProp(gauge_dial_warna_max_max_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_dial_warna_max_max_g, (const xmlChar*) "0");
	
	xmlNodePtr gauge_dial_warna_max_max_b = gauge_dial_warna_max_max->children;	
	gauge_dial_warna_max_max_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_max_max_b = xmlDocCopyNode(gauge_dial_warna_max_max_b, doc, 1);
	xmlAddChild(gauge_dial_warna_max_max, gauge_dial_warna_max_max_b);
	xmlSetProp(gauge_dial_warna_max_max_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_dial_warna_max_max_b, (const xmlChar*) "0");
	
	xmlNodePtr gauge_dial_warna_max_max_a = gauge_dial_warna_max_max->children;	
	gauge_dial_warna_max_max_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_max_max_a = xmlDocCopyNode(gauge_dial_warna_max_max_a, doc, 1);
	xmlAddChild(gauge_dial_warna_max_max, gauge_dial_warna_max_max_a);
	xmlSetProp(gauge_dial_warna_max_max_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_dial_warna_max_max_a, (const xmlChar*) "100");
	
	//warna_oor
	
	xmlNodePtr gauge_dial_warna_oor = gauge_dial_child->children;
	gauge_dial_warna_oor = xmlNewNode(NULL, (const xmlChar*)"object");	
	gauge_dial_warna_oor = xmlDocCopyNode(gauge_dial_warna_oor, doc, 1);
	xmlAddChild(gauge_dial_child, gauge_dial_warna_oor);
	xmlSetProp(gauge_dial_warna_oor, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(gauge_dial_warna_oor, (const xmlChar*) "id", (const xmlChar*) "warna_oor" );
	
	xmlNodePtr gauge_dial_warna_oor_r = gauge_dial_warna_oor->children;	
	gauge_dial_warna_oor_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_oor_r = xmlDocCopyNode(gauge_dial_warna_oor_r, doc, 1);
	xmlAddChild(gauge_dial_warna_oor, gauge_dial_warna_oor_r);
	xmlSetProp(gauge_dial_warna_oor_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(gauge_dial_warna_oor_r, (const xmlChar*) "100");
	
	xmlNodePtr gauge_dial_warna_oor_g = gauge_dial_warna_oor->children;	
	gauge_dial_warna_oor_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_oor_g = xmlDocCopyNode(gauge_dial_warna_oor_g, doc, 1);
	xmlAddChild(gauge_dial_warna_oor, gauge_dial_warna_oor_g);
	xmlSetProp(gauge_dial_warna_oor_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(gauge_dial_warna_oor_g, (const xmlChar*) "100");
	
	xmlNodePtr gauge_dial_warna_oor_b = gauge_dial_warna_oor->children;	
	gauge_dial_warna_oor_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_oor_b = xmlDocCopyNode(gauge_dial_warna_oor_b, doc, 1);
	xmlAddChild(gauge_dial_warna_oor, gauge_dial_warna_oor_b);
	xmlSetProp(gauge_dial_warna_oor_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(gauge_dial_warna_oor_b, (const xmlChar*) "100");
	
	xmlNodePtr gauge_dial_warna_oor_a = gauge_dial_warna_oor->children;	
	gauge_dial_warna_oor_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	gauge_dial_warna_oor_a = xmlDocCopyNode(gauge_dial_warna_oor_a, doc, 1);
	xmlAddChild(gauge_dial_warna_oor, gauge_dial_warna_oor_a);
	xmlSetProp(gauge_dial_warna_oor_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(gauge_dial_warna_oor_a, (const xmlChar*) "100");
	
}

void EditorWindow::new_xml_grafik()
{
	char grafik_baru[32];
	
	int x = 0;
	for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
	{		
		for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
		{
			if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_grafik") == 0))
			{
				x++;
			}
		}
	}
	
	if(x == 0)
		strcpy(grafik_baru, "new_grafik#1");
	else
	{
		int n = 2;
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"struct"), "obj_grafik") == 0))
				{
					char grafik_name[32];
					sprintf(grafik_name, "%s%d", "new_grafik#", n);
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), grafik_name) == 0)
						n++;
					if(strcmp((char*)(xmlGetProp(nodeLevel2, (const xmlChar*)"id")), grafik_name) != 0)
					{	
						strcpy(grafik_baru, grafik_name);
						n++;
					}
				}
			}
		}
	}
	nodeLevel1 = doc->children;	
	
	xmlNodePtr node_grafik = nodeLevel1->children;
	node_grafik = xmlNewNode(NULL, (const xmlChar*)"object");	
	node_grafik = xmlDocCopyNode(node_grafik, doc, 1);
	xmlAddChild(nodeLevel1, node_grafik);
	xmlSetProp(node_grafik, (const xmlChar*) "struct", (const xmlChar*) "obj_grafik" );
	xmlSetProp(node_grafik, (const xmlChar*) "id", (const xmlChar*)  grafik_baru);
	
	xmlNodePtr grafik_id_titik_data = node_grafik->children;	
	grafik_id_titik_data = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_id_titik_data = xmlDocCopyNode(grafik_id_titik_data, doc, 1);
	xmlAddChild(node_grafik, grafik_id_titik_data);
	xmlSetProp(grafik_id_titik_data, (const xmlChar*) "name", (const xmlChar*) "id_titik_data");
	xmlNodeSetContent(grafik_id_titik_data, (const xmlChar*) "10");
	
	xmlNodePtr grafik_caption = node_grafik->children;	
	grafik_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_caption = xmlDocCopyNode(grafik_caption, doc, 1);
	xmlAddChild(node_grafik, grafik_caption);
	xmlSetProp(grafik_caption, (const xmlChar*) "name", (const xmlChar*) "caption");
	xmlNodeSetContent(grafik_caption, (const xmlChar*) grafik_baru);
		
	xmlNodePtr grafik_width = node_grafik->children;	
	grafik_width = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_width = xmlDocCopyNode(grafik_width, doc, 1);
	xmlAddChild(node_grafik, grafik_width);
	xmlSetProp(grafik_width, (const xmlChar*) "name", (const xmlChar*) "width");
	xmlNodeSetContent(grafik_width, (const xmlChar*) "100");
	
	xmlNodePtr grafik_height = node_grafik->children;	
	grafik_height = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_height = xmlDocCopyNode(grafik_height, doc, 1);
	xmlAddChild(node_grafik, grafik_height);
	xmlSetProp(grafik_height, (const xmlChar*) "name", (const xmlChar*) "height");
	xmlNodeSetContent(grafik_height, (const xmlChar*) "100");
	
	xmlNodePtr grafik_show_text = node_grafik->children;	
	grafik_show_text = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_show_text = xmlDocCopyNode(grafik_show_text, doc, 1);
	xmlAddChild(node_grafik, grafik_show_text);
	xmlSetProp(grafik_show_text, (const xmlChar*) "name", (const xmlChar*) "show_text");
	xmlNodeSetContent(grafik_show_text, (const xmlChar*) "1");
	
	xmlNodePtr grafik_show_caption = node_grafik->children;	
	grafik_show_caption = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_show_caption = xmlDocCopyNode(grafik_show_caption, doc, 1);
	xmlAddChild(node_grafik, grafik_show_caption);
	xmlSetProp(grafik_show_caption, (const xmlChar*) "name", (const xmlChar*) "show_caption");
	xmlNodeSetContent(grafik_show_caption, (const xmlChar*) "1");
	
	xmlNodePtr grafik_auto_alarm_range = node_grafik->children;	
	grafik_auto_alarm_range = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_auto_alarm_range = xmlDocCopyNode(grafik_auto_alarm_range, doc, 1);
	xmlAddChild(node_grafik, grafik_auto_alarm_range);
	xmlSetProp(grafik_auto_alarm_range, (const xmlChar*) "name", (const xmlChar*) "auto_alarm_range");
	xmlNodeSetContent(grafik_auto_alarm_range, (const xmlChar*) "1");
	
	xmlNodePtr grafik_range_min = node_grafik->children;	
	grafik_range_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_range_min = xmlDocCopyNode(grafik_range_min, doc, 1);
	xmlAddChild(node_grafik, grafik_range_min);
	xmlSetProp(grafik_range_min, (const xmlChar*) "name", (const xmlChar*) "range_min");
	xmlNodeSetContent(grafik_range_min, (const xmlChar*) "0");
	
	xmlNodePtr grafik_range_max = node_grafik->children;	
	grafik_range_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_range_max = xmlDocCopyNode(grafik_range_max, doc, 1);
	xmlAddChild(node_grafik, grafik_range_max);
	xmlSetProp(grafik_range_max, (const xmlChar*) "name", (const xmlChar*) "range_max");
	xmlNodeSetContent(grafik_range_max, (const xmlChar*) "100");
	
	xmlNodePtr grafik_alarm_min_min = node_grafik->children;	
	grafik_alarm_min_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_alarm_min_min = xmlDocCopyNode(grafik_alarm_min_min, doc, 1);
	xmlAddChild(node_grafik, grafik_alarm_min_min);
	xmlSetProp(grafik_alarm_min_min, (const xmlChar*) "name", (const xmlChar*) "alarm_min_min");
	xmlNodeSetContent(grafik_alarm_min_min, (const xmlChar*) "10");
	
	xmlNodePtr grafik_alarm_min = node_grafik->children;	
	grafik_alarm_min = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_alarm_min = xmlDocCopyNode(grafik_alarm_min, doc, 1);
	xmlAddChild(node_grafik, grafik_alarm_min);
	xmlSetProp(grafik_alarm_min, (const xmlChar*) "name", (const xmlChar*) "alarm_min");
	xmlNodeSetContent(grafik_alarm_min, (const xmlChar*) "20");
	
	xmlNodePtr grafik_alarm_max = node_grafik->children;	
	grafik_alarm_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_alarm_max = xmlDocCopyNode(grafik_alarm_max, doc, 1);
	xmlAddChild(node_grafik, grafik_alarm_max);
	xmlSetProp(grafik_alarm_max, (const xmlChar*) "name", (const xmlChar*) "alarm_max");
	xmlNodeSetContent(grafik_alarm_max, (const xmlChar*) "80");
	
	xmlNodePtr grafik_alarm_max_max = node_grafik->children;	
	grafik_alarm_max_max = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_alarm_max_max = xmlDocCopyNode(grafik_alarm_max_max, doc, 1);
	xmlAddChild(node_grafik, grafik_alarm_max_max);
	xmlSetProp(grafik_alarm_max_max, (const xmlChar*) "name", (const xmlChar*) "alarm_max_max");
	xmlNodeSetContent(grafik_alarm_max_max, (const xmlChar*) "90");
	
	xmlNodePtr grafik_alarm_min_min_enable = node_grafik->children;	
	grafik_alarm_min_min_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_alarm_min_min_enable = xmlDocCopyNode(grafik_alarm_min_min_enable, doc, 1);
	xmlAddChild(node_grafik, grafik_alarm_min_min_enable);
	xmlSetProp(grafik_alarm_min_min_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_min_min_enable");
	xmlNodeSetContent(grafik_alarm_min_min_enable, (const xmlChar*) "1");
	
	xmlNodePtr grafik_alarm_min_enable = node_grafik->children;	
	grafik_alarm_min_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_alarm_min_enable = xmlDocCopyNode(grafik_alarm_min_enable, doc, 1);
	xmlAddChild(node_grafik, grafik_alarm_min_enable);
	xmlSetProp(grafik_alarm_min_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_min_enable");
	xmlNodeSetContent(grafik_alarm_min_enable, (const xmlChar*) "1");
	
	xmlNodePtr grafik_alarm_max_enable = node_grafik->children;	
	grafik_alarm_max_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_alarm_max_enable = xmlDocCopyNode(grafik_alarm_max_enable, doc, 1);
	xmlAddChild(node_grafik, grafik_alarm_max_enable);
	xmlSetProp(grafik_alarm_max_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_max_enable");
	xmlNodeSetContent(grafik_alarm_max_enable, (const xmlChar*) "1");
	
	xmlNodePtr grafik_alarm_max_max_enable = node_grafik->children;	
	grafik_alarm_max_max_enable = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_alarm_max_max_enable = xmlDocCopyNode(grafik_alarm_max_max_enable, doc, 1);
	xmlAddChild(node_grafik, grafik_alarm_max_max_enable);
	xmlSetProp(grafik_alarm_max_max_enable, (const xmlChar*) "name", (const xmlChar*) "alarm_max_max_enable");
	xmlNodeSetContent(grafik_alarm_max_max_enable, (const xmlChar*) "1");
		
	//child
	
	xmlNodePtr grafik_child = node_grafik->children;	
	grafik_child = xmlNewNode(NULL, (const xmlChar*)"child");	
	grafik_child = xmlDocCopyNode(grafik_child, doc, 1);
	xmlAddChild(node_grafik, grafik_child);
	
	//posisi
	
	xmlNodePtr grafik_posisi = grafik_child->children;
	grafik_posisi = xmlNewNode(NULL, (const xmlChar*)"object");	
	grafik_posisi = xmlDocCopyNode(grafik_posisi, doc, 1);
	xmlAddChild(grafik_child, grafik_posisi);
	xmlSetProp(grafik_posisi, (const xmlChar*) "struct", (const xmlChar*) "obj_titik" );
	xmlSetProp(grafik_posisi, (const xmlChar*) "id", (const xmlChar*) "posisi" );
	
	xmlNodePtr grafik_posisi_x = grafik_posisi->children;	
	grafik_posisi_x = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_posisi_x = xmlDocCopyNode(grafik_posisi_x, doc, 1);
	xmlAddChild(grafik_posisi, grafik_posisi_x);
	xmlSetProp(grafik_posisi_x, (const xmlChar*) "name", (const xmlChar*) "x");
	xmlNodeSetContent(grafik_posisi_x, (const xmlChar*) "0");
	
	xmlNodePtr grafik_posisi_y = grafik_posisi->children;	
	grafik_posisi_y = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_posisi_y = xmlDocCopyNode(grafik_posisi_y, doc, 1);
	xmlAddChild(grafik_posisi, grafik_posisi_y);
	xmlSetProp(grafik_posisi_y, (const xmlChar*) "name", (const xmlChar*) "y");
	xmlNodeSetContent(grafik_posisi_y, (const xmlChar*) "0");
	
	//warna
	
	xmlNodePtr grafik_warna = grafik_child->children;
	grafik_warna = xmlNewNode(NULL, (const xmlChar*)"object");	
	grafik_warna = xmlDocCopyNode(grafik_warna, doc, 1);
	xmlAddChild(grafik_child, grafik_warna);
	xmlSetProp(grafik_warna, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(grafik_warna, (const xmlChar*) "id", (const xmlChar*) "warna" );
	
	xmlNodePtr grafik_warna_r = grafik_warna->children;	
	grafik_warna_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_r = xmlDocCopyNode(grafik_warna_r, doc, 1);
	xmlAddChild(grafik_warna, grafik_warna_r);
	xmlSetProp(grafik_warna_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(grafik_warna_r, (const xmlChar*) "0");
	
	xmlNodePtr grafik_warna_g = grafik_warna->children;	
	grafik_warna_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_g = xmlDocCopyNode(grafik_warna_g, doc, 1);
	xmlAddChild(grafik_warna, grafik_warna_g);
	xmlSetProp(grafik_warna_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(grafik_warna_g, (const xmlChar*) "0");
	
	xmlNodePtr grafik_warna_b = grafik_warna->children;	
	grafik_warna_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_b = xmlDocCopyNode(grafik_warna_b, doc, 1);
	xmlAddChild(grafik_warna, grafik_warna_b);
	xmlSetProp(grafik_warna_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(grafik_warna_b, (const xmlChar*) "255");
	
	xmlNodePtr grafik_warna_a = grafik_warna->children;	
	grafik_warna_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_a = xmlDocCopyNode(grafik_warna_a, doc, 1);
	xmlAddChild(grafik_warna, grafik_warna_a);
	xmlSetProp(grafik_warna_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(grafik_warna_a, (const xmlChar*) "100");
	
	//warna_min_min
	
	xmlNodePtr grafik_warna_min_min = grafik_child->children;
	grafik_warna_min_min = xmlNewNode(NULL, (const xmlChar*)"object");	
	grafik_warna_min_min = xmlDocCopyNode(grafik_warna_min_min, doc, 1);
	xmlAddChild(grafik_child, grafik_warna_min_min);
	xmlSetProp(grafik_warna_min_min, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(grafik_warna_min_min, (const xmlChar*) "id", (const xmlChar*) "warna_min_min" );
	
	xmlNodePtr grafik_warna_min_min_r = grafik_warna_min_min->children;	
	grafik_warna_min_min_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_min_min_r = xmlDocCopyNode(grafik_warna_min_min_r, doc, 1);
	xmlAddChild(grafik_warna_min_min, grafik_warna_min_min_r);
	xmlSetProp(grafik_warna_min_min_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(grafik_warna_min_min_r, (const xmlChar*) "100");
	
	xmlNodePtr grafik_warna_min_min_g = grafik_warna_min_min->children;	
	grafik_warna_min_min_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_min_min_g = xmlDocCopyNode(grafik_warna_min_min_g, doc, 1);
	xmlAddChild(grafik_warna_min_min, grafik_warna_min_min_g);
	xmlSetProp(grafik_warna_min_min_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(grafik_warna_min_min_g, (const xmlChar*) "100");
	
	xmlNodePtr grafik_warna_min_min_b = grafik_warna_min_min->children;	
	grafik_warna_min_min_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_min_min_b = xmlDocCopyNode(grafik_warna_min_min_b, doc, 1);
	xmlAddChild(grafik_warna_min_min, grafik_warna_min_min_b);
	xmlSetProp(grafik_warna_min_min_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(grafik_warna_min_min_b, (const xmlChar*) "100");
	
	xmlNodePtr grafik_warna_min_min_a = grafik_warna_min_min->children;	
	grafik_warna_min_min_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_min_min_a = xmlDocCopyNode(grafik_warna_min_min_a, doc, 1);
	xmlAddChild(grafik_warna_min_min, grafik_warna_min_min_a);
	xmlSetProp(grafik_warna_min_min_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(grafik_warna_min_min_a, (const xmlChar*) "100");
	
	//warna_min
	
	xmlNodePtr grafik_warna_min = grafik_child->children;
	grafik_warna_min = xmlNewNode(NULL, (const xmlChar*)"object");	
	grafik_warna_min = xmlDocCopyNode(grafik_warna_min, doc, 1);
	xmlAddChild(grafik_child, grafik_warna_min);
	xmlSetProp(grafik_warna_min, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(grafik_warna_min, (const xmlChar*) "id", (const xmlChar*) "warna_min" );
	
	xmlNodePtr grafik_warna_min_r = grafik_warna_min->children;	
	grafik_warna_min_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_min_r = xmlDocCopyNode(grafik_warna_min_r, doc, 1);
	xmlAddChild(grafik_warna_min, grafik_warna_min_r);
	xmlSetProp(grafik_warna_min_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(grafik_warna_min_r, (const xmlChar*) "200");
	
	xmlNodePtr grafik_warna_min_g = grafik_warna_min->children;	
	grafik_warna_min_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_min_g = xmlDocCopyNode(grafik_warna_min_g, doc, 1);
	xmlAddChild(grafik_warna_min, grafik_warna_min_g);
	xmlSetProp(grafik_warna_min_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(grafik_warna_min_g, (const xmlChar*) "200");
	
	xmlNodePtr grafik_warna_min_b = grafik_warna_min->children;	
	grafik_warna_min_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_min_b = xmlDocCopyNode(grafik_warna_min_b, doc, 1);
	xmlAddChild(grafik_warna_min, grafik_warna_min_b);
	xmlSetProp(grafik_warna_min_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(grafik_warna_min_b, (const xmlChar*) "200");
	
	xmlNodePtr grafik_warna_min_a = grafik_warna_min->children;	
	grafik_warna_min_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_min_a = xmlDocCopyNode(grafik_warna_min_a, doc, 1);
	xmlAddChild(grafik_warna_min, grafik_warna_min_a);
	xmlSetProp(grafik_warna_min_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(grafik_warna_min_a, (const xmlChar*) "100");
	
	//warna_max
	
	xmlNodePtr grafik_warna_max = grafik_child->children;
	grafik_warna_max = xmlNewNode(NULL, (const xmlChar*)"object");	
	grafik_warna_max = xmlDocCopyNode(grafik_warna_max, doc, 1);
	xmlAddChild(grafik_child, grafik_warna_max);
	xmlSetProp(grafik_warna_max, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(grafik_warna_max, (const xmlChar*) "id", (const xmlChar*) "warna_max" );
	
	xmlNodePtr grafik_warna_max_r = grafik_warna_max->children;	
	grafik_warna_max_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_max_r = xmlDocCopyNode(grafik_warna_max_r, doc, 1);
	xmlAddChild(grafik_warna_max, grafik_warna_max_r);
	xmlSetProp(grafik_warna_max_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(grafik_warna_max_r, (const xmlChar*) "200");
	
	xmlNodePtr grafik_warna_max_g = grafik_warna_max->children;	
	grafik_warna_max_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_max_g = xmlDocCopyNode(grafik_warna_max_g, doc, 1);
	xmlAddChild(grafik_warna_max, grafik_warna_max_g);
	xmlSetProp(grafik_warna_max_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(grafik_warna_max_g, (const xmlChar*) "200");
	
	xmlNodePtr grafik_warna_max_b = grafik_warna_max->children;	
	grafik_warna_max_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_max_b = xmlDocCopyNode(grafik_warna_max_b, doc, 1);
	xmlAddChild(grafik_warna_max, grafik_warna_max_b);
	xmlSetProp(grafik_warna_max_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(grafik_warna_max_b, (const xmlChar*) "50");
	
	xmlNodePtr grafik_warna_max_a = grafik_warna_max->children;	
	grafik_warna_max_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_max_a = xmlDocCopyNode(grafik_warna_max_a, doc, 1);
	xmlAddChild(grafik_warna_max, grafik_warna_max_a);
	xmlSetProp(grafik_warna_max_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(grafik_warna_max_a, (const xmlChar*) "100");
	
	//warna_max_max
	
	xmlNodePtr grafik_warna_max_max = grafik_child->children;
	grafik_warna_max_max = xmlNewNode(NULL, (const xmlChar*)"object");	
	grafik_warna_max_max = xmlDocCopyNode(grafik_warna_max_max, doc, 1);
	xmlAddChild(grafik_child, grafik_warna_max_max);
	xmlSetProp(grafik_warna_max_max, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(grafik_warna_max_max, (const xmlChar*) "id", (const xmlChar*) "warna_max_max" );
	
	xmlNodePtr grafik_warna_max_max_r = grafik_warna_max_max->children;	
	grafik_warna_max_max_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_max_max_r = xmlDocCopyNode(grafik_warna_max_max_r, doc, 1);
	xmlAddChild(grafik_warna_max_max, grafik_warna_max_max_r);
	xmlSetProp(grafik_warna_max_max_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(grafik_warna_max_max_r, (const xmlChar*) "200");
	
	xmlNodePtr grafik_warna_max_max_g = grafik_warna_max_max->children;	
	grafik_warna_max_max_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_max_max_g = xmlDocCopyNode(grafik_warna_max_max_g, doc, 1);
	xmlAddChild(grafik_warna_max_max, grafik_warna_max_max_g);
	xmlSetProp(grafik_warna_max_max_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(grafik_warna_max_max_g, (const xmlChar*) "50");
	
	xmlNodePtr grafik_warna_max_max_b = grafik_warna_max_max->children;	
	grafik_warna_max_max_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_max_max_b = xmlDocCopyNode(grafik_warna_max_max_b, doc, 1);
	xmlAddChild(grafik_warna_max_max, grafik_warna_max_max_b);
	xmlSetProp(grafik_warna_max_max_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(grafik_warna_max_max_b, (const xmlChar*) "0");
	
	xmlNodePtr grafik_warna_max_max_a = grafik_warna_max_max->children;	
	grafik_warna_max_max_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_max_max_a = xmlDocCopyNode(grafik_warna_max_max_a, doc, 1);
	xmlAddChild(grafik_warna_max_max, grafik_warna_max_max_a);
	xmlSetProp(grafik_warna_max_max_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(grafik_warna_max_max_a, (const xmlChar*) "100");
	
	//warna_oor
	
	xmlNodePtr grafik_warna_oor = grafik_child->children;
	grafik_warna_oor = xmlNewNode(NULL, (const xmlChar*)"object");	
	grafik_warna_oor = xmlDocCopyNode(grafik_warna_oor, doc, 1);
	xmlAddChild(grafik_child, grafik_warna_oor);
	xmlSetProp(grafik_warna_oor, (const xmlChar*) "struct", (const xmlChar*) "color_rgba" );
	xmlSetProp(grafik_warna_oor, (const xmlChar*) "id", (const xmlChar*) "warna_oor" );
	
	xmlNodePtr grafik_warna_oor_r = grafik_warna_oor->children;	
	grafik_warna_oor_r = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_oor_r = xmlDocCopyNode(grafik_warna_oor_r, doc, 1);
	xmlAddChild(grafik_warna_oor, grafik_warna_oor_r);
	xmlSetProp(grafik_warna_oor_r, (const xmlChar*) "name", (const xmlChar*) "r");
	xmlNodeSetContent(grafik_warna_oor_r, (const xmlChar*) "100");
	
	xmlNodePtr grafik_warna_oor_g = grafik_warna_oor->children;	
	grafik_warna_oor_g = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_oor_g = xmlDocCopyNode(grafik_warna_oor_g, doc, 1);
	xmlAddChild(grafik_warna_oor, grafik_warna_oor_g);
	xmlSetProp(grafik_warna_oor_g, (const xmlChar*) "name", (const xmlChar*) "g");
	xmlNodeSetContent(grafik_warna_oor_g, (const xmlChar*) "100");
	
	xmlNodePtr grafik_warna_oor_b = grafik_warna_oor->children;	
	grafik_warna_oor_b = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_oor_b = xmlDocCopyNode(grafik_warna_oor_b, doc, 1);
	xmlAddChild(grafik_warna_oor, grafik_warna_oor_b);
	xmlSetProp(grafik_warna_oor_b, (const xmlChar*) "name", (const xmlChar*) "b");
	xmlNodeSetContent(grafik_warna_oor_b, (const xmlChar*) "100");
	
	xmlNodePtr grafik_warna_oor_a = grafik_warna_oor->children;	
	grafik_warna_oor_a = xmlNewNode(NULL, (const xmlChar*)"property");	
	grafik_warna_oor_a = xmlDocCopyNode(grafik_warna_oor_a, doc, 1);
	xmlAddChild(grafik_warna_oor, grafik_warna_oor_a);
	xmlSetProp(grafik_warna_oor_a, (const xmlChar*) "name", (const xmlChar*) "a");
	xmlNodeSetContent(grafik_warna_oor_a, (const xmlChar*) "100");
}

void EditorWindow::new_file_xml()
{

}

void EditorWindow::format_xml()
{
	xmlKeepBlanksDefault(0);
	xmlDocPtr doc1;
	doc1 = xmlParseFile(filename);
	xmlSaveFormatFile(filename,doc1,1);
	xmlFreeDoc(doc1);
	xmlKeepBlanksDefault(0);
}

void EditorWindow::delete_xml_object()
{
	if(m_obj_entry_name->get_text() == "")
	{
		printf("select object to be deleted!\n");
	}
	else
	{
		Glib::ustring str;
		const char *val = NULL;	
		
		str = m_obj_entry_name->get_text();
		locale_from_utf8 (str);
		val = str.c_str();
		
		for(nodeLevel1 = doc->children;	nodeLevel1 != NULL;	nodeLevel1 = nodeLevel1->next)
		{		
			for(nodeLevel2 = nodeLevel1->children; nodeLevel2 != NULL; nodeLevel2 = nodeLevel2->next)
			{
				if((strcmp((char*)nodeLevel2->name, "object") == 0) && (strcmp((const char*)xmlGetProp(nodeLevel2, (const xmlChar*)"id"), val) == 0))
				{
					xmlUnlinkNode(nodeLevel2);
					xmlFreeNode(nodeLevel2);
				}
			}
		}
		
		load_tree();
	}
}

void EditorWindow::on_button_delete()
{
	delete_xml_object();
}

void EditorWindow::load_objects()
{
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
		for(int g = 0; g < jumlah_grafik; g++)
		{
			for(int i = 0; i < 61; i++)
			{			
				m_obj_grafik[g].value[i] = 0;
				m_obj_grafik[g].temp[i] = 0;
			}
		}
	}
}

void EditorWindow::view_garis()
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

void EditorWindow::view_kotak()
{
	for(int k = 0; k < jumlah_kotak; k++)
	{
		int i=k;
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

void EditorWindow::view_gambar()
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

void EditorWindow::view_gambar_aktif()
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

void EditorWindow::view_animasi()
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

void EditorWindow::view_teks()
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

void EditorWindow::view_teks_aktif()
{
	for(int t = 0; t < jumlah_teks_aktif; t++)
	{		
		m_obj_teks_aktif[t].data = (double)((rand() % 100) + 0);
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

void EditorWindow::view_gauge_bar()
{
}

void EditorWindow::view_gauge_dial()
{
	char cDataTemp[32];
	for(int k=0; k < jumlah_gauge_dial; k++)
    {
        m_obj_gauge_dial[k].data = (double)((rand() % 100) + 0);
        
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
        //cr->restore();

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
                sprintf(cDataTemp,"%0.1f",m_obj_gauge_dial[k].data);
            else
                sprintf(cDataTemp,"%g",m_obj_gauge_dial[k].data);

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

void EditorWindow::view_grafik()
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
		cr->set_source_rgba(m_obj_grafik[g].warna_max.r/255, m_obj_grafik[g].warna.g/255, m_obj_grafik[g].warna.b/255, m_obj_grafik[g].warna.a/100);
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

void EditorWindow::view_grid()
{
	cr->save();
	cr->set_line_width(2);
	std::valarray< double > dashes(2);
	dashes[0] = 2.0;
	dashes[1] = 2.0;    
	cr->set_dash (dashes, 0.0);
	
	cr->set_source_rgba(100, 100, 100, 100);
	
	for(int a = 0; a < 50; a++)
	{
		cr->move_to((a*50), 0);
		cr->line_to((a*50), 650);
		cr->stroke();
	}
	
	for(int b = 0; b < 20; b++)
	{
		cr->move_to(0, (b*50));
		cr->line_to(1200, (b*50));
		cr->stroke();
	}
	cr->restore();
}

bool EditorWindow::on_drawingarea_preview_expose_event(GdkEventExpose* event)
{	
	if(isOpened == true)
	{	
		if(m_preview_area)
		{
			cr = m_preview_area->get_window()->create_cairo_context();
		
			cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
			cr->clip();
			cr->scale(1, 1);
			
			if(showGrid == true)
				view_grid();
			
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
		}
		return true;
	}
}

bool EditorWindow::on_drawingarea_preview_motion_notify_event(GdkEventMotion *event)
{
	if(isOpened == true)
    {
        x_mouse = event->x;
        y_mouse = event->y;
        char posisi[64];
		sprintf(posisi, "cursor: (x = %g, y = %g)", x_mouse, y_mouse);
        m_status_posisi->set_text(posisi);
    }
}

void EditorWindow::delete_objects()
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

void EditorWindow::reload_objects()
{	
	delete_objects();
	load_objects();	
	m_preview_area->queue_draw();
}

bool EditorWindow::timer_callback()
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
