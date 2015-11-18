#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <gtest/gtest.h>

#include    "oal.h"
#include    "com.h"

TEST(com_send, com_send_00)
{
    u32 i;
    u32 ret;
    com_pkt_t       p;

    com_init();


    for ( i = 0; i < COM_QUEUE_SIZE_CMD; i++ )
    {
        com_pack_pkt(COM_QUEUE_TYPE_CMD, 4, (u8 *)&i, &p);
        ret = com_send_pkt(&p);
        EXPECT_EQ(ret, 0);
    }

    sleep(1);

    for ( i = 100; i < 100 + COM_QUEUE_SIZE_CMD; i++ )
    {
        com_pack_pkt(COM_PKT_TYPE_CMD, 4, (u8 *)&i, &p);
        ret = com_send_pkt(&p);
        EXPECT_EQ(ret, 0);
    }
}
