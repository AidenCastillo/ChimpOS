// ramdisk_fs.c
#ifdef FS_RAMDISK
#include "filesystem.h"

void ramdisk_init(void) {
    // Ramdisk implementation
    memset(ramdisk_data, 0, RAMDISK_SIZE);
}

void ramdisk_format(void) {
    memset(ramdisk_data, 0, RAMDISK_SIZE);
}

file_t* ramdisk_fs_open(const char* path, int flags) {
    // Implementation
    file_t* file = heap_malloc(sizeof(file_t));
    if (!file) return NULL;

    strncpy(file->name, path, MAX_FILENAME_LENGTH - 1);
    file->name[MAX_FILENAME_LENGTH - 1] = '\0';
    file->size = 0;
    file->flags = flags;
    file->fs_data = (void*)0;
    return file;
}

int ramdisk_fs_close(file_t* file) {
    // Implementation
    if (file) {
        heap_free(file);
    }
    return 0;
}

fs_operations_t ramdisk_fs_ops = {
    .open = ramdisk_fs_open,
    .close = ramdisk_fs_close,
    .read = ramdisk_read,
    .write = ramdisk_write,
    .init = ramdisk_init,
    .format = ramdisk_format
};
#endif
