#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <libsysdev/sysdev.h>

void usage(char *progname)
{
	fprintf(stderr, "usage: %s /dev/node", progname);
	exit(1);
}

int main(int argc, char **argv)
{
	char *syspath;
	int sysfd = -1, vend = 0, dev = 0;
	struct stat st;

	if (argc < 2)
		usage(argv[0]);
	if (stat(argv[1], &st) || (!(st.st_mode & (S_IFCHR | S_IFBLK))))
		return 1;
	syspath = sysdev_getsyspath(major(st.st_rdev), minor(st.st_rdev),
				    S_ISCHR(st.st_mode));
	if (syspath)
		sysfd = open(syspath, O_RDONLY);
	if (sysfd > -1) {
		printf("%s", syspath);
		if (!sysdev_getproductids(&vend, &dev, sysfd))
			printf("\t%X\t%X", vend, dev);
		printf("\n");
	}
	if (sysfd > -1)
		close(sysfd);
	if (syspath)
		free(syspath);
	return 0;
}
