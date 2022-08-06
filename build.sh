#!/bin/sh

err(){
  printf "\e[31m%s\e[0m\n" "$1"
  exit 1
}

{

  PORT=17629
  REMOTE=wt88047usb
  ACT="$(basename "$0")"
  LIBS='libnm libelogind'

  if [ "$ACT" = 'build.sh' ]; then

    [ aarch64 = "$(uname -m)" ] || err 'need aarch64'
    # shellcheck disable=SC2046
    gcc -Wall -Wextra -std=gnu11 $(pkg-config --cflags $LIBS) -o coldspot main.c $(pkg-config --libs $LIBS)

  elif [ "$ACT" = 'run.sh' ]; then

    [ x86_64 = "$(uname -m)" ] || err 'need x86_64'
    ip -o -4 addr show dev enp0s20f0u2 | grep inet >/dev/null || sudo dhcpcd enp0s20f0u2
    if [ -n "$RUN" ]; then
      # echo "run" 1>&2
      clear 1>&2
      tput reset 1>&2
      ssh user@"$REMOTE" rm -f /tmp/coldspot
      scp -q coldspot "$REMOTE":/tmp/ 1>&2
      ssh root@"$REMOTE" ash -c '/tmp/coldspot; echo [exit $?];' 1>&2
    else
      echo 'waiting for packets...'
      # busybox nc -l -p "$PORT"
      busybox nc -lk -p "$PORT" -e env RUN=1 ./run.sh
      err 'should never reach here'
    fi

  else

    err 'unrecognized $0'

  fi

}