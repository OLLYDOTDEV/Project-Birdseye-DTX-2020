#!/bin/bash 
sudo ps aux | grep -ie RPI | grep -v grep | awk '{print $2}' | xargs kill -9

