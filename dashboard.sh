#!/bin/bash
BUILD_DIR=/home/Github/telegram_dashboard/build

until ping -c1 google.com &>/dev/null; do sleep 1; done

cd $BUILD_DIR
cmake -DCHAT_ID=<CHAT_ID> -DBOT_TOKEN=<BOT_TOKEN> ..
cd src
make
./dashboard
