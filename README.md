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
service <handle> type [<value>]                    - get or set service type

service <handle> constate                          - get connection state

service create [<type>]                            - create local service
service <handle> destroy                           - destroy local service

service <handle> connect | disconnect              - connect or disconnect service


service <handle> start | stop [channel] [payload]  - start or stop service
service <handle> send | read [channel] [payload]   - send or read data of service

quit                                               - quit the console
```

###Typical usage

Start the console with the command

```
$ ./d2d
```

Start the discovery procedure and limit its duration with 60 seconds

```
discovery start 60
```

Wait for Convergence Manager discovering nearby devices.
Eventually the discovered device info will be printed on the console as following:


```
Found device   MyTizenDevice   handle: 0a1b2c3d   type: mobile   id: ABCD-1234
Found device   MyTizenDevice   handle: 0a1b2c4e   type: TV   id: KLMN-5678
Found device   MyTizenDevice   handle: 0a1b2c5f   type: wearable   id: GGGG-3333
```

Browse services of device you are interested in:

```
device 0a1b2c3d services
```

Specified device services will be printed on the console as follows:


```
handle: 0bb11cd1  id: AppCommunication  version: 1.0
handle: 0bb53e76  id: RemoteAppControl  version: 1.0
```

Check service connection state


```
service 0bb53e76 constate

```

On the console it will be printed the current  connection connection state of the service,
for example:

```
CONNECTED
```

Start the service:

```
service 0bb53e76 start
```

In a moment the status of start operation will be printed on the console:

```
Listener status: OK
channel: {channel json data}
payload: {payload json data}
```

TODO




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
Many thanks to

 * https://github.com/SKKU-ESLAB-Tizen/hello-tizen-service for Tizen service template
 * https://github.com/kazuho/picojson for PicoJSON implementation

