#!/bin/bash
cd /opt/sei-bin
# python3 ddproxy.py -d $DEALER_PORT $CLIENT_NAME
python3 ddproxy.py -d $DEALER_PORT \
        -k /etc/doubledecker/a-keys.json \
        $CLIENT_NAME

