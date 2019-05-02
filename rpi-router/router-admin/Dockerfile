FROM balenalib/raspberrypi3

# webmin apt-show-versions fix
# https://askubuntu.com/questions/916199/install-apt-show-versions-inside-an-ubuntu-docker-container
RUN apt-get update && rm /etc/apt/apt.conf.d/docker-gzip-indexes
RUN apt-get purge apt-show-versions && rm /var/lib/apt/lists/*lz4
RUN apt-get -o Acquire::GzipIndexes=false update && apt-get install -y wget apt-show-versions


# webmin
RUN sh -c 'echo "deb http://download.webmin.com/download/repository sarge contrib" > /etc/apt/sources.list.d/webmin.list'
RUN wget -qO - http://www.webmin.com/jcameron-key.asc | sudo apt-key add -

RUN apt-get update && apt-get install -y openssl perl libnet-ssleay-perl \
  libauthen-pam-perl libpam-runtime libio-pty-perl python
RUN apt-get install -y webmin
RUN sed -i 's/ssl=1/ssl=0/g' /etc/webmin/miniserv.conf
RUN /usr/share/webmin/changepass.pl /etc/webmin root admin 


RUN apt install iptables

RUN mkdir /app
COPY start.sh /app
EXPOSE 10000

CMD /app/start.sh
