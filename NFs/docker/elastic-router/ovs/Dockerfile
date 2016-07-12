FROM      ubuntu
MAINTAINER iMinds

RUN apt-get update --fix-missing
RUN apt-get install -y openvswitch-switch openvswitch-common openssh-server
# need ifconfig
RUN apt-get install -y net-tools

RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config

ADD authorized_keys /root/.ssh/authorized_keys

# set permissions correctly, otherwise ssh will not be able to login via ssh keys 
RUN chmod 700 /root/.ssh
RUN chmod 600 /root/.ssh/authorized_keys

ADD start.sh start.sh
RUN chmod +x start.sh

CMD ./start.sh
