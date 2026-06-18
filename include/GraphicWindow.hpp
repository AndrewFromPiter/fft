#pragma once
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <string>
#include "RingBuffer.hpp"

template<typename el_type>
struct graphicData
{
	RingBuffer<el_type>* ptr;
	std::string name = "unnamed";
	float divVal = 1;
	int denum = 1;
	bool auto_scale = true;
	float max = 1;
};

template<typename el_type>
class GraphicWindow
{
private:
	GLFWwindow* window;
	int width, height;
	std::string winName;
	std::vector<graphicData<el_type>> data;
	//RingBuffer<el_type>* RB = nullptr;

	void draw_data()
	{
		if (data.empty())
		{
			glBegin(GL_TRIANGLES);

			glColor3f(1, 0, 0);
			glVertex2f(-1, -0.5);

			glColor3f(0, 1, 0);
			glVertex2f(0, 1);

			glColor3f(0, 0, 1);
			glVertex2f(1, -0.5);

			glEnd();

			return;
		}

		int num_pack = 0;
		const int total_packs = data.size();
		for (auto gr_data : data) {
			auto package = gr_data.ptr->Try_get_latest();
			if (package.empty())
			{
				++num_pack;
				continue;
			}
			const int pack_size = package.size();
			glBegin(GL_LINE_STRIP);

			glColor3f(0.0f, 1.0f, 1.0f);

			float max_el = gr_data.max;
			if (gr_data.auto_scale) {
				max_el = abs(
					*std::max_element(package.begin(), package.end(),
						[](el_type& left, el_type& right)
						{
							return abs(left) < abs(right);
						})
				);
			}


			if (max_el == 0)
				throw std::exception("max_el == 0");

			float x_step = gr_data.denum * 1.8f / pack_size;
			float y_height = 1.8f / total_packs;
			float y_offset = -0.9f + num_pack * y_height;
			for (size_t i = 0;i < package.size() / gr_data.denum;++i)
			{
				float x = i * x_step - 0.9f;

				float val = abs(package[i]);
				if (val > max_el)
					val = max_el;

				float y = (val / max_el) * y_height + y_offset;

				glVertex2f(x, y);
			}

			glEnd();

			// === Добавляем шкалу X только для спектра (output) ===
			if (num_pack == 0) {  // output
				glColor3f(0.8f, 0.8f, 0.0f);

				const float sample_rate = gr_data.divVal;           // должно быть 48000
				const int n_points = pack_size / gr_data.denum;     // обычно 2048
				const float freq_per_bin = sample_rate / pack_size; // важно! (48000 / 4096)

				float max_freq = sample_rate / gr_data.denum;

				// Рисуем метки каждые ~5000 Гц
				for (int k = 0; k <= 32; ++k) {
					float freq = k * (int)max_freq / 32;
					int i = (int)(freq / freq_per_bin + 0.5f);   // ближайшая бина
					if (i >= n_points) break;

					float x = -0.9f + i * x_step;   // используй тот же x_step, что и выше

					// Короткая засечка (не на всю высоту!)
					glBegin(GL_LINES);
					glVertex2f(x, y_offset - 0.02f);           // чуть ниже графика
					glVertex2f(x, y_offset + 0.02f);           // короткая вверх
					glEnd();

					// Текст частоты
					glRasterPos2f(x - 0.015f, y_offset - 0.1f);
					std::string label = std::to_string((int)freq);
					for (char c : label) {
						glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
					}
				}

				// Подпись "Frequency (Hz)" под осью
				glRasterPos2f(0.3f, y_offset - 0.28f);
				const char* axis = "Frequency (Hz)";
				for (char c : std::string(axis)) {
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
				}
			}
			++num_pack;
		}
	};
public:
	GraphicWindow(int w, int h, std::string name) :
		width(w), height(h),
		winName(name),
		data()
	{
		if (!glfwInit())
			throw std::exception("error glfwInit");

		window = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr);
		if (!window) {
			glfwTerminate();
			throw std::exception("error glfwCreateWindow");
		}
		glfwMakeContextCurrent(window);

		int argc = 1;
		char* argv[1] = { (char*)"fft" };
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	};

	~GraphicWindow()
	{
		if (window)
		{
			glfwDestroyWindow(window);
			window = nullptr;
		}
		glfwTerminate();
	}

	void PushData(graphicData<el_type> arg) {	data.push_back(arg);	};
	
	void RenderLoop()
	{
		while (!glfwWindowShouldClose(window)) {
			
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Тёмный фон
			glClear(GL_COLOR_BUFFER_BIT);

			//здесь вызов функция рисования
			draw_data();


			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}


	
};