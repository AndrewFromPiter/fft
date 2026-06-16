#pragma once
#include <vector>
#include <mutex>



// предназначен для работы двух потоков в режиме писатель-читатель
template <typename el_type >
class RingBuffer 
{
	std::vector<std::vector<el_type>> buffer;
	const size_t capacity;
	size_t writeIndex = 0;
	std::mutex mtx;

public:
	RingBuffer(size_t cap, size_t N) :
		capacity(cap),
		buffer(cap, std::vector<el_type>(N))
	{
	};
	
	void push(const std::vector<el_type>& data)
	{
		std::lock_guard<std::mutex> lg();
		size_t index = writeIndex % capacity;
		buffer[index] = data;
		++writeIndex;
	}

	std::vector<el_type> Try_get_latest() const
	{
		std::lock_guard<std::mutex> lg();
		if (writeIndex == 0)
			return {};

		size_t index = (writeIndex - 1) % capacity;
		return buffer.at(index);
	}
};