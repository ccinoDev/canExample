#pragma once
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define CAN_FRAME_MAX_LEN 8		// MAX len for CAN frame
#define CAN_ID 0x555			// CAN ID

int InitCanInterface(const char* ifname);																// Initialize CAN I/F
int TransmitCanFrame(const int sock, const uint32_t id, const uint8_t* data, const size_t data_len);	// Send CAN frame
int ReceiveCanFrame(const int sock);																	// Receive CAN frame
