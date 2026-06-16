#include "ConsoleWindow.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <string>




void ConsoleWindow::draw(std::vector<std::complex<double>> data)
{
	std::vector<std::string> buffer(15 ,std::string(data.size()/2,' '));

	double max_el = abs(
		*std::max_element(data.begin(), data.end(),
		[](std::complex<double>& left,std::complex<double>& right)
		{
			return abs(left) < abs(right);
		})
	);
	if (max_el == 0)
		throw std::exception("max_el == 0");

	for(size_t j = 0;j< buffer[0].size();++j)
	{
		size_t i = (buffer.size() - 1) * (1 - abs(data[j]) / max_el);
		if (i > buffer.size() - 1) i = buffer.size() - 1;
		buffer[i][j] = '-';
	}

	for(auto& str : buffer)
	{
		std::cout << str << '\n';
	}
}
