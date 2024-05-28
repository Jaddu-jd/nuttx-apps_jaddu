/****************************************************************************
 * apps/examples/mount/mount_main.c
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

#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/statfs.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include "/home/sangam/Desktop/jd/nuttx/fs/smartfs/smartfs.h"

#define MOUNT_POINT "/mount/test"
#define DATA "hello from nuttx apps"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mount_main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  int ret;

  // Mount the SMART filesystem
  ret = mount("/dev/smart0p0d1", MOUNT_POINT, "smartfs", 0, NULL);
  if (ret >= 0) {
    printf("Mount successful\n");
  } else {
    printf("Error: Mount failed\n");
    return 1;
  }

  // Open a file for read and write
  int fd = open(MOUNT_POINT "/test123.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (fd < 0) {
    printf("Failed to open file: %d\n", errno);
    // umount(MOUNT_POINT);
    return 1;
  } else {
    printf("Opened file\n");
  }

  // Write data to the file
  ret = write(fd, DATA, strlen(DATA));
  if (ret < 0) {
    printf("Failed to write the data: %d\n", errno);
    close(fd);
    // umount(MOUNT_POINT);
    return 1;
  } else {
    printf("Data written successfully\n");
  }
  umount(MOUNT_POINT);
  return 0;
}
