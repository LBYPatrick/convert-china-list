# Convert-China-List 
[![Build Status](https://travis-ci.org/LBYPatrick/convert-china-list.svg?branch=master)](https://travis-ci.org/LBYPatrick/convert-china-list)


# Description
Coverted China List from [felixonmars/dnsmasq-china-list](https://github.com/felixonmars/dnsmasq-china-list) and [gfwlist/gfwlist](https://github.com/gfwlist/gfwlist) for Bind/Shadowrocket/Shadowsocks-Windows Users

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
           --help                             : show help information
	 -s or --source-type                      : specify type of source file (dnsmasq (0)? GFWList (1)?)
	 -c or --convert-to                       : specify type of output file (shadowrocket (0)? bind (1)? shadowsocks-windows (2)? SwitchyOmega (3)?)
	 -i or --input-file                       : specify input file.
     -o or --output-file                      : specify output file.
     -d or --dns                              : specify preferred DNS (only required in bind mode).
	 
```

## Usage (For configs)
Just include the file you downloaded(located under ``Pre-made Configs``)/converted in

- Bind                : /etc/bind/named.conf.local
- Shadowrocket        : Make a QR code for the link of the file you want to download, then use the client device to scan it in **Shadowrocket App**. 
- Shadowsocks-Windows : Its root folder, replace **pac.txt**.
- SwitchyOmega        : Well, just import PAC.

## About GFWList Mode

Thanks to the base64 decode/encode module provided by [tylerliu](https://github.com/tylerliu), it is the most crucial module to this project. 
<br>
And I think this mode is similar to [gfwlist2pac](https://github.com/breakwa11/GFWList2PAC), but in C++ and supports much more output formats.

<h1>×îáá¤Ë¡¢–|—lÏ£×î¸ß!</h1>