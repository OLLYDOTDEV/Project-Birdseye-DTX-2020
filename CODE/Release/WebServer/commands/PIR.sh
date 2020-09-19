#!/bin/bash 
timeout 5 sudo ps -ef | grep ./../RPI/RPI | grep -v grep | awk '{print $2}' | xargs kill
sudo ./../RPI/RPI PIR 
