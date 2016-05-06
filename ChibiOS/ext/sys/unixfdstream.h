#ifndef FILESTREAM_H_
#define FILESTREAM_H_

#include "ch.h"

#define _unixfd_stream_data \
	int fd;

/**
 * @internal
 */
struct UnixfdStreamVMT {
  _base_sequential_stream_methods
};

/**
 * @extends BaseSequentialStream
 */
typedef struct {
  const struct UnixfdStreamVMT *vmt;_unixfd_stream_data
} UnixfdStream;

#ifdef __cplusplus
extern "C" {
#endif

void ufdObjectInit(UnixfdStream *sp, int fd);

#ifdef __cplusplus
}
#endif

#endif /* FILESTREAM_H_ */
