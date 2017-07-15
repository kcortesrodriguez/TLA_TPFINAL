#include <stdio.h>
#include "joutput.h"

FILE * jfile;
void outputinit() {
	jfile = fopen("output.java", "w");
}

void output(char * toput) {
	fprintf(jfile, "%s\n", toput);
}

void outputfinish() {
	fclose(jfile);
}