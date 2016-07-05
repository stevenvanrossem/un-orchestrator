#!/bin/bash
NODEORCH=$(pidof node-orchestrator)
docker-compose ps 
if [ -z "$NODEORCH" ]; then 
  echo -e "node-orchestrator\t\t\t\t Down" 
else 
  echo -e "node-orchestrator\t\t\t\t Up"
fi

echo ""
echo "Monitoring functions"
echo "--------------------" 
for n in $(docker ps -a --filter="name=ratemon*" | egrep -o -e '(ratemon-.+)') ; do
  echo -e "$n\t\t\t\t\t Up"
done
for n in $(docker ps -a --filter="name=cadvisor*" | egrep -o -e '(cadvisor-.+)') ; do
  echo -e "$n\t\t\t\t\t Up"
done

echo "" 
echo "Elastic router VNFs"
echo "-------------------"
for n in $(docker ps -a --filter="name=2_*" | egrep -o -e '(2_.+)') ; do 
  echo -e "$n\t\t\t\t\t\t Up"
done
