#!/bin/bash
CUR=$(pwd)
ORCHDIR=/home/unify/un-orchestrator/orchestrator
for n in $(docker version | grep Version | egrep -o -e  '([0-9\.]+)'); do
  if [ "$n" != "1.9.1" ] ; then
    echo "Expect version 1.9.1 of Docker, running version $n"
  fi
done
echo "Starting node-orchestrator"
cd $ORCHDIR
sudo ./node-orchestrator --d ./config/orchestrator_config_er_demo.ini  > >(tee $CUR/logs/node-orchestrator_stdout.log) 2> >(tee $CUR/logs/node-orchestrator_stderr.log >&2) &
cd $CUR
echo "Waiting for node-orchestrator to start.."
while ! nc -z localhost 8080; do   
  sleep 0.1 # wait for 1/10 of the second before check again
done
echo "Starting the other services.."
docker-compose up
echo "Stopped all backgrund containers"
echo "Stopping node-orchestrator.." 
sudo pkill -f node-orchestrator
echo "Stopping any leftover containers.."
docker ps -a --filter="name=ratemon*" | egrep -o -e '(ratemon-.+)'  | xargs docker rm -f
docker ps -a --filter="name=cadvisor*" | egrep -o -e '(cadvisor-.+)'  | xargs docker rm -f
docker ps -a --filter="name=*ctrl*" | egrep -o -e '(.+ctrl.+)'  | xargs docker rm -f
docker ps -a --filter="name=2_*" | egrep -o -e '(2_.+)'  | xargs docker rm -f

sudo ovs-vsctl del-br Bridge1
sudo ovs-vsctl del-br Bridge2


