
#!/bin/bash

port=2001

while true; do
    # uptime=$(uptime)
    # inode_usage=$(df -i / | awk 'NR==2{print $5}')
    # cur_memory=$(free -h | awk 'NR==2{printf "MEM: %s/%s (%.2f%%)\n", $3,$2,$3*100/$2}')
    # disk_space=$(df -h / | awk 'NR==2{printf "DISK: %d/%dGB (%s)\n", $3,$2,$5}')
    # syslog=$(tail -n 15 /var/log/syslog)
    # syslog=$(tail -n 15 /home/void/a.txt)
    response="HTTP/1.1 200 OK\nSystem uptime: $(uptime)\n"
    # response+="Inode usage: $inode_usage\n"
    # response+="Current memory usage: $cur_memory\n"
    # response+="Disk Space usage: $disk_space\n"
    # response+="Last 15 lines of syslog:\n\n$syslog"
    echo -e "$response" | nc -l -p $port
done
