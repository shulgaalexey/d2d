# d2d convergence console
Command line interface for d2d convergence
==========================================

The terminal of Tizen D2D Convergence Manager

## Commands

```
help                                               - print instructions

discovery start <timeout_seconds>                  - start or stop discovery

device <handle> services | id | name | type        - get device services and properties

service <handle> id | version | name [<value>]     - get or set service id, version or name
service <handle> poperty <prop_name> [<value>]     - get or set service property by name
service <handle> type                              - get service type
service create                                     - create local service
service <handle> destroy                           - destroy local service

service <handle> connect | disconnect              - connect or disconnect service
service <handle> start | stop [channel] [payload]  - start or stop service
service <handle> send | read [channel] [payload]   - send or read data of service

quit                                               - quit the console
```

## Prerequisites

1. GBS configuration (~/.gbs.conf)
1. Installation of GBS

## How to Build
You can build with following command for armv7l target board.

<code>$ gbs build -A armv7l --include-all --keep-packs</code>

## How to Check Log
You can determine the way to make logs by setting configurations on 'server/include/common.h'.

1. USE_DLOG_DEBUG: You can check logs by running 'dlogutil'
1. USE_SYSLOG_DEBUG: You can check logs by reading '/var/log/messages'
1. USE_FILE_DEBUG: You can check logs by reading '/var/log/messages'

## Credits
Many thanks to https://github.com/SKKU-ESLAB-Tizen/hello-tizen-service
