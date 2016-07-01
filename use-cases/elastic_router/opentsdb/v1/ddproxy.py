#!/usr/bin/python3
# coding=utf-8
__author__ = 'tamleva'
__author__ = 'berpec'
import json
import argparse
import requests
import logging
import time
from doubledecker.clientSafe import ClientSafe


class DDProxy(ClientSafe):
    def __init__(self, name, dealerurl, keyfile):
        # init DD client
        super().__init__(name, dealerurl, keyfile)
        self.tsdb_url = 'http://127.0.0.1:4242/api/put/'
        self.session = requests.Session()

    def on_reg(self):
        self.subscribe('measurement', 'all')

    def on_data(self, msg):
        try:
            message = json.loads(msg.decode('utf-8'))
            self.push_opentsdb(message)
        except (TypeError, ValueError):
            logging.error(msg.pop(0), 'sent', msg)

    # Override DD client's receiving function to implement alert
    # displaying.
    def on_pub(self, src, topic, data):
        try:
            message = json.loads(data.decode('utf-8'))
            logging.debug("%s", json.dumps(message))
            self.push_opentsdb(src, message)
        except (TypeError, ValueError):
            logging.error("JSON type or value error during push")

    def push_opentsdb(self, src, data):
        for i, j in data['results'].items():
            tsdb_json = {'metric': i,
                         'timestamp': int(time.time()),
                         'value': j,
                         'tags': dict(list(data['parameters'].items()) +
                                      list({'tool': data['label']}.items()) +
                                      list({'source': src.decode()}.items()))
                         }
            logging.debug("push_to_opentsdb \n%s", json.dumps(tsdb_json, indent=2))
            try:
                self.session.post(self.tsdb_url, data=json.dumps(tsdb_json))
            except requests.exceptions.ConnectionError:
                logging.warning('Failed to push_opentsdb :(((')

    def on_discon(self):
        logging.info("The client got disconnected")

    def on_error(self, code, msg):
        logging.info("ERROR n#%d : %s", code, msg)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='DoubleDecker OpenTSDB proxy')
    parser.add_argument("name", help="Identity of this client")
    parser.add_argument(
        '-d',
        "--dealer",
        help='URL to connect DEALER socket to, "tcp://1.2.3.4:5555"',
        nargs='?',
        default='tcp://127.0.0.1:5555')
    parser.add_argument(
        '-f',
        "--logfile",
        help='File to write logs to',
        nargs='?',
        default=None)
    parser.add_argument(
        '-l',
        "--loglevel",
        help='Set loglevel (DEBUG, INFO, WARNING, ERROR, CRITICAL)',
        nargs='?',
        default="INFO")
    parser.add_argument(
        '-k',
        "--keyfile",
        help='File containing the encryption/authentication keys)',
        nargs='?',
        default='')

    args = parser.parse_args()

    numeric_level = getattr(logging, args.loglevel.upper(), None)
    if not isinstance(numeric_level, int):
        raise ValueError('Invalid log level: %s' % args.loglevel)

    logging.basicConfig(format='%(levelname)s:%(message)s', filename=args.logfile, level=numeric_level)

    logging.info("Safe client")
    dd_proxy = DDProxy(name=args.name.encode('utf8'),
                       dealerurl=args.dealer,
                       keyfile=args.keyfile)

    logging.info("Starting DoubleDecker example client")
    logging.info("See ddclient.py for how to send/recive and publish/subscribe")
    dd_proxy.start()
