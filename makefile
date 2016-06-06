# for power.ludat.lth.se
#CFLAGS	= -m64 -O3 -g -Wall -Wextra -Werror -std=c99 -mcpu=970 -mtune=970 -maltivec

# the following works on any machine
CFLAGS	= -g -Wall -Wextra -Werror -std=c11
#CFLAGS	= --enable-checking -g -O0 -std=c11 -v -da -Q

CC	= gcc
OUT	= fm
OBJS	= main.o tpi12mhu_mki.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT) -lm
	./fm
	size tpi12mhu_mki.o

clean:
	rm -f $(OUT) $(OBJS)
