#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <time.h>

typedef struct { unsigned long long total, idle; } CpuSample;

static int read_cpu(CpuSample *s) {
    FILE *f = fopen("/proc/stat", "r");
    if (!f) return -1;
    unsigned long long user,nice,system,idle,iowait,irq,softirq,steal;
    int n = fscanf(f,"cpu %llu %llu %llu %llu %llu %llu %llu %llu",
        &user,&nice,&system,&idle,&iowait,&irq,&softirq,&steal);
    fclose(f);
    if (n < 4) return -1;
    s->idle = idle + (n >= 5 ? iowait : 0);
    s->total = user + nice + system + idle + (n >= 5 ? iowait : 0) +
               (n >= 6 ? irq : 0) + (n >= 7 ? softirq : 0) + (n >= 8 ? steal : 0);
    return 0;
}

static double cpu_usage_pct(void) {
    CpuSample a,b;
    if (read_cpu(&a) != 0) return -1;
    struct timespec pause = {0, 250000000L};
    nanosleep(&pause, NULL);
    if (read_cpu(&b) != 0) return -1;
    unsigned long long dt = b.total - a.total, di = b.idle - a.idle;
    return dt ? 100.0 * (double)(dt - di) / (double)dt : 0.0;
}

static long read_mem_kb(const char *wanted) {
    FILE *f=fopen("/proc/meminfo","r");
    if(!f) return -1;
    char key[64], unit[16]; long value;
    while(fscanf(f,"%63s %ld %15s",key,&value,unit)==3) {
        if(strcmp(key,wanted)==0){ fclose(f); return value; }
    }
    fclose(f); return -1;
}

static double disk_used_pct(const char *path) {
    struct statvfs fs;
    if(statvfs(path,&fs)!=0 || fs.f_blocks==0) return -1;
    return 100.0 * (1.0 - (double)fs.f_bavail/(double)fs.f_blocks);
}

static unsigned long long network_bytes(const char *direction) {
    FILE *f=fopen("/proc/net/dev","r");
    if(!f) return 0;
    char line[512]; unsigned long long total=0;
    fgets(line,sizeof(line),f); fgets(line,sizeof(line),f);
    while(fgets(line,sizeof(line),f)) {
        char iface[64];
        unsigned long long rx,tx;
        unsigned long long a,b,c,d,e,g,h,i,j,k,l,m,n,o;
        if(sscanf(line," %63[^:]: %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
            iface,&rx,&a,&b,&c,&d,&e,&g,&h,&tx,&i,&j,&k,&l,&m,&n,&o)==17) {
            if(strcmp(iface,"lo")!=0) total += strcmp(direction,"tx")==0 ? tx : rx;
        }
    }
    fclose(f); return total;
}

int main(int argc, char **argv) {
    int interval = argc > 1 ? atoi(argv[1]) : 5;
    if(interval < 1) interval=1;
    char host[256]="unknown"; gethostname(host,sizeof(host)-1);
    for(;;) {
        time_t now=time(NULL);
        printf("{\"hostname\":\"%s\",\"timestamp\":%ld,\"cpu_usage_pct\":%.2f,"
               "\"mem_total_kb\":%ld,\"mem_available_kb\":%ld,\"disk_used_pct\":%.2f,"
               "\"network_rx_bytes\":%llu,\"network_tx_bytes\":%llu}\n",
               host,(long)now,cpu_usage_pct(),read_mem_kb("MemTotal:"),read_mem_kb("MemAvailable:"),
               disk_used_pct("/"),network_bytes("rx"),network_bytes("tx"));
        fflush(stdout); sleep((unsigned)interval);
    }
}
