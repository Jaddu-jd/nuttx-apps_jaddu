/****************************************************************************
 * apps/examples/hello/hello_main.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

// #include <nuttx/config.h>
// #include "gpio_definitions.h"
// #include <nuttx/fs/ioctl.h>
// #include <stdio.h>
// #include <fcntl.h>
// #include <syslog.h>
// #include <stdlib.h>
// // #include <tioctl.h>
// /****************************************************************************
//  * Public Functions
//  ****************************************************************************/

// /****************************************************************************
//  * hello_main
//  ****************************************************************************/
// // void handshake(){
// //   uint8_t command_in[7]={'\0'};
// //   int ret, fd1 ,i;
// //   fd1 = open("/dev/ttyS1", O_RDWR);
// //   do{
// //     ret = read(fd1, &command_in[i], 1);
// //     if(ret >= 0){
// //       write(fd1, &command_in[i],1);
// //     }
// //     // memset(command_in, '\0', sizeof(command_in));
// //     // ret = read(fd1, command_in, sizeof(command_in));
// //     // if(command_in[0] == 0x53 | command_in[1] ==0x53){
// //     //   break;
// //     // }
// //   }while(i <=7);
// //       printf("%s \n", command_in);
 
  
// //   if(ret >= 0){
// //     ret = write(fd1, command_in, sizeof(command_in));
// //     if(ret >= 0){
// //       printf("Data sent back to OBC\n");
// //     }
// //   }
// //   ioctl(fd1, TCFLSH, 2);
// //   ioctl(fd1, TCDRN, NULL);
// //   close(fd1);

// // }



// int main(int argc, FAR char *argv[])
// {

//   printf("Hello, World!!\n");
//   uint32_t counter =0, counter2=0;
//   uint8_t fd, ret,data ;
//   uint8_t four[4];
//   uint8_t data1[3500], data2[3500];
//   uint8_t command[] ={'C','A','M','O','N'};
//   // sleep(2);
//   printf("****************************Starting handshake sequence********************\n");
//   handshake();
//   sleep(1);
//   handshake();
//   gpio_write(GPIO_OCP_EN, true);
//   sleep(3);
  
//   printf("command verified\n Enabling OCP\n");
//   // gpio_write(GPIO_OCP_EN, true);
//   fd = open("/dev/ttyS3", O_RDONLY);
//     // write(fd, command, sizeof(command));
//   while(1)
//   {
//   ret = read(fd, &data, 1);
//   data1[counter] = data;
//   if(data1[counter-1] == 0xff && data1[counter] ==0xd9){
//     // printf(" %02x %02x\n",data1[counter -1], data1[counter]);
//     break;
//   }
//   counter++;
//   }
//   close(fd);

//   printf("\n*******************Disabling OCP\n");

//   gpio_write(GPIO_OCP_EN, false);

//   fd=open("/dev/ttyS1",O_WRONLY);
//   printf("\nTotal size of data received : %d \n\n", counter);
//   for(int16_t i=0; i<=counter ;i++){
//     printf("%02x ", data1[i]);
//     write(fd, &data1[i], 1);
//   }
// // printf("-----------------RGB data starting-------\n");
// //  for(int16_t i=0; i<=counter2 ;i++){
// //     printf("%02x ", data2[i]);
// //     write(fd, &data2[i], 1);
// //   }
//   uint8_t d[]={0xff, 0xd9};
//   write(fd, d, 2);
//   write(fd, d, 2);

// printf("\nReached cpt.1\n");
//   write(fd, data1, counter-1);
//   close(fd);
  
//   return 0;
// }


#include <nuttx/config.h>
#include <nuttx/serial/serial.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include "gpio_definitions.h"
#include <syslog.h>


#define BUFFER_SIZE 3500
#define UART_IN_DEVICE "/dev/ttyS3"
#define UART_OUT_DEVICE "/dev/ttyS1"

void handshake(); // Ensure handshake is defined elsewhere
int gpio_write(uint32_t pin, uint8_t mode); // Ensure GPIO write function is defined

int main(int argc, FAR char *argv[])
{
    printf("Hello, World!!\n");
    uint32_t counter = 0;
    uint8_t fd, ret, data;
    uint8_t data1[BUFFER_SIZE];
    uint8_t command[] = {'C', 'A', 'M', 'O', 'N'};

    printf("****************************Starting handshake sequence********************\n");
    handshake();
    sleep(1);
    handshake();
    sleep(6);

    gpio_write(GPIO_OCP_EN, true);
    
    printf("Command verified\nEnabling OCP\n");

    // Open UART to read data
    fd = open(UART_IN_DEVICE, O_RDONLY);
    if (fd < 0) {
        printf("Failed to open %s: %d\n", UART_IN_DEVICE, errno);
        return -1;
    }

    // Read data until a specific sequence is encountered
    while (counter < BUFFER_SIZE) {
        ret = read(fd, &data, 1);
        if (ret < 0) {
            printf("Read error: %d\n", errno);
            break;
        } else if (ret == 0) {
            // No data read
            continue;
        }

        data1[counter] = data;

        // Break if the termination sequence is detected
        if (counter > 0 && data1[counter - 1] == 0xff && data1[counter] == 0xd9) {
            break;
        }

        counter++;
    }
    
    close(fd);
    printf("\n*******************Disabling OCP\n");
    gpio_write(GPIO_OCP_EN, false);

    // Open UART to write data
    fd = open(UART_OUT_DEVICE, O_WRONLY);
    if (fd < 0) {
        printf("Failed to open %s: %d\n", UART_OUT_DEVICE, errno);
        return -1;
    }

    printf("\nTotal size of data received: %d \n\n", counter);
    
    // Write the received data to the output UART
    for (uint32_t i = 0; i < counter; i++) {
        printf("%02x ", data1[i]);
        write(fd, &data1[i], 1);
    }

    // Send termination sequence twice
    uint8_t termination_sequence[] = {0xff, 0xd9};
    write(fd, termination_sequence, sizeof(termination_sequence));
    write(fd, termination_sequence, sizeof(termination_sequence));

    printf("\nReached cpt.1\n");
    close(fd);
    
    return 0;
}

void handshake() {
    uint8_t command_in[7] = {'\0'};
    int ret, fd1, i = 0;

    // Open the UART device
    fd1 = open("/dev/ttyS1", O_RDWR);
    if (fd1 < 0) {
        printf("Failed to open UART device: %d\n", errno);
        return;
    }

    // Read and echo until the buffer is filled or a specific command is detected
    while (i < 7) {
        ret = read(fd1, &command_in[i], 1);
        if (ret < 0) {
            if (errno == EAGAIN) {
                // No data available, continue polling
                continue;
            } else {
                printf("Read error: %d\n", errno);
                break;
            }
        } else if (ret > 0) {
            // Echo the received byte back
            write(fd1, &command_in[i], 1);
            i++;
        }
    }

    // Print the received command
    printf("Received command: ");
    for (int j = 0; j < i; j++) {
        printf("%02X ", command_in[j]);
    }
    printf("\n");

    // Check if specific condition is met (e.g., first byte is 0x53)
    if (command_in[0] == 0x53 | command_in[1] == 0x53) {
        printf("Handshake successful with command: %02X\n", command_in[0]);
    } else {
        printf("Handshake failed, unexpected command: %02X\n", command_in[0]);
    }

    // Clean up
    // ioctl(fd1, TCFLUSH, 0); // Clear the input buffer
    close(fd1);
}

int gpio_write(uint32_t pin, uint8_t mode)
{

  gpio_config_s gpio_numval;
  int fd = open(ETX_LED_DRIVER_PATH, O_WRONLY);
  if (fd < 0)
  {
    syslog(LOG_ERR, "Error opening %s for GPIO WRITE...", ETX_LED_DRIVER_PATH);
    close(fd);
    return -1;
  }
  gpio_numval.gpio_num = pin;
  gpio_numval.gpio_val = mode;
  if (gpio_numval.gpio_val > 1 || gpio_numval.gpio_num < 0)
  {
    syslog(LOG_ERR, "Undefined GPIO pin or set mode selected...\n");
    return -2;
  }
  int ret = write(fd, (const void *)&gpio_numval, sizeof(gpio_config_s));
  close(fd);
  if (ret < 0)
  {
    syslog(LOG_ERR, "Unable to write to gpio pin...\n");
  }
  return ret;
}