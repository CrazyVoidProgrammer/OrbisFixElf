#ifndef _MYELF_H_
#define _MYELF_H_

#include <stdint.h>

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

typedef struct OrbisElf_s *OrbisElfHandle_t;
typedef struct OrbisElfProgram_s *OrbisElfProgramHandle_t;
typedef struct OrbisElfSection_s *OrbisElfSectionHandle_t;


typedef struct
{
	uint8_t magic[4];
	uint8_t eclass;
	uint8_t data;
	uint8_t eversion;
	uint8_t osabi;
	uint8_t abiver;
	uint8_t pad[7];
	uint16_t type; /* See OrbisElfType_t */
	uint16_t machine;
	uint32_t version;
	uint64_t entry;
	uint64_t phoff;
	uint64_t shoff;
	uint32_t flags;
	uint16_t ehsize;
	uint16_t phentsize;
	uint16_t phnum;
	uint16_t shentsize;
	uint16_t shnum;
	uint16_t shstrndx;
} OrbisElfHeader_t;

typedef struct
{
	uint32_t type; /* see OrbisElfProgramType_t */
	uint32_t flags;
	uint64_t offset;
	uint64_t vaddr;
	uint64_t paddr;
	uint64_t filesz;
	uint64_t memsz;
	uint64_t align;
} OrbisElfProgramHeader_t;

typedef struct
{
	uint32_t name;
	uint32_t type; /* see OrbisElfSectionType_t */
	uint64_t flags;
	uint64_t addr;
	uint64_t offset;
	uint64_t size;
	uint32_t link;
	uint32_t info;
	uint64_t addralign;
	uint64_t entsize;
} OrbisElfSectionHeader_t;

typedef struct
{
	uint32_t name;
	uint8_t info;
	uint8_t other;
	uint16_t shndx;
	uint64_t value;
	uint64_t size;
} OrbisElfSymbolHeader_t;

typedef struct
{
	uint64_t offset;
	uint64_t info;
} OrbisElfRelocation_t;

typedef struct
{
	uint64_t offset;
	uint64_t info;
	int64_t addend;
} OrbisElfRelocationWithAddend_t;

typedef struct
{
	int64_t type; /* See OrbisElfDynamicType_t */
	uint64_t value;
} OrbisElfDynamic_t;


typedef struct OrbisElfLibraryInfo_s
{
	uint16_t version;
	uint16_t id;
	const char *name;
	uint32_t attr;
} OrbisElfLibraryInfo_t;

typedef struct OrbisElfModuleInfo_s
{
	uint16_t version;
	uint16_t id;
	const char *name;
	uint64_t attr;
} OrbisElfModuleInfo_t;

/*
typedef struct OrbisElfSymbol_s
{
	OrbisElfSymbolHeader_t header;
	const char *name;
	const OrbisElfModuleInfo_t *module;
	const OrbisElfLibraryInfo_t *library;
	enum OrbisElfSymbolBind_t bind;
	enum OrbisElfSymbolType_t type;
	uint64_t virtualBaseAddress;
	void *baseAddress;
} OrbisElfSymbol_t;
*/

typedef struct OrbisElfImport_s
{
	uint64_t offset;
	int64_t addend;
	uint32_t symbolIndex;
	uint32_t relType; /* see OrbisElfRelocationType_t */
} OrbisElfImport_t;

typedef struct OrbisElfRebase_s
{
	uint64_t offset;
	uint64_t value;
	uint32_t symbolIndex;
} OrbisElfRebase_t;

// Orbis Reversed Sections 

typedef struct orbis_special {
	void *test;
} orbis_special;

typedef struct orbis_process_param {
	uint64_t unknown_01;
	uint64_t pad_01;
	uint64_t process_name;
	uint64_t flag;
	uint64_t sdk_version;
	uint64_t pad_02[11];
} orbis_process_param;

typedef struct orbis_version {
	void *test;
} orbis_version;

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
	OrbisElfHeader_t *header;
	customStub stubs[];
} OpenElf;

// Add all sections to this
typedef struct
{
	char *buffer;
	OrbisElfHeader_t *header;
	// Orbis Custom Sections
	orbis_special *special;
	orbis_process_param *process_param;
	orbis_version *version;
	int errorCode;
} OrbisElf;


// Functions

OrbisElf loadElf(char *path);
void orbisParseHeader(OrbisElf *inputElf);

#endif