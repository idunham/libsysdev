/* Copyright Isaac Dunham, in the year of our Lord 2015 
 * No rights reserved, see LICENSE for details.
 */
#define _BSD_SOURCE /* for major()/minor()*/
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

/* given fd of device, return the directory in /sys
 * returns a calloc'd string (or NULL on failure)
 * sets errno = EINVAL if devfd is not a device
 */
char *sysdev_devfd_to_syspath(int devfd)
{
	struct stat st;

	if (fstat(devfd, &st) || !(st.st_mode&(S_IFCHR|S_IFBLK)))
	{
		if (!errno)
			errno = EINVAL;
		return NULL;
	}
	return sysdev_getsyspath(major(st.st_rdev), minor(st.st_rdev),
			S_ISCHR(st.st_mode));
}

/* given fd of device, return fd of corresponding syspath
 * returns -1 on failure, sets errno = EINVAL if devfd is not a device
 */
int sysdev_devfd_to_sysfd(int devfd)
{
	char *syspath;
	int sysfd;

	syspath = sysdev_devfd_to_syspath(devfd);
	if (!syspath)
		return -1;
	sysfd = open(syspath, O_RDONLY);
	free(syspath);
	return sysfd;
}

/* given fd of device, read vendor_id and device_id
 * returns 0 for success, nonzero for failure
 */
int sysdev_devfd_to_pciid(int *vendor_id, int *device_id, int devfd)
{
	int ret, sysfd = sysdev_devfd_to_sysfd(devfd);

	if (sysfd > -1) {
		ret = sysdev_getproductids(vendor_id, device_id, sysfd);
		close(sysfd);
		return ret;
	}
	return -1;
}
