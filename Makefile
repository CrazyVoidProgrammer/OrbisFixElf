COMPILER= clang
OUTPUTEXEC= orbisFixElf
SOURCE_FILES= main.c myelf.c toolbox.c logger.c dyngen.c varray.c
CFLAGS= -l elf -o

all: 
	$(COMPILER) $(SOURCE_FILES) $(CFLAGS) $(OUTPUTEXEC)
	
clean:
	rm $(OUTPUTEXEC)
