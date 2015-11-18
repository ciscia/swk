#ifdef __cplusplus
extern  "C" { 
#endif

#include    "oal.h"

void    oal_task_delete(oal_task_handle_t   task)
{
}
void    oal_task_suspend(oal_task_handle_t  task)
{
}
void    oal_task_resume(oal_task_handle_t   task)
{
}
u32     oal_get_tick_count(void)
{
    return  0;
}
u32     oal_get_tick_count_isr(void)
{
    return  0;
}
void    oal_start_os(void)
{
}

u32 oal_main_init(void)
{
    return  0;
}

#ifdef __cplusplus
}
#endif
