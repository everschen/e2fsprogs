
#include <stdio.h>
#include "config.h"

#include "ext2_fs.h"
#include "ext2fs.h"

#include <ctype.h>

#include <execinfo.h>


void fprint_hex_dump(FILE *fp,
                     const char *prefix,
                     const void *buf,
                     size_t len,
                     size_t rowsize,   /* 一行多少字节，内核常用 16 */
                     size_t groupsize) /* 1 / 2 / 4 */
{
    const unsigned char *data = buf;
    size_t i, j;

    for (i = 0; i < len; i += rowsize) {
        /* offset */
        fprintf(fp, "%s%08zx: ", prefix, i);

        /* hex part */
        for (j = 0; j < rowsize; j += groupsize) {
            if (i + j < len) {
                if (groupsize == 1)
                    fprintf(fp, "%02x ",
                            data[i + j]);
                else if (groupsize == 2 && i + j + 1 < len)
                    fprintf(fp, "%04x ",
                            *(uint16_t *)(data + i + j));
                else if (groupsize == 4 && i + j + 3 < len)
                    fprintf(fp, "%08x ",
                            *(uint32_t *)(data + i + j));
            } else {
                fprintf(fp, "%*s", groupsize * 2 + 1, "");
            }
        }

        /* ascii part */
        fprintf(fp, " |");
        for (j = 0; j < rowsize && i + j < len; j++) {
            unsigned char c = data[i + j];
            fprintf(fp, "%c", isprint(c) ? c : '.');
        }
        fprintf(fp, "|\n");
    }
}

void dump_stack_user(void)
{
    void *buffer[20];
    int nptrs;

    nptrs = backtrace(buffer, 20);
    backtrace_symbols_fd(buffer, nptrs, 2);
}
