/**
 * @file    lstrutil.h
 * @brief   MQTTLenString standar utilities
 * @details Provide MQTTLenString manipulation utilities like cstring
 * @version 1.0
 * @date    06/08/2015 17:01:42
 * @author  mribelotta
 */

#ifndef LSTRUTIL_H_
#define LSTRUTIL_H_

#include <string.h>
#include <MQTTPacket.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Type of iterate over MQTTLenString callback
 * @param u User data pointer (las argument of #foreach_lstr)
 * @param c Current character
 */
typedef void (lstr_iter_t)(void *u, char c);

extern int lstrcmpcstr(const MQTTLenString *lstr, const char *cstr);
extern int lstrcmp(const MQTTLenString *lstr1, const MQTTLenString *lstr2);
extern MQTTLenString *cstr2lstr(MQTTLenString *buf, const char *cstr);
extern const char *lstr2cstr(char *buf, int maxlen, const MQTTLenString *lstr);
extern void foreach_lstr(const MQTTLenString *lstr, lstr_iter_t *cb, void *u);

#ifdef __cplusplus
}
#endif

#endif /* LSTRUTIL_H_ */
