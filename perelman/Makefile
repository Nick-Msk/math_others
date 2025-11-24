CC=gcc #clang
CFLAGS= -I../utils/ -Wall -Wextra -std=gnu11 -g -O0
#-Wno-initializer-overrides
# -Wno-initializer-overrides -Wno-unused-function -Wno-format-zero-length
LDFLAGS=
LDLIBS=

all:

run:
		debug/check_textsearch

91task: src/91task.c
		$(CC) $(CFLAGS)  $^ -o out/$@

#  TODO: add compl+alg
UTIL_PATH = ../utils

# libs from utils
lib/log.o:
	cd $(UTIL_PATH) && $(MAKE) log
	@cp $(UTIL_PATH)/log.o lib/

lib/test.o:
		cd $(UTIL_PATH) && $(MAKE) test
		@cp $(UTIL_PATH)/test.o lib/

lib/error.o:
		cd $(UTIL_PATH) && $(MAKE) error
		@cp $(UTIL_PATH)/error.o lib/

lib/common.o:
		cd $(UTIL_PATH) && $(MAKE) error
		@cp $(UTIL_PATH)/common.o lib/

.PHONY: clean cleanlog cleanall no_targets__ list
clean:
		@-rm lib/*.o
		@-rm test/*_test
		@find . -name "*.dSYM" -type d -ls -maxdepth 2 -exec rm -r {} \;
		@find . -perm -u=x -type f -ls -maxdepth 2 -exec rm {} \;
		@find . -name "*.gch" -type f -maxdepth 2 -exec rm {} \;
		@find . -name "*.h~" -type f -maxdepth 2 -exec rm {} \;
		@find . -name "*.c~" -type f -maxdepth 2 -exec rm {} \;
		@find . -name "Makefile~" -type f -maxdepth 1 -exec rm {} \;

cleanlog:
		-rm *.log

cleanall: clean cleanlog

no_targets__:
list:
		sh -c "$(MAKE) -p no_targets__ | awk -F':' '/^[a-zA-Z0-9][^\$$#\/\\t=]*:([^=]|$$)/ {split(\$$1,A,/ /);for(i in A)print A[i]}' | grep -v '__\$$' | sort"


