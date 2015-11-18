#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    "oal.h"

#include    "gtest/gtest.h"

extern "C" 
{
    extern int swk_main(int argc, const char *argv[]);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc,argv);
    return  RUN_ALL_TESTS(); 
}

TEST(swk_main, swk_main_00)
{
    u32 ret = swk_main(0, NULL);
    EXPECT_EQ(ret, 0);
}
