CC=gcc #clang
#CFLAGS= -I../utils/ -Wall -Wextra -std=gnu11 -g -O0 -Wno-initializer-overrides
CFLANGS= -lm
LDFLAGS=
LDLIBS=

task3: task3.c
		$(CC) $(CFALGS) $^ -o out/run_$@

.PHONY: clean cleanlog cleanall no_targets__ list
clean:
		@-rm out/*
		@find . -name "*.dSYM" -type d -ls -maxdepth 2 -exec rm -r {} \;
		@find . -perm -u=x -type f -ls -maxdepth 2 -exec rm {} \;
		@find . -name "*.gch" -type f -maxdepth 2 -exec rm {} \;
		@find . -name "*.h~" -type f -maxdepth 2 -exec rm {} \;
		@find . -name "*.c~" -type f -maxdepth 2 -exec rm {} \;
		@find . -name "Makefile~" -type f -maxdepth 1 -exec rm {} \;

cleanlog:
		-rm log/*

cleanall: clean cleanlog

no_targets__:
list:
		sh -c "$(MAKE) -p no_targets__ | awk -F':' '/^[a-zA-Z0-9][^\$$#\/\\t=]*:([^=]|$$)/ {split(\$$1,A,/ /);for(i in A)print A[i]}' | grep -v '__\$$' | sort"


