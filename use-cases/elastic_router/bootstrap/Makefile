all:
	docker-compose up
stop:
	-docker-compose stop
	-docker ps -a --filter="name=ratemon*" | egrep -o -e '(ratemon-.+)'  | xargs docker kill
	-docker ps -a --filter="name=cadvisor*" | egrep -o -e '(cadvisor-.+)'  | xargs docker kill

rm: 
	-docker-compose rm
	-docker ps -a --filter="name=ratemon*" | egrep -o -e '(ratemon-.+)'  | xargs docker rm
	-docker ps -a --filter="name=cadvisor*" | egrep -o -e '(cadvisor-.+)'  | xargs docker rm

