#! /bin/bash

#This gives time to the UN to configure the network namespace of the container
sleep 3

#DD command
#./ryu_ddclient.py -k ./DoubleDecker-master/keys/a-keys.json -d tcp://172.17.0.1:5555 ryu a &

echo "Control app container started"

#echo "start ssh"
#service ssh start

echo "start zmq alarm sender"
python zmq_ryu_test_alarm_send.py &

echo "start ryu"
cd ryu_app/
ryu-manager ctrl_app_er_un_zmq_v1.py

# keep container running
#while true
#do
#	sleep 1
#done

