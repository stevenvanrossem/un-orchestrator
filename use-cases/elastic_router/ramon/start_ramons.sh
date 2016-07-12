#! /bin/sh

for n in $(seq 1 4)
do
    ramon_name=ramon${n}
    ramon_interface=veth$(((n-1)*2))
    ramon_port=$((55555+$n))
    config_port=$((54736+$n))
    ./ramon_dd.sh ramon${n} ${ramon_port} ${config_port}
done
