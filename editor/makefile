#makefile

gtk_file_source = main.cc editor.cc parseXMLeditor.cc

gtk_output = editor
nama_project = editor

app:
	echo "KOMPILASI $(nama_project)"
	g++ -g $(gtk_file_source) -o $(gtk_output) `pkg-config --cflags --libs gtkmm-2.4 libxml-2.0`
	echo SELESAI KOMPILASI
