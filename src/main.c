#include "app/compiler.h"
#include "app/util.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: sel <input> <output.s>\n");
		return 64;
	}
	char *source = read_file(argv[1]);

	FILE *out = fopen(argv[2], "w");
	if (out == NULL) {
		fprintf(stderr, "could not open \"%s\"\n", argv[2]);
		return 74;
	}
	compile(out, source);
	fclose(out);
	free(source);
	return 0;
}
