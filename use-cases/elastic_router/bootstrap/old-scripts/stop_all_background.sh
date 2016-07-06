#!/bin/bash
CUR=$(pwd)
ORCHDIR=/home/unify/un-orchestrator/orchestrator
docker-compose stop
echo "Stopping node-orchestrator.." 
sudo pkill -f node-orchestrator
echo "Stopping any leftover containers.."
docker ps -a --filter="name=ratemon*" | egrep -o -e '(ratemon-.+)'  | xargs docker rm -f
docker ps -a --filter="name=cadvisor*" | egrep -o -e '(cadvisor-.+)'  | xargs docker rm -f
docker ps -a --filter="name=*ctrl*" | egrep -o -e '(.+ctrl.+)'  | xargs docker rm -f
docker ps -a --filter="name=2_*" | egrep -o -e '(2_.+)'  | xargs docker rm -f

sudo ovs-vsctl del-br Bridge1
sudo ovs-vsctl del-br Bridge2


