#!/bin/sh

/etc/uhubon.sh device >> /tmp/ncm.log 2>&1
/etc/run_usb.sh probe ncm >> /tmp/ncm.log 2>&1
if test -e /usr/bin/burnd; then
  /etc/run_usb.sh probe acm >> /tmp/ncm.log 2>&1
fi
/etc/run_usb.sh start ncm >> /tmp/ncm.log 2>&1

sleep 0.5
ifconfig usb0 192.168.42.1

count=`ps | grep dnsmasq | grep -v grep | wc -l`
if [ ${count} -lt 1 ] ;then
  echo "/etc/init.d/S80dnsmasq start" >> /tmp/ncm.log 2>&1
  /etc/init.d/S80dnsmasq start >> /tmp/ncm.log 2>&1
fi

sleep 2
mkdir -p /lib/firmware
if test -e /usr/bin/burnd; then
  burnd &
  if test -e /lib/firmware/arduino.elf; then
    sleep 2
    echo stop  > /sys/class/remoteproc/remoteproc0/state
    echo start > /sys/class/remoteproc/remoteproc0/state
  fi
fi
