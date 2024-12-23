#ifndef HEADERS_H_
#define HEADERS_H_
#define _OPEN_SYS_ITOA_EXT
// In-Built Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/wait.h>
#include <time.h>
#include <stdarg.h>
#include <linux/limits.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <fcntl.h>
#include <time.h>
#include <termios.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <libgen.h>
#include <pwd.h>
#include <error.h>


// User-Created Libraries
#include "display.h"
#include "prompt.h"
#include "inputcomm.h"
#include "warp.h"
#include "proclore.h"
#include "pastevents.h"
#include "foreground.h"
#include "background.h"
#include "bg_process.h"
#include "peek.h"
#include "seek.h"
#include "activities.h"
#include "set_signals.h"
#include "fg_bg.h"
#include "rawmode.h"
#include "man_page.h"
#include "neonate.h"
#include "pipes_io.h"


#endif