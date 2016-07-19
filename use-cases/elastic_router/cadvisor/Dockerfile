FROM ubuntu:latest

MAINTAINER Umar Toseef umar.toseef@eict.de
RUN apt-get update

RUN DEBIAN_FRONTEND=noninteractive apt-get -y install python3-pip
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install python3-pkg-resources
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install python3-setuptools
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install python3-zmq
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install python3-urwid
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install python3-tornado
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install libffi-dev
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install python3-requests
RUN pip3 install pynacl
RUN pip3 install jsonrpcserver
RUN pip3 install jsonrpcclient
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install curl

# RUN pip3 install future
# Fetch and install Doubledecker
RUN mkdir /dd
RUN mkdir /measure
# ADD https://github.com/acreo/DoubleDecker-py/archive/master.tar.gz /dd
ADD https://github.com/Acreo/DoubleDecker-py/archive/threadfix.tar.gz /dd
WORKDIR /dd

RUN tar xzf threadfix.tar.gz
RUN rm threadfix.tar.gz
WORKDIR /dd/DoubleDecker-py-threadfix/
RUN python3 setup.py build
RUN python3 setup.py install


# Grab cadvisor from the staging directory.
ADD cadvisor /usr/bin/cadvisor
EXPOSE 8080

# start in /proxy
WORKDIR /proxy

ADD cadvproxy /proxy/cadvproxy
#ADD doubledecker /proxy/doubledecker
COPY main.py /proxy
COPY run.sh /proxy
CMD ./run.sh