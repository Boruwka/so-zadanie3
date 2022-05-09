#include <lib.h>
#include <minix/rs.h>
#include <stdio.h>

int get_pm_endpt(endpoint_t *pt)
{
        return minix_rs_lookup("pm", pt);
}


pid_t getlcapid(pid_t pid1, pid_t pid2)
{
        endpoint_t pm_pt;
        message m;
        m.m_u32.data[0] = pid1;
        m.m_u32.data[1] = pid2;
        printf("hw from funkcja biblioteczna, pidy to %d %d\n", pid1, pid2);
        if (get_pm_endpt(&pm_pt) != 0)
        {
                errno = ENOSYS;
                return -1;
        }
        if (_syscall(pm_pt, PM_GETLCAPID, &m) != 0)
        {
            return -1; // errno się ustawiło w syscallu
        }
        printf("zwrocony pid to %d\n", m.m_u32.data[0]);
        return m.m_u32.data[0];
}
