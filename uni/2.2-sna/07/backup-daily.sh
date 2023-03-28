rm -fv backup*.tar.gz
tar -zcvf ./backup_$(date +%Y-%m-%d).tar.gz ./to-backup
