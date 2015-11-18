#ifdef __cplusplus
extern  "C" { 
#endif

#ifndef    __OAL_H__
#define    __OAL_H__

#include    <stdio.h>
#include    <string.h>
#include    <assert.h>
#include    <pthread.h>
#include    <semaphore.h>

typedef unsigned char   u8;
typedef signed   char   s8;

typedef unsigned short  u16;
typedef signed   short  s16;


typedef unsigned int    u32;
typedef signed   int    s32;


typedef pthread_t    oal_task_handle_t;
typedef void        *(oal_task_code_t)(void *arg);

#define oal_task_create(_code, _name, _stack_size, _arg, _priority, _task) \
        pthread_create(_task, NULL, _code, _arg)

#define oal_task_join(task, arg) pthread_join(task, arg)

extern  void    oal_task_delete(oal_task_handle_t   task);
extern  void    oal_task_suspend(oal_task_handle_t  task);
extern  void    oal_task_resume(oal_task_handle_t   task);
extern  u32     oal_get_tick_count(void);
extern  u32     oal_get_tick_count_isr(void);
extern  void    oal_start_os(void);

extern  u32     oal_main_init(void);

typedef sem_t   oal_sem_t;

#define oal_sem_init    sem_init
#define oal_sem_wait    sem_wait
#define oal_sem_post    sem_post

#define swk_assert  assert
#define oal_memcpy  memcpy
#define oal_memset  memset


#define CFG_SRTP_TX_TASK_NAME       "srtp_tx"
#define CFG_SRTP_TX_TASK_STACK_SIZE  256
#define CFG_SRTP_TX_TASK_PRIORITY    2

#define CFG_SRTP_RX_TASK_NAME       "srtp_rx"
#define CFG_SRTP_RX_TASK_STACK_SIZE  256
#define CFG_SRTP_RX_TASK_PRIORITY    1

#define CFG_LINK_RX_TASK_NAME       "link_rx"
#define CFG_LINK_RX_TASK_STACK_SIZE  256
#define CFG_LINK_RX_TASK_PRIORITY    0

#define oal_task_enter_critical()
#define oal_task_exit_critical()

#endif
#ifdef __cplusplus
}
#endif
