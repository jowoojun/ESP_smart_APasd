#!/bin/sh

# initial bakup file
sudo cp /etc/hostapd/hostapd.conf /etc/hostapd/hostapd.initial_bakup

# run change_pwd.c
sudo ./change_pwd $1

# remove conf file and change bak file to conf file
sudo rm /etc/hostapd/hostapd.conf
sudo mv /etc/hostapd/hostapd.bak /etc/hostapd/hostapd.conf

