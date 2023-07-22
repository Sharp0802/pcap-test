NAME    := pcap-test
CC      := gcc
LD      := gcc
ARCH    := i686
BITS	:= 32


COMMONS := -Oz -Wl,--build-id=none
COMMONS += -fwhole-program -D __whopr__
COMMONS += -ffunction-sections -fdata-sections
COMMONS += -Wl,-s -Wl,--gc-sections
COMMONS += -fno-plt
COMMONS += -fno-unwind-tables -fno-asynchronous-unwind-tables
COMMONS += -fno-ident -Wl,-z,norelro
COMMONS += -lpcap
ifeq ("$(BITS)","32")
COMMONS += -m32 -fno-stack-protector
endif

CFLAGS  := $(COMMONS) -I/usr/include
CFLAGS  += -march=$(ARCH)
CFLAGS  += -std=gnu11 -Wall -Wextra

LDFLAGS := $(COMMONS)



RSECTIONS := .comment .note.gnu.version .eh_frame_hdr .eh_frame .gnu.hash .gnu.version

SRC	:= $(wildcard *.c)
OBJ	:= $(SRC:.c=.o)

all: $(NAME)

clean:
	rm -rf bin
	rm -rf obj

$(NAME): $(OBJ)
	mkdir -p bin
	$(LD) $(LDFLAGS) $(addprefix obj/,$^) -o bin/$@
	strip -s $(addprefix -R ,$(RSECTIONS)) bin/$@
	stat bin/$@

%.o: %.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $^ -o obj/$@

.PHONY: all clean
