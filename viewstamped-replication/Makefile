PROJECT_NAME := viewstamped-replication
DEPENDS = \
	libuv >= 1.40.0
CFLAGS := \
	-Wall \
	$(shell pkg-config --cflags "$(DEPENDS)")
LDFLAGS := \
	-Wl,--no-undefined \
	-Wl,--as-needed \
	$(shell pkg-config --libs "$(DEPENDS)")
SOURCES := \
	main.c
BUILD_TYPE := debug

ifeq ($(BUILD_TYPE),debug)
	CFLAGS := $(CFLAGS) -fsanitize=address -g
	LDFLAGS := -lasan $(LDFLAGS)
endif

.PHONY: all
all: $(PROJECT_NAME)

include $(SOURCES:.c=.d)

.PHONY: clean
clean: SOURCES += tests.c
clean:
	rm -f tests $(PROJECT_NAME) $(SOURCES:.c=.d) $(SOURCES:.c=.d.*) $(SOURCES:.c=.o)

.PHONY: test
tests: tests.o
	$(CC) -o $@ $^ \
		-Wl,--version-script=$(PROJECT_NAME).sym $(LDFLAGS) \
		-Wl,--start-group -l:libcheck_pic.a -pthread -lrt -lm -lsubunit -Wl,--end-group

$(PROJECT_NAME): $(SOURCES:.c=.o)
	$(CC) -o $@ $^ -Wl,--version-script=$(PROJECT_NAME).sym $(LDFLAGS)

%.d: %.c
	@set -e; rm -f $@; \
		$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$
