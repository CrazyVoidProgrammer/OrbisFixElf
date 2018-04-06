#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "toolbox.h"
#include "myelf.h"

/*
char *read_section(FILE *fp, OrbisSection sec)
{
	char *buff = malloc(sec.size);
	lseek(fp, (off_t)sec.offset, SEEK_SET);
	read(fp, buff, sec.size);
	return buff;
}
*/

// Not Tested- writting loose code for later
void write_section_data(OrbisSection *sec, FILE *exportElf)
{
	fseek(exportElf, sec->offset, SEEK_SET);
	fwrite(sec->data, sec->dataSize, 1, exportElf);
}