#!/bin/bash
sudo DDNAME=tcp1 KEYFILE=/etc/doubledecker/public-keys.json DEALER=tcp://127.0.0.1:5555 ./ddtcpreplay -K -i dummy0 -l 0 /run/tdata/rise-er1.pcap /run/tdata/high-er1.pcap /run/tdata/fall-er1.pcap /run/tdata/low-er1.pcap  
sudo DDNAME=tcp2 KEYFILE=/etc/doubledecker/public-keys.json DEALER=tcp://127.0.0.1:5555 ./ddtcpreplay -K -i dummy0 -l 0 /run/tdata/rise-er1.pcap /run/tdata/high-er1.pcap /run/tdata/fall-er1.pcap /run/tdata/low-er1.pcap  
sudo DDNAME=tcp3 KEYFILE=/etc/doubledecker/public-keys.json DEALER=tcp://127.0.0.1:5555 ./ddtcpreplay -K -i dummy0 -l 0 /run/tdata/rise-er1.pcap /run/tdata/high-er1.pcap /run/tdata/fall-er1.pcap /run/tdata/low-er1.pcap  
sudo DDNAME=tcp4 KEYFILE=/etc/doubledecker/public-keys.json DEALER=tcp://127.0.0.1:5555 ./ddtcpreplay -K -i dummy0 -l 0 /run/tdata/rise-er1.pcap /run/tdata/high-er1.pcap /run/tdata/fall-er1.pcap /run/tdata/low-er1.pcap  
