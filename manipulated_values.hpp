#pragma once


template<typename data_t>
class manipulated_data {
private:
	data_t* data_address{nullptr};
	data_t old_data{};
	data_t new_data{};
public:
	void restore_data();
	void set_data(data_t new_data);

	data_t read_data() const;

	explicit manipulated_data(data_t _old_data, data_t _new_data, data_t* _data_address)
		: old_data(_old_data), new_data(_new_data), data_address(_data_address) {

	}

};

template<typename data_t>
void manipulated_data<data_t>::restore_data() {
	(*this->data_address) = this->old_data;
}
	
template<typename data_t>
void manipulated_data<data_t>::set_data(data_t new_data) {
	this->new_data = new_data;
	(*this->data_address) = new_data;
}


template<typename data_t>
data_t manipulated_data<data_t>::read_data() const {
	return *this->data_address;
}

