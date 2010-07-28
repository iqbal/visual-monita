#include <gtkmm.h>
#include <iostream>
#include "editor.hh"

int main (int argc, char **argv)
{
	Gtk::Main kit(argc, argv);
	EditorWindow* EditorWindow = 0;
	
	//Load the GtkBuilder file and instantiate its widgets:
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
	#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try
	{
		refBuilder->add_from_file("editor.glade");
	}
	catch(const Glib::FileError& ex)
	{
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	}
	catch(const Gtk::BuilderError& ex)
	{
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
	}
	#else
	std::auto_ptr<Glib::Error> error;

	if (!refBuilder->add_from_file("editor.glade", error))
	{
		std::cerr << error->what() << std::endl;
		return 1;
	}
	#endif /* !GLIBMM_EXCEPTIONS_ENABLED */
 
	refBuilder->get_widget_derived("EditorWindow", EditorWindow);

	if(EditorWindow)
	{
		kit.run(*EditorWindow);
	}

	return 0;
}

