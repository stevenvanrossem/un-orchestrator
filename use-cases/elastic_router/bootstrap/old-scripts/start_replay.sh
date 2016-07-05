#!/bin/bash
LOOPS=9999
if [ "$(whoami)" != "root" ]; then
  echo "Sorry, you need to sudo this!"
  exit 1
fi

echo "starting 4 tcp replays, looping $LOOPS times, "
ip netns exec sap1 tcpreplay -i veth0 -l $LOOPS -K ../traffic/er1.pcap &
ip netns exec sap2 tcpreplay -i veth1 -l $LOOPS -K ../traffic/er2.pcap &
ip netns exec sap3 tcpreplay -i veth2 -l $LOOPS -K ../traffic/er3.pcap &
ip netns exec sap4 tcpreplay -i veth3 -l $LOOPS -K ../traffic/er4.pcap 
# if ctrl-c kill all
killall -9 tcpreplay
