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
from jsonrpcserver import dispatch, Methods
import jsonrpcserver.exceptions

class DDProxy(ClientSafe):
    def __init__(self, name, dealerurl, keyfile, tsdb_addr):
        # init DD client
        super().__init__(name, dealerurl, keyfile)
        self.tsdb_url = tsdb_addr.replace('tcp', 'http')+'/api/put/'
        self.session = requests.Session()
        self.methods = Methods()
        self.methods.add_method(self.measurement)
        self.methods.add_method(self.rate_data)

    def on_reg(self):
        self.subscribe('measurement', 'all')

    def on_data(self, src, msg):
        self.handle_jsonrpc(src=src, topic=None, msg=msg)
        # try:
        #     message = json.loads(msg.decode('utf-8'))
        #     self.push_opentsdb(message)
        # except (TypeError, ValueError):
        #     logging.error(msg.pop(0), 'sent', msg)

    # Override DD client's receiving function to implement alert
    # displaying.
    def on_pub(self, src, topic, data):
        self.handle_jsonrpc(src=src, topic=topic, msg=data)

        # try:
        #     message = json.loads(data.decode('utf-8'))
        #     logging.debug("%s", json.dumps(message))
        #     self.push_opentsdb(src, message)
        # except (TypeError, ValueError):
        #     logging.error("JSON type or value error during push")

    def handle_jsonrpc(self, src, msg, topic=None):
        logging.info("handling JSON-RPC")
        request = json.loads(msg.decode('UTF-8'))
        logging.info("got request %s "%str(request))
        if 'error' in request:
            logging.error("Got error response from: %s" % src)
            logging.error(str(request['error']))
            return

        if 'result' in request:
            logging.info("Got response from %s" % src)
            logging.info(str(request['result']))
            return

        # include the 'ddsrc' parameter so the
        # dispatched method knows where the message came from
      
        if 'params' not in request:
            request['params'] = {}

        if isinstance(request['params'], str):
            if len(request['params']) < 1:
                request['params'] = {}

        # print("request: ", request)
        # print("Src: ", src.decode())
        request['params']['ddsrc'] = src.decode()
        response = 1
        response = dispatch(self.methods, request)
        logging.info("handle_json, got response %s"%(str(response)))
        # if the http_status is 200, its request/response, otherwise notification
        if response.http_status == 200:
            logging.info("Replying to %s with %s" % (str(src), str(response)))
            self.sendmsg(src, str(response))
        # notification, correctly formatted
        elif response.http_status == 204:
            pass
        # if 400, some kind of error
        # return a message to the sender, even if it was a notification
        elif response.http_status == 400:
            logging.info("sending response to %s  message: %s"%(str(str), str(response)))
            self.sendmsg(src, str(response))
            logging.error("Recived bad JSON-RPC from %s, error %s" % (str(src), str(response)))
        else:
            logging.error(
                "Recived bad JSON-RPC from %s \nRequest: %s\nResponose: %s" % (str(src), msg.decode(), str(response)))


    def measurement(self, ddsrc, result):
        result['parameters']['tool'] = 'cadvisor'
        result['parameters']['source'] = ddsrc
        
        for i, j in result['results'].items():
            tsdb_json = {'metric': i,
                         'timestamp': int(time.time()),
                         'value': j,
                         'tags': result['parameters']
                         }
            logging.debug("push_to_opentsdb \n%s", json.dumps(tsdb_json, indent=2))
            try:
                self.session.post(self.tsdb_url, data=json.dumps(tsdb_json))
            except requests.exceptions.ConnectionError:
                logging.warning('Failed to push_opentsdb :(((')

    def rate_data(self, ddsrc, overload_risk, name, lm, linerate, lsd):
        tsdb_json = {'metric': 'overload_risk',
                         'timestamp': int(time.time()),
                         'value': overload_risk,
                         'tags': {'tool': 'ratemon', 'source': ddsrc}
                         }
        try:
            self.session.post(self.tsdb_url, data=json.dumps(tsdb_json))
        except requests.exceptions.ConnectionError:
            logging.warning('Failed to push_opentsdb :(((')

#        tsdb_json = {'metric': 'overload_risk_tx',
#                         'timestamp': int(time.time()),
#                         'value': overload_risk_tx,
#                         'tags': {'tool': 'ratemon', 'source': ddsrc}
#                         }
#        try:
#            self.session.post(self.tsdb_url, data=json.dumps(tsdb_json))
#        except requests.exceptions.ConnectionError:
#            logging.warning('Failed to push_opentsdb :(((')

        tsdb_json = {'metric': 'lsd',
                         'timestamp': int(time.time()),
                         'value': lsd,
                         'tags': {'tool': 'ratemon', 'source': ddsrc}
                         }
        try:
            self.session.post(self.tsdb_url, data=json.dumps(tsdb_json))
        except requests.exceptions.ConnectionError:
            logging.warning('Failed to push_opentsdb :(((')
        tsdb_json = {'metric': 'lm',
                         'timestamp': int(time.time()),
                         'value': lm,
                         'tags': {'tool': 'ratemon', 'source': ddsrc}
                         }
        try:
            self.session.post(self.tsdb_url, data=json.dumps(tsdb_json))
        except requests.exceptions.ConnectionError:
            logging.warning('Failed to push_opentsdb :(((')
               
        
    def on_discon(self):
        logging.info("The client got disconnected")

    def on_error(self, code, msg):
        from doubledecker import proto as DD
        if code ==DD.E_REGFAIL:
            logging.error("DD - Registration failed, reason: {0!s}".format(msg[0]))
        elif code == DD.E_VERSION:
            logging.error("DD - Registration failed, wrong protocol version!")
        elif code == DD.E_NODST:
            logging.error("DD - No destination: {0!s}".format(msg))
        else:
            logging.error("DD - unknown ({0:d},{1!s})".format(code, msg))


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
        default="WARNING")
    parser.add_argument(
        '-k',
        "--keyfile",
        help='File containing the encryption/authentication keys)',
        nargs='?',
        default='')
    parser.add_argument(
        '-t',
        "--tsdb_address",
        help='Address of the OpenTSDB isntance',
        nargs='?',
        default='http://127.0.0.1:4242')
    args = parser.parse_args()

    numeric_level = getattr(logging, args.loglevel.upper(), None)
    if not isinstance(numeric_level, int):
        raise ValueError('Invalid log level: %s' % args.loglevel)

    logging.basicConfig(format='%(levelname)s:%(message)s', filename=args.logfile, level=numeric_level)
    
    dealer = args.dealer 
    if dealer is None:
        dealer = 'tcp://ddbroker:5555'
    
    logging.info("Safe client")
    dd_proxy = DDProxy(name=args.name.encode('utf8'),
                       dealerurl=args.dealer,
                       keyfile=args.keyfile,
                       tsdb_addr=args.tsdb_address)

    logging.info("Starting DoubleDecker example client")
    logging.info("See ddclient.py for how to send/recive and publish/subscribe")
    dd_proxy.start()
