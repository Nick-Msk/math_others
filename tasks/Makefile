CC=gcc #clang
CFLAGS= -I../utils/ -Wall -Wextra -Wno-override-init -std=gnu11 -g -O0
LDFLAGS= -lm
LDLIBS=

4task: src/4task.c
		$(CC) $(CFLAGS)  $^ -o out/$@

5task: src/5task.c lib/log.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

6task: src/6task.c
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

7task: src/7task.c lib/eng_runner.o lib/log.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@
	
8task: src/8task.c lib/eng_runner.o lib/log.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

9task: src/9task.c lib/eng_runner.o lib/log.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

10task: src/10task.c lib/eng_runner.o lib/log.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

11task: src/11task.c lib/eng_runner.o lib/log.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

12task: src/12task.c lib/eng_runner.o lib/log.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

13task: src/13task.c lib/eng_runner.o lib/log.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

14task: src/14task.c lib/eng_runner.o lib/log.o lib/checker.o config/14task_func.c
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

15task_func: src/15task_func.c lib/eng_launcher.o lib/eng_runner.o lib/log.o lib/checker.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

test1: src/test1.c
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

isoc23_checker: src/isoc23_checker.c
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

16task_1.02: src/16task_func.c lib/eng_launcher.o lib/eng_runner.o lib/log.o lib/checker.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

17task_6.02: src/17task_func.c lib/eng_launcher.o lib/eng_runner.o lib/log.o lib/checker.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

18task_8.02: src/18task_func.c lib/eng_launcher.o lib/eng_runner.o lib/log.o lib/checker.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

19task_10.02: src/19task_func.c lib/eng_launcher.o lib/eng_runner.o lib/log.o lib/checker.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

20task_11.02: src/20task_func.c lib/eng_launcher.o lib/eng_runner.o lib/log.o lib/checker.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

21task_11.02: src/21task_func.c lib/eng_launcher.o lib/eng_runner.o lib/log.o lib/checker.o lib/error.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

22task_15.02: src/22task_func.c lib/eng_launcher.o lib/eng_runner.o lib/log.o lib/checker.o lib/error.o
		$(CC) $(CFLAGS)  $^ $(LDFLAGS) -o out/$@

UTIL_PATH = ../utils

# libs from utils
lib/log.o:
	cd $(UTIL_PATH) && $(MAKE) log.o
	@cp $(UTIL_PATH)/lib/log.o lib/

lib/checker.o:
	cd $(UTIL_PATH) && $(MAKE) checker.o
	@cp $(UTIL_PATH)/lib/checker.o lib/

lib/test.o:
	cd $(UTIL_PATH) && $(MAKE) test.o
	@cp $(UTIL_PATH)/test.o lib/

lib/error.o:
	cd $(UTIL_PATH) && $(MAKE) error.o
	@cp $(UTIL_PATH)/error.o lib/

lib/common.o:
	cd $(UTIL_PATH) && $(MAKE) common.o
	@cp $(UTIL_PATH)/common.o lib/

lib/eng_runner.o:
	cd $(UTIL_PATH) && $(MAKE) eng_runner.o
	@cp $(UTIL_PATH)/lib/eng_runner.o lib/

lib/eng_launcher.o:
	cd $(UTIL_PATH) && $(MAKE) eng_launcher.o
	@cp $(UTIL_PATH)/lib/eng_launcher.o lib/

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


