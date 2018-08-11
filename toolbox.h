#ifndef _toolbox_h_
#define _toolbox_h_

#include "varray.h"
#include "myelf.h"

#define warnx(fmt, args...) fprintf(stderr, __FILE__ ": " fmt, ##args)
#define warn(fmt, args...) warnx(fmt ": %s", ##args, strerror(errno))
#define errx(retval, args...) do {warnx(args); exit(retval);} while (0)
#define err(retval, args...) do {warn(args); exit(retval);} while (0)


#define FAIL_EX(label, function, fmt...) do { \
	function(fmt); \
	goto label; \
} while (0)
#define FAIL(fmt...) FAIL_EX(failure, warn, fmt)
#define FAILX(fmt...) FAIL_EX(failure, warnx, fmt)
#define FAILE(fmt, args...) FAIL_EX(failure, warnx, fmt ": %s", ##args, elf_errmsg(-1))
#define ASSERT(condition) do { \
	if (!(condition)) FAILX("Assertion failed: (" #condition ")"); \
} while (0)

#define SYS_ASSERT(cmd) if ((cmd) < 0) FAIL(#cmd " failed")
#define ELF_ASSERT(cmd) if (!(cmd)) FAILE(#cmd " failed")

// =========================================================================
// ============= FUNCTIONS START HERE ======================================
// =========================================================================
	
// Util Functions
void print_help_section();
void dump_args(int argc, char *argv[]);
int simpleHexDump(const void *data);
int hexDump(const void *data);

// Elf Editing Functions
static Elf_Scn *find_section_by_name(Elf *elf, const char *section_name);
Elf_Scn *orb_create_section(OrbisElf *inputElf, const char *name, void *scnData, size_t scnSize);

// custom stub parsing function
int load_stubs(Elf_Scn *inScn, int *num_stubs, orbis_elf_stub_t **stubs, char *name);

#endif 