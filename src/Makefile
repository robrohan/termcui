
CC=clang

CFLAGS:=-Wall
# CFLAGS:=-std=c99 -Wall -Wpedantic -Werror -pedantic -pedantic-errors
# https://stackoverflow.com/questions/6491019/struct-sigaction-incomplete-error
# CFLAGS:=-std=c99 -Wall -Wpedantic -pedantic -D_XOPEN_SOURCE=700

ifdef CONFIG_DEBUG
CFLAGS+= -Og -g 
else 
CFLAGS+= -Os
endif 

CFLAGS+= $(CFLAGS) -ggdb -funsigned-char -I$(LOAD_PATH):../build

ifdef TARGET_ARCH_X86
CFLAGS+=-march=i386 -falign-functions=0
endif

DEFINES=-DHAVE_QE_CONFIG_H

LIBS+=-lm

SRC=.
LOAD_PATH=../build

NO_BUILT_INS=-fno-builtin-sin -fno-builtin-cos -fno-builtin-tan \
	-fno-builtin-ceil -fno-builtin-floor \
	-fno-builtin-pow -fno-builtin-round  \
	-fno-builtin-abs -fno-builtin-sqrt \
	-fno-builtin-rand -fno-builtin-srand

#############################################################################

TARGETLIBS:=
TARGETS+=main$(EXE)

_OBJS=math.o wefx.o main.o
OBJS = $(patsubst %,$(LOAD_PATH)/%,$(_OBJS))

export CC
export CFLAGS

#############################################################################

all: preflight $(TARGETLIBS) $(TARGETS)

preflight:
	mkdir -p ../build

main$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) \
		-o $(LOAD_PATH)/$@ $^ \
		$(LIBS) \
		-L$(LOAD_PATH) \
		$(NO_BUILT_INS) \
		$(PLUGS) 


# $(CC) \
# 		--target=wasm32 \
# 		-std=c99 \
# 		-Wall \
# 		-g \
# 		-nostdlib \
# 		-Os -flto \
# 		-Wl,--allow-undefined \
# 		-Wl,--export-dynamic \
# 		-Wl,--no-entry \
# 		-Wl,--lto-O3 \
# 		-Wl,-z,stack-size=$(STACK_SIZE) \
# 		$(NO_BUILT_INS) \
# 		-o build/wefx.wasm \
# 		src/walloc.c src/math.c src/wefx.c $(MAIN)


$(LOAD_PATH)/%.o : %.c
	$(CC) $(DEFINES) $(CFLAGS) \
		$(NO_BUILT_INS) \
		-o $@ \
		-c $(SRC)/$<

clean:
	rm -rf ../build

