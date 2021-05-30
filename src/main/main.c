#include "../file_manager/os_API.h"
#include "../file_manager/osfile.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    os_mount("discos/simdiskfilled.bin", 1);
}
