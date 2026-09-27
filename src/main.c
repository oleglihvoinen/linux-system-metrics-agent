#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <time.h>
typedef struct { unsigned long long total, idle; } CpuSample;
static int read_cpu(CpuSample *s){FILE *f=fopen("/proc/stat","r");if(!f)return -1;unsigned long long u,n,sy,id,io,ir,so,st;int c=fscanf(f,"cpu %llu %llu %llu %llu %llu %llu %llu %llu",&u,&n,&sy,&id,&io,&ir,&so,&st);fclose(f);if(c<4)return -1;s->idle=id+(c>=5?io:0);s->total=u+n+sy+id+(c>=5?io:0)+(c>=6?ir:0)+(c>=7?so:0)+(c>=8?st:0);return 0;}
static double cpu_usage_pct(void){CpuSample a,b;if(read_cpu(&a))return -1;struct timespec p={0,250000000L};nanosleep(&p,NULL);if(read_cpu(&b))return -1;unsigned long long dt=b.total-a.total,di=b.idle-a.idle;return dt?100.0*(double)(dt-di)/(double)dt:0.0;}
static long mem_kb(const char *wanted){FILE *f=fopen("/proc/meminfo","r");if(!f)return -1;char k[64],unit[16];long v;while(fscanf(f,"%63s %ld %15s",k,&v,unit)==3){if(strcmp(k,wanted)==0){fclose(f);return v;}}fclose(f);return -1;}
static double disk_pct(void){struct statvfs fs;if(statvfs("/",&fs)||!fs.f_blocks)return -1;return 100.0*(1.0-(double)fs.f_bavail/(double)fs.f_blocks);}
static unsigned long long net_bytes(int tx){FILE *f=fopen("/proc/net/dev","r");if(!f)return 0;char line[512];unsigned long long total=0;fgets(line,sizeof(line),f);fgets(line,sizeof(line),f);while(fgets(line,sizeof(line),f)){char iface[64];unsigned long long rx,a,b,c,d,e,g,h,t,i,j,k,l,m,n,o;if(sscanf(line," %63[^:]: %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",iface,&rx,&a,&b,&c,&d,&e,&g,&h,&t,&i,&j,&k,&l,&m,&n,&o)==17&&strcmp(iface,"lo"))total+=tx?t:rx;}fclose(f);return total;}
int main(int argc,char **argv){int interval=argc>1?atoi(argv[1]):5;if(interval<1)interval=1;char host[256]="unknown";gethostname(host,sizeof(host)-1);for(;;){printf("{\"hostname\":\"%s\",\"timestamp\":%ld,\"cpu_usage_pct\":%.2f,\"mem_total_kb\":%ld,\"mem_available_kb\":%ld,\"disk_used_pct\":%.2f,\"network_rx_bytes\":%llu,\"network_tx_bytes\":%llu}\n",host,(long)time(NULL),cpu_usage_pct(),mem_kb("MemTotal:"),mem_kb("MemAvailable:"),disk_pct(),net_bytes(0),net_bytes(1));fflush(stdout);sleep((unsigned)interval);}}
