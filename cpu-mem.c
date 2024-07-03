#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Get number of CPU cores
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cores == -1) {
        perror("sysconf");
        exit(EXIT_FAILURE);
    }

    // Open /proc/meminfo for reading memory information
    int meminfo_fd = open("/proc/meminfo", O_RDONLY);
    if (meminfo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    long total_mem, free_mem;

    // Read memory information from /proc/meminfo
    while ((bytes_read = read(meminfo_fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';

        // Extract total memory and free memory
        if (strstr(buffer, "MemTotal:") != NULL) {
            sscanf(buffer, "MemTotal: %ld kB", &total_mem);
        } else if (strstr(buffer, "MemFree:") != NULL) {
            sscanf(buffer, "MemFree: %ld kB", &free_mem);
        }
    }

    close(meminfo_fd);

    // Check for errors during reading
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Print results
    printf("CPU Cores: %d\n", num_cores);
    printf("Total Memory: %ld KB\n", total_mem);
    printf("Free Memory: %ld KB\n", free_mem);

    return EXIT_SUCCESS;
}
