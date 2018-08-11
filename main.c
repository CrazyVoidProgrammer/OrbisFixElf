/*	orbisFixElf 
 *	Version 0.35u
 *	By CrazyVoid
 *	This tool is intended to fix Orbis SDK elfs to have proper sections
 *	like offical sony elfs so the ps4 will run our custom elfs
 *  the idea was based from the Vita SDK Design and Tools!
 *	--------------------------------------------------------------------
 *	-------------------  Thanks to  ------------------------------------
 *		Kiwidog ( for advice and pointers and help reversing sce elf format )
 *	--------------------------------------------------------------------
 */

#include <err.h>
#include <fcntl.h>
 
#include <elf.h>
#include <gelf.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <unistd.h>


#include "myelf.h"
#include "toolbox.h"
#include "logger.h"
#include "dyngen.h"

int appDebug = 0;
int appReturn = 0;

OrbisElf importElf;

int main(int argc, char **argv)
{

	
	int fd;
	char *k;
	Elf_Kind ek;
	
	// Comment out in production (DEBUG ONLY)
	//dump_args(argc, argv);
	
	if(argc != 3)
	{
		printf("Usage : ./orbisFixElf input.elf output.elf\n");
		return -1;
	}
	else
	{
		if(elf_version(EV_CURRENT) == EV_NONE)
		{
			printf("[INFO] Failed to initalize elf library\n");
			return -5;
		}
		else
		{
			printf("[INFO] libelf initalized properly!\n");
			
			// O_RDONLY - O_RDWR
			if((fd = open(argv[1], O_RDWR, 0)) < 0)
			{
				printf("[ERROR] Failed to open %s\n", argv[1]);
				return -2;
			}
			else
			{
				Elf_Cmd openCMD;
				//openCMD = ELF_C_READ;
				openCMD = ELF_C_RDWR;
				
				if((importElf.elfData = elf_begin(fd, openCMD, (Elf *) 0)) == 0)
				{
					printf("[ERROR] elf_begin() failed\n");
					return -3;
				}
				else
				{
					ek = elf_kind(importElf.elfData);
					
					switch(ek)
					{
						case ELF_K_AR:
							k = "ar(1) archive";
						break;
						case ELF_K_ELF:
							k = "elf object";
						break; 
						case ELF_K_NONE:
							k = "data";
						break;
						default: 
							k = "unrecognized";
					} 

					if(ek == ELF_K_ELF)
					{
						printf("[INFO] loaded %s into elf struct\n", argv[1]);
						
						
						ASSERT(varray_init(&importElf.fstubs_va, sizeof(int), 8));
						ASSERT(varray_init(&importElf.vstubs_va, sizeof(int), 4));
						
						Elf_Scn *ie_scn, *ee_scn;
						GElf_Ehdr ee_ehdr;
						GElf_Shdr ie_shdr, ee_shdr;
						size_t ie_shstrndx, ee_shstrndx;
						char *name;
						
						int cs_count = 0, cv_count = 0; // custom stub count
						
						ELF_ASSERT(gelf_getehdr(importElf.elfData, &ee_ehdr));
						
						// Find the right type
						// if(ee_ehdr.e_machine != EM_ARM) { give message it wrong type and exit program }

						//printf("Machine Type : %s\n", (char *)ee_ehdr.e_machine);
						
						if(ee_ehdr.e_ident[EI_CLASS] == ELFCLASS64)
						{
							printf("[INFO] Elf 64 Bit check : PASSED\n");
							
							ELF_ASSERT(elf_getshdrstrndx(importElf.elfData, &ee_shstrndx) == 0);
							
							ee_scn = NULL;
							
							printf("[INFO] Scanning SHDR for Stubs\n");
							while((ee_scn = elf_nextscn(importElf.elfData, ee_scn)) != NULL)
							{
								ELF_ASSERT(gelf_getshdr(ee_scn, &ee_shdr));
								
								ELF_ASSERT(name = elf_strptr(importElf.elfData, ee_shstrndx, ee_shdr.sh_name));


								if(ee_shdr.sh_type == SHT_PROGBITS && strncmp(name, ".orbislink.fstubs", strlen(".orbislink.fstubs")) == 0)
								{
									// Get elf data
									Elf_Data *custom_stub_sdat = elf_rawdata(ee_scn, NULL);
						
									// Add info to varray 
									int ndxscn = elf_ndxscn(ee_scn);
									varray_push(&importElf.fstubs_va, &ndxscn);
									
									// Lets dump to console our stub
									printf("CUSTOM STUB : %s - Size : %lu - Offset : %lu\n", name, ee_shdr.sh_size, ee_shdr.sh_offset);
									simpleHexDump(custom_stub_sdat->d_buf);
									
									// Increase cs count
									cs_count++;
								}
								else
								{
									int ndxscn = elf_ndxscn(ee_scn);
									printf("SHDR ENTRY : %s - Size : %lu - Offset : %lu\n", name, ee_shdr.sh_size, ee_shdr.sh_offset);

								}
								

								
							}
							
							if(cs_count != 0)
							{
								printf("[INFO] Parsing %d Custom Stubs to Sony Stubs\n", cs_count);
								printf("[INFO] Parsed %d out of %d Stubs\n", cv_count, cs_count);
							}
							
						}
						else
						{
							printf("[ERROR] %s is not 64bit elf\n", argv[1]);
						}
	


						// START CREATING SECTIONS HERE
	
						// GENERATING SCE COMMENT STRUCT DATA
						struct orbis_comment my_orbis_comment;
						my_orbis_comment = generate_sce_comment("d:/dev/cp09/Hoes/Bin64/Shadow_Shipping.elf");
						printf("[GEN] Orbis Comment Section (%lu) has been Generated!\n", sizeof(my_orbis_comment));
						int size_of_path_moc = strlen(my_orbis_comment.path);
						int projected_size_of_section = (size_of_path_moc + 16);
						void *oelf_comment_section = consolidate_orbis_comment(my_orbis_comment);
						// CREATE SCE COMMENT SECTION 
						Elf_Scn *testScn = orb_create_section(&importElf, ".sce_comment", oelf_comment_section, (size_t)projected_size_of_section);
						

						

					}
					else
					{
						printf("[ERROR] %s is not an elf\n", argv[1]);
						return -6;
					} // END of file type check
				} // END of elf_begin
			} // END of if OPEN of ELF File
		} // END OF elf_version CHECK		
	} // END OF ARGC CHECK

	// End of everything going on, lets clean up
	elf_end(importElf.elfData);
	close(fd);
	
	return appReturn;
failure:
	return -10;
}