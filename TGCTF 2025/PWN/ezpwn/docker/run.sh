#!/bin/bash
 
IMAGE_NAME=localhost/pwn
CONTAINER_HOME=/home/ctf
PROG_NAME=vuln
NC_PORT=9999
DBG_PORT=8888
 
# load image
if [ "$(podman images | grep ${IMAGE_NAME} | wc -l)" -lt "1" ]; then
    podman load -i ${IMAGE_NAME}.tar
fi
 
# start continer
if [ "$(podman ps | grep ${IMAGE_NAME} | wc -l)" -lt "1" ]; then
#   podman run --privileged -itd -p ${DBG_PORT}:${DBG_PORT} -p ${NC_PORT}:${NC_PORT} ${IMAGE_NAME}
    podman run --privileged -itd --net=host ${IMAGE_NAME}
fi
 
# get continer id
CONTAINER_ID=$(podman ps -q --filter "ancestor=${IMAGE_NAME}")
 
# cp files
podman cp ./tools/gdbserver ${CONTAINER_ID}:${CONTAINER_HOME}
podman cp ./tools/netcat ${CONTAINER_ID}:${CONTAINER_HOME}
podman cp ./${PROG_NAME} ${CONTAINER_ID}:${CONTAINER_HOME}
 
# start run
podman exec -itd -w ${CONTAINER_HOME} ${CONTAINER_ID} /bin/bash -c "./netcat -lvp ${NC_PORT} -e ./${PROG_NAME}"
read
 
podman exec -it -w ${CONTAINER_HOME} ${CONTAINER_ID} /bin/bash -c "ps -ef | grep ${PROG_NAME} | grep -v 'grep' | grep -v '\-c' | awk '{print \$2}' | xargs ./gdbserver :${DBG_PORT} --attach"
read
 
#podman stop ${CONTAINER_ID}
#podman rm ${CONTAINER_ID}
 
podman stop $(podman ps -a -q)
podman rm $(podman ps -a -q)