/* Copyright Isaac Dunham, in the year of our Lord 2015 
 * No rights reserved, see LICENSE for details.
 */
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <libsysdev/sysdev.h>

/* Given major/minor/type, return the directory in /sys 
 * Returns NULL on failure.
 */
char * sysdev_getsyspath(unsigned int major, unsigned int minor, int ischar)
{
	char tpath[256], *syspath = NULL;
	ssize_t len;
	
	errno = 0;
	len = snprintf(tpath, sizeof(tpath), "/sys/dev/%s/%u:%u",
		ischar ? "char" : "block", major, minor);
	if (len < sizeof(tpath)) {
		syspath = calloc(PATH_MAX, 1);
		if (syspath) {
			len = readlink(tpath, syspath, PATH_MAX);
			if ((len < 6) || (len > PATH_MAX)) {
				free(syspath);
				syspath = NULL;
			}
		}
		/* Overwrite the start of syspath (../..) with "/sys/" */
		if (syspath) memcpy(syspath, "/sys/", 5);
	}
	return syspath;
}
