/****************************************************************************
 * apps/examples/cubus_app/MT25QL01_Flash_write.c
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
 * Name: open_to_write
 ****************************************************************************/

void open_to_write(FAR char *target, FAR char *filename){
    int fd = open(target, filename, O_WRONLY | O_CREAT, 0666);
    if (fd < 0) {
        perror("ERROR: Failed to open file");
        umount(target);
        return -1;
    } else {
        printf("Opened file\n");
        return OK;
    }

}

/****************************************************************************
 * Name: mount_fileystem
 ****************************************************************************/

int mount_filesystem(FAR char *source, FAR char *target, FAR char *fs){
    int ret;
    ret  = mount(source, target, fs, 0, NULL);
    if (ret<0){
        perror("Error: Mount failed\r\n");
        return ret;
    }else{
        printf("SMARTFS: mount successful\r\n");
        return ret;
    }
}


/****************************************************************************
 * Name: write_data
 ****************************************************************************/

int write_data(){

}


/****************************************************************************
 * Name: read_data
 ****************************************************************************/

int read_data(){

}




