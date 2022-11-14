#! /bin/bash

item=$(echo -e "链接视频\n剪辑视频\n转换码率" | fzf --header 选择操作)

case $item in
  "链接视频")
    echo "# file 'xxx.mp4'" > list.txt
    for file in `ls *.mp4`; do
      echo "file '$file'" >> list.txt
    done
    nvim list.txt
    ffmpeg -f concat -i list.txt -c copy out.mp4
    ;;
  "剪辑视频")
    input=$(ls | grep 'mp4\|mpv' | fzf --header 选择待剪辑的视频)
    [ ! "$input" ] && exit 0
    read -p "请输入开始时间(00:00:00): " start
    [ ! "$start" ] && start="00:00:00"
    read -p "请输入结束时间(00:00:00): " end
    ffmpeg -i $input -ss $start -t $end -vcodec copy -acodec copy out.mp4
    ;;
  "转换码率")
    input=$(ls | grep 'mp4\|mpv' | fzf --header 选择待转换的视频)
    [ ! "$input" ] && exit 0
    rate=$(echo -e '1000k\n2000k\n3000k\n4000k\n5000k\n6000k\n7000k\n8000k\n9000k\n10000k' | fzf --header 选择码率)
    [ ! "$rate" ] && exit 0
    ffmpeg -i $input -b:v $rate out.mp4
    exit 0
    ;;
esac
