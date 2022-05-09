#include <stdio.h>
#include "pm.h"
#include "mproc.h" // ten tajemniczy include przyda się nam w zadaniu 3 ;)
#include <limits.h> // w sumie to ten też

int do_getlcapid(void)
{
    pid_t pid1 = m_in.m_u32.data[0];
    pid_t pid2 = m_in.m_u32.data[1];
    printf("Hello world from pm, pids are %d and %d\n", pid1, pid2);
    mp->mp_reply.m_u32.data[0] = pid1 + pid2; // na razie 
    return 0;
}
