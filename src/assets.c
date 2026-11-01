#include "assets.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define change_dir _chdir
#else
#include <unistd.h>
#define change_dir chdir
#endif

static int file_exists(const char* path) {
	FILE* file = fopen(path, "rb");
	if (file) fclose(file);
	return file != NULL;
}

static void executable_folder(char* folder, size_t size) {
	folder[0] = '\0';
#ifdef _WIN32
	{
		char* path = NULL;
		if (_get_pgmptr(&path) != 0 || !path) return;
		snprintf(folder, size, "%s", path);
	}
#else
	{
		ssize_t length = readlink("/proc/self/exe", folder, size - 1);
		if (length <= 0) return;
		folder[length] = '\0';
	}
#endif
	{
		char* slash = strrchr(folder, '/');
		char* backslash = strrchr(folder, '\\');
		if (backslash && (!slash || backslash > slash)) slash = backslash;
		if (slash) *slash = '\0';
		else folder[0] = '\0';
	}
}

void assets_enter_folder(const char* app_id) {
	char folder[1024], relative[2][160], candidate[1400];
	int i;

	if (file_exists("main.xml")) return;
	executable_folder(folder, sizeof(folder));
	if (!folder[0]) return;
	snprintf(relative[0], sizeof(relative[0]), "assets");
	snprintf(relative[1], sizeof(relative[1]), "../share/%s", app_id);
	for (i = 0; i < 2; i++) {
		snprintf(candidate, sizeof(candidate), "%s/%s/main.xml", folder, relative[i]);
		if (file_exists(candidate)) {
			snprintf(candidate, sizeof(candidate), "%s/%s", folder, relative[i]);
			if (change_dir(candidate) == 0) return;
		}
	}
}
