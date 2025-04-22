/**
 * @file fat.c
 * @brief FAT12 Filesystem Reader Implementation
 *
 * This implementation provides functionality to read FAT12 filesystems, commonly used
 * in floppy disks and small storage devices. The code includes structures and functions
 * for parsing and reading FAT12 filesystem components.
 *
 * Key Components:
 * - BootSector: Structure containing filesystem metadata and parameters
 * - DirectoryEntry: Structure representing file/directory entries
 * - FAT table handling
 * - Root directory parsing
 *
 * Key Functions:
 * @fn readBootSector - Reads the boot sector from the disk image
 * @fn readSectors - Reads specified sectors from the disk
 * @fn readFat - Reads the File Allocation Table into memory
 * @fn readRootDirectory - Loads the root directory entries
 * @fn findFile - Searches for a file in the root directory
 * @fn readFile - Reads the contents of a file using FAT chain
 *
 * Memory Management:
 * - Dynamically allocates memory for FAT table
 * - Allocates memory for root directory entries
 * - Proper cleanup of allocated resources
 *
 * Limitations:
 * - Supports only FAT12 filesystem format
 * - Maximum volume size of 32MB
 * - Only reads from root directory (no subdirectory support)
 * - Files must be contiguous in the root directory
 *
 * Usage:
 * Program expects two command-line arguments:
 * 1. Path to disk image file
 * 2. Name of file to read from the image
 *
 * @note This implementation uses packed structures to match disk format
 * @warning Assumes little-endian architecture
 * 
 * @author [Niraj Sharma]
 * @date [22-04-25]
 */
        free(g_Fat);
        free(g_RootDirectory);
        return -5;
    }

    uint8_t* buffer = (uint8_t*) malloc(fileEntry->Size + g_BootSector.BytesPerSector);
    if (!readFile(fileEntry, disk, buffer)) {
        fprintf(stderr, "Could not read file %s!\n", argv[2]);
        free(g_Fat);
        free(g_RootDirectory);
        free(buffer);
        return -5;
    }

    for (size_t i = 0; i < fileEntry->Size; i++)
    {
        if (isprint(buffer[i])) fputc(buffer[i], stdout);
        else printf("<%02x>", buffer[i]);
    }
    printf("\n");

    free(buffer);
    free(g_Fat);
    free(g_RootDirectory);
    return 0;
}