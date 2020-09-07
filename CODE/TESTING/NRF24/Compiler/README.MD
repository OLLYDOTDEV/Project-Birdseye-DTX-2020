# used to compile NRF24 lib code into C++

as seen with https://github.com/nRF24/RF24/issues/622
`g++ -marm -march=armv6zk -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=soft -Ofast -Wall -pthread -I/usr/local/include/RF24/.. -I.. -L/usr/local/lib BULK_READ.cpp -lrf24 -o BULK_READ`
                                                                                    


