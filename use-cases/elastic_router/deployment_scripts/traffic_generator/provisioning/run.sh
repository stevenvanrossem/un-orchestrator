#!/bin/bash
#Generate pcap files
echo "generating pcap file trace1"
echo "generating pcap file trace2"
echo "generating pcap file trace3"
echo "generating pcap file trace4"

#Generate traffic
sudo tcpreplay -i eth1 /vagrant/provisioning/trace1.pcap &
sudo tcpreplay -i eth2 /vagrant/provisioning/trace2.pcap &
sudo tcpreplay -i eth3 /vagrant/provisioning/trace3.pcap &
sudo tcpreplay -i eth4 /vagrant/provisioning/trace4.pcap &