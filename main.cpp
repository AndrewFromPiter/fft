#include <iostream>
#include "fft.hpp"



int main()
{
	std::vector<int> P(8); // P = 1 + 2x + 3x^2 + 4x^3...
	int cnt = 0;
	std::generate(P.begin(), P.end(),
		[&cnt]() {return ++cnt; });

	std::cout << "P(x) = "<< P[0];
	for (int i = 1; i< P.size();++i)
	{
		std::cout << " + " << P[i] << "*x^" << i;
	}
	std::cout << '\n';

	auto num = FFT(P);

	for(auto x:num)
	{
		std::cout << x.real() << " + i(" << (x.imag()) << ")\n";
	}
}