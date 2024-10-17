# Telegram Dashboard

## Prerequisites

Ensure you have the following dependencies installed:

```bash
sudo apt-get install libboost-all-dev curl libcurl4-openssl-dev libncurses5-dev libncursesw5-dev libjsoncpp-dev wireless-tools libssl-dev libopencv-dev
sudo ln -s /usr/include/jsoncpp/json/ /usr/include/json
```
## Building

```bash
mkdir build &
cmake -DCHAT_ID=<CHAT_ID> -DBOT_TOKEN=<BOT_TOKEN> ..
make
```

## Fomrat all the source and header files
```bash
find . -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i
```

## Running with Docker

Tested with Docker for Boost version 1.80.0. All the prerequisites mentioned above must be installed.

To run the application using Docker, execute:

```bash
docker run -it -v ${PWD}:/workdir/raspberry_pi_monorepo:delegated boost:1.80.0 /bin/bash
```

## Adding the Application as a Service

To manage the application as a service:

1. Navigate to the systemd directory:
    ```bash
    cd /etc/systemd/system
    ```

2. Use the following commands to control the service:

    - Start the service:
        ```bash
        sudo systemctl start dashboard.service
        ```

    - Stop the service:
        ```bash
        sudo systemctl stop dashboard.service
        ```

    - Check the status of the service:
        ```bash
        sudo systemctl status dashboard.service
        ```

3. To view the logs:
    ```bash
    cd /var/log
    vim syslog
    ```