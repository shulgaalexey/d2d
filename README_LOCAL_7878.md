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

On the console it will be printed the current connection connection state of the service,
for example:

```
CONNECTED
```

Note it the service state is DISCONNECTED, you should use the command:

```
service 0bb53e76 connect
```

and wait for the confirmation

```
service 0bb53e76 CONNECTED
```

Start the service.
Channel and payload are optional parameter.
Both should be formatted as json strings.

```
service 0bb53e76 start {"id": "channel1", "uri": "services/myservice/1"}  {"data": "my data item", "another_data": "my another data item"}
```

In a moment the status of start operation will be printed on the console:

```
Listener status: OK
channel: {channel json data}
payload: {payload json data}
```

Send a payload to the specified service channel.
Channel and payload are optional parameter.
Both should be formatted as json strings.

```
service 0bb53e76 send {"id": "channel1", "uri": "services/myservice/1"}  {"data": "my data item", "another_data": "my another data item"}
```

If the service provides response on this request, the console indicates it
(similarly to the start operation):

```
Listener status: OK
channel: {channel json data}
payload: {payload json data}
```

Note, that the read and stop requests to the server are used similarly to the send request.


When service is not needed, it should be stoped and disconnected:

```
service 0bb53e76 stop {"id": "channel1", "uri": "services/myservice/1"}  {"data": "my data item", "another_data": "my another data item"}
service 0bb53e76 disconnect
```

TODO
1. Prepare script for App Comm Service (both client and server instances)
2. Prepare script for Remote App Control Service

TODO it would be good to check the features of voice control.


###UI

Consider following variants of user interface:
 * command line interface
 * graphical user interface
 * voice user interface

TODO Think about remote sensor sharing.


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

