/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2024, halal-beef <umer.uddin@mentallysanemainliners.org>
 */

#include <libusb-1.0/libusb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <UnifiedFlashingPlatformCommands.h>

void ThrowFlashError(int errorCode)
{
    char *subMessage;

    switch (errorCode)
    {
    case 0x0008:
        subMessage = "Unsupported protocol / Invalid options";
        break;
    case 0x000F:
        subMessage = "Invalid sub block count";
        break;
    case 0x0010:
        subMessage = "Invalid sub block length";
        break;
    case 0x0012:
        subMessage = "Authentication required";
        break;
    case 0x000E:
        subMessage = "Invalid sub block type";
        break;
    case 0x0013:
        subMessage = "Failed async message";
        break;
    case 0x1000:
        subMessage = "Invalid header type";
        break;
    case 0x1001:
        subMessage = "FFU header contains unknown extra data";
        break;
    case 0x0001:
        subMessage = "Couldn't allocate memory";
        break;
    case 0x1106:
        subMessage = "Security header validation failed";
        break;
    case 0x1105:
        subMessage = "Invalid hash table size";
        break;
    case 0x1104:
        subMessage = "Invalid catalog size";
        break;
    case 0x1103:
        subMessage = "Invalid chunk size";
        break;
    case 0x1102:
        subMessage = "Unsupported algorithm";
        break;
    case 0x1101:
        subMessage = "Invalid struct size";
        break;
    case 0x1100:
        subMessage = "Invalid signature";
        break;
    case 0x1202:
        subMessage = "Invalid struct size";
        break;
    case 0x1203:
        subMessage = "Unsupported algorithm";
        break;
    case 0x1204:
        subMessage = "Invalid chunk size";
        break;
    case 0x1005:
        subMessage = "Data not aligned correctly";
        break;
    case 0x0009:
        subMessage = "Locate protocol failed";
        break;
    case 0x1003:
        subMessage = "Hash mismatch";
        break;
    case 0x1006:
        subMessage = "Couldn't find hash from security header for index";
        break;
    case 0x1004:
        subMessage = "Security header import missing / All FFU headers have not been imported";
        break;
    case 0x1304:
        subMessage = "Invalid platform ID";
        break;
    case 0x1307:
        subMessage = "Invalid write descriptor info";
        break;
    case 0x1306:
        subMessage = "Invalid write descriptor info";
        break;
    case 0x1305:
        subMessage = "Invalid block size";
        break;
    case 0x1303:
        subMessage = "Unsupported FFU version";
        break;
    case 0x1302:
        subMessage = "Unsupported struct version";
        break;
    case 0x1301:
        subMessage = "Invalid update type";
        break;
    case 0x100B:
        subMessage = "Too much payload data, all data has already been written";
        break;
    case 0x1008:
        subMessage = "Internal error";
        break;
    case 0x1007:
        subMessage = "Payload data does not contain all data";
        break;
    case 0x0004:
        subMessage = "Flash write failed";
        break;
    case 0x000D:
        subMessage = "Flash verify failed";
        break;
    case 0x0002:
        subMessage = "Flash read failed";
        break;
    default:
        subMessage = "Unknown Error";
        break;
    }

    printf("Fatal error, %s\n", subMessage);
}

int sendMessage(libusb_device_handle *handle, const char *message)
{
    int bytesTransferred = 0;
    int result = libusb_bulk_transfer(handle, 0x01, (unsigned char *)message, strlen(message), &bytesTransferred, 1000);

    if(result == 0)
    {
        printf("Sent %i bytes, data sent: %s\n", bytesTransferred, message);
    }
    else
    {
        printf("Error sending message: %s\n", libusb_error_name(result));
    }
    return result;
}

int receiveMessage(libusb_device_handle *handle, unsigned char *buffer, int length)
{
    int bytesTransferred = 0;
    int result = libusb_bulk_transfer(handle, 0x81, buffer, length, &bytesTransferred, 1000);  // Note: 0x81 is typical for IN endpoint 1

    if(result == 0)
    {
        buffer[bytesTransferred] = '\0';  // Null-terminate the received data
        printf("Received %i bytes, data received: %s\n", bytesTransferred, buffer);
    }
    else
    {
        printf("Error receiving message: %s\n", libusb_error_name(result));
	    return -300; // Lets not conflict with UFP errors lol
    }

    result = (buffer[6] << 8) + buffer[7];
    return result;
}
void reverse_bytes(uint8_t *data, size_t len) {
    size_t i;
    for (i = 0; i < len / 2; i++) {
        uint8_t temp = data[i];
        data[i] = data[len - i - 1];
        data[len - i - 1] = temp;
    }
}
int main() {
    libusb_context *ctx = NULL;
    int result = libusb_init(&ctx);
    if (result != 0) {
        printf("Error initializing libusb: %s\n", libusb_error_name(result));
        return 1;
    }

    libusb_device_handle *handle = NULL;
    libusb_device **devs;
    ssize_t cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        printf("Error getting device list: %s\n", libusb_error_name(cnt));
        libusb_free_device_list(devs, 1);
        libusb_exit(ctx);
        return 1;
    }

    for (size_t i = 0; i < cnt; ++i) {
        struct libusb_device_descriptor desc;
        result = libusb_get_device_descriptor(devs[i], &desc);
        if (result != 0) {
        printf("Error getting device descriptor: %s\n", libusb_error_name(result));
        continue;
        }

        if (desc.idVendor == 0x045E && desc.idProduct == 0x066B) {
        // Found the target device
        result = libusb_open(devs[i], &handle);
        if (result != 0) {
            printf("Error opening device: %s\n", libusb_error_name(result));
            continue;
        }
        break;
        }
    }

    libusb_free_device_list(devs, 1);

    if (handle == NULL) {
        printf("Target device not found.\n");
        libusb_exit(ctx);
        return 1;
    }

    uint8_t buffer[2048];  // Adjust the size as needed

    sendMessage(handle, HelloSignature);

    receiveMessage(handle, buffer, sizeof(buffer));

    if(strcmp(HelloSignature, buffer) != 0)
    {
        printf("Fatal error, device did not answer properly to the Hello message sent. Exiting.\n");
        libusb_close(handle);
        libusb_exit(ctx);
        exit(-1);
    }

    sendMessage(handle, RebootSignature);

    receiveMessage(handle, buffer, sizeof(buffer));

    // Close the device and exit
    libusb_close(handle);
    libusb_exit(ctx);
    return 0;
}

