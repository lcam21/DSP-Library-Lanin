/**
 * @file    fakesys.c
 * @brief   Fake system calls
 * @details Fake system calls with dummy implementation for testing only
 * @version 0.1
 * @date    08/08/2015 15:57:38
 * @author  mribelotta
 */

#if 0
void _exit(int n) {}
void *_sbrk(int n) {}
void _kill(int p) {}
int _getpid(void) {}
int _write(int f, const void *p, int n) {}
void _close(int n) {}
void _fstat(int n, void *p) {}
int _isatty(void) {}
int _lseek(int f, int c, int n) {}
int _read(int f, const void *p, int n) {}
#endif
