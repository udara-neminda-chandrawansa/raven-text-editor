#include "app.h"

#include <stdio.h>

static struct {
	Rid window;
} app;

void app_register_actions(void) {
	/* No actions needed for basic typing */
}

void app_window_ready(Rid window) {
	app.window = window;
}
