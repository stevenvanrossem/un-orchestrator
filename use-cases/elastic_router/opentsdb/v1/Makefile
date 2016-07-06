all:
	docker build --tag="gitlab.testbed.se:5000/tmpostdb" .
	
push:
	docker push "gitlab.testbed.se:5000/tmpostdb"

stop:
	docker stop tmpost
rm:
	docker rm tmpost
start:
	docker run -d -p 8070:8070 -p 4242:4242 -p 60030:60030	 --link ddbroker:broker --name=tmpost  gitlab.testbed.se:5000/tmpostdb
inter:
	docker run -it -p 8070:8070 -p 4242:4242 -p 60030:60030	--link ddbroker:broker --name=tmpost  gitlab.testbed.se:5000/tmpostdb
bash:
	docker run -it -p 8070:8070 -p 4242:4242 -p 60030:60030	--link ddbroker:broker --name=tmpost  gitlab.testbed.se:5000/tmpostdb /bin/bash

            