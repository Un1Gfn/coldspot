#!/bin/bash

{

  F1=/home/darren/coldspot
  T1=/home/darren/wt88047.pmbootstrap/chroot_buildroot_aarch64/home/darren/coldspot.bindmount

  function err {
    printf "\e[31m%s\e[0m\n" "$1"
    exit
  }

  mkdir -p "$T1"
  [ -z "$(ls -A "$T1")" ] || err "dest dir not empty"
  sudo mount -v --bind "$F1" "$T1" || err "bind mount failed"
  alacrittytitle.sh 'darren@chroot_buildroot_aarch64'
  pmbootstrap chroot -b aarch64
  echo

  UMOUNT=(sudo umount -v "$T1")
  SHUTDOWN=(pmbootstrap shutdown)
  echo ":; ${UMOUNT[@]}"
  echo
  echo ":; ${SHUTDOWN[@]}"
  echo
  "${UMOUNT[@]}" || err "unmount failed"
  "${SHUTDOWN[@]}" || err "shutdown failed"

}; exit
