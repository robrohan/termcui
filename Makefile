
CC=clang

# CFLAGS:=-std=c99 -Wall 
# https://stackoverflow.com/questions/6491019/struct-sigaction-incomplete-error
# CFLAGS:=-std=c99 -Wall -Wpedantic -pedantic -D_XOPEN_SOURCE=700

SRC=./src
OUT_PATH=./build

CFLAGS:=-Wall -Wpedantic -Werror -pedantic -pedantic-errors -std=c99 -funsigned-char -I$(OUT_PATH)

ifdef CONFIG_DEBUG
CFLAGS+= -Og -ggdb
else 
CFLAGS+= -Os
endif 

ifdef TARGET_ARCH_X86
CFLAGS+=-march=i386 -falign-functions=0
endif

CFLAGS+=-D_GNU_SOURCE

LIBS+=-lm

CFLAGS+=-fno-builtin-sin -fno-builtin-cos -fno-builtin-tan \
	-fno-builtin-ceil -fno-builtin-floor \
	-fno-builtin-pow -fno-builtin-round  \
	-fno-builtin-abs -fno-builtin-sqrt \
	-fno-builtin-rand -fno-builtin-srand

TARGETS:=main$(EXE)

_OBJS=math.o wefx.o main.o
OBJS = $(patsubst %,$(SRC)/%,$(_OBJS))

all: preflight $(TARGETS)

preflight:
	mkdir -p $(OUT_PATH)

main$(EXE): $(OBJS)
	$(CC) $(CFLAGS) \
		-o $(OUT_PATH)/$@ $^ \
		$(LIBS) \
		-L$(SRC)

$(OUT_PATH)/%.o : %.c
	$(CC) $(CFLAGS) \
		-o $(OUT_PATH)/$@ \
		-c $(SRC)/$<

clean:
	rm -rf $(OUT_PATH)
	rm -f $(SRC)/*.o
