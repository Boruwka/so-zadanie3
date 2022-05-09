#include <stdio.h>
#include "pm.h"
#include "mproc.h" // ten tajemniczy include przyda się nam w zadaniu 3 ;)
#include <limits.h> // w sumie to ten też
#include <errno.h>

int get_parent_idx(int idx)
{
    int res = mproc[idx].mp_parent;

    if (res == idx)
    {
        return -1; // nie ma ojca
    }

    return res;
}

int get_idx_from_pid(pid_t pid)
{
    for (int i = 0; i < NR_PROCS; i++)
    {
	    if ((mproc[i].mp_flags & IN_USE) && mproc[i].mp_pid == pid)
		{
            return i;
        }
    }
    return -1;
}

pid_t get_pid_from_idx(int idx)
{
    pid_t res = mproc[idx].mp_pid;
    return res;
}

int MAX_NR_OF_PROCESSES = 256;

pid_t find_lca(pid_t pid1, pid_t pid2)
{
    int path_end1 = 0;
    int path_end2 = 0; // długość ścieżek pid1 i pid2 do korzenia
    int path1[MAX_NR_OF_PROCESSES];
    int path2[MAX_NR_OF_PROCESSES]; // ścieżki przodków do korzenia pid1 i pid2

    for (int i = 0; i < MAX_NR_OF_PROCESSES; i++)
    {
        path1[i] = 0;
        path2[i] = 0;
    }

    int idx1 = get_idx_from_pid(pid1);
    int idx2 = get_idx_from_pid(pid2);
    path1[0] = idx1;
    path2[0] = idx2;

    if (idx1 == -1 || idx2 == -1)
    {
        errno = EINVAL;
        return -errno;
        // któryś nie jest aktualnie działającym procesem
    }
    
    for (int i = 1; i < MAX_NR_OF_PROCESSES; i++)    
    {
        path1[i] = get_parent_idx(path1[i - 1]);
        path_end1++;
        if (path1[i] == -1)
        {
            break;
        }
    }

    for (int i = 1; i < MAX_NR_OF_PROCESSES; i++)    
    {
        path2[i] = get_parent_idx(path2[i - 1]);
        path_end2++;
        if (path2[i] == -1)
        {
            break;
        }
    }

    pid_t res = -1;
    int path_idx1 = path_end1 - 1;
    int path_idx2 = path_end2 - 1;

    for (int i = 0; i < MAX_NR_OF_PROCESSES; i++)
    {
        if (path1[path_idx1] == path2[path_idx2])
        {
            res = get_pid_from_idx(path1[path_idx1]);
            break;
        }
        if (path_idx1 == 0 || path_idx2 == 0)
        {
            errno = ESRCH;
            return -errno;
        }
        path_idx1--;
        path_idx2--;
    }
    return res;
}

int do_getlcapid(void)
{
    pid_t pid1 = m_in.m_u32.data[0];
    pid_t pid2 = m_in.m_u32.data[1];
    printf("Hello world from pm, pids are %d and %d\n", pid1, pid2);
    mp->mp_reply.m_u32.data[0] = find_lca(pid1, pid2); 
    return 0;
}
