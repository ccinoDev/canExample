#include "canAPI.h"
#include <pthread.h>

void* tThread()
{
	//pthread_t tid;
	//tid = pthread_self();
	int pid = syscall(__NR_gettid);

	int sock = InitCanInterface("vcan0");
	if (sock < 0) return -1;
	printf("[T] Socket: %d\n", sock);

	while (1)
	{
		uint8_t can_data[CAN_FRAME_MAX_LEN] = {0,};

		printf("[%d] Input Data\n-> ", pid);
		scanf("%s", can_data);

		printf("[%d] ", pid);
		for (int i = 0; i < CAN_FRAME_MAX_LEN; i++)
		{
			printf("0x%02X ", can_data[i]);
		}
		printf("\n");

		TransmitCanFrame(sock, CAN_ID, can_data, sizeof(can_data));		// Transmit CAN frame

		while (getchar() != '\n');
	}
}

void* rThread()
{
	//pthread_t tid;
	//tid = pthread_self();
	int pid = syscall(__NR_gettid);

	int sock = InitCanInterface("vcan0");
	if (sock < 0) return -1;
	printf("[R] Socket: %d\n", sock);

	while (1)
	{
		ReceiveCanFrame(sock);	// Receive CAN frame
		printf("[%d]---------------------\n", pid);
	}
}

int main(void)
{
	pthread_t threads[2];

	pthread_create(&threads[0], NULL, tThread, NULL);
	pthread_create(&threads[1], NULL, rThread, NULL);

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	return 0;
}