#include "test_framework.h"
#include "filesystem.h"
#include "string.h"
#include "terminal.h"
#include "memory.h"

static bool test_ramdisk_read(void) {
    const char* expected_data = "Expected file contents";
    memcpy(ramdisk_data, expected_data, strlen(expected_data) + 1); // Initialize RAM disk with expected data

    char buffer[128];
    file_t file = { .name = "testfile.txt", .fs_data = (void*)0 }; // Example file
    int bytes_read = ramdisk_read(&file, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        terminal_writestring("Error reading from RAM disk\n");
        return false; // Read failed
    }

    // Check the contents of the buffer
    if (strcmp(buffer, "Expected file contents") != 0) {
        terminal_writestring("Read data does not match expected contents\n");
        return false; // Contents do not match
    }

    return true; // Read successful
}

static bool test_fs_open(void) {
    file_t* file = fs_open("testfile.txt", 0);
    if (file == NULL) {
        return false; // Open failed
    }

    // Check if the file name is set correctly
    if (strcmp(file->name, "testfile.txt") != 0) {
        heap_free(file);
        return false; // File name does not match
    }

    heap_free(file); // Clean up
    return true; // Open successful
}

static bool test_ramdisk_write(void) {
    const char* test_data = "Test data for RAM disk";
    file_t file = { .name = "testfile.txt", .fs_data = (void*)0 }; // Example file
    int bytes_written = ramdisk_write(&file, test_data, strlen(test_data) + 1);
    if (bytes_written < 0) {
        terminal_writestring("Error writing to RAM disk\n");
        return false; // Write failed
    }

    // Check if the data was written correctly
    char buffer[128];
    memset(buffer, 0, sizeof(buffer)); // Clear the buffer
    ramdisk_read(&file, buffer, sizeof(buffer)); // Read back the data

    // terminal_writestring("Written data: ");
    // terminal_writestring(buffer);
    // terminal_writestring("\n");
    // terminal_writestring("Expected: ");
    // terminal_writestring(test_data);
    if (strcmp(buffer, test_data) != 0) {
        terminal_writestring("Written data does not match expected contents\n");
        return false; // Contents do not match
    }
    // terminal_writestring("Data written successfully to RAM disk\n");
    // Check if the file size was updated
    if (file.size != strlen(test_data) + 1) {
        terminal_writestring("File size does not match expected size\n");
        return false; // Size mismatch
    }
    // terminal_writestring("RAM disk write test passed\n");
    // Clean up
    heap_free(file.fs_data);
    return true; // Write successful
}

static bool test_fs_read(void) {
    file_t* file = fs_open("testfile.txt", 0);
    if (file == NULL) {
        terminal_writestring("Error opening file for reading\n");
        return false; // Open failed
    }

    char buffer[128];
    int bytes_read = fs_read(file, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        terminal_writestring("Error reading from file\n");
        fs_close(file);
        return false; // Read failed
    }

    // Check the contents of the buffer
    if (strcmp(buffer, "Test data for RAM disk") != 0) {
        terminal_writestring("Read data does not match expected contents\n");
        terminal_writestring(buffer);
        fs_close(file);
        return false; // Contents do not match
    }

    fs_close(file);
    return true; // Read successful
}

static bool test_fs_write(void) {
    file_t* file = fs_open("testfile.txt", 0);
    if (file == NULL) {
        terminal_writestring("Error opening file for writing\n");
        return false; // Open failed
    }

    const char* test_data = "Test data for filesystem";
    int bytes_written = fs_write(file, test_data, strlen(test_data) + 1);
    if (bytes_written < 0) {
        terminal_writestring("Error writing to file\n");
        fs_close(file);
        return false; // Write failed
    }

    char buffer[128];
    int bytes_read = fs_read(file, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        terminal_writestring("Error reading from file\n");
        fs_close(file);
        return false; // Read failed
    }

    // Check if the data was written correctly
    if (strcmp(buffer, test_data) != 0) {
        terminal_writestring("Written data does not match expected contents\n");
        terminal_writestring(buffer);
        fs_close(file);
        return false; // Contents do not match
    }

    fs_close(file);
    return true; // Write successful
}

static bool test_fs_close(void) {
    file_t* file = fs_open("testfile.txt", 0);
    if (file == NULL) {
        terminal_writestring("Error opening file for closing\n");
        return false; // Open failed
    }

    int result = fs_close(file);
    if (result < 0) {
        terminal_writestring("Error closing file\n");
        return false; // Close failed
    }

    return true; // Close successful
}

void register_filesystem_tests(void) {
    test_suite_t* suite = create_test_suite("filesystem");

    // Add test cases for filesystem operations
    add_test_case(suite, "ramdisk_read", "Tests reading from the RAM disk", test_ramdisk_read);
    add_test_case(suite, "ramdisk_write", "Tests writing to the RAM disk", test_ramdisk_write);
    add_test_case(suite, "fs_open", "Tests opening a file in the filesystem", test_fs_open);
    add_test_case(suite, "fs_close", "Tests closing a file in the filesystem", test_fs_close);
    add_test_case(suite, "fs_read", "Tests reading from a file in the filesystem", test_fs_read);
    add_test_case(suite, "fs_write", "Tests writing to a file in the filesystem", test_fs_write);
    // add_test_case(suite, "fs_mount", "Tests mounting the filesystem", fs_mount);
    // add_test_case(suite, "fs_format", "Tests formatting the filesystem", fs_format);
    // add_test_case(suite, "fs_init", "Tests initializing the filesystem", fs_init);
    // add_test_case(suite, "fs_create_file", "Tests creating a file in the filesystem", fs_create_file);
    // add_test_case(suite, "fs_delete_file", "Tests deleting a file in the filesystem", fs_delete_file);
    // add_test_case(suite, "fs_list_files", "Tests listing files in the filesystem", fs_list_files);
    // add_test_case(suite, "fs_get_file_info", "Tests getting file information from the filesystem", fs_get_file_info);
    // add_test_case(suite, "fs_rename_file", "Tests renaming a file in the filesystem", fs_rename_file);
    // add_test_case(suite, "fs_copy_file", "Tests copying a file in the filesystem", fs_copy_file);
    // add_test_case(suite, "fs_move_file", "Tests moving a file in the filesystem", fs_move_file);
    // add_test_case(suite, "fs_check_space", "Tests checking available space in the filesystem", fs_check_space);
    // add_test_case(suite, "fs_check_file_exists", "Tests checking if a file exists in the filesystem", fs_check_file_exists);
    // add_test_case(suite, "fs_get_file_size", "Tests getting the size of a file in the filesystem", fs_get_file_size);
    // add_test_case(suite, "fs_get_file_flags", "Tests getting the flags of a file in the filesystem", fs_get_file_flags);
    // add_test_case(suite, "fs_set_file_flags", "Tests setting the flags of a file in the filesystem", fs_set_file_flags);
    // add_test_case(suite, "fs_get_file_creation_time", "Tests getting the creation time of a file in the filesystem", fs_get_file_creation_time);
    // add_test_case(suite, "fs_get_file_modification_time", "Tests getting the modification time of a file in the filesystem", fs_get_file_modification_time);
    // add_test_case(suite, "fs_get_file_access_time", "Tests getting the access time of a file in the filesystem", fs_get_file_access_time);
    // add_test_case(suite, "fs_set_file_creation_time", "Tests setting the creation time of a file in the filesystem", fs_set_file_creation_time);
    // add_test_case(suite, "fs_set_file_modification_time", "Tests setting the modification time of a file in the filesystem", fs_set_file_modification_time);
    // add_test_case(suite, "fs_set_file_access_time", "Tests setting the access time of a file in the filesystem", fs_set_file_access_time);
    // add_test_case(suite, "fs_get_file_permissions", "Tests getting the permissions of a file in the filesystem", fs_get_file_permissions);
    // add_test_case(suite, "fs_set_file_permissions", "Tests setting the permissions of a file in the filesystem", fs_set_file_permissions);
    // add_test_case(suite, "fs_get_file_owner", "Tests getting the owner of a file in the filesystem", fs_get_file_owner);
    // add_test_case(suite, "fs_set_file_owner", "Tests setting the owner of a file in the filesystem", fs_set_file_owner);


    register_test_suite(suite);
}

REGISTER_TEST_SUITE(register_filesystem_tests)
