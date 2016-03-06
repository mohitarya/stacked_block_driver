#include <linux/fs.h>

int reg_blkdev(unsigned int major_no, const char *name)
{
    return register_blkdev(major_no, name);
}
