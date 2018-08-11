#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <elf.h>
#include <gelf.h>

#include "varray.h"
#include "toolbox.h"
#include "myelf.h"

extern int appDebug;

// ============================================================================
// UTIL FUNCTIONS START HERE ==================================================
// ============================================================================

void print_help_section()
{
	printf("\norbisFixElf - by Crazyvoid\n");
	printf("Version : 0.1 (POC) \n\n");
	printf("Example Usage  :  ./orbisFixElf input.elf output.elf\n\n");
}

void dump_args(int argc, char *argv[])
{
	printf("ARGC : %d\n", argc);
	for(int x = 0; x < argc; x++)
	{
		printf("ARGV[%d] : %s\n", x, argv[x]);
	}
}

// HexDump ripped from Wildcard Kernel Dumper
// Thanks WildCard
int hexDump(const void *data)
{
	size_t size = sizeof(data);
	unsigned char *d = (unsigned char *)data;
	size_t consoleSize = 16;
	char b[consoleSize + 3];
	size_t i;

	if(data == NULL){
		return -1;
		}
	b[0] = '|';
	b[consoleSize + 1] = '|';
	b[consoleSize + 2] = '\0';
	
	printf("\n-------HEX DUMP------\n");
	for (i = 0; i < size; i++)
	{
		if ((i % consoleSize) == 0)
		{
			if (i != 0){
				printf("  %s\n", b);
				}
			printf("%16s ", (unsigned char *)data + i);
		}

		if(i % consoleSize == 8)
			printf(" ");
		printf(" %02x", d[i]);

		if (d[i] >= ' ' && d[i] <= '~')
			b[i % consoleSize + 1] = d[i];
		else
			b[i % consoleSize + 1] = '.';
	}
	while((i % consoleSize) != 0)
	{
		if(i % consoleSize == 8)
			printf("    ");
		else
			printf("   ");
		b[i % consoleSize + 1] = '.';
		i++;
	}
	printf("  %s\n", b);
	return 0;
}

// ============================================================================
// UTIL FUNCTIONS END HERE ====================================================
// ============================================================================

// ============================================================================
// ELF Editing Functions =======================================================
// =============================================================================

static Elf_Scn *find_section_by_name(Elf *elf, const char *section_name) {
	size_t sections_num;
	if (elf_getshdrnum(elf, &sections_num)) {
		return NULL;
	}

	size_t shstrndx;
	if (elf_getshdrstrndx(elf, &shstrndx)) {
		return NULL;
	}

	for (size_t i = 0; i < sections_num; ++i) {
		Elf_Scn *s = elf_getscn(elf, i);
		if (s == NULL) {
			return NULL;
		}

		GElf_Shdr sh;
		if (!gelf_getshdr(s, &sh)) {
			return NULL;
		}

		char *name = elf_strptr(elf, shstrndx, sh.sh_name);
		if (name == NULL) {
			return NULL;
		}

		if (strcmp(name, section_name) == 0) {
			return s;
		}
	}

	return NULL;
}

Elf_Scn *orb_create_section(OrbisElf *inputElf, const char *name, void *scnData, size_t scnSize)
{
	if(find_section_by_name(inputElf->elfData, name) == NULL)
	{
		printf("[INFO] Attempting to create section : %s (size %zu)\n", name, scnSize);
		Elf_Scn *new_scn = elf_newscn(inputElf->elfData);
		Elf_Data *scn_data;
		
		if(new_scn == NULL)
		{
			printf("[ERROR] (0x1) Failed to create new section %s\n", name);
			return NULL;
		}
		
		scn_data = elf_newdata(new_scn);
		scn_data->d_align = 1;
		scn_data->d_off = 0LL;
		scn_data->d_buf = scnData;
		scn_data->d_type = ELF_T_BYTE;
		scn_data->d_size = scnSize;
		scn_data->d_version = EV_CURRENT;
		
		GElf_Shdr shdr;
		if(!gelf_getshdr(new_scn, &shdr))
		{
			printf("[ERROR] (0x2) Failed to get shdr\n");
			return NULL;
		}
		
		// Work on shdr
//		shdr.sh_name = 16; // or name ?
//		shdr.sh_type = SHT_NOTE;
//		shdr.sh_flags = SHF_ALLOC;
		//shdr.sh_entsize = scnSize; // or 0
		shdr.sh_size = scnSize;
		shdr.sh_addralign = 1;
		
		gelf_update_shdr(new_scn, &shdr);
		
		/*
		if(elf_update(inputElf->elfData, ELF_C_NULL) < 0)
		{
			printf("[ERROR] Failed to update elf\n");
			return NULL;
		}
		*/
		
		// Lets add section name to .shstrtab 
		Elf_Scn *shstrtab = find_section_by_name(inputElf->elfData, ".shstrtab");
		if(shstrtab == NULL)
		{
			printf("[ERROR] (0x3) Failed to get .shstrtab\n");
			return NULL;
		}
		
		GElf_Shdr shstrtab_shdr;
		if(!gelf_getshdr(shstrtab, &shstrtab_shdr))
		{
			printf("[ERROR] (0x4) Failed to get .shstrtab shdr\n");
			return NULL;
		}
		
		Elf_Data *shstrtab_data = elf_newdata(shstrtab);
		if(shstrtab_data == NULL)
		{
			printf("[ERROR] (0x5) Failed to create new shstrdata\n");
			return NULL;
		}
		
		shstrtab_data->d_buf = strdup(name);
		shstrtab_data->d_size = strlen(name) + 1;
		shstrtab_data->d_align = 1;
		
		shdr.sh_name = shstrtab_shdr.sh_size;
		shstrtab_shdr.sh_size += shstrtab_data->d_size;
		
		if(!gelf_update_shdr(new_scn, &shdr))
		{
			printf("[ERROR] 0x6 - Failed to update shdr\n");
			return NULL;
		}
		
		if(!gelf_update_shdr(shstrtab, &shstrtab_shdr))
		{
			printf("[ERROR] 0x7 Failed to update shstrtab\n");
			return NULL;
		}
		
		if(elf_update(inputElf->elfData, ELF_C_WRITE) < 0)
		{
			printf("[ERROR] Failed to update elf\n");
			return NULL;
		}
		
		printf("[INFO] Created Section Name : %s\n", name);
		return new_scn;

	}
	else
	{
		printf("[ERROR] %s Section already Exists in provided Elf\n", name);
		return NULL;
	}
}

// =============================================================================
// Elf Editing Functions End Here ==============================================
// =============================================================================

// =============================================================================
// Custom Stub Parsing Functions  ==============================================
// =============================================================================

int load_stubs(Elf_Scn *inScn, int *num_stubs, orbis_elf_stub_t **stubs, char *name)
{
		GElf_Shdr shdr;
		Elf_Data *data;
		uint32_t *stub_data;
		int chunk_offset, total_bytes;
		orbis_elf_stub_t *curstub;
		int old_num;
		
		gelf_getshdr(inScn, &shdr);
		
		old_num = *num_stubs;
		*num_stubs = old_num + shdr.sh_size/16;
		*stubs = realloc(*stubs, *num_stubs * sizeof(orbis_elf_stub_t));
		
		memset(&(*stubs)[old_num], 0, sizeof(orbis_elf_stub_t) * shdr.sh_size / 16);
		
		name = strrchr(name, '.')+1;
		printf("STUB MODULE (%s)\n", name);
		
		curstub = *stubs;
		curstub = &curstub[*num_stubs - (shdr.sh_size / 16)];
		
		data = NULL; total_bytes = 0;
		
		data = elf_getdata(inScn, data);
		
		/*
		while(total_bytes < shdr.sh_size && (data = elf_getdata(inScn, data)) != NULL)
		{
			
		}
		*/
		
		/*
		old_num = *num_stubs;
		*num_stubs = old_num + shdr.sh_size/16;
		*stubs = realloc(*stubs, *num_stubs * sizeof(orbis_elf_stub_t));
		
		memset(&(*stubs)[old_num], 0, sizeof(orbis_elf_stub_t) * shdr.sh_size / 16);
		
		name = strrchr(name, '.')+1;
		
		curstub = *stubs;
		curstub = &curstub[*num_stubs - (shdr.sh_size / 16)];
		
		data = NULL; total_bytes = 0;
		while(total_bytes < shdr.sh_size && (data = elf_getdata(inScn, data)) != NULL)
		{
				for(stub_data = (uint32_t *)data->d_buf, chunk_offset = 0; chunk_offset < data->d_size; stub_data+= 4, chunk_offset += 16)
				{
						curstub->addr = shdr.sh_addr + data->d_off + chunk_offset;
			//curstub->module = vita_imports_module_new(name,false,0,0,0);
			curstub->module->flags = le32toh(stub_data[0]);
			curstub->module_nid = le32toh(stub_data[1]);
			curstub->target_nid = le32toh(stub_data[2]);
			curstub++;
				}
				
				total_bytes += data->d_size;
				
		}
		*/
		
		return 1;
}

// =============================================================================
// Custom Stub Parsing Functions End Here ======================================
// =============================================================================


// =====================================================
// ============ WORK IN PROGRESS STUFF =================
// =====================================================
// ==== DO NOT TOUCH THIS UNLESS YOU UNDERSTAND IT =====
// =====================================================

// NID FUNCTION - UNTESTED (WIP)

// nid_decode_object_id & nid_encode_object_id provided by kiwidog
// Charset also provided by kiwidog
//const uint8_t k_DecodingCharset[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0x3F, 0xFF, 0xFF, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
//const char k_EncodingCharset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

/*
int32_t nid_decode_object_id(uint8_t* data, uint32_t length)
{
    int32_t result = 0;

    if (length > 4)
        return -1;

    for (uint32_t i = 0; i < length; ++i)
    {
        // Pull out the index
        int32_t index = (int32_t)data[i];
        if (index >= sizeof(k_DecodingCharset))
            return -1;

        int32_t value = (int32_t)k_DecodingCharset[index];
        if (value == 0xFF)
            return -1;

        result <<= 6;
        result |= value;
    }

    return result;
}

int32_t nid_encode_object_id(uint8_t index)
{
    int32_t result = (int32_t)k_EncodingCharset[(index & 0xF) << 2];

    int buffer = index >> 4;
    while (buffer != 0)
    {
        result += k_EncodingCharset[buffer & 0x3F];
        buffer = buffer >> 6;
    }

    return _byteswap_ulong(result);
}*/