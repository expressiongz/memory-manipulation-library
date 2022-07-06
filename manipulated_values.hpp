#pragma once


template<typename data_t>
class manipulated_data {
private:
	data_t* data_address{nullptr};
	data_t data_restore_buf{};
	data_t new_data{};
public:
	void restore_data();
	void set_data(data_t new_data);

	data_t read_data() const;
	data_t* ret_data_address() const;

	explicit manipulated_data(data_t _data_restore_buf, data_t _new_data, data_t* _data_address)
		: data_restore_buf(_data_restore_buf), new_data(_new_data), data_address(_data_address) {

	}

};

template<typename data_t>
void manipulated_data<data_t>::restore_data() {
	(*this->data_address) = this->data_restore_buf;
}

template<typename data_t>
void manipulated_data<data_t>::set_data(data_t _new_data) {
	this->data_restore_buf = this->new_data;

	this->new_data = _new_data;

	*(this->data_address) = _new_data;

}

template<typename data_t>
data_t manipulated_data<data_t>::read_data() const {
	return *this->data_address;
}

template<typename data_t>
data_t* manipulated_data<data_t>::ret_data_address() const {
	return this->data_address;
}
