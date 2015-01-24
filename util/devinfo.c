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

int shownode(char *arg)
{
	char *syspath;
	int sysfd = -1, vend = 0, dev = 0, ret = 0;
	struct stat st;

	if (stat(arg, &st) || (!(st.st_mode & (S_IFCHR | S_IFBLK))))
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
		close(sysfd);
	} else
		ret = 1;
	if (syspath)
		free(syspath);
	return ret;
}

int main(int argc, char **argv)
{
	int i, ret = 0;

	if (argc < 2)
		usage(argv[0]);

	for (i=1; i < argc;) {
		ret |= shownode(argv[i++]);
	}
	return ret;
}
