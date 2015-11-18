#ifdef __cplusplus
extern  "C" { 
#endif

#include    "oal.h"
#include    "oam.h"
#include    "com.h"

void link_isr(void)
{
    /*sem_post(&sem_dat_ready);*/
}

int fake_recv = 3000-1;
u32 link_recv(u8 *dat, s32  *size)
{
    com_pkt_t  p;
    fake_recv++;
    if(fake_recv%3 == 0)
    {
        com_pack_pkt(0, 4, (u8 *)&fake_recv, &p);
        oal_memcpy(dat, &p, sizeof(p));
        *size = 4;
        oam_log_dbg("link recv type=%d size=%d dat=%d\n", 
                p.type, p.size, *(int *)p.dat);
        fflush(stdout);
        return  0;
    }
    else
    {
        return  1;
    }
}
s32 link_can_send(void)
{
    return  1;
}
u32 link_send(u8 *dat, u8  size)
{
    com_pkt_t *p = (com_pkt_t *)dat;
    oam_log_dbg("link send type=%d size=%d dat=%d\n", 
            p->type, p->size, *(int *)p->dat);
    fflush(stdout);
    return 0;
}
u32 link_init(void)
{
    return  0;
}

#ifdef __cplusplus
}
#endif
