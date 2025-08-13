// ramdisk_fs.c
#ifdef FS_RAMDISK
#include "filesystem.h"
#include "string.h"

#define MAX_FILES 128

file_t* ramdisk_files[MAX_FILES];

uint8_t ramdisk_data[RAMDISK_SIZE];

void ramdisk_init(void) {
    memset(ramdisk_data, 0, RAMDISK_SIZE);
    
    for (int i = 0; i < MAX_FILES; i++) {
        ramdisk_files[i] = NULL;
    }
}

void ramdisk_format(void) {
    memset(ramdisk_data, 0, RAMDISK_SIZE);
}

int ramdisk_read(file_t* file, void* buffer, size_t size) {
    if (!file || !buffer || size == 0) {
        return -1; // Invalid parameters
    }

    if (file->flags & O_RDONLY) {
        uint32_t offset = (uint32_t)file->fs_data;
        if (offset + size > RAMDISK_SIZE) {
            size = RAMDISK_SIZE - offset; // Prevent overflow
        }
        
        memcpy(buffer, &ramdisk_data[offset], size);
        return size;
    }
    return -1;
}

int ramdisk_write(file_t* file, const void* buffer, size_t size) {
    if (!file || !buffer || size == 0) {
        return -1; // Invalid parameters
    }

    if (file->flags & O_WRONLY) {
        uint32_t offset = (uint32_t)file->fs_data;
        if (offset + size > RAMDISK_SIZE) {
            size = RAMDISK_SIZE - offset; // Prevent overflow
        }

        memcpy(&ramdisk_data[offset], buffer, size);
        file->size += size; // Update file size
        return size;
    }
    return -1;
}

file_t* ramdisk_fs_open(const char* path, int flags) {
    file_t* file = NULL;
    for (int i = 0; i < MAX_FILES; i++) {
        if (ramdisk_files[i] && strcmp(ramdisk_files[i]->name, path) == 0) {
            file = heap_malloc(sizeof(file_t));
            if (!file) return NULL;
            
            memcpy(file, ramdisk_files[i], sizeof(file_t));
            return file;
        }
    }

    if (flags & O_CREAT) {
        file = heap_malloc(sizeof(file_t));
        if (!file) return NULL;

        strncpy(file->name, path, MAX_FILENAME_LENGTH - 1);
        file->name[MAX_FILENAME_LENGTH - 1] = '\0';
        file->size = 0;
        file->flags = flags;
        file->fs_data = (void*)0;

        for (int i = 0; i < MAX_FILES; i++) {
            if (!ramdisk_files[i]) {
                ramdisk_files[i] = heap_malloc(sizeof(file_t));
                if (!ramdisk_files[i]) {
                    heap_free(file);
                    return NULL;
                }
                memcpy(ramdisk_files[i], file, sizeof(file_t));
                break;
            }
        }
    }

    return file;
}

int ramdisk_fs_close(file_t* file) {
    if (!file) {
        return -1;
    }
    
    heap_free(file);
    return 0;
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
