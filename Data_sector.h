#include "Boot_sector.h"

bool set_Data_sector_0_1(HANDLE descriptor, unsigned char* name) {

    struct Data_sector_0 {
        unsigned char device_name[11] = { 0x4B, 0x49, 0x4E, 0x47, 0x53, 0x54, 0x4F, 0x4E, 0x20, 0x20, 0x20 };
        unsigned char info[117] = {
                                                                             0x08, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x83,    0xAC, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x42, 0x20, 0x00, 0x49, 0x00, 0x6E, 0x00, 0x66,    0x00, 0x6F, 0x00, 0x0F, 0x00, 0x72, 0x72, 0x00,
        0x6D, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00,    0x6F, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00,
        0x01, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74,    0x00, 0x65, 0x00, 0x0F, 0x00, 0x72, 0x6D, 0x00,
        0x20, 0x00, 0x56, 0x00, 0x6F, 0x00, 0x6C, 0x00,    0x75, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x65, 0x00,
        0x53, 0x59, 0x53, 0x54, 0x45, 0x4D, 0x7E, 0x31,    0x20, 0x20, 0x20, 0x16, 0x00, 0x1B, 0x5C, 0x83,
        0xAC, 0x50, 0xAC, 0x50, 0x00, 0x00, 0x5D, 0x83,    0xAC, 0x50, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00
        };
        unsigned char zero[384]={ 0 };
    }default_data_0;

    struct Data_sector_1 {
        unsigned char info[256] = {
        0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,    0x20, 0x20, 0x20, 0x10, 0x00, 0x1B, 0x5C, 0x83,
        0xAC, 0x50, 0xAC, 0x50, 0x00, 0x00, 0x5D, 0x83,    0xAC, 0x50, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x2E, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,    0x20, 0x20, 0x20, 0x10, 0x00, 0x1B, 0x5C, 0x83,
        0xAC, 0x50, 0xAC, 0x50, 0x00, 0x00, 0x5D, 0x83,    0xAC, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x42, 0x74, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,    0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0xCE, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x01, 0x57, 0x00, 0x50, 0x00, 0x53, 0x00, 0x65,    0x00, 0x74, 0x00, 0x0F, 0x00, 0xCE, 0x74, 0x00,
        0x69, 0x00, 0x6E, 0x00, 0x67, 0x00, 0x73, 0x00,    0x2E, 0x00, 0x00, 0x00, 0x64, 0x00, 0x61, 0x00,
        0x57, 0x50, 0x53, 0x45, 0x54, 0x54, 0x7E, 0x31,    0x44, 0x41, 0x54, 0x20, 0x00, 0x20, 0x5C, 0x83,
        0xAC, 0x50, 0xAC, 0x50, 0x00, 0x00, 0x5D, 0x83,    0xAC, 0x50, 0x04, 0x00, 0x0C, 0x00, 0x00, 0x00,
        0x42, 0x47, 0x00, 0x75, 0x00, 0x69, 0x00, 0x64,    0x00, 0x00, 0x00, 0x0F, 0x00, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x01, 0x49, 0x00, 0x6E, 0x00, 0x64, 0x00, 0x65,    0x00, 0x78, 0x00, 0x0F, 0x00, 0xFF, 0x65, 0x00,
        0x72, 0x00, 0x56, 0x00, 0x6F, 0x00, 0x6C, 0x00,    0x75, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x65, 0x00,
        0x49, 0x4E, 0x44, 0x45, 0x58, 0x45, 0x7E, 0x31,    0x20, 0x20, 0x20, 0x20, 0x00, 0x2B, 0x60, 0x83,
        0xAC, 0x50, 0xAC, 0x50, 0x00, 0x00, 0x61, 0x83,    0xAC, 0x50, 0x05, 0x00, 0x4C, 0x00, 0x00, 0x00
        };
        unsigned char zero[256] = { 0 };
    }default_data_1;


    for (int i = 0; i < 11; i++) {
        default_data_0.device_name[i] = name[i];
    }


    // ������ ���� �������� ��� ��������� � ����

    DeviceIoControl(descriptor, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL);
   
    if (WriteFile(descriptor, &default_data_0, sizeof(struct Data_sector_0), NULL, NULL) == 0) {
        printf("\n Can not write Data_sector_0 in Your device. \n");
        return false;
    }

    DeviceIoControl(descriptor, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL);

    /*DeviceIoControl(descriptor, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL);

    if (WriteFile(descriptor, &default_data_1, sizeof(struct Data_sector_1), NULL, NULL) == 0) {
        printf("\n Can not write Data_sector_1 in Your device. \n");
        return false;
    }

    DeviceIoControl(descriptor, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL);*/
   
    return true;
}
