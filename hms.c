/*
 * Extension for sqlite to parse a duration.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1
#include <assert.h>
#define hms "duration"
/*
 * Parse an hour:min:sec.s string into a float of seconds. Return -1.0 on
 * parsing errors.
 * 
 * Shortcomings: %f accepts a negative number; there could be input beyond the
 * accepted string.
 */
static float hms_to_sec(const char *duration)
{
    int hours = 0, minutes = 0;
    float seconds = 0.0;
    int items;

    items = sscanf(duration, "%u:%u:%f", &hours, &minutes, &seconds);
    if (items == 3) {
        return (float) hours *3600.0 + (float) minutes *60.0 + seconds;
    }
    items = sscanf(duration, "%u:%f", &minutes, &seconds);
    if (items == 2) {
        return (float) minutes *60.0 + seconds;
    }
    items = sscanf(duration, "%f", &seconds);
    if (items == 1) {
        return seconds;
    }
    return -1.0;
}

/*
 * Format a positive float duration (seconds) into a hh:mm:ss.s string buffer
 * provided by the caller. Returns the string buffer on success or NULL on
 * failure.
 */
static char *sec_to_hms(float seconds, char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size <= 0 || seconds < 0) {
        return NULL;
    }
    double whole_seconds = floor(seconds);
    double rem = fmod(seconds, 60.0);

    int hours = (int) (whole_seconds / 3600);
    int secs = (int) whole_seconds % 3600;
    int mins = secs / 60;

    int ret = snprintf(buffer, buffer_size, "%d:%02d:%06.3f",
                       hours, mins, rem);

    if (ret < 0 || (size_t) ret >= buffer_size) {
        return NULL;
    }
    return buffer;
}

/*
 * Implementation for sqlite
 */
static void
hms_to_sec_func(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    if (argc != 1) {
        sqlite3_result_error(context, hms " requires exactly one argument",
                             -1);
        return;
    }
    const unsigned char *text = sqlite3_value_text(argv[0]);
    if (text == NULL) {
        sqlite3_result_null(context);
        return;
    }
    double seconds = hms_to_sec((const char *) text);
    if (seconds >= 0.0)
        sqlite3_result_double(context, seconds);
    else
        sqlite3_result_error(context, hms " failed to parse argument", -1);

    return;
}

/*
 * Registration of functions in this file when this library is loaded
 */

#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_hms_init(sqlite3 *db,
                     char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    (void) pzErrMsg;            /* Unused parameter */
    rc = sqlite3_create_function(db, hms, 1,
                                 SQLITE_UTF8 | SQLITE_INNOCUOUS |
                                 SQLITE_DETERMINISTIC, 0, hms_to_sec_func,
                                 0, 0);
    return rc;
}
