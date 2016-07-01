#!/bin/bash
#Generate pcap files
cd /vagrant/provisioning/UNIFY-ER-aggregator/traffic_generator/
./tgen-psk.py --shape_file shape_example --ipdst 10.0.2.3,10.0.3.3,10.0.4.3 er1.pcap
./tgen-psk.py --shape_file shape_example --ipdst 10.0.1.3,10.0.3.3,10.0.4.3 er2.pcap
./tgen-psk.py --shape_file shape_example --ipdst 10.0.1.3,10.0.2.3,10.0.4.3 er3.pcap
./tgen-psk.py --shape_file shape_example --ipdst 10.0.1.3,10.0.2.3,10.0.3.3 er4.pcap

#Generate traffic
sudo tcpreplay -i eth1 er1.pcap &
sudo tcpreplay -i eth2 er2.pcap &
sudo tcpreplay -i eth3 er3.pcap &
sudo tcpreplay -i eth4 er4.pcap &