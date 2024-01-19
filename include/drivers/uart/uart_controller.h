#include <types.h>

struct 

struct uart_controller {
	uchar_t id;
	uchar_t parity_mode;
	uchar_t data_bits;
	uchar_t stop_bits;
	uint_t baud_rate;
	void *private;
};
