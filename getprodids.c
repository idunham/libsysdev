/* Copyright Isaac Dunham, in the year of our Lord 2015 
 * No rights reserved, see LICENSE for details.
 */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <libsysdev/sysdev.h>

/* Read up to bufsiz bytes of name into buf, return result of read */
static int sdp_readfileat(char *buf, ssize_t bufsiz, int dfd, char *name)
{
	int result, fd;
	fd = openat(dfd, name, O_RDONLY);
	if (fd < 0)
		return -1;

	memset(buf, 0, bufsiz);
	result = read(fd, buf, bufsiz - 1);
	close(fd);
	return result;
}

/* given fd of syspath, set vendor_id and device_id; return 0 for success */
int sysdev_getproductids(int *vendor_id, int *device_id, int sysfd)
{
	char id[8];

	if (sdp_readfileat(id, 8, sysfd, "vendor") < 6)
		return -1;
	*vendor_id = (int)strtol(id, (char**)0, 16);

	if (sdp_readfileat(id, 8, sysfd, "device") < 6)
		return -1;
	*device_id = (int)strtol(id, (char**)0, 16);
	return 0;
}
