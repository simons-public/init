/* config.h - config file for init */

#define HOSTNAME "hostname"
#define RC_PATH "/etc/rc"

#define FIRST_CONSOLE_TTY 1
#define NUM_CONSOLE_TTYS 3
#define CONSOLE_TTY_CMDLINE "/sbin/agetty -o '-p -- \\u' --noclear tty%i linux &"

#define MAX_CMDLINE_LEN 255
