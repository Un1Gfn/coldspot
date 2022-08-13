#!/bin/sh

err(){
  printf "\e[31m%s\e[0m\n" "$1"
  exit 1
}

{

  source ./transferconf
  REMOTE=wt88047usb

  # run on host only
  [ x86_64 = "$(uname -m)" ] || err 'need x86_64'

  # usb ether
  ip -o -4 addr show dev enp0s20f0u2 | grep inet >/dev/null || sudo dhcpcd enp0s20f0u2

  if [ -n "$RUN" ]; then

    # do transfer
    {
      # clear
      # tput reset
      # echo "run"
      # echo "[$(date +%T)]"
      ssh user@"$REMOTE" rm -f /tmp/"$BIN"
      scp "$O/$BIN" "$REMOTE":/user/local/bin/
      # ssh root@"$REMOTE" ash -c '"echo \"[$(date)]\"; /tmp/"$BIN"; echo [exit $?]"'
    } 1>&2

  else

    # issue transfer
    echo 'waiting for packets...'
    # busybox nc -l -p "$PORT"
    busybox nc -lk -p "$PORT" -e env RUN=1 "$0"
    err 'should never reach here'

  fi

}; exit
