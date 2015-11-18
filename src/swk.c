#ifdef __cplusplus
extern  "C" { 
#endif

#include    "oal.h"
#include    "oam.h"
#include    "hal.h"
#include    "alg.h"
#include    "ext.h"
#include    "com.h"

#ifdef PRE_UST
int swk_main(int argc, const char *argv[])
#else
int main(int argc, const char *argv[])
#endif
{
    oal_main_init();
    oam_main_init();
    hal_main_init();
    alg_main_init();
    
    oal_start_os();
    oam_log_err("hello=10\n");
    oam_log_warn("hello=10\n");
    oam_log_hpy("hello=10\n");
    oam_log_dbg("hello=10\n");

    return 0;
}


#ifdef __cplusplus
}
#endif
