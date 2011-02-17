#include "../header/config.h"
#include <string.h>

#include "../header/rexy_browser.h"

#include "../header/util.h"

int util_strncasecmp(const char *a, const char *b, int len)
{
    int i; 

    for (i=0; i< len; i++) {
        if ( toupper(a[i]) != toupper(b[i]) )
            return a[i]-b[i]; 
    }
    return 0; 
}

#ifdef WIN32
int strcasecmp(const char *s1, const char *s2)
{
    return util_strncasecmp( s1, s2, strlen(s1));
}
#endif

