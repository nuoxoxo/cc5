name	:= inception

all	:
	@ echo " ➜ Build and launch ${name} ... "
	@ sh srcs/requirements/wordpress/tools/make_dir.sh
	@ docker-compose -f ./srcs/docker-compose.yml --env-file srcs/.env up -d

build	: all

down	:
	@ echo " ➜ Stopping ${name} ... "
	@ docker-compose -f ./srcs/docker-compose.yml --env-file srcs/.env down

re	: down
	@ echo " ➜ Re-launch ${name} ... "
	@ docker-compose -f ./srcs/docker-compose.yml --env-file srcs/.env up -d --build

clean	: down
	@ echo " ➜ Cleaning up ${name} ... "
	@ docker system prune -a
	@#@ sudo rm -rf ~/data/wordpress/*
	@#@ sudo rm -rf ~/data/mariadb/*
 
fclean	:
	@ echo " ➜ Cleaning up "
	@# if [ `docker ps -qa` = "" ]; then docker stop `docker ps -qa`; endif
	@ docker stop `docker ps -qa`
	@ docker system prune --all --force --volumes
	@ docker network prune --force
	@ docker volume prune --force
	@#@ sudo rm -rf ~/data/wordpress/*
	@#@ sudo rm -rf ~/data/mariadb/*
	@ docker ps

f	: fclean

.PHONY	: all build down re clean fclean
