 
#############################################################################
#
# Makefile for librf24 examples on Linux
#
# License: GPL (General Public License)
# Author:  gnulnulf <arco@appeltaart.mine.nu> | Edited OLLYDOTDEV  
# Date:    2013/02/07 (version 1.0            | 2020/09/6
#
# Description:
# ------------
# use make s
#

ifeq ($(wildcard /projects/rf24libs/RF24/Makefile.inc), )
        $(error Configuration not found. Run ./configure in NRF24 LIB first)
endif

include /projects/rf24libs/RF24/Makefile.inc

# define all programs
PROGRAMS = Communication_NRF24_Test

include /projects/Project-Birdseye-DTX-2020/CODE/libraries/RF24/examples_linux/Makefile.examples

