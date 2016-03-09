#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include "helper_fun.h"

#define DEVICE_MAX_LEN 32

unsigned int major_no = 0;
char device_name[DEVICE_MAX_LEN];
struct gendisk disk;

setup_gendisk(struct gendisk)

int setup_disk()
{
    int minors = 16;
    int ret;
    struct block_device bdev;
    struct gendisk *gendisk;
    //Look for the scsi block dev
    bdev = lookup_bdev("/dev/sdb")
    if (IS_ERR(bdev)){
        ret = PTR_ERR(bdev)
        printk("Lookup Bdev Return %d\n", ret);
        return ERR_PTR(ret);
    }
    gendisk = alloc_disk(minors);
    if (gendisk == NULL){
        printk("Can not allocate gendisk");
        return -1;
    }
    
}

static int __init sdfdrc_init(void)
{
    int ret_val = 0;
    memset(device_name, 0x0, DEVICE_MAX_LEN);
    strncpy(device_name, "mda", sizeof("mda"));
    major_no = reg_blkdev(major_no, device_name);
    if (unlikely(major_no < 0)){
        printk("No major number is available Error: %d", major_no);
    }
    printk("Major No = %d\n", major_no);

    ret_val = setup_device(void);
    return ret_val;
}

static void __exit sdfdrv_exit(void)
{
    unregister_blkdev(major_no, device_name);
    printk("Exit from the sdfdrc\n");
}

module_init(sdfdrc_init);
module_exit(sdfdrv_exit);
MODULE_LICENSE("GPL");
