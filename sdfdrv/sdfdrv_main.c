#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/export.h>
#include <linux/bio.h>
#include <linux/list.h>
#include "helper_fun.h"
#include "sdfdrv_main.h"

unsigned int major_no = 0;
char device_name[DISK_NAME_LEN];

static struct block_device_operations blk_fops = {
    .owner = THIS_MODULE,
};

static void end_io(struct bio *bio, int error)
{
    bio_put(bio);
}

struct bio *prepare_bio(struct bio *bio)
{
    struct bio *bio_clone;
    //clone bio
    bio_clone = bio_clone(bio, GFP_KERNEL);
    if(bio_clone == NULL){
        printk("Can not clone the bio\n");
        retutn NULL;
    }
    bio_clone->bi_bdev = mda->bd_dev;
    bio_clone->bi_end_io = end_io;
    return bio_clone;
}

void make_req_fun(struct request_queue *q, struct bio *bio)
{
    struct bio *bio_clone;
    bio_clone = prepare_bio(bio);
    
}

struct mda_dev {
    struct request_queue *bd_queue;
    struct block_device *bd_bdev;
    struct gendisk *bd_gendisk;
    } *mda = NULL;

int setup_device(void)
{
    int minors = 1;
    int ret;
    //Allocate memory to the private data structure mda_dev
    mda = (struct mda_dev *) kmalloc(sizeof(struct mda_dev), GFP_KERNEL);
    
    //Look for the scsi block dev
    mda->bd_bdev = lookup_bdev("/dev/sdb");
    if (IS_ERR(mda->bd_bdev)){
        ret = PTR_ERR(mda->bd_bdev);
        printk("Lookup Bdev Return %d\n", ret);
        goto out;
    }

    //Allocate gendisk
    mda->bd_gendisk = alloc_disk(minors);
    if (mda->bd_gendisk == NULL){
        printk("Can not allocate gendisk");
        ret = -1;
        goto out;
    }
    //Filling up gendisk with appropriate data
    mda->bd_gendisk->major = major_no;
    mda->bd_gendisk->first_minor = 1;
    mda->bd_gendisk->minors = minors;
    strncpy(mda->bd_gendisk->disk_name, device_name, DISK_NAME_LEN);
    mda->bd_gendisk->fops = &blk_fops;
    mda->bd_gendisk->queue = blk_alloc_queue(GFP_KERNEL);
    if(mda->bd_gendisk->queue == NULL){
        printk("blk Queue Allocation fail\n");
        ret = -1;
        goto out;
    }
    blk_queue_make_request(mda->bd_gendisk->queue, make_req_fun);
    mda->bd_gendisk->private_data = mda;
    set_capacity(mda->bd_gendisk, get_capacity(mda->bd_bdev->bd_disk));
    add_disk(mda->bd_gendisk);
    return 0; 
out:
    if(mda) {
        if(mda->bd_bdev){
            mda->bd_bdev = NULL;
        }
        if(mda->bd_gendisk){
            del_gendisk(mda->bd_gendisk);
            put_disk(mda->bd_gendisk);
            mda->bd_gendisk = NULL;
        }
        if(mda->bd_queue){
            blk_cleanup_queue(mda->bd_queue);
            mda->bd_queue = NULL;
        }
        kfree(mda);
    }
    return -1;
}

static int __init sdfdrc_init(void)
{
    int ret_val = 0;
    memset(device_name, 0x0, DISK_NAME_LEN);
    strncpy(device_name, "mda", sizeof("mda"));
    major_no = reg_blkdev(major_no, device_name);
    if (unlikely(major_no < 0)){
        printk("No major number is available Error: %d", major_no);
    }
    printk("Major No = %d\n", major_no);

    ret_val = setup_device();
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
