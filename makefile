#makefile

gtk_file_source = main.cc visualmonita.cc parseXMLmonita.cc sql_routine.cc

gtk_output = visualmonita
nama_project = visualmonita

app:
	echo "KOMPILASI $(nama_project)"
	g++ -g $(gtk_file_source) -o $(gtk_output) `pkg-config --cflags --libs gtkmm-2.4 libxml-2.0` `mysql_config --libs --cflags`
	echo SELESAI KOMPILASI
