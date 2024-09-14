/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2022, halal-beef <umer.uddin@mentallysanemainliners.org>
 */

#include <libusb-1.0/libusb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <UnifiedFlashingPlatformCommands.h>

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
    }

        for (int i = 0; i < bytesTransferred; i++)
    {
        printf("%02x ", buffer[i]);  // Print each byte as a two-digit hex value
    }
    printf("\n");
    return result;
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

