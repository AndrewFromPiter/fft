#pragma once
#include <complex>
#include <vector>
#include <cmath>
#include <numbers>
#include <algorithm>

using std::vector;
using std::complex;
using std::numbers::pi;

template <typename T>
std::vector<std::complex<double>> FFT(std::vector<T> P)
{
	std::vector<std::complex<double>> res;

	for (auto x : P)
	{
		res.push_back(std::complex<double>(x, 0.0));
	}
	return FFT(res);
}


template <>
std::vector<std::complex<double>> FFT(std::vector<std::complex<double>> P)
{
	int n = P.size();

	if (n == 1)
		return P; //выход из рекурсии когда полином константа

	complex w = std::polar(1.0, 2 * pi / (double)n);

	vector<complex<double>> Pe(n / 2), Po(n / 2);
	for (size_t j = 0; j < n / 2; ++j)
	{
		Pe[j] = P[2*j];
		Po[j] = P[2*j + 1];
	}

	vector<complex<double>> ye = FFT(Pe);
	vector<complex<double>> yo = FFT(Po);

	vector<complex<double>> Y(n);

	for (size_t j = 0; j < n / 2; ++j)
	{
		Y[j] = ye[j] + pow(w, j) * yo[j];
		Y[j + n / 2] = ye[j] - pow(w, j) * yo[j];
	}
	return Y;
}

