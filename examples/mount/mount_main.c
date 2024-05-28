// /****************************************************************************
//  * apps/examples/mount/mount_main.c
//  *
//  * Licensed to the Apache Software Foundation (ASF) under one or more
//  * contributor license agreements.  See the NOTICE file distributed with
//  * this work for additional information regarding copyright ownership.  The
//  * ASF licenses this file to you under the Apache License, Version 2.0 (the
//  * "License"); you may not use this file except in compliance with the
//  * License.  You may obtain a copy of the License at
//  *
//  *   http://www.apache.org/licenses/LICENSE-2.0
//  *
//  * Unless required by applicable law or agreed to in writing, software
//  * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
//  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
//  * License for the specific language governing permissions and limitations
//  * under the License.
//  *
//  ****************************************************************************/

// /****************************************************************************
//  * Included Files
//  ****************************************************************************/

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
// #include "/home/sangam/Desktop/jd/nuttx/fs/smartfs/smartfs.h"
// #include "/home/sangam/Desktop/jd/nuttx/drivers/mtd/smart.c"
#include "nuttx/fs/fs.h"
// #include "/home/sangam/Desktop/jd/nuttx/include/nuttx/fs/fs.h"

#define MOUNT_POINT "mtd0"
#define MOUNT_POINT1 "mtd1"
#define MOUNT_POINT2 "mtd2"
#define MOUNT_POINT3 "mtd3"


#define DATA "hello from nuttx 28 may"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mount_main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  int ret, fd;
  // ret  = nx_mount("/dev/flash", "/mnt/flash", "littlefs", 0, "autoformat");
  // Mount the SMART filesystem
  // struct inode             *fs_blkdriver;
  // char                     *fs_rwbuffer, *fs_workbuffer;
  // uint8_t fs_rootsector;
  // struct smartfs_mountpt_s fs = {
  //   .fs_mounted = false,
  //   // .fs_blkdriver = fs_blkdriver,
  //   // fs_rwbuffer = fs_rwbuffer,
  //   .fs_rootsector = fs_rootsector,
  // } ;
  // ret = smartfs_mount(&fs, 3);
  // if (ret >= 0) {
  //   printf("Mount successful 22\n");
  // } else {
  //   printf("Error: Mount failed %d",ret);
  //   return 1;
  // }
  ret = mount("/dev/smart0p0d1", MOUNT_POINT, "smartfs", 0, NULL);
  if (ret >= 0) {
    printf("Mount successful and check ls /dev\n");
  } else {
    perror("Error: Mount failed");
    return 1;
  }

  // Open a file for writing
  fd = open(MOUNT_POINT "/numbers.txt", O_WRONLY | O_CREAT | O_APPEND, 0755);
  if (fd < 0) {
    perror("Failed to open file");
    umount(MOUNT_POINT);
    return 1;
  } else {
    printf("Opened file\n");
  }


  // struct smart_struct_s *smart;
  // struct inode *inode;
  // ret = smart_open(inode);
  // if (ret >= 0) {
  //   printf("main smartMount successful\n");
  // } else {
  //   perror("Error:smart main Mount failed");
  //   return 1;
  // }
  // Write data to the file
  // char DATA1[] = argv[0];
  // int j;
  // for(j = 0; j<argc; j++){
  //   printf("Got the argument in %d as %s",j , argv[j]);
  // }
 char buffer[3]; int j;
  // printf("the argument received is %d",argv[0]);
  ret = write(fd, DATA, strlen(DATA));

  if (ret < 0) {
    perror("Failed to write the data");
    close(fd);
    umount(MOUNT_POINT);
    return 1;
  } else {
    printf("Data written successfully\n");
  }

  // Close the file after writing
  close(fd);

  // Reopen the file for reading
  fd = open(MOUNT_POINT "/numbers.txt", O_RDONLY);
  if (fd < 0) {
    perror("Failed to open file for reading");
    umount(MOUNT_POINT);
    return 1;
  }

  // Read data from the file
  char texts[30];
  ret = read(fd, texts, sizeof(texts) - 1);
  if (ret < 0) {
    perror("Failed to read the data");
    close(fd);
    umount(MOUNT_POINT);
    return 1;
  } else {
    texts[ret] = '\0';  // Null-terminate the string
    printf("Data read successfully. Data is: %s\n", texts);
  }

  // Close the file after reading
  close(fd);
 

  return 0;

}




// #include <nuttx/config.h>
// #include <stdio.h>
// #include <sys/mount.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <string.h>
// #include <errno.h>
// #include </home/sangam/Desktop/jd/nuttx/fs/littlefs/littlefs/lfs.h>
// #include <nuttx/fs/fs.h>

// #define MOUNT_POINT "/mnt/"
// #define DATA_TO_WRITE "Hello from NuttX app!"

// static int read_block(const struct lfs_config *c, lfs_block_t block,
//                       lfs_off_t offset, void *buffer, lfs_size_t size) {
//   // Implement read operation for your specific block device
//   // Example: return block_device_read(c->context, block, offset, buffer, size);
//   return 0; // Return 0 on success
// }

// static int prog_block(const struct lfs_config *c, lfs_block_t block,
//                       lfs_off_t offset, const void *buffer, lfs_size_t size) {
//   // Implement program operation for your specific block device
//   // Example: return block_device_prog(c->context, block, offset, buffer, size);
//   return 0; // Return 0 on success
// }

// static int erase_block(const struct lfs_config *c, lfs_block_t block) {
//   // Implement erase operation for your specific block device
//   // Example: return block_device_erase(c->context, block);
//   return 0; // Return 0 on success
// }

// static int lfs_sync(const struct lfs_config *c) {
//   // Implement sync operation for your specific block device
//   // Example: return block_device_sync(c->context);
//   return 0; // Return 0 on success
// }

// static struct lfs lfs;
// static struct lfs_file file;

// // Configuration of the filesystem provided by this struct
// const struct lfs_config cfg = {
//   // Block device operations
//   .read  = read_block,
//   .prog  = prog_block,
//   .erase = erase_block,
//   .sync  = lfs_sync,

//   /* block device configuration */
//   .read_size = 1,            // Adjust based on the flash memory page size
//   .prog_size = 256,          // Adjust based on the flash memory programming size
//   .block_size = 4096,        // Adjust based on the flash memory block size
//   .block_count = 256,        // Adjust based on the total number of blocks in the flash memory
//   .block_cycles = 500,       // Set according to flash memory specifications or LittleFS defaults
//   .cache_size = 16,          // Adjust based on available RAM and LittleFS performance requirements
//   .lookahead_size = 16,      // Adjust based on available RAM and LittleFS performance requirements
//   .name_max = 255,           // Maximum length of filenames supported
//   .file_max = 1024,          // Maximum number of files supported
//   .attr_max = 1024,
// };

// int main(int argc, char *argv[]) {
//   int ret;

//   // Mount the LittleFS filesystem
//   ret = lfs_mount(&lfs, &cfg);
//   if (ret < 0) {
//     printf("Mount unsuccessful, formatting...%d\n",ret);

//     // If mount fails, format the filesystem and try mounting again
//     ret = lfs_format(&lfs, &cfg);
//     if (ret < 0) {
//       printf("Failed to format LittleFS: %d\n", ret);
//       return 1;
//     }

//     ret = lfs_mount(&lfs, &cfg);
//     if (ret < 0) {
//       printf("Failed to mount LittleFS after format: %d\n", ret);
//       return 1;
//     }
//   }

//   printf("Mount successful\n");

//   // Open a file for writing (create if it doesn't exist)
//   ret = lfs_file_open(&lfs, &file, "data.txt", LFS_O_WRONLY | LFS_O_CREAT);
//   if (ret < 0) {
//     printf("Failed to open file: %d\n", ret);
//     lfs_unmount(&lfs);
//     return 1;
//   }

//   // Write data to the file
//   ret = lfs_file_write(&lfs, &file, DATA_TO_WRITE, strlen(DATA_TO_WRITE));
//   if (ret < 0) {
//     printf("Failed to write data: %d\n", ret);
//     lfs_file_close(&lfs, &file);
//     lfs_unmount(&lfs);
//     return 1;
//   }

//   printf("Successfully wrote data to %s/data.txt\n", MOUNT_POINT);

//   // Close the file
//   lfs_file_close(&lfs, &file);

//   // Reopen the file for reading
//   ret = lfs_file_open(&lfs, &file, "data.txt", LFS_O_RDONLY);
//   if (ret < 0) {
//     printf("Failed to open file for reading: %d\n", ret);
//     lfs_unmount(&lfs);
//     return 1;
//   }

//   // Read data from the file
//   char buffer[256];
//   memset(buffer, 0, sizeof(buffer));
//   ret = lfs_file_read(&lfs, &file, buffer, sizeof(buffer) - 1);
//   if (ret < 0) {
//     printf("Failed to read data: %d\n", ret);
//     lfs_file_close(&lfs, &file);
//     lfs_unmount(&lfs);
//     return 1;
//   }

//   printf("Successfully read data: %s\n", buffer);

//   // Close the file
//   lfs_file_close(&lfs, &file);

//   // Unmount the filesystem
//   ret = lfs_unmount(&lfs);
//   if (ret < 0) {
//     printf("Error: Unmount failed: %d\n", ret);
//     return 1;
//   }

//   printf("Unmount successful\n");
//   return 0;
// }
