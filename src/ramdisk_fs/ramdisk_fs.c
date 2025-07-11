// ramdisk_fs.c
#ifdef FS_RAMDISK
#include "filesystem.h"
#include "string.h"

void ramdisk_init(void) {
    memset(ramdisk_data, 0, RAMDISK_SIZE);
}

void ramdisk_format(void) {
    memset(ramdisk_data, 0, RAMDISK_SIZE);
}

int ramdisk_read(file_t* file, void* buffer, size_t size) {
    uint32_t offset = (uint32_t)file->fs_data;
    if (offset + size > RAMDISK_SIZE) {
        size = RAMDISK_SIZE - offset; // Prevent overflow
    }

    memcpy(buffer, &ramdisk_data[offset], size);
    return size;
}

int ramdisk_write(file_t* file, const void* buffer, size_t size) {
    uint32_t offset = (uint32_t)file->fs_data;
    if (offset + size > RAMDISK_SIZE) {
        size = RAMDISK_SIZE - offset; // Prevent overflow
    }

    memcpy(&ramdisk_data[offset], buffer, size);
    file->size += size; // Update file size
    return size;
}

file_t* ramdisk_fs_open(const char* path, int flags) {
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
    if (file) {
        heap_free(file);
        return 0;
    }
    return -1;
}


fs_operations_t ramdisk_fs_ops = {
    .init = ramdisk_init,
    .format = ramdisk_format,
    .read = ramdisk_read,
    .write = ramdisk_write,
    .close = ramdisk_fs_close,
    .open = ramdisk_fs_open
};
#endif
