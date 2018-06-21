CPPFLAGS += -Wall -Wextra -Wpedantic
CPPFLAGS += -D_XOPEN_SOURCE -pthread -std=c11 -lm -lc -DDEBUG -g
ARFLAGS += -U

DEBUG = -DDEBUG -g

BINS = pretreatment

FILES = pretreatment.o ../lib/listener.o ../lib/payloads.o ../lib/response.o ../lib/chemicals.o ../lib/graph.o

all: build

.PHONY: all

# debug: CFLAGS += -DDEBUG -g
debug:  $(FILES)
	gcc49 -o $(BINS) $(FILES) $(CPPFLAGS) $(CFLAGS)
	$(MAKE) clean

profile: CFLAGS += -pg
profile: CPPFLAGS += -pg
profile: $(FILES)
	gcc49 -o $(BINS) $(FILES) $(CPPFLAGS) $(CFLAGS)
	$(MAKE) clean

build: $(FILES)
	gcc49 -o $(BINS) $(FILES) $(CPPFLAGS) $(CFLAGS)
	$(MAKE) clean

clean:
	$(RM) *.o

cleanAll:
	$(RM) $(BINS) *.o