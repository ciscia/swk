#ifdef __cplusplus
extern  "C" { 
#endif

#ifndef    __OAM_H__
#define    __OAM_H__
    
extern  u32 oam_main_init(void);
extern  u32 g_oam_log_seq;
extern  u32 g_oam_log_level;
extern  u32 g_oam_color;

extern void oam_log(u32 level, const char* fmt, ...);

enum
{
    OAM_LOG_LEVEL_ERR,
    OAM_LOG_LEVEL_WARN,
    OAM_LOG_LEVEL_HPY,
    OAM_LOG_LEVEL_DBG,

    OAM_LOG_LEVEL_CNT
};

#define oam_log_err(fmt, arg...)    oam_log(OAM_LOG_LEVEL_ERR, fmt, ##arg)
#define oam_log_warn(fmt, arg...)   oam_log(OAM_LOG_LEVEL_WARN, fmt, ##arg)
#define oam_log_hpy(fmt, arg...)    oam_log(OAM_LOG_LEVEL_HPY, fmt, ##arg)
#define oam_log_dbg(fmt, arg...)    oam_log(OAM_LOG_LEVEL_DBG, fmt, ##arg)

#endif

#ifdef __cplusplus
}
#endif
