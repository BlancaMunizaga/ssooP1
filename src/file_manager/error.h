#pragma once

typedef enum os_error {
    OS_SEGFAULT,
    OS_FILENOTFOUND,
    OS_DISKNOTFOUND,
    OS_PARTITIONOTFOUND,
    OS_SIZE,
    OS_MBTFULL,
    OS_INVALIDBITMAP,
    OS_INVALIDID,
    OS_FILEEXISTS,
    OS_WRONGMODE,

} OS_ERROR;

void os_strerror(OS_ERROR err);