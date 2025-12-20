CC=gcc #clang
CFLAGS= -I../utils/ -Wall -Wextra -std=gnu11 -g -O0
#-Wno-initializer-overrides
# -Wno-initializer-overrides -Wno-unused-function -Wno-format-zero-length
LDFLAGS=
LDLIBS=

all:

#run:
#		debug/check_textsearch

91task: src/91task.c lib/log.o
		$(CC) $(CFLAGS)  $^ -o out/$@

92task: src/92task.c lib/log.o lib/eng_runner.o
		$(CC) $(CFLAGS)  $^ -o out/$@

93task: src/93task.c lib/log.o lib/eng_runner.o
		$(CC) $(CFLAGS)  $^ -o out/$@

#  TODO: add compl+alg
UTIL_PATH = ../utils

# libs from utils
lib/log.o:
	cd $(UTIL_PATH) && $(MAKE) log.o
	@cp $(UTIL_PATH)/lib/log.o lib/

lib/test.o:
	cd $(UTIL_PATH) && $(MAKE) test.o
	@cp $(UTIL_PATH)/lib/test.o lib/

lib/error.o:
	cd $(UTIL_PATH) && $(MAKE) error.o
	@cp $(UTIL_PATH)/lib/error.o lib/

lib/common.o:
	cd $(UTIL_PATH) && $(MAKE) common.o
	@cp $(UTIL_PATH)/lib/common.o lib/

lib/eng_runner.o:
	cd $(UTIL_PATH) && $(MAKE) eng_runner.o
	@cp $(UTIL_PATH)/lib/eng_runner.o lib/


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


