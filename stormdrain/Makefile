CPPFLAGS += -Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline -I
CPPFLAGS += -D_XOPEN_SOURCE -pthread
CFLAGS += -std=c11 -lm 
ARFLAGS += -U

DEBUG = -DDEBUG -g

BINS = stormdrain

FILES = stormdrain.o ../lib/listener.o ../lib/payloads.o ../lib/response.o ../lib/chemicals.o ../lib/graph.o ../lib/util.o
FILES += ../lib/crypt.o ../lib/libscrypt/libscrypt.a ../lib/reporting.o
all: build

.PHONY: all

debug: CFLAGS += -DDEBUG -g
debug: CPPFLAGS += -DDEBUG -g
debug:  $(FILES)
	gcc -o $(BINS) $(FILES) $(CPPFLAGS) $(CFLAGS)
	$(MAKE) clean

profile: CFLAGS += -pg
profile: CPPFLAGS += -pg
profile: $(FILES)
	gcc -o $(BINS) $(FILES) $(CPPFLAGS) $(CFLAGS)
	$(MAKE) clean

build: $(FILES)
	gcc -o $(BINS) $(FILES) $(CPPFLAGS) $(CFLAGS)
	$(MAKE) clean

clean:
	$(RM) *.o *.a

cleanAll:
	$(RM) $(BINS) *.o *.a