//#define _XOPEN_SOURCE 700

#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/mount.h>
#include <linux/reboot.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

void mount_fs()
{
	/* man 2 mount */
	/* man 7 mount_namespaces */
	/*    source, target,  type,   mountflags,  data */
	mount("proc", "/proc", "proc", MS_NOSUID + MS_NOEXEC, "");
	mount("sys", "/sys", "sysfs", MS_NOSUID + MS_NOEXEC, "");
	mount("tmp", "/tmp", "tmpfs", MS_NOSUID, "");
	mount("dev", "/dev", "devtmpfs", MS_NOSUID, "");
	mount("devpts", "/dev/pts", "devpts", MS_NOSUID + MS_NOEXEC, "");
	mount("shm", "/dev/shm", "tmpfs", MS_NOSUID + MS_NOEXEC, "");
	system("/usr/bin/mount -o remount,rw /");
	system("/usr/bin/mount -a");
}

void start_ttys()
{
	system("/sbin/agetty -o '-p -- \\u' --noclear tty2 linux &");
	system("/sbin/agetty -o '-p -- \\u' --noclear tty3 linux &");
	system("/sbin/agetty -o '-p -- \\u' --noclear tty4 linux &");
}

void reboot_handler(int signum)
{
	sync();
	reboot(LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART2);
}

int main()
{
	sigset_t set;
	int status;

	// only run if pid 1
	if (getpid() !=1) return 1;

	printf("starting\n");

	sigfillset(&set);
	sigprocmask(SIG_BLOCK, &set, 0);

	if (fork()) for (;;) wait(&status);

	sigprocmask(SIG_UNBLOCK, &set, 0);

	setsid();
	setpgid(0, 0);
	
	printf("setting hostname to %s\n", HOSTNAME);
	sethostname(HOSTNAME, strlen(HOSTNAME));

	printf("mounting fs\n");
	mount_fs();

	printf("starting ttys\n");
	start_ttys();

	printf("registering sighup for reboot\n");
	signal(SIGHUP, reboot_handler);

	printf("running /root/init/rc\n");
	return execve("/root/init/rc", (char *[]){ "rc", 0 }, (char *[]){ 0 });
}