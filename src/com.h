#ifndef    __COM_H__
#define    __COM_H__

#ifdef __cplusplus
extern  "C" { 
#endif

/*
 * Swk Realtime protocol
 */

#include    "oal.h"
#define COM_MAX_PAYLOAD_SIZE   31

enum
{
    COM_PKT_TYPE_CMD = 0,
    COM_PKT_TYPE_LOG,
    COM_PKT_TYPE_CNT,
};

enum
{
    COM_QUEUE_TYPE_CMD = 0,
    COM_QUEUE_TYPE_LOG,
    COM_QUEUE_TYPE_CNT,
};
enum
{
    COM_QUEUE_SIZE_CMD = 5,
    COM_QUEUE_SIZE_LOG = 10,

    COM_QUEUE_SIZE_TOTAL = COM_QUEUE_SIZE_CMD + COM_QUEUE_SIZE_LOG,
};

typedef struct
{
    u8  type    :   3;
    u8  size    :   5;
    u8  dat[COM_MAX_PAYLOAD_SIZE];
} __attribute__((packed)) com_pkt_t;

#define COM_HEADER_SIZE  1

extern s32 com_init(void);
extern s32 com_pack_pkt(u8 type, u8 size, u8 *dat, com_pkt_t *pkt);
extern s32 com_send_pkt(com_pkt_t *pkt);
extern s32 com_recv_pkt(com_pkt_t *pkt);

#ifdef __cplusplus
}
#endif
#endif
