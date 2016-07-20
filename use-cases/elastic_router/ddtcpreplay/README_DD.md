Can connect and listen to commands on DoubleDecker

Expects four pcap files as input, which it will cycle through, depending on incoming commands.

publish on "tcpreplay" topic, or send directly. 
"start" -> start replaying traffic
"stop" -> stop replaying traffic
"high" -> go from low to high
"low" -> go from high to low

Configuration is done throught environmental variables:
DDNAME=tcp1 KEYFILE=/etc/doubledecker/public-keys.json DEALER=ipc:///etc/doubledecker/ddbroker 

The order of the files are important, and are expected in this order /-\_ 
ddtcpreplay -K -i veth0 -l 0 ../traffic/rise-er1.pcap ../traffic/high-er1.pcap ../traffic/fall-er1.pcap ../traffic/low-er1.pcap  
