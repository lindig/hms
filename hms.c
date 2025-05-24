#include <stdio.h>
#include <string.h>
#include <math.h>

float
hms_to_sec(const char *duration)
{
    int             hours = 0, minutes = 0;
    float           seconds = 0.0;
    int             values;

    values = sscanf(duration, "%u:%u:%f", &hours, &minutes, &seconds);
    if (values == 3) {
        return (float)hours *3600.0 + (float)minutes *60.0 + seconds;
    }
    values = sscanf(duration, "%u:%f", &minutes, &seconds);
    if (values == 2) {
        return (float)minutes *60.0 + seconds;
    }
    values = sscanf(duration, "%f", &seconds);
    if (values == 1) {
        return seconds;
    }
    return -1.0;
}

char           *
sec_to_hms(float seconds, char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size <= 0 || seconds < 0) {
        return NULL;
    }
    double          whole_seconds = floor(seconds);
    double          fractional_part = fmod(seconds, 1.0);

    int             hours = (int)(whole_seconds / 3600);
    int             secs = (int)whole_seconds % 3600;
    int             tenths = (int)(fractional_part * 10.0);

    int             ret = snprintf(buffer, buffer_size, "%d:%02d:%02d.%1d",
                                   hours, secs / 60, secs % 60, tenths);

    if (ret < 0 || (size_t) ret >= buffer_size) {
        return NULL;
    }
    return buffer;
}


int
main()
{
    char            buf[20];

    char            duration1[] = "1:30:15.5";
    char            duration2[] = "5:45.25";
    char            duration3[] = "0:10:00";
    char            duration4[] = "2:05.3";
    char            duration5[] = "invalid";
    char            duration6[] = "1:a0:10";

    float           seconds1 = hms_to_sec(duration1);
    float           seconds2 = hms_to_sec(duration2);
    float           seconds3 = hms_to_sec(duration3);
    float           seconds4 = hms_to_sec(duration4);
    float           seconds5 = hms_to_sec(duration5);
    float           seconds6 = hms_to_sec(duration6);

    if (seconds1 >= 0) {
        printf("%s = %.2f seconds\n", duration1, seconds1);
        printf("%s\n", sec_to_hms(seconds1, buf, sizeof buf));
    } else
        printf("Error converting %s\n", duration1);

    if (seconds2 >= 0) {
        printf("%s = %.2f seconds\n", duration2, seconds2);
        printf("%s\n", sec_to_hms(seconds2, buf, sizeof buf));
    } else
        printf("Error converting %s\n", duration2);

    if (seconds3 >= 0) {
        printf("%s = %.2f seconds\n", duration3, seconds3);
        printf("%s\n", sec_to_hms(seconds3, buf, sizeof buf));
    } else
        printf("Error converting %s\n", duration3);

    if (seconds4 >= 0) {
        printf("%s = %.2f seconds\n", duration4, seconds4);
        printf("%s\n", sec_to_hms(seconds4, buf, sizeof buf));
    } else
        printf("Error converting %s\n", duration4);

    if (seconds5 >= 0) {
        printf("%s = %.2f seconds\n", duration5, seconds5);
        printf("%s\n", sec_to_hms(seconds5, buf, sizeof buf));
    } else
        printf("Error converting %s\n", duration5);

    if (seconds6 >= 0) {
        printf("%s = %.2f seconds\n", duration6, seconds6);
        printf("%s\n", sec_to_hms(seconds6, buf, sizeof buf));
    } else
        printf("Error converting %s\n", duration6);

    return 0;
}
