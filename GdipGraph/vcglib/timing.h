/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         timing.h                                           */
/*   version:      1.00.00                                            */
/*   creation:     11.11.93                                           */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Time measurement for debugging/profiling           */
/*   status:       in work                                            */
/*                                                                    */
/*--------------------------------------------------------------------*/




#ifndef TIMING_H
#define TIMING_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <psapi.h>
#include <errno.h>
#include <time.h>
#include <winsock2.h>

#define EPOCHFILETIME (116444736000000000LL)

//#define ENODATA         61
#define	RUSAGE_SELF	     0
#define	RUSAGE_CHILDREN	-1

struct timezone
{
	int tz_minuteswest; /* minutes W of Greenwich */
	int tz_dsttime;     /* type of dst correction */
};

typedef struct
{
	long            tv_sec;
	long            tv_usec;
} timeval;

typedef struct rusage
{
	timeval  ru_utime;   /* user time used                      */
	timeval  ru_stime;   /* system time used                    */
	long     ru_maxrss;  /* integral max resident set size      */
	long     ru_ixrss;   /* integral shared text memory size    */
	long     ru_idrss;   /* integral unshared data size         */
	long     ru_isrss;   /* integral unshared stack size        */
	long     ru_minflt;  /* page reclaims                       */
	long     ru_majflt;  /* page faults                         */
	long     ru_nswap;   /* swaps                               */
	long     ru_inblock; /* block input operations              */
	long     ru_oublock; /* block output operations             */
	long     ru_msgsnd;  /* messages sent                       */
	long     ru_msgrcv;  /* messages received                   */
	long     ru_nsignals;/* signals received                    */
	long     ru_nvcsw;   /* voluntary context switches          */
	long     ru_nivcsw;  /* involuntary context switches        */
} rusage;


typedef union file_t
{
	FILETIME ft;
	long long lt;
} file_t;

int getrusage(int who, rusage* usage);
int gettimeofday(struct timeval* tv, struct timezone* tz);
void start_time(void);
void stop_time(char* x);

#endif /* TIMING_H */

