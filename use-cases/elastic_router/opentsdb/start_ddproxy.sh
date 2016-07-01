#!/bin/bash
cd /root
python3 ddproxy.py -d $DEALER_PORT \
        -k /keys/public-keys.json \
        -t $OTSDB_PORT_4242_TCP \
        otsdb_ddproxy
