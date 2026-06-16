#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include "RingBuffer.hpp"

template<typename el_type>
class GraficWindow
{
private:
	GLFWwindow* window;
	int width, height;
	std::string winName;

	RingBuffer<el_type>* RB = nullptr;

	void draw_data()
	{
		if (RB == nullptr)
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

		auto data = RB->Try_get_latest();
		if (data.empty())
			return;

		glBegin(GL_LINE_STRIP);
		
		glColor3f(0.0f, 1.0f, 1.0f);
		
		float max_el = abs(
			*std::max_element(data.begin(), data.end(),
				[](el_type& left, el_type& right)
				{
					return abs(left) < abs(right);
				})
		);

		if (max_el == 0)
			throw std::exception("max_el == 0");

		float x_step = 2.0f / data.size();
		for (size_t i = 0;i < data.size() / 2;++i)
		{
			float x = 1.8f * i * x_step - 0.9f;
			float y = 1.8f * abs(data[i]) / max_el -0.9f;

			glVertex2f(x, y);
		}

		glEnd();
	};
public:
	GraficWindow(int w,int h,std::string name):
		width(w),height(h),
		winName(name)
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

	~GraficWindow()
	{
		if (window)
		{
			glfwDestroyWindow(window);
			window = nullptr;
		}
		glfwTerminate();
	}

	void SetData(RingBuffer<el_type>* arg) {	RB = arg;	};
	
	void Render()
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