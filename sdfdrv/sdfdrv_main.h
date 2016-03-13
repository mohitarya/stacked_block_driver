#ifndef __SDFDRV_MAIN__
#define __SDFDRV_MAIN__

void make_req_fun(struct request_queue *, struct bio *);
int setup_device(void);

#endif
