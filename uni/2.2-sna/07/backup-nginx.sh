rm -fv backup*.tar.gz
find /usr/share/nginx/ -type f -name index.html -exec dirname {} \; | sort | uniq > to-backup.txt
tar -zcvf ./backup_$(date +%Y-%m-%d).tar.gz -T to-backup.txt
rm to-backup.txt
