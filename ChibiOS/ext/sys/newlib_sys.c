/**
 * System calls
 */

#include <stddef.h>
#include <errno.h>
#include <sys/file.h>
#include <unistd.h>

#include <ch.h>
#include <ff.h>

#define MAX_FILES 10

static FIL fileEntries[MAX_FILES];

static int get_freeEntry(void) {
	int i;
	for (i = 0; i < MAX_FILES; i++)
		if (fileEntries[i].fs == NULL)
			return i;
	return -1;
}

int _fstat(int fd) {
  (void) fd;
  errno = ENOSYS;
  return -1;
}

int _isatty(int file) {
	(void) file;
	return 0;
}

/**
 * Transform UNIX O_* to Chan's FATFS flags
 * @param flags O_*
 * @return FA_* representing the options
 */
static int unix2mode(int flags) {
	int mode = FA_READ;
	if (flags & O_WRONLY)
		mode |= FA_WRITE;
	if ((flags & O_ACCMODE) & O_RDWR)
		mode |= FA_WRITE;
	if (flags & O_CREAT)
		mode |= FA_OPEN_ALWAYS;
	if (flags & O_TRUNC)
		mode |= FA_CREATE_ALWAYS;
	if (flags & O_EXCL)
		mode |= FA_CREATE_NEW;
	return mode;
}

/**
 * Chan's FATFS error code to ERRNO nomenclature
 * @param e Chan's FATFS error
 * @return errno compatible flag
 */
static int fferr2errno(FRESULT e) {
	switch (e) {
	case FR_OK:
		return 0;
	case FR_NO_FILE:
	case FR_NO_PATH:
	case FR_NO_FILESYSTEM:
		return ENOENT;
	case FR_INVALID_NAME:
		return EINVAL;
	case FR_EXIST:
	case FR_INVALID_OBJECT:
		return EEXIST;
	case FR_DISK_ERR:
	case FR_NOT_READY:
	case FR_INT_ERR:
		return EIO;
	case FR_WRITE_PROTECTED:
	case FR_DENIED:
		return EROFS;
	case FR_MKFS_ABORTED:
		return EINVAL;
	default:
		return -1;
	}
}

_off_t _lseek(int fd, _off_t offset, int whence) {
	DWORD ofs = offset;
	FIL *fp = &fileEntries[fd];
	if (whence == SEEK_CUR)
		ofs += f_tell(fp);
	else if (whence == SEEK_END)
		ofs = f_size(fp) - offset;
	errno = fferr2errno(f_lseek(fp, ofs));
	return errno != 0 ? ((off_t) -1) : ((off_t) f_tell(fp));
}

_ssize_t _read(int fd, void *buf, size_t count) {
	FIL *fp = &fileEntries[fd];
	size_t readed = 0;
	errno = fferr2errno(f_read(fp, buf, count, &readed));
	return readed;
}

_ssize_t _write(int fd, const void *buf, size_t count) {
	FIL *fp = &fileEntries[fd];
	size_t writed = 0;
	errno = fferr2errno(f_write(fp, buf, count, &writed));
	return writed;
}

int _close(int fd) {
	f_close(&fileEntries[fd]);
	fileEntries[fd].fs = NULL;
	return 0;
}

int open(const char *file, int flags, ...) {
	int fd = get_freeEntry();
	if (fd == -1) {
		errno = ENFILE;
		return -1;
	}
	if (f_open(&fileEntries[fd], file, unix2mode(flags)) != FR_OK) {
		errno = EIO;
		return -1;
	}
	return fd;
}

int fstat(int fd, struct stat *st) {
	(void) fd;
	(void) st;
	// TODO Implement stat
	errno = ENOSYS;
	return -1;
}

#if 1
#include <reent.h>

void *_sbrk_r(struct _reent *r, ptrdiff_t incr) {
#if CH_USE_MEMCORE
	void *p = chCoreAlloc((size_t) incr);
	if (p == NULL) {
		r->_errno = ENOMEM;
		return (void*) -1;
	}
	return (void*) p;
#else
	__errno_r(r) = ENOMEM;
	return (caddr_t)-1;
#endif
}
#endif
