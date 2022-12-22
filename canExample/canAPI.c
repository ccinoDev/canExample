#include "canAPI.h"

// Initialize CAN I/F
int InitCanInterface(const char* ifname)
{
	// Create CAN socket
	int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (sock == -1)
	{
		printf("Fail to create can socket for %s - %m\n", ifname);
		return -1;
	}
	printf("Success to create can socket for %s\n", ifname);

	// Get CAN I/F number
	struct ifreq ifr;
	strcpy(ifr.ifr_name, ifname);
	int ret = ioctl(sock, SIOCGIFINDEX, &ifr);
	if (ret == -1)
	{
		perror("Fail to get can interface index - ");
		return -1;
	}
	printf("Success to get can interface index: %d\n", ifr.ifr_ifindex);

	// Bind CAN socket
	struct sockaddr_can addr;
	memset(&addr, 0, sizeof(addr));

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == -1)
	{
		perror("Fail to bind can socket -");
		return -1;
	}
	printf("Success to bind can socket\n");

	return sock;
}

// Send CAN frame
int TransmitCanFrame(const int sock, const uint32_t id, const uint8_t* data, const size_t data_len)
{
	// Set CAN frame to send
	struct can_frame frame;
	frame.can_id = id & 0x1fffffff;
	frame.can_id |= (1 << 31);
	memcpy(frame.data, data, data_len);
	frame.can_dlc = data_len;

	// Send
	int tx_bytes = write(sock, &frame, sizeof(frame));
	if (tx_bytes == -1)
	{
		perror("Fail to transmit can frame -");
		return -1;
	}
	printf("Success to transmit can frame - %d bytes is transmitted\n", tx_bytes);
	return 0;
}

// Receive CAN frame
int ReceiveCanFrame(const int sock)
{
	struct can_frame frame;

	int rx_bytes = read(sock, &frame, sizeof(frame));
	if (rx_bytes < 0)
	{
		perror("Fail to receive can frame - ");
		return -1;
	}
	else if (rx_bytes < (int)sizeof(struct can_frame))
	{
		printf("Incomplete can frame is received - rx_bytes: %d\n", rx_bytes);
		return -1;
	}
	else if (frame.can_dlc > CAN_FRAME_MAX_LEN)
	{
		printf("Invalid dlc: %u\n", frame.can_dlc);
		return -1;
	}

	// Process data case by Frame
	if (((frame.can_id >> 29) & 1) == 1)
	{
		printf("Error frame is received\n");
	}
	else if (((frame.can_id >> 30) & 1) == 1)
	{
		printf("RTR frame is received\n");
	}
	else
	{
		if (((frame.can_id >> 31) & 1) == 1)
		{
			printf("11bit long std can frame is received\n");
		}
		else
		{
			printf("29bit long ext can frame is received\n");
		}

		// Process frame
		printf("TODO: Process frame...\n");
	}

	printf("## End of Receiving ##\n");

	return 0;
}
