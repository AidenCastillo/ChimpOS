#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stddef.h>
#include "types.h"
#include "memory.h"

#define MAX_FILENAME_LENGTH 25

#define O_RDONLY  0x0001 // Open for reading only
#define O_WRONLY  0x0002 // Open for writing only
#define O_RDWR    (O_RDONLY | O_WRONLY) // Open for reading and writing
#define O_CREAT   0x0008 // Create file if it does not exist
#define O_TRUNC   0x0010 // Truncate file to zero length
#define O_APPEND  0x0020 // Append to the end of the file

typedef struct {
    char name[MAX_FILENAME_LENGTH]; // Name of the file
    uint32_t size; // Size of the file in bytes
    uint32_t flags; // File flags (e.g., read, write, execute)
    time_t created; // Timestamp for creation
    time_t modifed; // Timestamp for last modification
    void* fs_data; // Pointer to filesystem-specific data (e.g., offset in RAM disk)
} file_t;

typedef struct {
    void (*init)(void);
    void (*format)(void);
    int (*read)(file_t* file, void* buffer, size_t size);
    int (*write)(file_t* file, const void* buffer, size_t size);
    file_t* (*open)(const char* path, int flags);
    int (*close)(file_t* file);
} fs_operations_t;

#ifndef RAMDISK_SIZE
#define RAMDISK_SIZE (1024 * 1024)
#endif

extern uint8_t ramdisk_data[RAMDISK_SIZE];

int ramdisk_read(file_t* file, void* buffer, size_t size);
int ramdisk_write(file_t* file, const void* buffer, size_t size);
int fs_init(void);
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
