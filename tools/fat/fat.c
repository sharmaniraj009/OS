/**
 * @file fat.c
 * @brief FAT12 filesystem reader implementation
 *
 * This file implements functionality to read FAT12 filesystem structures and data.
 * It includes definitions for the boot sector structure and functions to read from disk.
 *
 * @struct BootSector
 * @brief Represents the structure of a FAT12 boot sector
 * Contains fields for filesystem parameters and boot record information
 *
 * @var BootSector::BootJumpInstruction
 * Boot jump instructions, first 3 bytes of the boot sector
 * @var BootSector::oem
 * OEM identifier (typically "MSWIN4.1")
 * @var BootSector::bytes_per_sector
 * Number of bytes per sector (typically 512)
 * @var BootSector::sectors_per_cluster
 * Number of sectors per cluster
 * @var BootSector::reserved_sectors
 * Number of reserved sectors
 * @var BootSector::fat_count
 * Number of FAT copies
 * @var BootSector::dir_entries_count
 * Maximum number of root directory entries
 * @var BootSector::total_sectors
 * Total number of sectors in the filesystem
 * @var BootSector::media_descriptor_type
 * Media descriptor type
 * @var BootSector::sectors_per_fat
 * Number of sectors per FAT
 * @var BootSector::sectors_per_track
 * Number of sectors per track
 * @var BootSector::heads
 * Number of heads
 * @var BootSector::hidden_sectors
 * Number of hidden sectors
 * @var BootSector::large_sector_count
 * Total number of sectors (if total_sectors is 0)
 *
 * @function ReadBootSector
 * @brief Reads the boot sector from the disk image
 * @param disk Pointer to the opened disk image file
 * @return BOOL TRUE if successful, FALSE otherwise
 *
 * @function ReadSector
 * @brief Reads specified sectors from the disk image
 * @param disk Pointer to the opened disk image file
 * @param lba Logical Block Address of the sector to read
 * @param count Number of sectors to read
 * @param bufferout Buffer to store the read data
 * @return BOOL TRUE if successful, FALSE otherwise
 *
 * @function main
 * @brief Entry point of the program
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return int 0 on success, negative values on error
 */
#include <stdio.h>
#include <windows.h>
#include <stdint.h>

typedef uint8_t bool;
#define true 1
#define false 0

typedef struct  {

    uint8_t BootJumpInstruction[3];                    // "MSWIN4.1"
    uint8_t oem[8];                    // "MSWIN4.1"
    uint16_t bytes_per_sector;      // 512
    uint8_t sectors_per_cluster;    // 1
    uint16_t reserved_sectors;      // 1
    uint8_t fat_count;             // 2
    uint16_t dir_entries_count;     // 0xE0
    uint16_t total_sectors;         // 2880
    uint8_t media_descriptor_type;  // 0xF0
    uint16_t sectors_per_fat;       // 9
    uint16_t sectors_per_track;     // 18
    uint16_t heads;                 // 2
    uint32_t hidden_sectors;        // 0
    uint32_t large_sector_count;    // 0

    // Extended boot record
    uint8_t drive_number;           // 0x00
    uint8_t reserved;               // 0
    uint8_t signature;              // 0x29
    uint8_t volume_id[4];          // {0x12, 0x34, 0x56, 0x78}
    uint8_t volume_label[11];         // "Niraj OS   "
    uint8_t system_id[8];             // "FAT12   "
} BootSector;

// Initialize the boot sector
// struct BootSector bs = {
//     .BootJumpInstruction = 
//     .oem = "MSWIN4.1",
//     .bytes_per_sector = 512,
//     .sectors_per_cluster = 1,
//     .reserved_sectors = 1,
//     .fat_count = 2,
//     .dir_entries_count = 0xE0,
//     .total_sectors = 2880,
//     .media_descriptor_type = 0xF0,
//     .sectors_per_fat = 9,
//     .sectors_per_track = 18,
//     .heads = 2,
//     .hidden_sectors = 0,
//     .large_sector_count = 0,
//     .drive_number = 0,
//     .reserved = 0,
//     .signature = 0x29,
//     .volume_id = {0x12, 0x34, 0x56, 0x78},
//     .volume_label = "Niraj OS   ",
//     .system_id = "FAT12   "
// };


BootSector g_BootSector;
uint8_t* g_fat  = NULL


BOOL ReadBootSector(FILE* disk){

    // g_BootSector.bytes_per_sector = 512;
    return fread(&g_BootSector, sizeof(g_BootSector), 1, disk) > 0;

}


BOOL ReadSector(FILE* disk, uint32_t lba, uint32_t count, void* bufferout){
    bool ok = true;


     // Seek to the correct byte offset in the file
    ok = ok && (fseek(disk, lba *  g_BootSector.bytes_per_sector, SEEK_SET) == 0);

    // Read 'count' sectors of data into bufferout
    ok = ok && (fread(bufferout, g_BootSector.bytes_per_sector, count, disk) == count);
    return ok;

// Calculates byte offset for sector lba.
// Moves file pointer there.
// Reads count sectors into bufferout.
// Returns true if both operations succeeded.
}



BOOL ReadFat(FILE* disk){

    g_fat = (uint8_t) malloc(g_BootSector.sectors_per_fat * g_BootSector.bytes_per_sector);
    return readSector(disk, g_BootSector.reserved_sectors, g_BootSector.Ssectors_per_fat. g_fat); 

}

int main(int argc, char** argv){

    

    if(argc < 4){
        printf("Syntax %s <disk image> <filename>\n", argv[0]);
        return -1;
    }


    FILE* disk = fopen(argv[1], "rb");

    if(!disk){
        fprints(stderr, "Cannot open disk image %s!", argv[1]);
        return -1;
    }

    if(!ReadBootSector){
        fprints(stderr, "Could not read boot sector %s!", argv[1]);
        return -2;
    }


    if(!ReadFat){
        fprints(stderr, "Could not read FAT %s!", argv[1]);
        free(g_fat);
        return -3;
    }

}