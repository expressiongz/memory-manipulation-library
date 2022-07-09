#pragma once
#include "includes.hpp"

template<typename array_data_t, std::size_t array_sz>
class manipulated_array_data
{
private:
	array_data_t* array_address;
	std::array<array_data_t, array_sz> overwritten_array{};
	std::array<array_data_t, array_sz> new_array{};
public:

	void set_overwritten_array(std::span<array_data_t> array);
	void set_new_array(std::span<array_data_t> array);
	void set_array_index(const std::uint32_t index, array_data_t value) const;

	void overwrite_array() const;
	void restore_array() const;

	array_data_t read_array_index(const std::uint32_t index) const;
	std::vector<array_data_t> read_array() const;

	array_data_t* ret_array_address() const;

	explicit manipulated_array_data(std::span<array_data_t> _overwritten_array, std::span<array_data_t> _new_array, array_data_t* _array_address)
		: overwritten_array(_overwritten_array), new_array(_new_array), array_address(_array_address)
	{

	}

};

template<typename data_t>
class manipulated_data 
{
private:
	data_t* data_address{nullptr};
	data_t overwritten_data{};
	data_t new_data{};
public:
	void restore_data();
	void set_data(data_t new_data);

	data_t read_data() const;
	data_t* ret_data_address() const;

	explicit manipulated_data(data_t _overwritten_data, data_t _new_data, data_t* _data_address)
		: overwritten_data(_overwritten_data), new_data(_new_data), data_address(_data_address) 
	{

	}

};

// regular data class 
template<typename data_t>
void manipulated_data<data_t>::restore_data() 
{
	(*this->data_address) = this->overwritten_data;
}

template<typename data_t>
void manipulated_data<data_t>::set_data(data_t _new_data) 
{
	this->overwritten_data = this->new_data;

	this->new_data = _new_data;

	*(this->data_address) = _new_data;

}

template<typename data_t>
data_t manipulated_data<data_t>::read_data() const 
{
	return *this->data_address;
}

template<typename data_t>
data_t* manipulated_data<data_t>::ret_data_address() const 
{
	return this->data_address;
}


// array data class

template<typename array_data_t, std::size_t array_sz>
array_data_t* manipulated_array_data<array_data_t, array_sz>::ret_array_address() const {
	return this->array_address;
}

template<typename array_data_t, std::size_t array_sz>
void manipulated_array_data<array_data_t, array_sz>::set_overwritten_array(std::span<array_data_t> array) 
{
	this->overwritten_array = array;
}

template<typename array_data_t, std::size_t array_sz>
void manipulated_array_data<array_data_t, array_sz>::set_new_array(std::span<array_data_t> array) 
{
	this->new_array = array;
}

template<typename array_data_t, std::size_t array_sz>
void manipulated_array_data<array_data_t, array_sz>::set_array_index(const std::uint32_t index, array_data_t val)  const
{
	this->array_address[index] = val;
}

template<typename array_data_t, std::size_t array_sz>
void manipulated_array_data<array_data_t, array_sz>::overwrite_array()  const
{
	// overwriting method for now. later updates will feature std::array methods to overwrite the array.
	for (auto idx = 0u; idx < array_sz; idx++) {
		this->array_address[idx] = this->new_array[idx];
	}
}

template<typename array_data_t, std::size_t array_sz>
void manipulated_array_data<array_data_t, array_sz>::restore_array() const
{
	// overwriting method for now. later updates will feature std::array methods to overwrite the array.
	for (auto idx = 0u; idx < array_sz; idx++) {
		this->array_address[idx] = this->overwritten_array[idx];
	}
}

template<typename array_data_t, std::size_t array_sz>
array_data_t manipulated_array_data<array_data_t, array_sz>::read_array_index(const std::uint32_t index) const
{
	return this->array_address[index];
}

template<typename array_data_t, std::size_t array_sz>
std::vector<array_data_t> manipulated_array_data<array_data_t, array_sz>::read_array() const
{
	std::vector<array_data_t> read_array{};
	for (auto idx = 0u; idx < array_sz; idx++) {
		read_array.push_back(this->array_address[idx]);
	}
	return read_array;
}

