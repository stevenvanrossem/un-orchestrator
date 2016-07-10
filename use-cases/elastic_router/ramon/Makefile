all:
	docker build  --tag="gitlab.testbed.se:5000/ratemon" .
	
push:
	docker push "gitlab.testbed.se:5000/ratemon"

stop:
	docker stop ratemon
rm:
	docker rm ratemon
start:
	docker run -d  --volume=/etc/doubledecker:/keys:ro  --volume=/sys:/sys:ro  --link ddbroker:broker --name=ratemon  gitlab.testbed.se:5000/ratemon
inter:
	docker run -it --volume=/etc/doubledecker:/keys:ro  --volume=/sys:/sys:ro  --link ddbroker:broker --name=ratemon  gitlab.testbed.se:5000/ratemon
bash:
	docker run -it --volume=/etc/doubledecker:/keys:ro   --volume=/sys:/sys:ro  --link ddbroker:broker --name=ratemon  gitlab.testbed.se:5000/ratemon /bin/bash

