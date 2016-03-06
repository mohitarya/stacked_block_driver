#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include "helper_fun.h"

#define DEVICE_MAX_LEN 32

unsigned int major_no = 0;
char device_name[DEVICE_MAX_LEN];

static int __init sdfdrc_init(void)
{
    memset(device_name, 0x0, DEVICE_MAX_LEN);
    strncpy(device_name, "mda", sizeof("mda"));
    major_no = reg_blkdev(major_no, device_name);
    if (unlikely(major_no < 0)){
        printk("No major number is available Error: %d", major_no);
    }
    printk("Major No = %d\n", major_no);
    return 0;
}

static void __exit sdfdrv_exit(void)
{
    unregister_blkdev(major_no, device_name);
    printk("Exit from the sdfdrc\n");
}

module_init(sdfdrc_init);
module_exit(sdfdrv_exit);
MODULE_LICENSE("GPL");
