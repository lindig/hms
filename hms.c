/*
 * Extension for sqlite to parse a duration.
 *
 * Copyright (c) 2025, Christian Lindig <lindig@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1
#include <assert.h>
/*
 * Parse a duration string into a float of seconds.
 * Recognizes formats: H:M:S.f, M:S.f, and S.f
 * Returns the total seconds as a float, or -1.0 on parsing errors.
 *
 * Remaining shortcomings (as noted in the original):
 * - %f accepts a negative number (this is standard behavior for scanf).
 * - There could be input beyond the accepted string (this is also standard scanf behavior).
 */
static float hms_to_sec(const char *duration)
{
    int hours = 0, minutes = 0;
    float seconds = 0.0;
    int items;

    items = sscanf(duration, "%u:%u:%f", &hours, &minutes, &seconds);
    if (items == 3) {
        return (float)hours *3600.0 + (float)minutes *60.0 + seconds;
    }
    items = sscanf(duration, "%u:%f", &minutes, &seconds);
    if (items == 2) {
        return (float)minutes *60.0 + seconds;
    }
    items = sscanf(duration, "%f", &seconds);
    if (items == 1) {
        return seconds;
    }
    return -1.0;
}

/*
 * Format a positive float duration (seconds) into a hh:mm:ss.sss string
 * buffer provided by the caller. Returns the string buffer on success
 * or NULL on failure.
 */
static char *sec_to_hms(double seconds, char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size <= 0 || seconds < 0) {
        return NULL;
    }
    double whole_seconds = floor(seconds);
    double rem = fmod(seconds, 60.0);

    int hours = (int)(whole_seconds / 3600);
    int secs = (int)whole_seconds % 3600;
    int mins = secs / 60;

    int ret = snprintf(buffer, buffer_size, "%d:%02d:%06.3f",
                       hours, mins, rem);

    if (ret < 0 || (size_t) ret >= buffer_size) {
        return NULL;
    }
    return buffer;
}

/*
 * Implementation for the 'duration' sqlite function (string to seconds)
 */
static void hms_to_sec_func(sqlite3_context * context, int argc,
                             sqlite3_value ** argv)
{
    if (argc != 1) {
        sqlite3_result_error(context,
                             "duration() requires exactly one argument.",
                             -1);
        return;
    }
    const unsigned char *text = sqlite3_value_text(argv[0]);
    if (text == NULL) {
        sqlite3_result_null(context);
        return;
    }
    double seconds = hms_to_sec((const char *)text);
    if (seconds >= 0.0) {
        sqlite3_result_double(context, seconds);
    } else {
        sqlite3_result_error(context,
                       "duration() failed to parse the duration string.",
                             -1);
    }
    return;
}

/*
 * Implementation for the 'hms' sqlite function (seconds to string)
 */
static void sec_to_hms_func(sqlite3_context * context,
                             int argc, sqlite3_value ** argv)
{
    if (argc != 1) {
        sqlite3_result_error(context,
                             "hms() requires exactly one argument.", -1);
        return;
    }
    int value_type = sqlite3_value_numeric_type(argv[0]);
    double seconds;
    switch (value_type) {
    case SQLITE_INTEGER:
        seconds = (double)sqlite3_value_int64(argv[0]);
        break;
    case SQLITE_FLOAT:
        seconds = sqlite3_value_double(argv[0]);
        break;
    case SQLITE_NULL:
        sqlite3_result_null(context);
        break;
    default:
        sqlite3_result_error(context, "hms() argument must be a number.", -1);
        break;
    }

    char buffer[64];
    char *result = sec_to_hms(seconds, buffer, sizeof(buffer));
    if (!result) {
        sqlite3_result_error(context, "hms() conversion failed.", -1);
    } else {
        sqlite3_result_text(context, result, -1, SQLITE_TRANSIENT);
    }
}

/*
 * Registration of functions when this library is loaded
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
    int sqlite3_hms_init(sqlite3 * db,
                      char **pzErrMsg, const sqlite3_api_routines * pApi)
{
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    (void)pzErrMsg;             /* Unused parameter */

    rc = sqlite3_create_function(db, "duration", 1,
                                 SQLITE_UTF8 | SQLITE_INNOCUOUS |
                                 SQLITE_DETERMINISTIC, NULL,
                                 hms_to_sec_func, NULL, NULL);
    if (rc != SQLITE_OK)
        return rc;

    rc = sqlite3_create_function(db, "hms", 1,
                                 SQLITE_UTF8 | SQLITE_INNOCUOUS |
                                 SQLITE_DETERMINISTIC, NULL,
                                 sec_to_hms_func, NULL, NULL);
    if (rc != SQLITE_OK)
        return rc;

    return rc;
}
