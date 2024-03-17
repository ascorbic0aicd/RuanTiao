#!/user/bin/env bash
cd srcs
zip main.zip *
mkdir -p /mnt/d/RuanTiao
mv main.zip /mnt/d/RuanTiao
cd ..