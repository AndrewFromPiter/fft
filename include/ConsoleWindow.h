#pragma once
#include <vector>
#include <complex>


class ConsoleWindow
{
	std::vector<std::complex<double>>* data;
	double delta_f;
public:
	ConsoleWindow();
	void SetData(std::vector<std::complex<double>>*);
	void draw();
};

