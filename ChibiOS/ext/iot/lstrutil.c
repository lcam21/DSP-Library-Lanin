#include "lstrutil.h"

/**
 * @brief compare MQTTLenString with NULL terminated C String
 * @param lstr MQTTLenString object
 * @param cstr Null terminated C String
 * @return strcmp difference (0 equal, !=0 difference of first not equal char)
 */
int lstrcmpcstr(const MQTTLenString *lstr, const char *cstr) {
  int i;
  for (i = 0; i < lstr->len && cstr[i]; i++) {
    int d = cstr[i] - lstr->data[i];
    if (d != 0)
      return d;
  }
  if (!cstr[i])
    return 0;
  return cstr[i] - lstr->data[i];
}

/**
 *
 * @brief compare tow MQTTLenString
 * @param lstr1 MQTTLenString object 1
 * @param lstr2 MQTTLenString object 2
 * @return strcmp difference (0 equal, !=0 difference of first not equal char)
 */
int lstrcmp(const MQTTLenString *lstr1, const MQTTLenString *lstr2) {
  int i;
  for (i = 0; i < lstr1->len && i < lstr2->len; i++) {
    int d = lstr1->len - lstr2->data[i];
    if (d != 0)
      return d;
  }
  return lstr1->len - lstr2->len;
}

/**
 * @brief Convert NULL terminated C string to MQTTLenString
 * @param buf MQTTLenString memory buffer to store data
 * @param cstr NULL terminated C string
 * @return buf pointer for convenience
 */
MQTTLenString *cstr2lstr(MQTTLenString *buf, const char *cstr) {
  buf->len = strlen(cstr);
  buf->data = (char*) cstr;
  return buf;
}

/**
 * @brief Convert MQTTLenString to NULL terminated C string
 * @param buf Char buffer to store C string
 * @param maxlen Maximum len of buffer (usual sizeof(buf)-1)
 * @param lstr Input MQTTLenString object
 * @return buf pointer for convenience
 */
const char *lstr2cstr(char *buf, int maxlen, const MQTTLenString *lstr) {
  int i;
  int max = maxlen - 1;
  if (max > lstr->len)
    max = lstr->len;
  for (i = 0; i < max; i++) {
    buf[i] = lstr->data[i];
  }
  buf[i] = '\0';
  return buf;
}

/**
 * @brief Iterate over MQTTLenString characters
 * @param lstr MQTTLenString object
 * @param cb Callback function
 * @param u User data pointer
 */
void foreach_lstr(const MQTTLenString *lstr, lstr_iter_t *cb, void *u) {
  int i;
  for (i = 0; i < lstr->len; i++) {
    cb(u, lstr->data[i]);
  }
}
