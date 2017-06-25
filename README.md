# Convert-China-List 
[![Build Status](https://travis-ci.org/LBYPatrick/convert-china-list.svg?branch=master)](https://travis-ci.org/LBYPatrick/convert-china-list)

## Usage (For generators)

There are two modes available:

- console "interface" mode
- commandline mode

#### Console "Interface" Mode
Just follow instructions shown in program when running convert-china-list.exe located under ``builds``
<br>

#### Commandline Mode
Try these just like using a linux software.

```bash
          --help         : show help information
    -s or --shadowrocket : convert specified file to a shadowrocket-compatible file.
    -b or --bind         : convert specified file to a bind9-compatible file.
    -i or --input-file   : specify input file.
    -o or --output-file  : specify output file.
    -d or --dns          : specify preferred DNS (only required in bind mode).
```

## Usage (For configs)
Coverted China List from [felixonmars/dnsmasq-china-list](https://github.com/felixonmars/dnsmasq-china-list) for Bind/Shadowrocket Users
Just include the file you downloaded(located under ``Pre-made Configs``)/converted in

- Bind         : /etc/bind/named.conf.local
- Shadowrocket : Make a QR code for the link of the file you want to download, then use the client device to scan it in **Shadowrocket App**. 



