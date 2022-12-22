#include "canAPI.h"
#include <pthread.h>

void* tThread(int sock)
{
	//pthread_t tid;
	//tid = pthread_self();
	int pid = syscall(__NR_gettid);

	printf("[T] Socket: %d\n", sock);
	while (1)
	{
		int num;

		printf("[%d] Input(1:Send): ", pid);
		scanf("%d", &num);

		if (num == 1)
		{
			// Transmit CAN frame
			uint8_t can_data[CAN_FRAME_MAX_LEN] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
			TransmitCanFrame(sock, CAN_ID, can_data, sizeof(can_data));
		}
		else
		{
			printf("[%d] Please Input Number '1'\n", pid);
		}

		getchar();
	}
}

void* rThread(int sock)
{
	//pthread_t tid;
	//tid = pthread_self();
	int pid = syscall(__NR_gettid);

	printf("[R] Socket: %d\n", sock);
	while (1)
	{
		printf("[%d] Receiving....\n", pid);

		// Receive CAN frame
		ReceiveCanFrame(sock);
	}
}

int main(void)
{
	int sock = InitCanInterface("vcan0");
	if (sock < 0)	return -1;
	else printf("[M] Socket: %d\n", sock);

	pthread_t threads[2];

	// First Transmit CAN frame
	uint8_t can_data[CAN_FRAME_MAX_LEN] = { 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 };
	TransmitCanFrame(sock, CAN_ID, can_data, sizeof(can_data));

	pthread_create(&threads[0], NULL, tThread, sock);
	pthread_create(&threads[1], NULL, rThread, sock);

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	return 0;
}