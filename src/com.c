#ifdef __cplusplus
extern  "C" { 
#endif

#include    "oal.h"
#include    "oam.h"
#include    "link.h"
#include    "com.h"

typedef struct queue
{
    u8          rd_idx;
    u8          wr_idx;
    u8          rewind;
    u8          size;
    com_pkt_t   *pkt;
} queue_t;

typedef struct mux_queue
{
    oal_sem_t       can_dequeue;
    queue_t         queues[COM_QUEUE_TYPE_CNT];
    com_pkt_t       packets[COM_QUEUE_SIZE_TOTAL];
} mux_queue_t;

u8  g_com_tx_queue_size[] = 
{
    COM_QUEUE_SIZE_CMD,
    COM_QUEUE_SIZE_LOG,
};

u8  g_com_rx_queue_size[] = 
{
    COM_QUEUE_SIZE_CMD,
    COM_QUEUE_SIZE_LOG,
};

mux_queue_t g_com_tx_queue;
mux_queue_t g_com_rx_queue;

s32 com_parse_pkt(com_pkt_t *pkt);
s32 com_null_pkt(com_pkt_t *pkt);

static inline s32 queue_empty(queue_t *queue)
{
    swk_assert(queue != NULL);

    return   (queue->rd_idx == queue->wr_idx) &&  !queue->rewind;
}

static inline s32 queue_full(queue_t *queue)
{
    swk_assert(queue != NULL);

    return   (queue->rd_idx == queue->wr_idx) &&  queue->rewind;
}

static inline queue_t *find_enqueue(mux_queue_t *mux_queue, u8 pkt_type)
{
    swk_assert(pkt_type < COM_QUEUE_TYPE_CNT);

    queue_t *queue = mux_queue->queues + pkt_type;

    return  queue_full(queue) ? NULL : queue;
}
static inline queue_t *find_dequeue(mux_queue_t *mux_queue)
{
    queue_t    *queue;

    for ( queue = mux_queue->queues; queue < mux_queue->queues + COM_QUEUE_TYPE_CNT; queue++ )
    {
        if (!queue_empty(queue))
        {
            return  queue;
        }
    }

    return NULL;
}

static void queue_enqueue(com_pkt_t *pkt, queue_t *queue)
{
    oal_memcpy((u8 *)(queue->pkt + queue->wr_idx), (u8 *)pkt, pkt->size + COM_HEADER_SIZE);

    queue->wr_idx++;
    if (queue->wr_idx >= queue->size)
    {
        queue->wr_idx = 0;
        swk_assert(queue->rewind == 0);
        queue->rewind = 1;
    }
}

static void queue_dequeue(com_pkt_t *pkt, queue_t *queue)
{
    com_pkt_t  *p;

    p = queue->pkt + queue->rd_idx;

    oal_memcpy((u8 *)pkt, (u8 *)p, p->size + COM_HEADER_SIZE);

    queue->rd_idx++;
    if (queue->rd_idx >= queue->size)
    {
        queue->rd_idx = 0;
        swk_assert(queue->rewind == 1);
        queue->rewind = 0;
    }
}

static s32 mux_enqueue(com_pkt_t *pkt, mux_queue_t *mux_queue)
{
    s32     ret = -1;

    swk_assert(pkt != NULL);

    oal_task_enter_critical();
    {
        queue_t *queue = find_enqueue(mux_queue, pkt->type);
        if(queue)
        {
            queue_enqueue(pkt, queue);
            ret = 0;
        }
        else
            oam_log_warn("queue full\n");
    }
    oal_task_exit_critical();

    if(!ret)
    {
        oal_sem_post(&mux_queue->can_dequeue);
    }

    return ret;
}
static s32 mux_dequeue(com_pkt_t *pkt, mux_queue_t *mux_queue)
{
    s32     ret = -1;

    swk_assert(pkt != NULL);

    oal_task_enter_critical();
    {
        queue_t *queue = find_dequeue(mux_queue);
        if(queue)
        {
            queue_dequeue(pkt, queue);
            ret = 0;
        }
    }
    oal_task_exit_critical();

    return ret;
}
void mux_queue_init(mux_queue_t *mux_queue, u8 size[])
{
    s32     i;
    s32     sum = 0;

    oal_memset((u8 *)mux_queue, 0, sizeof(mux_queue_t));
    oal_sem_init(&mux_queue->can_dequeue, 0, 0);

    for ( i = 0; i < COM_QUEUE_TYPE_CNT; i++ )
    {
        mux_queue->queues[i].pkt   = mux_queue->packets + (size[i] - size[0]);
        mux_queue->queues[i].size  = size[i];
        sum += size[i];
    }

    swk_assert(sum == COM_QUEUE_SIZE_TOTAL);
}

void *com_tx_task(void *arg)
{
    com_pkt_t       pkt;
    s32              size;
    mux_queue_t     *mux_queue = (mux_queue_t *)arg;

    for (;;)
    {
        if(!oal_sem_wait(&mux_queue->can_dequeue))
        {
            while(!link_recv((u8 *)&pkt, &size))
            {
                if(!com_null_pkt(&pkt))
                {
                    com_recv_pkt(&pkt);
                }
                else
                    oam_log_warn("error null data\n");
            }

            while(link_can_send() && !mux_dequeue(&pkt, mux_queue))
            {
                link_send((u8 *)&pkt, pkt.size + COM_HEADER_SIZE);
            }
        }
    }
    return NULL;
}
void *com_rx_task(void *arg)
{
    com_pkt_t       pkt;
    s32              fail;
    mux_queue_t     *mux_queue = (mux_queue_t *)arg;

    for (;;)
    {
        if(!oal_sem_wait(&mux_queue->can_dequeue))
        {
            fail = mux_dequeue(&pkt, mux_queue);
            
            if(!fail) 
            {
                com_parse_pkt(&pkt);
            }
            else
            {
                oam_log_warn("mux_dequeue:failed\n");
            }
        }
    }
    return NULL;
}
s32 com_init(void)
{
    oal_task_handle_t   tx_task_handle;
    oal_task_handle_t   rx_task_handle;

    mux_queue_init(&g_com_tx_queue, g_com_tx_queue_size);
    mux_queue_init(&g_com_rx_queue, g_com_rx_queue_size);

    oal_task_create(com_tx_task, CFG_COM_TX_TASK_NAME, CFG_COM_TX_TASK_STACK_SIZE, 
                        &g_com_tx_queue, CFG_TX_TASK_PRIORITY, &tx_task_handle);

    oal_task_create(com_rx_task, CFG_COM_RX_TASK_NAME, CFG_COM_RX_TASK_STACK_SIZE,
                        &g_com_rx_queue, CFG_RX_TASK_PRIORITY, &rx_task_handle);

    return 0;
}
s32 com_null_pkt(com_pkt_t *pkt)
{
    return  pkt->type < COM_PKT_TYPE_CNT 
        &&  pkt->size < COM_MAX_PAYLOAD_SIZE ? 0: 1;
}
s32 com_pack_pkt(u8 type, u8 size, u8 *dat, com_pkt_t *pkt)
{
    oal_memset(pkt, 0, COM_HEADER_SIZE + COM_MAX_PAYLOAD_SIZE);
    pkt->type = type;
    pkt->size = size;
    oal_memcpy(pkt->dat, dat, size);

    return  0;
}
s32 com_send_pkt(com_pkt_t *pkt)
{
    oam_log_dbg("crtp_send_pkt, size=%d\n", pkt->size);
    return mux_enqueue(pkt, &g_com_tx_queue);
}
s32 com_recv_pkt(com_pkt_t *pkt)
{
    oam_log_dbg("crtp_recv_pkt, size=%d\n", pkt->size);
    return mux_enqueue(pkt, &g_com_rx_queue);
}
s32 com_parse_pkt(com_pkt_t *pkt)
{
    oam_log_dbg("parse pkt:type=%d size=%d dat=%d\n", 
            pkt->type, pkt->size, *(int *)pkt->dat);
    return  0;
}
#ifdef __cplusplus
}
#endif
