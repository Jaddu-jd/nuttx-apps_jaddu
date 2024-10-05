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

#include <nuttx/config.h>
#include <stdio.h>
#include <fcntl.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * hello_main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  printf("Hello, World!!\n");
  uint32_t counter =0;
  uint8_t fd, ret,data ;
  uint8_t data1[3500];
  uint8_t command[] ={'C','A','M','O','N'};
  fd = open("/dev/ttyS1", O_RDWR);
    write(fd, command, sizeof(command));

  while(1){
  ret = read(fd, &data, 1);
  // counter++;
  data1[counter] = data;
  // if(counter %100 ==0){
  //   printf("Counter is %d\n", counter);
  // }
  if(data1[counter-1] == 0xff && data1[counter] ==0xd9){
    printf(" %02x %02x\n",data1[counter -1], data1[counter]);
    break;
  }
  counter++;
  // printf("%d ",data);
  }
  close(fd);
  printf("Total size of data received : %d \n", counter);

  
  return 0;
}
