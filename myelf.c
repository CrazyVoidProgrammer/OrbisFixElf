#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <elf.h>

#include "myelf.h"


OrbisElf loadElf(char *path)
{
	OrbisElf returnElf;
	returnElf.errorCode = 0;
	
	FILE *elfFH = fopen(path, "rb");
	long elfSize;
	
	printf("[INFO] Loading %s \n", path);
	if(elfFH != NULL)
	{
		fseek(elfFH, 0, SEEK_END);
		elfSize = ftell(elfFH);
		fseek(elfFH, 0, SEEK_SET);
		returnElf.buffer = malloc(elfSize + 1);
		fread(returnElf.buffer, elfSize, 1, elfFH);
		fclose(elfFH);
		
		printf("[INFO] Magic Check : 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx\n", returnElf.buffer[0], returnElf.buffer[1], returnElf.buffer[2], returnElf.buffer[3]);
			
		
		if(returnElf.buffer[0] == 0x7f && returnElf.buffer[1] == 'E' && returnElf.buffer[2] == 'L' && returnElf.buffer[3] == 'F')
		{
			printf("[INFO] %s passed ELF Magic Check!\n", path);

			return returnElf;
		}
		else
		{
			returnElf.errorCode = -2;
			return returnElf;
		}
		
	}
	else
	{
		returnElf.errorCode = -1;
		return returnElf;
	}
}

void orbisParseHeader(OrbisElf *inputElf)
{
	unsigned long hsize = sizeof(OrbisElfHeader_t);
	
	OrbisElfHeader_t *ehdr = (OrbisElfHeader_t *)inputElf->buffer;
	inputElf->header = ehdr;
	

	printf("\nInput Elf Header\n============================================================\n");
	printf("Magic:\t\t\t\t\t0x%02x 0x%02x 0x%02x 0x%02x\n", inputElf->header->magic[0], inputElf->header->magic[1], inputElf->header->magic[2], inputElf->header->magic[3]);
	printf("Class:\t\t\t\t\t%d\n", inputElf->header->eclass);
	printf("Data:\t\t\t\t\t%d\n", inputElf->header->data);
	printf("Version:\t\t\t\t%d\n", inputElf->header->eversion);
	printf("OS/ABI:\t\t\t\t\t0x%02x\n", inputElf->header->osabi);
	printf("ABI Version:\t\t\t\t%d\n", inputElf->header->abiver);
	//printf("Pad:\t0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", inputElf->header->pad[0], inputElf->header->pad[1], inputElf->header->pad[2], inputElf->header->pad[3], inputElf->header->pad[4], inputElf->header->pad[5], inputElf->header->pad[6]);
	printf("Type:\t\t\t\t\t0x%d\n", inputElf->header->type);
	printf("Machine:\t\t\t\t0x%02x\n", inputElf->header->machine);
	printf("Version:\t\t\t\t0x%d\n", inputElf->header->version);
	printf("Entry Point Address:\t\t\t0x%jx\n", inputElf->header->entry);
	printf("Start of Program Headers:\t\t%lu\n", inputElf->header->phoff);
	printf("Start of section headers:\t\t%lu \n", inputElf->header->shoff);
	printf("Flags:\t\t\t\t\t0x%d\n", inputElf->header->flags);
	printf("Size of this header:\t\t\t%d \n", inputElf->header->ehsize);
	printf("Size of Program Headers:\t\t%d \n", inputElf->header->phentsize);
	printf("Number of Program Headers:\t\t%d \n", inputElf->header->phnum);
	printf("Size of section headers:\t\t%d (Bytes)\n", inputElf->header->shentsize);
	printf("Number of section headers:\t\t%d\n", inputElf->header->shnum);
	printf("Section header string table index:\t%d\n", inputElf->header->shstrndx);
	printf("============================================================\n");
}