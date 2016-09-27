# d2d convergence console
Command line interface for d2d convergence
==========================================

The terminal of Tizen D2D Convergence Manager

## Prerequisites

1. GBS configuration (~/.gbs.conf)
1. Installation of GBS

## How to Build
You can build with following command for armv7l target board.

<code>$ gbs build -A armv7l --include-all --keep-packs/code>

## How to Check Log
You can determine the way to make logs by setting configurations on 'server/include/common.h'.

1. USE_DLOG_DEBUG: You can check logs by running 'dlogutil'
1. USE_SYSLOG_DEBUG: You can check logs by reading '/var/log/messages'
1. USE_FILE_DEBUG: You can check logs by reading '/var/log/messages'

## Basics
Many thanks to https://github.com/SKKU-ESLAB-Tizen/hello-tizen-service
