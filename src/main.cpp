#include <iostream>
#include <thread>
#include "fft.hpp"
#include "ConsoleWindow.h"
#include "GraficWindow.hpp"
#include "RingBuffer.hpp"


int main()
{
	
	

	int N = 256;
	std::vector<complex<double>> input_data(N);
	RingBuffer<complex<double>> output_data(8,N);

	std::jthread th([&output_data]()
		{
			GraficWindow<std::complex<double>> GWin(1080, 920, "FFT");
			GWin.SetData(&output_data);
			GWin.Render();
		}
	);
	
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
		
		output_data.push(FFT(input_data));


		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		system("cls");
		ConsoleWindow win;
		std::cout << "\n\n ============ input ==============\n";
		win.draw(input_data);

		std::cout << "\n\n ============ FFT ==============\n";
		win.draw(output_data.Try_get_latest());
	}
}