#include <iostream>
#include <thread>
#include "fft.hpp"
#include "GraphicWindow.hpp"
#include "RingBuffer.hpp"
#include "AudioReader.h"

const int Samples_In_Package = 4096;
const int Sample_Rate = 48000;

int main()
{
	RingBuffer<complex<double>> input_data(8,Samples_In_Package);
	RingBuffer<complex<double>> output_data(8,Samples_In_Package);

	std::jthread Graphic([&output_data,&input_data]()
		{
			try {
				GraphicWindow<std::complex<double>> GWin(1920, 1080, "FFT");

				graphicData out{ &output_data , "output", Sample_Rate , 2, false , 1000.0f};
				graphicData input{ &input_data , "input", Sample_Rate , 1 , false , 1.5f };

				GWin.PushData(out);
				GWin.PushData(input);
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
	std::jthread Render([&output_data, &input_data]()
		{
			while (1) {
				auto data = input_data.Try_get_latest();

				if (!data.empty())
				{
					output_data.push(FFT(data));
				}
			}
		}
	);
}