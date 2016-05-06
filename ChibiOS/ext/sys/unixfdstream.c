#include "unixfdstream.h"

#include <unistd.h>

static size_t ufd_wr(void *p, const uint8_t *bp, size_t n) {
	return write(((UnixfdStream*) p)->fd, bp, n);
}

static size_t ufd_rd(void *p, uint8_t *bp, size_t n) {
	return read(((UnixfdStream*) p)->fd, bp, n);
}

static msg_t ufd_put(void *p, uint8_t b) {
	return (ufd_wr(p, &b, 1) == 1 ? Q_OK : Q_RESET);
}

static msg_t ufd_get(void *p) {
	uint8_t b;

	return (ufd_rd(p, &b, 1) == 1 ? b : Q_RESET);
}

static const struct UnixfdStreamVMT vmt = { ufd_wr, ufd_rd, ufd_put, ufd_get, };

void ufdObjectInit(UnixfdStream *sp, int fd) {
	sp->vmt = &vmt;
	sp->fd = fd;
}
