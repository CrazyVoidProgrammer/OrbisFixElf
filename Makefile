COMPILER= clang-5.0
OUTPUTEXEC= orbisFixElf
SOURCE_FILES= main.c myelf.c
CFLAGS= -o

all: 
	$(COMPILER) $(SOURCE_FILES) $(CFLAGS) $(OUTPUTEXEC)
	
clean:
	rm $(OUTPUTEXEC)