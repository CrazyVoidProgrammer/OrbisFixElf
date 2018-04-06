/*	orbisFixElf 
 *	Version 0.1
 *	By CrazyVoid
 *	This tool is intended to fix Orbis SDK elfs to have proper sections
 *	like offical sony elfs so the ps4 will run our custom elfs
 *	--------------------------------------------------------------------
 *	-------------------  Thanks to  ------------------------------------
 *		DHrpcs3 ( for making orbis-elf )
 *		Kiwidog ( for advice and pointers )
 *		Frangarcj ( for advice and pointers )
 *	--------------------------------------------------------------------
 */

 
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <elf.h>

#include "myelf.h"

int appReturn = 0;
OrbisElf importElf, exportElf;

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

int main(int argc, char **argv)
{
	//dump_args(argc, argv);
		
	if(argc == 3)
	{
	

		OpenElf importElf;
		
		printf("[INFO] Loading : %s\n", argv[1]);
		
		importElf = loadElf(argv[1]);
		
		if(verifyElf(&importElf) == 1) 
		{
			printf("[INFO] %s ELF Magic Check Passed!\n", argv[1]);
			
			verboseElf(importElf);
		} 
		else
		{
			printf("Elf Magic Check Failed!\n");
			appReturn = -1;
		}
		
		
	}
	else
	{
		printf("ARG DUMP\n================================\n");
		dump_args(argc, argv);
		printf("================================\n\n");
		print_help_section();
	}

	return appReturn;
}