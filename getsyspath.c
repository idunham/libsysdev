/* Copyright Isaac Dunham, in the year of our Lord 2015 
 * No rights reserved, see LICENSE for details.
 */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/* Given major/minor/type, return the directory in /sys 
 * Returns NULL on failure.
 */
char * sysdev_getsyspath(unsigned int major, unsigned int minor, int ischar)
{
	char tpath[256], *syspath = NULL;
	size_t len;
	
	errno = 0;
	if (sizeof(tpath) <= snprintf(tpath, sizeof(tpath), "/sys/dev/%s/%u:%u",
			ischar ? "char" : "block", major, minor)) {
		syspath = calloc(_PC_PATH_MAX, 1);
		if (syspath && (readlink(tpath, syspath, _PC_PATH_MAX)
				> _PC_PATH_MAX) || errno ) {
			free(syspath);
			syspath = NULL;
		}
		/* Overwrite the start of syspath (../..) with "/sys/" */
		if (syspath) memcpy(syspath, "/sys/", 5);
		return syspath;
	}
	return NULL;
}
