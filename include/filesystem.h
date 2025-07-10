#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stddef.h>
#include "types.h"
#include "memory.h"

#define MAX_FILENAME_LENGTH 25

typedef struct {
    char name[MAX_FILENAME_LENGTH]; // Name of the file
    uint32_t size; // Size of the file in bytes
    uint32_t flags; // File flags (e.g., read, write, execute)
    time_t created; // Timestamp for creation
    time_t modifed; // Timestamp for last modification
    void* fs_data; // Pointer to filesystem-specific data (e.g., offset in RAM disk)
} file_t;

typedef struct {
    file_t* (*open)(const char* path, int flags);
    int (*close)(file_t* file);
    int (*read)(file_t* file, void* buffer, size_t size);
    int (*write)(file_t* file, const void* buffer, size_t size);
    void (*init)(void);
    void (*format)(void);
} fs_operations_t;

#ifndef RAMDISK_SIZE
#define RAMDISK_SIZE (1024 * 1024)
#endif

extern uint8_t ramdisk_data[RAMDISK_SIZE];

int ramdisk_read(file_t* file, void* buffer, size_t size);
int ramdisk_write(file_t* file, const void* buffer, size_t size);
void fs_init(void);
void fs_format(void);
void fs_mount(void);

file_t* fs_open(const char* path, int flags);
int fs_close(file_t* file);
int fs_read(file_t* file, void* buffer, size_t size);
int fs_write(file_t* file, const void* buffer, size_t size);

typedef enum {
    FS_TYPE_RAMDISK,
    FS_TYPE_HARDDRIVE
} fs_type_t;

fs_type_t get_fs_type(void);

extern fs_operations_t* current_fs_ops;
extern fs_operations_t ramdisk_fs_ops;
extern fs_operations_t harddrive_fs_ops; // For future use

#endif
