#ifndef _ENG_LAUNCHER_H
#define _ENG_LAUNCHER_H

#include "eng_runner.h"

// ...

struct eng_launcher_data {
	const char  *func_description;	// ex. x + y + z = 100
	const char  *elogfile;			// ex. log/<taskname>.log TODO: will be removed
	const char  *task_description;	// ex. task from KR or zen at 15.10.2026
};

extern struct eng_launcher_data		eng_data;

#endif /* _ENG_LAUNCHER_H */

 
