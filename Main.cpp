#define _CRT_SECURE_NO_WARNINGS
#include <fcntl.h>	/* open() and O_XXX flags */
#include <sys/stat.h>	/* S_IXXX flags */
#include <sys/types.h>	/* mode_t */
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <windows.h>
#include <winioctl.h>
#include <iostream>

#include "FSInfo.h"
#include "Data_sector.h"
#include "FAT_table.h"
#include <string>

const int NotUsed = system("color f4");

wchar_t* getTheLetterOfTheDevice() {

    printf("Input the letter of the desired volume in a format of 'C:' - ");
    char input_string[5];
    scanf("%s", input_string);
    char symbols[25] = "\\\\.\\";
    strcat(symbols, input_string);
    size_t newsize = strlen(symbols) + 1;
    wchar_t* wcstring = new wchar_t[newsize];
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wcstring, newsize, symbols, _TRUNCATE);
    return wcstring;
    
}


unsigned char* new_device_name() {
       
    bool chek;

    char buffer[100] = { };
    unsigned char* name=(unsigned char*)malloc(sizeof(unsigned char)*11);

    do {
        chek = true;
        printf("Enter new device's name ( max 11 simbols)\n");
        scanf("%s", buffer);
        if (buffer[11] != 0) {
            printf("You enter more 11 simbols\n");
            for (int i = 0; i < 12; i++) {
                buffer[i] = 0;

            }
            chek = false;
            continue;
        }
        for (int i = 0; i < 11; i++) {
            if ((buffer[i]== '.') || (buffer[i] == ';' ) || (buffer[i] == ':') || (buffer[i] == '\"') || 
                (buffer[i] == '/') || (buffer[i] == ',') || (buffer[i] == '<') || (buffer[i] == '>') || 
                (buffer[i] == '?') || (buffer[i] == '!') || (buffer[i] == '*') || (buffer[i] == '\\') || 
                (buffer[i] == '\'') || (buffer[i] == '[') || (buffer[i] == ']') || (buffer[i] == '{') || (buffer[i] == '}')) {
                chek = false;
            }
            
        }
    } while (!chek);

    for (int i = 0; i < 11; i++) {
        name[i] = buffer[i];
    }
    
    return name;
}

int main(int argc, char const* argv[]) {

    puts("________________ Dicky formatting ________________\nPress Enter to start");
    getchar();
    
    ULARGE_INTEGER free1;

    wchar_t* device_path = getTheLetterOfTheDevice();
    unsigned char* new_name = new_device_name();

    GetDiskFreeSpaceEx("E:\\",  NULL, &free1, NULL);
    std::cout<<"Size: "<<free1.QuadPart<<std::endl;

    int device_size = free1.QuadPart;
    int quantity_all_sectors = device_size / 512;
    printf("\n quantity_all_sectors = %d  \n", quantity_all_sectors);
    system("pause");
   
    HANDLE current_device_descriptor = open_device(device_path);
    
    DWORD dwBytes;
    BYTE pzero[65536] = { 0 };
    STORAGE_PROPERTY_QUERY Query = { StorageAccessAlignmentProperty, PropertyStandardQuery };
    STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR Alignment = {};
    /*if (DeviceIoControl(
        current_device_descriptor,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &Query,
        sizeof Query,
        &Alignment,
        sizeof Alignment,
        &dwBytes,
        NULL
    ))
    {
        if (Alignment.BytesOffsetForSectorAlignment)
            puts("Warning This disk has 'alignment offset'");
    }*/
   

    SetFilePointer(current_device_descriptor, 0, NULL, FILE_BEGIN);

    // �������� ������ ���� � ������������ ������ �������
    puts("Start formating\n");

    DeviceIoControl(current_device_descriptor, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL);
    
    bool flag = true;

    int temp_size = 0;

    do {    
        flag = WriteFile(current_device_descriptor, pzero, 65536, &dwBytes, NULL);
        
        if (flag == false) {
            break;
        }
        
        temp_size = temp_size + 128;

        if (temp_size > quantity_all_sectors)
            break;
        
    } while (flag);

    DeviceIoControl(current_device_descriptor, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL);

    printf("\n %d - temp_size \n", temp_size);
    system("pause");

    // ����� ��������� � ������
    SetFilePointer(current_device_descriptor, 0, NULL, FILE_BEGIN);
   

    DeviceIoControl(current_device_descriptor, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL);

    // �������� �� �������
    if ((WriteFile(current_device_descriptor, pzero, 65536, &dwBytes, NULL)) == 0) {
        printf("\n Disconnection with device \n");
        exit(0);
    }

    DeviceIoControl(current_device_descriptor, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL);

    // ����� ��������� � ������
    SetFilePointer(current_device_descriptor, 0, NULL, FILE_BEGIN);


    int size_table_FAT_bute = quantity_all_sectors * 4;

    int  size_table_FAT_sector;

    if ((size_table_FAT_bute % 512) == 0) {
        size_table_FAT_sector = size_table_FAT_bute / 512;
    }
    else {
        size_table_FAT_sector = (size_table_FAT_bute / 512) + 1;
    }

    int begin_first_FAT = 32;

    int begin_second_FAT = begin_first_FAT + size_table_FAT_sector;

    int begin_date = begin_second_FAT + size_table_FAT_sector;      // ��� ��������� +2 ������ ��� ��������� ���������� �� 2 �������

    int quantity_empty_sectors = quantity_all_sectors - begin_date;

    
    int quantity_not_empty_sectors = begin_date+2;

    // ���������� ��������� Boot
    if (set_default_Boot_sector(current_device_descriptor, quantity_all_sectors, size_table_FAT_sector) == false) {
        printf("\n Error with setting Boot sector  \n");
        exit(0);
    }

    // ���������� ��������� FSInfo
    if (set_default_FSInfo(current_device_descriptor, quantity_empty_sectors) == false) {
        printf("\n Error with setting FSInfo sector  \n");
        exit(0);
    }

    // ����� ��������� �� 6 �������� �� ������ �����
    if (seek_descriptor(current_device_descriptor, 6 * 512) == false) {
        printf("\n Error in 1 seek \n");
        exit(0);
    }

    // ���������� ����� ��������� Boot
    if (set_default_Boot_sector(current_device_descriptor, quantity_all_sectors, size_table_FAT_sector) == false) {
        printf("\n Error with setting backup Boot sector  \n");
        exit(0);
    }

    // ���������� ����� ��������� FSInfo
    if (set_default_FSInfo(current_device_descriptor, quantity_empty_sectors) == false) {
        printf("\n Error with setting backup FSInfo sector  \n");
        exit(0);
    }

    // ����� ��������� �� 32 ������� �� ������ �����  (� ������ FAT1)
    if (seek_descriptor(current_device_descriptor, begin_first_FAT * 512) == false) {
        printf("\n Error in 2 seek \n");
        exit(0);
    }

    // ���������� FAT1
    if (set_default_FAT_table(current_device_descriptor) == false) {
        printf("\n Error with setting FAT1 sector  \n");
        exit(0);
    }

    // ����� ��������� �� ������ ����� �� 32 ������� + size_table_FAT_sector  (� ������ FAT2)
    if (seek_descriptor(current_device_descriptor, begin_second_FAT * 512) == false) {
        printf("\n Error in 3 seek \n");
        exit(0);
    }

    // ���������� FAT2
    if (set_default_FAT_table(current_device_descriptor) == false) {
        printf("\n Error with setting FAT2 sector  \n");
        exit(0);
    }

    // ����� ��������� �� ������ ����� �� ������� begin_date  (� ������ Data)
    if (seek_descriptor(current_device_descriptor, begin_date * 512) == false) {
        printf("\n Error in 4 seek \n");
        exit(0);
    }
    
    // ���������� Data sectors
    if (set_Data_sector_0_1(current_device_descriptor, new_name) == false) {
        printf("\n Error with Data sector  \n");
        exit(0);
    }
    
    printf("\n Formatting was successfully!! \n");

    free(new_name);

    return 0;
}
