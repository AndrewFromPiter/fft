#pragma once
#include <GLFW/glfw3.h>
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
			if(gr_data.auto_scale){
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

			float x_step = gr_data.denum * 2.0f / pack_size;
			float y_height = 1.8f / total_packs;
			float y_offset = -0.9f + num_pack * y_height;
			for (size_t i = 0;i < package.size() / gr_data.denum ;++i)
			{
				float x = 1.8f * i * x_step - 0.9f;

				float val = abs(package[i]);
				if (val > max_el)
					val = max_el;

				float y = (val / max_el) * y_height + y_offset;

				glVertex2f(x, y);
			}

			glEnd();
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