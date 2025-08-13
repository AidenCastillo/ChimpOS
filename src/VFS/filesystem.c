#include <stddef.h>
#include "filesystem.h"
#include "string.h"

fs_type_t current_fs_type = FS_TYPE_RAMDISK;  // Default
fs_operations_t* current_fs_ops = NULL;

fs_type_t get_fs_type(void) {
    return current_fs_type;
}

void set_fs_type(fs_type_t type) {
    current_fs_type = type;
}



int fs_init(void) {
    // Set up the function pointers based on filesystem type
    if (get_fs_type() == FS_TYPE_RAMDISK) {
        current_fs_ops = &ramdisk_fs_ops;
    } else {
        // current_fs_ops = &harddrive_fs_ops;
    }
    
    // Initialize the selected filesystem
    current_fs_ops->init();

    return 0;
}

void fs_format(void) {
    // Format the filesystem, e.g., clear the RAM disk
    memset(ramdisk_data, 0, RAMDISK_SIZE);
}
void fs_mount(void) {
    // Mount the filesystem, e.g., prepare the RAM disk for use
    // This could involve setting up file structures or metadata
    fs_init();
}

file_t* fs_open(const char* path, int flags) {
    return current_fs_ops->open(path, flags);
}

int fs_close(file_t* file) {
    if (file) {
        heap_free(file);
    } else {
        return -1; // Error: file is NULL
    }
    return 0; // Return 0 on success
}

int fs_read(file_t* file, void* buffer, size_t size) {
    return current_fs_ops->read(file, buffer, size);
}

int fs_write(file_t* file, const void* buffer, size_t size) {
    return current_fs_ops->write(file, buffer, size);
}

