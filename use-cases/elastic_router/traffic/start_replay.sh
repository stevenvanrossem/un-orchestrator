#!/bin/bash
LOOPS=9999
echo "starting 4 tcp replays, looping $LOOPS times, "
ip netns exec sap1 tcpreplay -i veth0 -l $LOOPS -K er1.pcap &
ip netns exec sap2 tcpreplay -i veth1 -l $LOOPS -K er2.pcap &
ip netns exec sap3 tcpreplay -i veth2 -l $LOOPS -K er3.pcap &
ip netns exec sap4 tcpreplay -i veth3 -l $LOOPS -K er4.pcap 
# if ctrl-c kill all
killall -9 tcpreplay
