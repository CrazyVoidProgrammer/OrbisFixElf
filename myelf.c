#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <elf.h>
#include <gelf.h>

#include "varray.h"
#include "toolbox.h"
#include "myelf.h"


/*
	OLD FUNCTIONS FROM BEFORE REFACTOR
int verifyElf(OpenElf *inputElf)
{
	if(inputElf->buffer[0] == 0x7f && inputElf->buffer[1] == 'E' && inputElf->buffer[2] == 'L' && inputElf->buffer[3] == 'F')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void verboseElf(OpenElf inputElf)
{
	// Elf Header Dump
	printf("\nInput Elf Header\n============================================================\n");
	printf("Magic:\t\t\t\t\t0x%02x 0x%02x 0x%02x 0x%02x\n", inputElf.header.magic[0], inputElf.header.magic[1], inputElf.header.magic[2], inputElf.header.magic[3]);
	printf("Class:\t\t\t\t\t%d\n", inputElf.header.eclass);
	printf("Data:\t\t\t\t\t%d\n", inputElf.header.data);
	printf("Version:\t\t\t\t%d\n", inputElf.header.eversion);
	printf("OS/ABI:\t\t\t\t\t0x%02x\n", inputElf.header.osabi);
	printf("ABI Version:\t\t\t\t%d\n", inputElf.header.abiver);
	//printf("Pad:\t0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", inputElf.header.pad[0], inputElf.header.pad[1], inputElf.header.pad[2], inputElf.header.pad[3], inputElf.header.pad[4], inputElf.header.pad[5], inputElf.header.pad[6]);
	printf("Type:\t\t\t\t\t0x%d\n", inputElf.header.type);
	printf("Machine:\t\t\t\t0x%02x\n", inputElf.header.machine);
	printf("Version:\t\t\t\t0x%d\n", inputElf.header.version);
	printf("Entry Point Address:\t\t\t0x%jx\n", inputElf.header.entry);
	printf("Start of Program Headers:\t\t%lu\n", inputElf.header.phoff);
	printf("Start of section headers:\t\t%lu \n", inputElf.header.shoff);
	printf("Flags:\t\t\t\t\t0x%d\n", inputElf.header.flags);
	printf("Size of this header:\t\t\t%d \n", inputElf.header.ehsize);
	printf("Size of Program Headers:\t\t%d \n", inputElf.header.phentsize);
	printf("Number of Program Headers:\t\t%d \n", inputElf.header.phnum);
	printf("Size of section headers:\t\t%d (Bytes)\n", inputElf.header.shentsize);
	printf("Number of section headers:\t\t%d\n", inputElf.header.shnum);
	printf("Section header string table index:\t%d\n", inputElf.header.shstrndx);
	printf("============================================================\n");
	// Program Header Dump
	printf("\n\nInput Elf Program Header\n============================================================\n");
	printf("type:\t\t\t\t\t0x%d\n", inputElf.pheader.type);
	printf("flags:\t\t\t\t\t0x%d\n", inputElf.pheader.flags);
	printf("offset:\t\t\t\t\t%lu\n", inputElf.pheader.offset);
	printf("vaddr:\t\t\t\t\t0x%jx\n", inputElf.pheader.vaddr);
	printf("paddr:\t\t\t\t\t%lu\n", inputElf.pheader.paddr);
	printf("filesz:\t\t\t\t\t%lu\n", inputElf.pheader.filesz);
	printf("memsz:\t\t\t\t\t%lu\n", inputElf.pheader.memsz);
	printf("align:\t\t\t\t\t%lu\n", inputElf.pheader.align);
	printf("============================================================\n");
}
*/