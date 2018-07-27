CPPFLAGS += -Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline -I
CPPFLAGS += -D_XOPEN_SOURCE -pthread
CFLAGS += -std=c11 -lm 
ARFLAGS += -U

DEBUG = -DDEBUG -g

BINS = testing

FILES = testing.o ../lib/util.o

all: build

.PHONY: all

build: $(FILES)
	gcc -o $(BINS) $(FILES) $(CPPFLAGS) $(CFLAGS)
	$(MAKE) clean

clean:
	$(RM) *.o *.a

cleanAll:
	$(RM) $(BINS) *.o *.a