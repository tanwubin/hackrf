#include <hackrf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FREQ 900000000

hackrf_device* device = NULL;
unsigned char* mbuffer = "012345678901234567890123456789";

void init_hackrf()
{
	hackrf_init();
	hackrf_open(&device);

	hackrf_set_sample_rate(device, 8000000);
	hackrf_set_baseband_filter_bandwidth(device, 8000000 * 0.75);
	hackrf_set_freq(device, FREQ);
	hackrf_set_txvga_gain(device, 40);
	hackrf_set_lna_gain(device, 40);
	hackrf_set_amp_enable(device, 1);

	printf("HACKRF init done.\n");
}

int _hackrf_tx_callback(hackrf_transfer* transfer)
{
	printf("transfer->valid_length is %d\n", transfer->valid_length);
	// memcpy(transfer->buffer,mbuffer,transfer->valid_length);
	if (strlen(mbuffer) >= transfer->valid_length) {
		printf("send txt len overflow!\n");
	}
	memcpy(transfer->buffer, mbuffer, strlen(mbuffer));
	printf("HACKRF send finish.%s\n", transfer->buffer);
	return 0;
}

int main(void)
{
	int i;
	for (i = 0; i < 100; i++) {
		init_hackrf();
		hackrf_start_tx(device, _hackrf_tx_callback, NULL);
		hackrf_stop_tx(device);
		hackrf_close(device);
		// _sleep(1000);
	}
	return EXIT_SUCCESS;
}
