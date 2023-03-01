#pragma once
#include "windows.h"
#include "psapi.h"


class MemoryToolWOS
{
private:
	const static int BiKB = 1024;
	const static int BiMB = 1048576;
	const static int BiGB = 1073741824;
	double targetRAMUsage;//in GB
public:
	MemoryToolWOS();
	MemoryToolWOS(double);
	~MemoryToolWOS() = default;
	/*
	Returns the target RAM usage in GB
	*/
	double desiredRAMUsage();
	/*
	Sets the target RAM usage in GB
	*/
	void desiredRAMUsage(double);
	/*
	Returns a double of the RAM Usage by the current program
	Input determines unit (Defaults to GB)
	null=bytes
	k=kilobytes
	m=megabytes
	g=gigabytes
	*/
	double getRAMUsage(char);
	/*
	Returns the amount of RAM usable by the program in GB
	*/
	double getUseableRAMsize();
	/*
	Returns the amount of usable RAM already in use in GB
	*/
	double getUsedRAM();
	/*
	Returns true if the current program is using more RAM than
	the target amount
	*/
	bool aboveTargetUsage();
};

