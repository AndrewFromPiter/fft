#include <iostream>
#include <thread>
#include "fft.hpp"
#include "ConsoleWindow.h"
#include "GraphicWindow.hpp"
#include "RingBuffer.hpp"
#include "AudioReader.h"

const int Samples_In_Package = 1024;
const int Sample_Rate = 44100;

int main()
{
	RingBuffer<complex<double>> input_data(8,Samples_In_Package);
	RingBuffer<complex<double>> output_data(8,Samples_In_Package);

	std::jthread Graphic([&output_data,&input_data]()
		{
			try {
				GraphicWindow<std::complex<double>> GWin(1920, 1080, "FFT");
				GWin.PushData(&output_data);
				GWin.PushData(&input_data);
				GWin.RenderLoop();
			}
			catch (std::exception exc) {
				std::cout << exc.what();
				return 1;
			}
		}
	);
	std::jthread Audio([&input_data]()
		{
			try {
				AudioReader ARead(Samples_In_Package, Sample_Rate);
				ARead.SetBuffer(&input_data);
				ARead.ReadLoop();
			}
			catch (std::exception exc) {
				std::cout << exc.what();
				return 1;
			}
		}
	);
	
	/*int cnt = 0;
	int T1 = 1, T2 = Samples_In_Package, T3 = Samples_In_Package/2;*/
	while(1){
		/*std::vector<complex<double>> package(Samples_In_Package);
		T1 += 2; T2--;T3 += 3;
		std::generate(package.begin(), package.end(),
			[&]() {
				++cnt;
				double val = 0.3*sin((T1%Samples_In_Package) * cnt * 2 * pi / Samples_In_Package + pi / 6)
					+ cos((T2 % Samples_In_Package) * cnt * 2 * pi / Samples_In_Package)
					+ 0.7*cos((T3 % Samples_In_Package) * cnt * 2 * pi / Samples_In_Package);
				return std::complex<double>(val);
			});
		input_data.push(package);*/

		auto data = input_data.Try_get_latest();

		if (!data.empty())
		{
			output_data.push(FFT(data));
		}


		//std::this_thread::sleep_for(std::chrono::milliseconds(10));

		/*system("cls");

		ConsoleWindow win;
		std::cout << "\n\n ============ FFT ==============\n";
		win.draw(output_data.Try_get_latest());

		std::cout << "\n\n ============ input ==============\n";
		win.draw(input_data.Try_get_latest());*/

		
	}
}