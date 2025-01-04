#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* == BATTERY THRESHOLD == */
/* Program to limit the battery charge on a Lenovo Thinkpad T480 */
/* Should work on ther models too, but I didn't test it */

// Paths to the battery threshold files
// TODO: support multiple batteries
const char *start_threshold_path = "/sys/class/power_supply/BAT0/charge_start_threshold";
const char *stop_threshold_path = "/sys/class/power_supply/BAT0/charge_stop_threshold";

int set_threshold(const char *path, const char *value) {
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }
    fprintf(fp, "%s", value);
    fclose(fp);
    return 0;
}

int read_file(const char *path, char** out) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        perror("Failed to open file");
        return -1;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;
    *out = string;

    return 0;
}

int main(int argc, char *argv[]) {

    // Check for correct usage
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <custom|normal|get>\n", argv[0]);
        return 1;
    }

    // Set thresholds based on the argument
    bool failed = false;
    if (strcmp(argv[1], "get") == 0) {
        char* start;
        char* end;
        read_file(start_threshold_path, &start);
        read_file(start_threshold_path, &end);

        printf("start: %send:   %s", start, end);

        free(start);
        free(end);

        return 0;
    } else if (strcmp(argv[1], "custom") == 0) {
        // Set the thresholds so the battery charges up to 80%
        failed = set_threshold(start_threshold_path, "75") != 0;
        failed = set_threshold(stop_threshold_path, "80") != 0;
    } else if (strcmp(argv[1], "normal") == 0) { 
        // Set the thresholds so the battery charges up to 100%
        failed = set_threshold(start_threshold_path, "96") != 0;
        failed = set_threshold(stop_threshold_path, "100") != 0;
    } else {
        fprintf(stderr, "Invalid argument: %s\n", argv[1]);
        fprintf(stderr, "Usage: %s <custom|normal>\n", argv[0]);
        return 1;
    }

    // if failed to write to the file
    if (failed) {
        fprintf(stderr, "Failed to set thresholds.\n");
        return 1;
    }

    printf("Battery thresholds successfully set to %s mode.\n", argv[1]);
    return 0;
}

