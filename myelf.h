#ifndef _MYELF_H_
#define _MYELF_H_

#include <stdint.h>
#include <stdbool.h>
#include "varray.h"

// Sony Shit 

#define ET_SCE_EXEC     0xfe00
#define ET_SCE_DYNEXEC  0xfe10		/* Main module - ASLR */
#define ET_SCE_RELEXEC  0xfe04		/* PRX */
#define ET_SCE_STUBLIB  0xfe0c		/* Stub library */
#define ET_SCE_DYNAMIC  0xfe18		/* PRX */

// UNKNOWN
#define ET_HIOS		0xfeff		/* OS-specific range end */
#define ET_LOPROC	0xff00		/* Processor-specific range start */
#define ET_HIPROC	0xffff		/* Processor-specific range end */

#define SHT_SCENID	  0x61000001

#define PT_SCE_DYNLIBDATA	0x61000000
#define PT_SCE_PROCPARAM	0x61000001
#define PT_SCE_MODULEPARAM	0x61000002
#define PT_SCE_RELRO		0x61000010

#define PT_SCE_COMMENT  0x6fffff00
#define PT_SCE_VERSION	0x6fffff01



#define DT_SCE_FINGERPRINT			0x61000007	// DATA : 0x0
#define DT_SCE_ORIGINAL_FILENAME	0x61000009  // DATA : 0x27
#define DT_SCE_MODULE_INFO			0x6100000d	// DATA : 0x10100000086
#define DT_SCE_NEEDED_MODULE		0x6100000f	// DATA : 0x1010100000018  AND   0x2010100000022  - I think these might be prx names
#define DT_SCE_MODULE_ATTR			0x61000011  // DATA : 0x0
#define DT_SCE_EXPORT_LIB			0x61000013
#define DT_SCE_IMPORT_LIB			0x61000015	// DATA : 0x100000018
#define DT_SCE_EXPORT_LIB_ATTR		0x61000017
#define DT_SCE_IMPORT_LIB_ATTR		0x61000019	// DATA 0x9
#define DT_SCE_STUB_MODULE_NAME		0x6100001d
#define DT_SCE_STUB_MODULE_VERSION	0x6100001f
#define DT_SCE_STUB_LIBRARY_NAME	0x61000021
#define DT_SCE_STUB_LIBRARY_VERSION	0x61000023
#define DT_SCE_HASH					0x61000025	// DATA : 0x308
#define DT_SCE_PLTGOT				0x61000027	// DATA : 0x41f0
#define DT_SCE_JMPREL				0x61000029	// DATA : 0x1d0
#define DT_SCE_PLTREL				0x6100002b	// DATA : 0x7
#define DT_SCE_PLTRELSZ				0x6100002d	// DATA : 0x78
#define DT_SCE_RELA					0x6100002f	// DATA : 0x248
#define DT_SCE_RELASZ				0x61000031	// DATA : 0xc0
#define DT_SCE_RELAENT				0x61000033	// DATA : 0x18
#define DT_SCE_STRTAB				0x61000035	// DATA : 0x18
#define DT_SCE_STRSZ				0x61000037	// DATA : 0xf2
#define DT_SCE_SYMTAB				0x61000039	// DATA : 0x110
#define DT_SCE_SYMENT				0x6100003b	// DATA : 0x18
#define DT_SCE_HASHSZ				0x6100003d	// DATA : 0x30
#define DT_SCE_SYMTABSZ				0x6100003f	// DATA : 0xc0

// MAGICS

#define SCEPROCPARAMS_MAGIC 0x4942524F // 'ORBI'
#define ORBIS_COMMENT_MAGIC 0x48544150 // 'PATH' - 0x48544150



// Orbis Process Param aka sce_process_param
// Things need more reversing to figure out how this data is used.
/*
typedef struct orbis_process_param
{
    uint64_t Size;
    union
    {
        uint32_t Magic;
        char MagicBits[4];
    };
    uint32_t ThingCount;    // 1
    uint16_t SDK_Build;         // SDK Build Version : 0x0000
    uint8_t SDK_Minor;          // SDK Minor Version : 0x00
    uint8_t SDK_Major;          // SDK Major Version : 0x01
	uint32_t pad[11];
//    uint8_t Unknown[36];	// Most likely just padding...
//    uint64_t Things[1];       // Things[ThingCount]; (gta is 0) but normally 2
} orbis_process_param;
*/

typedef struct orbis_process_param
{
    uint64_t Size;
	uint32_t Magic; // 'ORBI'
    uint32_t ThingCount;    // 1
    uint16_t SDK_Build;         // SDK Build Version : 0x0000
    uint8_t SDK_Minor;          // SDK Minor Version : 0x00
    uint8_t SDK_Major;          // SDK Major Version : 0x01
	uint32_t pad[11];
} orbis_process_param;

// Orbis Comment Section 


#define CM_MAXSIZE 60

struct orbis_comment_entry
{
    uint32_t Length; // Length of string with null term
    char* Data; // Bytes with null term
};

struct orbis_comment
{
    uint32_t Magic;
	uint32_t OtherSize; // Sizeof(OtherSize + EntrySize + Entry[])
    uint32_t EntrySize; // Total entry size with padding to 4 byte alignment
    //struct orbis_comment_entry Entry;
	//char path[CM_MAXSIZE];
	char *path;
	uint32_t pad;
};

// orbis_version padding 0x20 

struct orbis_version_entry
{
	char *section_name; // example ".sceversion:."
	uint8_t snPad; // 0x20
	uint32_t unknown_vs_one; // "0000"
	uint8_t vs_one_Pad; // 0x20
	uint64_t unknown_vs_two; // "09637274"
	uint8_t vs_two_Pad; // 0x20
	uint64_t unknown_vs_three; // "313a0175"
	uint8_t vs_three_Pad; // 0x20
	uint64_t unknown_vs_four; // "00610963"
	uint8_t vs_four_Pad; // 0x20
	uint64_t unknown_vs_five; // "7274693a"
	uint16_t vs_five_Pad; // 0x2020
};

struct orbis_version
{
	char *elfName; // example .hello.elf
	uint32_t elfPad;
	uint8_t elfPad2;
	char elfType[18]; // "file format elf64-x86-64"
	char elfSecPrefix[15]; // "..Contents of section"
	uint8_t elfSecPrefixPad;
	struct orbis_version_entry ovEntrys[10];
};

// Its 20 bytes on 1.76 and 16 bytes on 4.50 -> sce_fingerprint
// Flatz suggest looking at orbis-bin and or publishing tools
typedef struct sce_fingerprint
{
	char *fingerprint;
} sce_fingerprint;

typedef struct prxFiles
{
	char *prxFileName; // example : libc.prx libkernel.prx
	uint8_t pad; // example : 0x00 (1 byte)
} prxFiles;

typedef struct moduleName
{
	char *moduleName; // example : libc libkernel
	uint8_t pad; // example : 0x00 (1 byte)
} moduleName;

typedef struct nidEntry
{
	char nid[11];
	char moduleIndex[4];
	uint8_t pad; // 0x00 - 1 byte
} nidEntry;

typedef struct orbis_dynlib {

	sce_fingerprint fingerprint;
	
	prxFiles *prxFileList;
	moduleName moduleNameList;	

	char *elfBuildPath; // original build path of elf
	uint8_t pad_01; // 1 byte of 0x00 padding	

	nidEntry nidEntryList;

	// SOME PADDING 
	
	// Prx Data?
	
} orbis_dynlib;

// Open Source SDK Stubs

typedef struct customStubFunctions {
	char *name;
	char *nid;
} customStubFunctions;

typedef struct customStub {
	char *Module;
	customStubFunctions functions[];
} customStub;

// Section Structs

typedef struct OrbisSection {
	char *name;
	uint64_t offset;
	char *data;
	long dataSize;
	char *type;
	int alignment;
} OrbisSection;

typedef struct OpenElf {
	char *buffer;
	//OrbisElfHeader_t header;
	//OrbisElfProgramHeader_t pheader;
	customStub stubs[];
} OpenElf;

/*
// Add all sections to this
typedef struct
{
	char *buffer;
	OrbisElfHeader_t header;
	OrbisElfProgramHeader_t pheader;
	// Orbis Custom Sections
	orbis_special *special;
	orbis_process_param *process_param;
	struct orbis_version *version;
	int errorCode;
} OrbisElf;
*/

// IMPORT STUFF FROM VITA SDK 


typedef struct {
	char *name;
	uint32_t NID;
} orbis_imports_common_fields;

typedef struct {
	char *name;
	uint32_t NID;
} orbis_imports_stub_t;

typedef struct {
	char *name;
	uint32_t NID;
	bool is_kernel;
	orbis_imports_stub_t **functions;
	orbis_imports_stub_t **variables;
	int n_functions;
	int v_variables;
	uint32_t flags;
} orbis_imports_module_t;

typedef struct {
	char *name;
	uint32_t NID;
	orbis_imports_module_t **modules;
	int n_modules;
} orbis_imports_lib_t;

typedef struct {
	orbis_imports_lib_t **libs;
	int n_libs;
} orbis_imports_t;

// STRUCTS FROM VITA SDK
typedef struct orbis_elf_symbol_t {
	const char *name;
	const char *moduleIndex;
	Elf32_Addr value;
	uint8_t type;
	uint8_t binding;
	int shndx;
} orbis_elf_symbol_t;

typedef struct orbis_elf_rela_t {
	uint8_t type;
	orbis_elf_symbol_t *symbol;
	Elf32_Addr offset;
	Elf32_Sword addend;
} orbis_elf_rela_t;

typedef struct orbis_elf_rela_table_t {
	orbis_elf_rela_t *relas;
	int num_relas;
	int target_ndx;
	struct orbis_elf_rela_table_t *next;
} orbis_elf_rela_table_t;

typedef struct orbis_elf_stub_t {
	Elf32_Addr addr;
	uint32_t module_nid;
	uint32_t target_nid;
	
	orbis_elf_symbol_t *symbol;
	
	orbis_imports_module_t *module;
	orbis_imports_stub_t *target;
} orbis_elf_stub_t;




typedef struct orbis_elf_segment_info_t {
	Elf32_Word type;	/* Segment type */
	Elf32_Addr vaddr;	/* Top of segment space on TARGET */
	Elf32_Word memsz;	/* Size of segment space */

	/* vaddr_top/vaddr_bottom point to a reserved, unallocated memory space that
	 * represents the segment space in the HOST.  This space can be used as
	 * pointer targets for translated data structures. */
	 
	const void *vaddr_top;
	const void *vaddr_bottom;
} orbis_elf_segment_info_t;

typedef struct OrbisElf
{
	int fd;
	int mode;
	Elf *elfData;

	varray fstubs_va;
	varray vstubs_va;
	
	int symtab_ndx;
	orbis_elf_symbol_t *symtab;
	int num_symbols;

	orbis_elf_rela_table_t *rela_tables;
	
	orbis_elf_stub_t *fstubs;
	orbis_elf_stub_t *vstubs;
	int num_fstubs;
	int num_vstubs;
	
	orbis_elf_segment_info_t *segments;
	int num_segments;
} OrbisElf;

// OLD Functions from before Refactor
//void verboseElf(OpenElf inputElf);
//int verifyElf(OpenElf *inputElf);



#endif