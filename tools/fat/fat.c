
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t bool;
#define true 1
#define false 0

typedef struct {

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
} __attribute__((packed)) BootSector ;

 typedef struct 
{
   uint8_t name[11];
   uint8_t attributes;
   uint8_t reserved;
   uint8_t createdTimeInTenths;
   uint16_t creation_time; //hour(5) + min(6) + secs(5) = 16
   uint16_t creation_date; //year(7) + month(4)+day(5)  = 16                                                       
   uint16_t last_access;
   uint16_t last_modification_time;
   uint16_t last_modification_date;
   uint16_t first_cluster_for_entry;
   uint32_t size;
} __attribute__((packed)) DirectoryEntry;
 

BootSector g_BootSector;
uint8_t* g_fat  = NULL;
DirectoryEntry g_RootDirectory = NULL;


BOOL ReadBootSector(FILE* disk){

    return fread(&g_BootSector, sizeof(g_BootSector), 1, disk) > 0;
}

BOOL ReadSectors(FILE* disk, uint32_t lba, uint32_t count, void* bufferout){
   
    bool ok = true;
    ok = ok && (fseek(disk, lba *  g_BootSector.bytes_per_sector, SEEK_SET) == 0);
    ok = ok && (fread(bufferout, g_BootSector.bytes_per_sector, count, disk) == count);
    return ok;
}



BOOL ReadFat(FILE* disk){

    g_fat = (uint8_t) malloc(g_BootSector.sectors_per_fat * g_BootSector.bytes_per_sector);
    return ReadSectors(disk, g_BootSector.reserved_sectors, g_BootSector.sectors_per_fat, g_fat); 
}


BOOL readRootDirectory(FILE* disk){

    uint32_t lba = (g_BootSector.reserved_sectors + g_BootSector.sectors_per_fat) * g_BootSector.fat_count;
    uint32_t size = sizeof(DirectoryEntry) * g_BootSector.dir_entries_count;
    uint32_t sectors = (size / g_BootSector.bytes_per_sector);

    if(size % g_BootSector.bytes_per_sector > 0){
        sectors++;
    }

    g_RootDirectory = (g_RootDirectory*) malloc(sectors * g_BootSector.bytes_per_sector);
    return ReadSectors(disk, lba, sectors, g_RootDirectory);

}

DirectoryEntry* FindFile(const char* name){
    for(int i = 0; i < g_BootSector.dir_entries_count; i++){
        if(memcmp(name, g_RootDirectory[i].name, 11) == 0){
            return &g_RootDirectory[i];
        }
        else{
            return NULL;
        }
    }
}

int main(int argc, char** argv){

    

    if(argc < 4){
        printf("Syntax %s <disk image> <filename>\n", argv[0]);
        return -1;
    }


    FILE* disk = fopen(argv[1], "rb");

    if(!disk){
        fprintf(stderr, "Cannot open disk image %s!", argv[1]);
        return -1;
    }

    if(!ReadBootSector(disk)){
        fprintf(stderr, "Could not read boot sector %s!", argv[1]);
        return -2;
    }


    if(!ReadFat(disk)){
        fprintf(stderr, "Could not read FAT %s!", argv[1]);
        free(g_fat);
        return -3;
    }
    
    if(!readRootDirectory(disk)){
        fprintf(stderr, "Could not read root directory %s!", argv[1]);
        free(g_fat);
        free(g_RootDirectory);
        return -4;
    }
    
    DirectoryEntry* FindEntry = FindFile(argv[2]);
    if(!FindEntry(disk)){
        fprintf(stderr, "Could not find file %s!", argv[2]);
        free(g_fat);
        free(g_RootDirectory);
        return -5;
    }

    free(g_fat);
    free(g_RootDirectory);
    return 0;

}