#ifdef  __BROWSER_UTIL_H__
#define __BROWSER_UTIL_H__

extern WND hBrowserWnd;

extern int util_strncasecmp(const char *a, const char *b, int len);
#ifdef WIN32
extern int strcasecmp(const char *s1, const char *s2);
#endif


#endif
