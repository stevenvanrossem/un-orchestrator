FROM ubuntu:16.04

MAINTAINER Bertrand Pechenot <berpec@acreo.se>
# based on petergrace/opentsdb 
# Thanks to Peter Grace (original author)
# Thanks to Pontus Sköldström and Tamas Levai

#Install packages
RUN DEBIAN_FRONTEND=noninteractive apt-get -y update \
&& DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends \
                                                  -y \
                                                     curl \
                                                     unzip \
                                                     python3 \
                                                     python3-zmq \
                                                     python3-requests \
						     python3-setuptools \ 
                                                     python3-pip \
                                                     libffi-dev \
                                                     libsodium-dev \
                                                     python3-dev \
                                                     build-essential \
&&  DEBIAN_FRONTEND=noninteractive apt-get clean all

# Install DoubleDecker proxy
WORKDIR /root

RUN pip3 install PyNaCl
RUN pip3 install jsonrpcserver
ADD https://github.com/acreo/DoubleDecker-py/archive/master.tar.gz /root/
RUN tar xzf master.tar.gz
RUN rm master.tar.gz
WORKDIR /root/DoubleDecker-py-master/
RUN python3 setup.py build
RUN python3 setup.py install
ADD start_ddproxy.sh ddproxy.py /root/

# ENV CLIENT_NAME opentsdb
# ENV DEALER_PORT tcp://172.17.0.1:5555

RUN DEBIAN_FRONTEND=noninteractive apt-get remove -yq make autoconf
# default run command
CMD /root/start_ddproxy.sh
