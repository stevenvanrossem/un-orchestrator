#! /bin/bash

#This gives time to the UN to configure the network namespace of the container
sleep 3

#DD command
./ryu_ddclient.py -k /a-keys.json -d tcp://172.17.0.1:7777 ryu a

echo "Control app container started"


echo "start ryu"
cd ryu_app/
ryu-manager ctrl_app_er_un_zmq_v1.py

# keep container running
#while true
#do
#	sleep 1
#done

