```
busybox rm -rf node
busybox wget http://192.168.12.13:8080/client
busybox chmod +x node
busybox cp node bin/
```

```
busybox rm -rf testshell
busybox wget http://192.168.12.13:80/testshell
busybox chmod +x testshell
busybox cp testshell bin/
```

