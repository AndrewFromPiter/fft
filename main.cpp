#include <iostream>
#include <thread>
#include "fft.hpp"
#include "ConsoleWindow.h"



int main()
{
	int N = 256;
	std::vector<complex<double>> input_data(N);
	int cnt = 0;
	int T1 = 1, T2 = 128, T3 = 64;
	while(1){
		T1 += 2; T2--;T3 += 3;
		std::generate(input_data.begin(), input_data.end(),
			[&]() {
				++cnt;
				double val = 0.3*sin((T1%128) * cnt * 2 * pi / N + pi / 6)
					+ cos((T2 % 128) * cnt * 2 * pi / N)
					+ 0.7*cos((T3 % 128) * cnt * 2 * pi / N);
				return std::complex<double>(val);
			});

		auto output_data = FFT(input_data);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		system("cls");
		ConsoleWindow win;
		std::cout << "\n\n ============ input ==============\n";
		win.SetData(&input_data);
		win.draw();

		std::cout << "\n\n ============ FFT ==============\n";
		win.SetData(&output_data);
		win.draw();
	}
}