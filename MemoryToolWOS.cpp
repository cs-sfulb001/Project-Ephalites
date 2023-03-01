#include "MemoryToolWOS.h"
MemoryToolWOS::MemoryToolWOS():MemoryToolWOS(2){}
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
		double physMemUsedByMe = pmc.WorkingSetSize;
		if (unit == NULL)
			return physMemUsedByMe; //Returns B
		else if (unit == 'k'||unit=='K')
			return physMemUsedByMe / BiKB; //Returns KB
		else if (unit == 'm'||unit=='M')
			return physMemUsedByMe / BiMB; //Returns MB
		else
			return physMemUsedByMe / BiGB; //Defualt returns in GB
}
double MemoryToolWOS::getUseableRAMsize()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG TotalPhysMem = memInfo.ullTotalPhys;
	return TotalPhysMem /(1.* BiGB);
}
double MemoryToolWOS::getUsedRAM()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	return physMemUsed / BiGB;
}
bool MemoryToolWOS::aboveTargetUsage()
{
	return getRAMUsage('g') >= targetRAMUsage;
}