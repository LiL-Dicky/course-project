#ifndef BOOT_SECTOR_H
#define BOOT_SECTOR_H

#include <string.h>
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>

struct Boot_Sector {
    unsigned char bootstrap[3];                                  // 0-2
    unsigned char OEM_name[8];                                   // 3-10
    unsigned char bytes_per_sector[2];                           // 11-12      
    unsigned char sectors_per_cluster;                           // 13
    unsigned char number_reserved_sectors[2];                    // 14-15
    unsigned char number_FAT_copies;                             // 16
    unsigned char number_root_entries[2];                        // 17-18
    unsigned char total_number_sectors_NOFAT32[2];               // 19-20
    unsigned char media_descriptor_type;                         // 21
    unsigned char number_of_sectors_per_FAT_NOFAT32[2];          // 22-23; 
    unsigned char number_of_sectors_per_track[2];                // 24-25
    unsigned char number_of_heads[2];                            // 26-27
    unsigned char number_of_hidden_sectors[4];                   // 28-31
    unsigned char total_number_of_sectors[4];                    // 32-35     
    unsigned char number_of_sectors_per_FAT[4];                  // 36-39
    unsigned char mirror_flags[2];                               // 40-41
    unsigned char filesystem_version[2];                         // 42-43
    unsigned char first_cluster_root_directory[4];               // 44-47
    unsigned char filesystem_information_sector[2];              // 48-49
    unsigned char backup_boot_sector_location[2];                // 50-51
    unsigned char reserved[12];                                  // 52-63
    unsigned char locival_drive_number;                          // 64
    unsigned char reserver_for_WindowsNT;                        // 65
    unsigned char extended_signature_0x29;                       // 66
    unsigned char serial_number_partition[4];                    // 67-70
    unsigned char volume_label[11];                              // 71-81
    unsigned char filesystem_type[8];                            // 82-89
    unsigned char bootstrap_data[420];                           // 90-509
    unsigned char signature_55AA[2];                              // 510-511
};

unsigned char* reversed_short_to_char(unsigned short num) {
    unsigned char str[2];
    unsigned char first = (unsigned char)(num);
    unsigned char second = (unsigned char)(num >> 8);


    str[0] = first;
    str[1] = second;
    return str;
}

unsigned char* reversed_int_to_char(unsigned int num) {
    unsigned char str[4];
    unsigned char first = (unsigned char)(num);
    unsigned char second = (unsigned char)(num >> 8);
    unsigned char third = (unsigned char)(num >> 16);
    unsigned char fourth = (unsigned char)(num >> 24);

    str[0] = first;
    str[1] = second;
    str[2] = third;
    str[3] = fourth;

    return str;
}

bool set_default_Boot_sector(HANDLE descriptor, int quantity_all_sectors, int size_table_FAT_sector)
{

   
    struct Boot_Sector default_Boot_Sector {

        { 0xEB, 0x58, 0x90 },                                               // 0-2
        { 'M', 'S', 'D', 'O', 'S','5', '.', '0' },                          // 3-10
        { 0x00, 0x02 },                                                     // 11-12
        { 0x01 },                                                              // 13
        { 0x20, 0x00 },                                                     // 14-15
        { 0x02 },                                                              // 16
        { 0x00 ,0x00 },                                                     // 17-18
        { 0x00 ,0x00 },                                                     // 19-20
        { 0xf8, },                                                          // 21
        { 0x00 ,0x00 },                                                     // 22-23; 
        { 0x0C, 0x00 },                                                     // 24-25
        { 0x02, 0x00 },                                                     // 26-27
        { 0x02 ,0x00,0x00 ,0x00 },                                          // 28-31
        {    /*посчитать*/ },                                               // 32-35
        {     /*посчитать*/ },                                              // 36-39
        { 0x00 ,0x00 },                                                     // 40-41
        { 0x00 ,0x00 },                                                     // 42-43
        { 0x02, 0x00, 0x00 ,0x00 },                                         // 44-47
        { 0x01, 0x00 },                                                     // 48-49
        { 0x06, 0x00 },                                                     // 50-51
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00 },                              // 52-63
        { 0x80 },                                                           // 64
        { 0x00 },                                                           // 65
        { 0x29 },                                                           // 66
        { 0x1F ,0xC2 ,0xFC ,0xEC },                                         // 67-70
        {  },                                                  
        { 0x55, 0xAA },                                                     // 510-511

    };

    unsigned char* temp = reversed_int_to_char(quantity_all_sectors);
    for (int i = 0; i < 4; i++) {
        default_Boot_Sector.total_number_of_sectors[i] = temp[i];
    }
    
    temp = reversed_int_to_char(size_table_FAT_sector);
    for (int i = 0; i < 4; i++) {
        default_Boot_Sector.number_of_sectors_per_FAT[i] = temp[i];
    }
   
    // дальше надо записать эту структуру в файл

    DeviceIoControl(descriptor, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL);

    if (WriteFile(descriptor, &default_Boot_Sector, sizeof(struct Boot_Sector), NULL, NULL) == 0) {
        printf("\n Can not write Boot Sector in Your device. \n");
        return false;
    }

    DeviceIoControl(descriptor, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL);

    return true;
}

const char* device_path = "F:/";

void error_open_device(wchar_t* file_path)                   // нужны
{
    printf("\n \'%s\' : No such directory, or cannot open your device for reading. Please check Your device and try again\n", file_path);
    exit(0);
}

HANDLE open_device(wchar_t* device_path)                     // нужны
{
    
    HANDLE hDevice =
        CreateFileW(device_path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    // path = "\\\\.\\f:"
   

    if (hDevice == INVALID_HANDLE_VALUE) // невозможно открыть устройство
    {
        error_open_device(device_path);

    }
    return hDevice;
}

bool seek_descriptor(HANDLE device_descriptor, unsigned long long offset)
{
    _LARGE_INTEGER integer;
    integer.QuadPart = offset;
    if (SetFilePointerEx(device_descriptor, integer, NULL, FILE_BEGIN) == 0) {
        printf("\n Move's error in file \n");
        return false;
    }

    return true;
}

#endif BOOT_SECTOR_H

