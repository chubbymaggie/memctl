#include "memctl_common.h"

#include "memctl_error.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

bool
mmap_file(const char *file, const void **data, size_t *size) {
	int fd = open(file, O_RDONLY);
	if (fd < 0) {
		error_open(file, errno);
		return false;
	}
	struct stat st;
	int err = fstat(fd, &st);
	if (err) {
		close(fd);
		error_io(file);
		return false;
	}
	size_t s = st.st_size;
	const void *d = mmap(NULL, s, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	if (d == MAP_FAILED) {
		error_out_of_memory();
		return false;
	}
	*data = d;
	*size = s;
	return true;
}