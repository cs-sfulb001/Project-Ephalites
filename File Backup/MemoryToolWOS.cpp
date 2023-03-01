#include "MemoryToolWOS.h"

MemoryToolWOS::MemoryToolWOS(double target)
{
	targetRAMUsage = target;
}
void MemoryToolWOS::desiredRAMUsage(double target)
{
	targetRAMUsage = target;
}
double MemoryToolWOS::desiredRAMUsage()
{
	return targetRAMUsage;
}
double MemoryToolWOS::getRAMUsage(char unit)
{
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
		if (unit == NULL)
			return physMemUsedByMe;
		else if (unit == 'k'||unit=='K')
			return physMemUsedByMe / BiKB;
		else if (unit == 'm'||unit=='M')
			return physMemUsedByMe / BiMB;
		else
			return physMemUsedByMe / BiGB;
}
double MemoryToolWOS::getUseableRAMsize()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG TotalPhysMem = memInfo.ullTotalPhys;
	return TotalPhysMem / BiGB;
}
double MemoryToolWOS::getUsedRAM()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	return physMemUsed / BiGB;
}
bool MemoryToolWOS::aboveTargetUsage()
{
	return getRAMUsage('g') > targetRAMUsage;
}