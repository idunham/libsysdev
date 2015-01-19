/* libsysdev public defines.
 * This contains only a description of the API provided by libsysdev,
 * and as such the author does not consider it copyrightable and
 * waives any and all claims to copyright.
 * In jurisdictions where this is not recognized by law,
 * it is subject to the following copyright, license,
 * and disclaimer of warranty:

Copyright Isaac Dunham, in the year of our Lord 2015.
 
Permission to use, copy, modify, and/or distribute this software for any
purpose, with or without fee, is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

 *
 * libsysdev uses the sysdev_ namespace for public APIs
 * The sdp_ namespace is used internally; if it's visible, it's broken.
 */
 
#ifndef _LIBSYSDEV_SYSDEV_H
#define _LIBSYSDEV_SYSDEV_H 1

/* given fd of syspath, set vendor_id and device_id
 * returns 0 for success, nonzero for failure
 */
int sysdev_getproductids(int *vendor_id, int *device_id, int sysfd);

/* given major/minor/type, return the directory in /sys
 * returns a calloc'd string (or NULL on failure)
 */
char * sysdev_getsyspath(unsigned int major, unsigned int minor, int ischar);

/* given fd of device, return the directory in /sys
 * returns a calloc'd string (or NULL on failure)
 * sets errno = EINVAL if devfd is not a device
 */
char *sysdev_devfd_to_syspath(int devfd);

/* given fd of device, return fd of corresponding syspath
 * returns -1 on failure, sets errno = EINVAL if devfd is not a device
 */
int sysdev_devfd_to_sysfd(int devfd);

/* given fd of device, read vendor_id and device_id
 * returns 0 for success, nonzero for failure
 */
int sysdev_devfd_to_pciid(int *vendor_id, int *device_id, int devfd);

#endif
