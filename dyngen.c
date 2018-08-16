#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <elf.h>
#include <gelf.h>

#include "varray.h"
#include "toolbox.h"
#include "myelf.h"


#include "dyngen.h"



/*
struct orbis_version
{
	char *elfName; // example .hello.elf
	uint32_t elfPad;
	uint8_t elfPad2;
	char elfType[18]; // "file format elf64-x86-64"
	char elfSecPrefix[15]; // "..Contents of section"
	uint16_t elfSecPrefixPad;
	
	struct orbis_version_entry ovEntrys[10];
};
*/

/*
struct orbis_version *generate_sce_version()
{
	orbis_version temp_version;
	
	temp_version.elfName = ".homebrew.elf";

	// apply custom padding
	temp_version.elfPad = 0x20202020;
	temp_version.elfPad2 = 0x20;
	temp_version.elfType = "file format elf64-x86-64";
	temp_version.elfSecPrefix = "..Contents of section";
	temp_version.elfSecPrefixPad = 0x0A0A;
	
	// Still need to generate section informations
	
	return temp_version;
}
*/

/*
orbis_process_param generate_orbis_process_param(uint16_t iSDK_Build, uint8_t iSDK_Major, uint8_t iSDK_Minor)
{
	orbis_process_param ropp;
	
	ropp.Magic = SCEPROCPARAMS_MAGIC;
	ropp.ThingCount = 1;
	
	ropp.SDK_Build = iSDK_Build;
	ropp.SDK_Minor = iSDK_Minor;
	ropp.SDK_Major = iSDK_Major;
	
	// Generate padding
	for(int x = 0; x < 11; x++)
	{
		ropp.pad[x] = 0x00000000;
	}

	// Note
	// In the future maybe calc this! but since the 
	// size should not change, we can leave this static
	ropp.Size = 64;
	
	return ropp;
}

*/

/*
void *generate_sce_process_param(int type)
{
	// As of right now, im only designing this for game homebrews
	// People can add support for prx and modules etc.
	// TODO
	// Add prx process param as ``case 2``
	switch(type)
	{
		case 1:
			// Returns game process param
			return (void *){ 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x52, 0x42, 0x49, 0x03, 0x00, 0x00, 0x00, 0x01, 0x81, 0x50, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x44, 0x59, 0x01, 0x00, 0x00, 0x00, 0x00, 0x50, 0x45, 0x59, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x45, 0x59, 0x01, 0x00, 0x00, 0x00, 0x00 };
		break;
		default:
			// Returns game process param as default
			return (void *){ 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x52, 0x42, 0x49, 0x03, 0x00, 0x00, 0x00, 0x01, 0x81, 0x50, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x44, 0x59, 0x01, 0x00, 0x00, 0x00, 0x00, 0x50, 0x45, 0x59, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x45, 0x59, 0x01, 0x00, 0x00, 0x00, 0x00 };
		break;
	}
}
*/

struct orbis_comment generate_sce_comment(char *path)
{
	struct orbis_comment gen_comment;
	
	gen_comment.Magic = ORBIS_COMMENT_MAGIC;
	gen_comment.path = path;
	gen_comment.pad = 0x00000000;
	
	int path_size = strlen(path);
	
	gen_comment.OtherSize = (path_size + 8);
	gen_comment.EntrySize = (path_size + 1);
	
	return gen_comment;
}

void *consolidate_orbis_comment(struct orbis_comment ioc)
{
	int size_of_path = strlen(ioc.path);
	int projected_size = (size_of_path + 16 + 1);
	int current_size = 0;

	void *tempBuff = malloc(projected_size);
	
	memcpy(tempBuff, &ioc.Magic, sizeof(ioc.Magic)+1);
	current_size += sizeof(ioc.Magic);

	memcpy(tempBuff+current_size, &ioc.OtherSize, sizeof(ioc.OtherSize)+1);
	current_size += sizeof(ioc.OtherSize);

	memcpy(tempBuff+current_size, &ioc.EntrySize, sizeof(ioc.EntrySize)+1);
	current_size += sizeof(ioc.EntrySize);

	memcpy(tempBuff+current_size, ioc.path, strlen(ioc.path)+1);
	current_size += strlen(ioc.path);

	memcpy(tempBuff+current_size, &ioc.pad, sizeof(ioc.pad)+1);
	current_size += sizeof(ioc.pad);

	return tempBuff;
}


void generate_sce_special()
{
	// Void
}