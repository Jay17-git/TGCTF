podman build -t "pwn1" .
podman run -d -p "0.0.0.0:8090:80" -p "9999:9999" --name="pwn1" pwn1
