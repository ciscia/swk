#ifndef    __LINK_H__
#define    __LINK_H__

#include    "oal.h"

#ifdef __cplusplus
extern  "C" { 
#endif

extern  s32 link_send(u8 *dat, s32  size);
extern  s32 link_recv(u8 *dat, s32 *size);
extern  s32 link_can_send(void);

#ifdef __cplusplus
}
#endif
#endif
