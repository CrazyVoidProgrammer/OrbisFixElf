#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "toolbox.h"
#include "myelf.h"

// Not Tested- writting loose code for later
void write_section_data(OrbisSection *sec, FILE *exportElf)
{
	fseek(exportElf, sec->offset, SEEK_SET);
	fwrite(sec->data, sec->dataSize, 1, exportElf);
}