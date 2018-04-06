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
 
 quu..__
 $$$b  `---.__
  "$$b        `--.                          ___.---uuudP
   `$$b           `.__.------.__     __.---'      $$$$"              .
     "$b          -'            `-.-'            $$$"              .'|
       ".                                       d$"             _.'  |
         `.   /                              ..."             .'     |
           `./                           ..::-'            _.'       |
            /                         .:::-'            .-'         .'
           :                          ::''\          _.'            |
          .' .-.             .-.           `.      .'               |
          : /'$$|           .@"$\           `.   .'              _.-'
         .'|$u$$|          |$$,$$|           |  <            _.-'
         | `:$$:'          :$$$$$:           `.  `.       .-'
         :                  `"--'             |    `-.     \
        :##.       ==             .###.       `.      `.    `\
        |##:                      :###:        |        >     >
        |#'     `..'`..'          `###'        x:      /     /
         \                                   xXX|     /    ./
          \                                xXXX'|    /   ./
          /`-.                                  `.  /   /
         :    `-  ...........,                   | /  .'
         |         ``:::::::'       .            |<    `.
         |             ```          |           x| \ `.:``.
         |                         .'    /'   xXX|  `:`M`M':.
         |    |                    ;    /:' xXXX'|  -'MMMMM:'
         `.  .'                   :    /:'       |-'MMMM.-'
          |  |                   .'   /'        .'MMM.-'
          `'`'                   :  ,'          |MMM<
            |                     `'            |tbap\
             \                                  :MM.-'
              \                 |              .''
               \.               `.            /
                /     .:::::::.. :           /
               |     .:::::::::::`.         /
               |   .:::------------\       /
              /   .''               >::'  /
              `',:                 :    .'
                 
			ENJOY THIS FREE TOOL, IF YOU PAID FOR
			ANYTHING IN THIS SDK, ASK FOR YOUR 
			MONEY BACK BECAUSE YOU HAVE BEEN RIPPED
			OFF BUDDY!
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
FILE *input_elf_fh, *output_elf_fh;
char *input_elf_buffer;

OrbisElf importElf, *exportElf;

int orbis_load_elf(char *path)
{

	return 0;
}

int orbis_parse_header()
{

	return 0;
}

int orbis_elf_rewrite_stubs()
{
	
	
	return 0;
}

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
		
		importElf = loadElf(argv[1]);
		orbisParseHeader(&importElf);
		
		
		if(importElf.errorCode == 0)
		{
			
		}
		else
		{
			printf("ELF Error Code : %i \n\n", importElf.errorCode);
			appReturn = -1;
		}
		
		
	}
	else
	{
		print_help_section();
	}

	return appReturn;
}