#include <cstdio>
#include <cstdlib>
#include <unistd.h>


#define die(...) { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); }

struct cpu_info {
    long unsigned utime, ntime, stime, itime;
    long unsigned iowtime, irqtime, sirqtime;
    long unsigned stealtime, guesttime;
};

struct mem_info {
    long unsigned mem_total, mem_free, buffers, cached;
};

int ReadCpuInfo(struct cpu_info *p_cpu_info)
{
    FILE *fp = NULL;
    fp = fopen("/proc/stat", "r");
    if(!fp)
    {
        die("Could not open /proc/stat.\n");
    }
    fscanf(fp, "cpu %lu %lu %lu %lu %lu %lu %lu %lu %lu", &p_cpu_info->utime, &p_cpu_info->ntime, 
            &p_cpu_info->stime, &p_cpu_info->itime, &p_cpu_info->iowtime, 
            &p_cpu_info->irqtime, &p_cpu_info->sirqtime, &p_cpu_info->stealtime, &p_cpu_info->guesttime);
    fclose(fp);
    
    return 0;
}

int ReadMemInfo(struct mem_info *p_mem_info)
{
    FILE *fp = NULL;
    fp = fopen("/proc/meminfo", "r");
    if(!fp)
    {
        die("Could not open /proc/meminfo.\n");
    }
    fscanf(fp, "%*s%lu%*s\n%*s%lu%*s\n%*s%lu%*s\n%*s%lu%*s\n", &p_mem_info->mem_total, 
            &p_mem_info->mem_free, &p_mem_info->buffers, &p_mem_info->cached);
    fclose(fp);
    
    return 0;
}

int main(void)
{
    struct cpu_info cpu_info_old, cpu_info_new;

    memset(&cpu_info_old, 0, sizeof(cpu_info_old));
    memset(&cpu_info_new, 0, sizeof(cpu_info_new));
    
    ReadCpuInfo(&cpu_info_old);

    sleep(2);

    ReadCpuInfo(&cpu_info_new);

    long unsigned total_delta_time = (cpu_info_new.utime + cpu_info_new.ntime + cpu_info_new.stime + 
            cpu_info_new.itime + cpu_info_new.iowtime + cpu_info_new.irqtime + cpu_info_new.sirqtime + 
            cpu_info_new.stealtime + cpu_info_new.guesttime) - 
            (cpu_info_old.utime + cpu_info_old.ntime + cpu_info_old.stime + cpu_info_old.itime + 
             cpu_info_old.iowtime + cpu_info_old.irqtime + cpu_info_old.sirqtime + 
             cpu_info_old.stealtime + cpu_info_old.guesttime);

    printf("Cpu(s): %.1f%%us, %.1f%%sy,  %.1f%%ni, %.1f%%id,  %.1f%%wa,  %.1f%%hi,  %.1f%%si,  %.1f%%st\n", 
        (float)(cpu_info_new.utime - cpu_info_old.utime) * 100 / total_delta_time,
        (float)(cpu_info_new.stime - cpu_info_old.stime) * 100 / total_delta_time,
        (float)(cpu_info_new.ntime - cpu_info_old.ntime) * 100 / total_delta_time,
        (float)(cpu_info_new.itime - cpu_info_old.itime) * 100 / total_delta_time,
        (float)(cpu_info_new.iowtime - cpu_info_old.iowtime) * 100 / total_delta_time,
        (float)(cpu_info_new.irqtime - cpu_info_old.irqtime) * 100 / total_delta_time,
        (float)(cpu_info_new.sirqtime - cpu_info_old.sirqtime) * 100 / total_delta_time,
        (float)(cpu_info_new.stealtime - cpu_info_old.stealtime) * 100 / total_delta_time);

    printf("Cpu Usage: %.1f%%\n", 
        100 - (float)(cpu_info_new.itime - cpu_info_old.itime) * 100 / total_delta_time);

    struct mem_info mem_info;
    ReadMemInfo(&mem_info);
    printf("Mem: %dk total, %dk free, %dk buffers, %dk cached\n", mem_info.mem_total, mem_info.mem_free, mem_info.buffers, mem_info.cached);

    printf("Mem Usage: %.1f%%\n", 
        (float)(mem_info.mem_total - mem_info.mem_free - mem_info.buffers - mem_info.cached) * 100 / mem_info.mem_total);

    return 0;
}
