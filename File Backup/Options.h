#pragma once
class Options
{
private:
	int _minTimesUsed;
	double _MaxRAM;
public:
	Options();
	Options(int, double);
	void minTimesUsed(int);
	int minTimesUsed();
	void MaxRAM(double);
	double MaxRAM();
	void printOptions();
	void LoadFile();
	void SaveFile();
};

