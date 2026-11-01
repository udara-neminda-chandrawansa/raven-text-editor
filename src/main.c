#include <Raven.h>

#include <stdio.h>

#include "app.h"
#include "assets.h"

static void on_error(RError code, RSeverity severity, const char* detail) {
	fprintf(stderr, "[Raven] %s (%s): %s\n", Rerror_to_string(code), Rseverity_to_string(severity), detail);
}

int main(void) {
	Rid window;

	Rerror_set_callback(on_error);
	assets_enter_folder("text_editor");

	if (!Rinit(R_BACKEND_VULKAN)) {
		fprintf(stderr, "Cannot start Raven. Is a Vulkan driver installed?\n");
		return 1;
	}

	Rlocale_load_csv("strings.csv");
	Rlocale_set_language(Rlocale_get_system_language());

	app_register_actions();

	window = Rwindow_create_from_xml_file("main.xml");
	if (window == R_INVALID_ID) {
		fprintf(stderr, "Cannot load main.xml (the program must run in the assets/ folder).\n");
		Rshutdown();
		return 1;
	}
	app_window_ready(window);

	while (Rwait_frame(-1.0)) {
	}

	Rshutdown();
	return 0;
}
