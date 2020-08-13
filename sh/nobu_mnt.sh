#!/bin/bash

sudo mount -t cifs --verbose -o rw,user=kobayash,uid=`id -u`,gid=`id -g`,vers=2.0 //fs-1/kobayash ~/mnt/kobayash_miho
sudo mount -t cifs --verbose -o rw,user=kobayash,uid=`id -u`,gid=`id -g`,vers=2.0 //fs-1/np1a/cagra/gr/2016campaign/ana/koba/GRUTinizer_renewed_jisaak_ver_20200525 ~/mnt/GRUTinizer
