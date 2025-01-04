#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *micmute_path = "/sys/class/leds/platform::micmute/brightness";
const char *mute_path = "/sys/class/leds/platform::mute/brightness";

int set_led(const char *path, const char* value) {
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }
    fprintf(fp, "%s", value);
    fclose(fp);
    return 0;
}

int get_led(const char *path, char** out) {
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
    if (argc != 3) {
        fprintf(stderr, "Wrong argument count. Usage: %s <micmute|mute> <on|off|get>\n", argv[0]);
        return 1;
    }

    // Set thresholds based on the argument
    if (strcmp(argv[1], "micmute") == 0) {
        if (strcmp(argv[2], "get") == 0) {
            char* out;
            if (get_led(micmute_path, &out) != 0) return 1;
            printf("Current value of micmute is: %s", out);
            free(out);
            return 0;
        } else if (strcmp(argv[2], "on") == 0) {
            if (set_led(micmute_path, "1") != 0) return 1;
        } else if (strcmp(argv[2], "off") == 0) {
            if (set_led(micmute_path, "0") != 0) return 1;
        } else {
            fprintf(stderr, "Invalid argument: %s\n", argv[2]);
            fprintf(stderr, "Usage: %s %s <on|off|get>\n", argv[0], argv[1]);
            return 1;
        }
    } else if (strcmp(argv[1], "mute") == 0) {
        if (strcmp(argv[2], "get") == 0) {
            char* out;
            if (get_led(micmute_path, &out) != 0) return 1;
            printf("Current value of micmute is: %s", out);
            free(out);
            return 0;
        } else if (strcmp(argv[2], "on") == 0) {
            if (set_led(mute_path, "1") != 0) return 1;
        } else if (strcmp(argv[2], "off") == 0) {
            if (set_led(mute_path, "0") != 0) return 1;
        } else {
            fprintf(stderr, "Invalid argument: %s\n", argv[2]);
            fprintf(stderr, "Usage: %s %s <on|off|get>\n", argv[0], argv[1]);
            return 1;
        }
    } else {
        fprintf(stderr, "Invalid argument: %s\n", argv[1]);
        fprintf(stderr, "Usage: %s <micmute|mute> <on|off|get>\n", argv[0]);
        return 1;
    }
    printf("%s leds successfully set to %s.\n", argv[1], argv[2]);
    return 0;
}
