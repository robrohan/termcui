
CFLAGS:=-Wall
# CFLAGS:=-std=c99 -Wall -Wpedantic -Werror -pedantic -pedantic-errors
# https://stackoverflow.com/questions/6491019/struct-sigaction-incomplete-error
# CFLAGS:=-std=c99 -Wall -Wpedantic -pedantic -D_XOPEN_SOURCE=700

ifdef CONFIG_DEBUG
CFLAGS+= -Og -g 
else 
CFLAGS+= -Os
endif 

CFLAGS+= $(CFLAGS) -funsigned-char -I$(LOAD_PATH):./

ifdef TARGET_ARCH_X86
CFLAGS+=-march=i386 -falign-functions=0
endif

DEFINES=-DHAVE_QE_CONFIG_H

LIBS+=-lm

LOAD_PATH=./

#############################################################################

TARGETLIBS:=
TARGETS+=main$(EXE)

_OBJS=main.o # display.o tty.o
OBJS = $(patsubst %,$(LOAD_PATH)/%,$(_OBJS))

export CC
export CFLAGS

#############################################################################

all: $(TARGETLIBS) $(TARGETS)

main$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) -o $(LOAD_PATH)/$@ $^ $(LIBS) -L$(LOAD_PATH) $(PLUGS) 

#main$(EXE): qe_g$(EXE)
#	rm -f ../$@
#	cp $(LOAD_PATH)/$< ../$@
#	@echo ''
#	@echo '** Your qi is strong. **'
#	@ls -alFh ../$@
#	@echo ''

$(LOAD_PATH)/%.o : %.c
	$(CC) $(DEFINES) $(CFLAGS) -o $@ -c $<

clean:
#	make -C plugins clean
	rm -f *.o 
	rm -f ../main$(EXE)

