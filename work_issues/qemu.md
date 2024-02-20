### 配置虚拟网卡

```
ip tuntap add dev tap1 mode tap
ip link set dev tap1 up
ip address add dev tap1 192.168.8.0/24
ifconfig

```

