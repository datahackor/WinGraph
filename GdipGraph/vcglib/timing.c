
#include "timing.h"


int getrusage(int who, rusage* usage)
{
	HANDLE proc_hand;
	file_t c_time, x_time, s_time, u_time;
	int cb = 0, err = -1;

	if (who != RUSAGE_SELF)
	{
		errno = (who == RUSAGE_CHILDREN ? ENODATA : EINVAL);
		return err;
	}

	proc_hand = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	if (GetProcessTimes(proc_hand, &(c_time.ft), &(x_time.ft), &(s_time.ft), &(u_time.ft)))
	{
		PROCESS_MEMORY_COUNTERS ctrs;

		/* The units returned by GetProcessTimes are 100 nanoseconds */
		u_time.lt = (u_time.lt + 5) / 10;
		s_time.lt = (s_time.lt + 5) / 10;

		usage->ru_utime.tv_sec = (long)(u_time.lt / 1000000ll);
		usage->ru_stime.tv_sec = (long)(s_time.lt / 1000000ll);
		usage->ru_utime.tv_usec = (long)(u_time.lt % 1000000ll);
		usage->ru_stime.tv_usec = (long)(s_time.lt % 1000000ll);

		if (GetProcessMemoryInfo(proc_hand, &ctrs, sizeof(ctrs)))
		{
			PERFORMANCE_INFORMATION perf_info;
			GetPerformanceInfo(&perf_info, sizeof(perf_info));
			usage->ru_maxrss = (DWORD)(ctrs.WorkingSetSize / perf_info.PageSize);
			usage->ru_majflt = ctrs.PageFaultCount;
			err = 0;
		}
	}

	if (err)
		errno = EACCES;
	CloseHandle(proc_hand);
	return err;
}

int gettimeofday(struct timeval* tv, struct timezone* tz)
{
	FILETIME        ft;
	LARGE_INTEGER   li;
	__int64         t;
	static int      tzflag;

	if (tv)
	{
		GetSystemTimeAsFileTime(&ft);
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		t = li.QuadPart;
		t -= EPOCHFILETIME;
		t /= 10;
		tv->tv_sec = (long)(t / 1000000);
		tv->tv_usec = (long)(t % 1000000);
	}

	if (tz)
	{
		if (!tzflag)
		{
			_tzset();
			tzflag++;
		}
		tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime = _daylight;
	}

	return 0;
}

#ifndef CHECK_TIMING

void start_time(void){}
void stop_time(char* x) {}

#else

unsigned long start_syssec;
unsigned long start_sysmycsec;
unsigned long start_usersec;
unsigned long start_usermycsec;
struct timeval  tpstart;
struct timezone tzpstart;

void start_time(void)
{
	struct rusage r;

	getrusage(RUSAGE_SELF, &r);

	start_syssec = r.ru_stime.tv_sec;
	start_sysmycsec = r.ru_stime.tv_usec;
	start_usersec = r.ru_utime.tv_sec;
	start_usermycsec = r.ru_utime.tv_usec;
	gettimeofday(&tpstart, &tzpstart);
}

unsigned long stop_syssec;
unsigned long stop_sysmycsec;
unsigned long stop_usersec;
unsigned long stop_usermycsec;
struct timeval  tpend;
struct timezone tzpend;


void stop_time(char* x)
{
	struct rusage r;
	unsigned long sec;
	long int usec;

	gettimeofday(&tpend, &tzpend);
	getrusage(RUSAGE_SELF, &r);

	stop_syssec = r.ru_stime.tv_sec;
	stop_sysmycsec = r.ru_stime.tv_usec;
	stop_usersec = r.ru_utime.tv_sec;
	stop_usermycsec = r.ru_utime.tv_usec;

	sec = stop_usersec - start_usersec;
	usec = stop_usermycsec - start_usermycsec;
	if (usec < 0) { sec--; usec += 1000000; }
	printf("%s:\n", x);
	printf("Time: User: %ld.%03ld sec ",
		sec, usec / 1000);
	sec = stop_syssec - start_syssec;
	usec = stop_sysmycsec - start_sysmycsec;
	if (usec < 0) { sec--; usec += 1000000; }
	printf("System: %ld.%03ld sec ",
		sec, usec / 1000);
	sec = tpend.tv_sec - tpstart.tv_sec;
	usec = tpend.tv_usec - tpstart.tv_usec;
	if (usec < 0) { sec--; usec += 1000000; }
	printf("Real: %ld.%03ld sec\n", sec, usec / 1000);
}


#endif /* CHECK_TIMING */