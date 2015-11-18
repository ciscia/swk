#ifdef __cplusplus
extern  "C" { 
#endif

#include    <stdarg.h>
#include    "oal.h"
#include    "oam.h"

u32 g_oam_log_seq   = 0;
u32 g_oam_log_level = OAM_LOG_LEVEL_DBG;
u32 g_oam_color     = 1;

u32 oam_main_init(void)
{
    return  0;
}

const char *g_oam_color_table[] =
{
    "\033[0;31m[ERR]",  // 31=RED
    "\033[0;33m[WRN]",  // 33=YELLOw
    "\033[0;32m[HPY]",  // 32=GREEN
    "\033[0;30m[DBG]",  // 30=NONE
};
const char *g_oam_nocolor_table[] =
{
    "[ERR]",  
    "[WRN]",  
    "[HPY]",  
    "[DBG]",  
};

void oam_log(u32 level, const char* fmt, ...) 
{
    va_list args;

    va_start(args, fmt);

    if(level > g_oam_log_level)
    {
        return;
    }
    else  if(level > OAM_LOG_LEVEL_CNT)
    {
        level = OAM_LOG_LEVEL_ERR;
    }

    if (g_oam_color)
    {
        printf("%s", g_oam_color_table[level]);   // 1 = RED 2=yello 3=green 4=none
        vprintf(fmt, args);
        printf("\033[m");  // Resets the terminal to default.
    }
    else
    {
        printf("%s", g_oam_nocolor_table[level]); 
        vprintf(fmt, args);
    }

    va_end(args);
}


#ifdef __cplusplus
}
#endif
