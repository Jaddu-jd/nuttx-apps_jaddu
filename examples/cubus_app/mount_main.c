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

// #include "/home/sangam/Desktop/jd/nuttx/drivers/mtd/smart.c"

// #include "nuttx/fs/fs.h"
// #include "/home/sangam/Desktop/jd/nuttx/include/nuttx/fs/fs.h"

#include <nuttx/fs/smart.h>
#include <nuttx/fs/fs.h>
#include <../apn_bus/fs/smartfs/>

#define MOUNT_POINT "/m1"
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
  //this one is for littlefs
  //ret = nx_mount("/dev/flash", "/mnt/flash", "littlefs", 0, "autoformat");

  // Mount the SMART filesystem
  ret = mount("/dev/smart0p0", MOUNT_POINT, "smartfs", 0, NULL);
  if (ret >= 0) {
    printf("Mount successful\n");
  } else {
    perror("Error: Mount failed");
    return 1;
  }

  // // Open a file for writing
  // int fd = open(MOUNT_POINT "/first.txt", O_WRONLY | O_CREAT, 0666);
  // if (fd < 0) {
  //   perror("Failed to open file");
  //   umount(MOUNT_POINT);
  //   return 1;
  // } else {
  //   printf("Opened file\n");
  // }
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
  // ret = write(fd, DATA, strlen(DATA));
  // if (ret < 0) {
  //   perror("Failed to write the data");
  //   close(fd);
  //   umount(MOUNT_POINT);
  //   return 1;
  // } else {
  //   printf("Data written successfully\n");
  // }

  // // Close the file after writing
  // close(fd);

  // // Reopen the file for reading
  // fd = open(MOUNT_POINT "/test123.txt", O_RDONLY);
  // if (fd < 0) {
  //   perror("Failed to open file for reading");
  //   umount(MOUNT_POINT);
  //   return 1;
  // }

  // Read data from the file
  // char texts[30];
  // ret = read(fd, texts, sizeof(texts) - 1);
  // if (ret < 0) {
  //   perror("Failed to read the data");
  //   close(fd);
  //   umount(MOUNT_POINT);
  //   return 1;
  // } else {
  //   texts[ret] = '\0';  // Null-terminate the string
  //   printf("Data read successfully. Data is: %s\n", texts);
  // }

  // // Close the file after reading
  // close(fd);
 

  // Unmount the filesystem
  ret = umount(MOUNT_POINT);
  if (ret < 0) {
    perror("Error: Unmount failed");
    return 1;
  } else {
    printf("Unmount successful\n");
  }

  return 0;
}