//
// Created by mahsa.meymari on 12/19/18.
//

#include "types.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "Process_timeTable.h"

#define NUM_OF_PROCESSES 20

int stdout = 1;
char buf[8192];

void
bigwrite(void)
{
    int fd, sz;


    unlink("bigwrite");
    for(sz = 499; sz < 12*55; sz += 471){
        fd = open("bigwrite", O_CREATE | O_RDWR);
        if(fd < 0){
            printf(1, "cannot create bigwrite\n");
            exit();
        }
        int i;
        for(i = 0; i < 2; i++){
            int cc = write(fd, buf, sz);
            if(cc != sz){
                exit();
            }
        }
        close(fd);
        unlink("bigwrite");
    }

    printf(1, "IO : ");
}

void writetest(void)
{


    int fd;
    int i;

    // printf(stdout, "small file test\n");
    fd = open("small", O_CREATE | O_RDWR);
    if (fd >= 0)
    {
        // printf(stdout, "creat small succeeded; ok\n");
    }
    else
    {
        // printf(stdout, "error: creat small failed!\n");
        exit();
    }
    for (i = 0; i < 100; i++)
    {
        if (write(fd, "aaaaaaaaaa", 10) != 10)
        {
            // printf(stdout, "error: write aa %d new file failed\n", i);
            exit();
        }
        if (write(fd, "bbbbbbbbbb", 10) != 10)
        {
            // printf(stdout, "error: write bb %d new file failed\n", i);
            exit();
        }
    }
    // printf(stdout, "writes ok\n");
    close(fd);
    fd = open("small", O_RDONLY);
    if (fd >= 0)
    {
    }
    else
    {
        exit();
    }
    i = read(fd, buf, 2000);
    if (i == 2000)
    {
    }
    else
    {
        exit();
    }
    close(fd);

    if (unlink("small") < 0)
    {
        // printf(stdout, "unlink small failed\n");
        exit();
    }
    // printf(stdout, "small file test ok\n");
}

void cpuIntensiveTask()
{
    double k = 1000000000;
    for (int i = 0; i < 100000; i++)
    {
        for(int j = 0 ; j<50000;j++)
            k *=k;
    }
    printf(1,"CPU  ");
}

int main(int argc, char *argv[])
{
    struct process_Timetable * pTimetable  = malloc(sizeof(struct process_Timetable));


    for (int i = 0; i < NUM_OF_PROCESSES; i++)
    {
        int proc = fork();
        if (proc < 0)
        {
            printf(stdout, "fork error");
            exit();
        }
        if (proc == 0)
        {
            if (i % 2 == 0)
            {
                bigwrite();
                getTimetable(pTimetable,getpid());
//                printf(1, " %d:    %d,     %d   %d\n",getpid(), pTimetable->end_cycle[getpid()] ,pTimetable->start_cycle[getpid()] ,pTimetable->first_schedule_cycle[getpid()]);
                exit();

            }
            else
            {
                cpuIntensiveTask();
                getTimetable(pTimetable,getpid());
//                printf(1, " %d:    %d,     %d   %d\n", getpid(), pTimetable->end_cycle[getpid()] ,pTimetable->start_cycle[getpid()] ,pTimetable->first_schedule_cycle[getpid()]);

                exit();
            }
        }
    }
    for (int i = 0; i < NUM_OF_PROCESSES; i++)
    {
        wait();
    }

    exit();
}