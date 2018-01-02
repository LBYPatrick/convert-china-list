# Convert-China-List 
[![Build Status](https://travis-ci.org/LBYPatrick/convert-china-list.svg?branch=master)](https://travis-ci.org/LBYPatrick/convert-china-list)

[中文说明](https://github.com/LBYPatrick/convert-china-list/wiki/%E4%B8%AD%E6%96%87%E8%AF%B4%E6%98%8E)

# Description
Coverted China List from [felixonmars/dnsmasq-china-list](https://github.com/felixonmars/dnsmasq-china-list) and [gfwlist/gfwlist](https://github.com/gfwlist/gfwlist) for Bind/Shadowrocket/Shadowsocks-Windows/Proxy SwitchyOmega Users

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
     -d or --dns                              : specify preferred DNS (only works in bind mode).
	 -p or --proxy                            : specify preferrred proxy (only works in SwitchyOmega PAC mode)
	 
```
## Example
```bash
convert-china-list -s dnsmasq -c shadowrocket -i accelerated-domains.china.conf -o accelerated-domains.china.shadowrocket.conf 
convert-china-list -s dnsmasq -c 1 -i accelerated-domains.china.conf -o accelerated-domains.china.bind.unblockYouku.conf  -d 158.69.209.100
convert-china-list -s dnsmasq -c bind -i accelerated-domains.china.conf -o accelerated-domains.china.bind.conf
convert-china-list -s dnsmasq -c shadowsocks-windows -i accelerated-domains.china.conf -o pac.txt
convert-china-list -s dnsmasq -c switchyomega -i accelerated-domains.china.conf -o accelerated-domains.china.switchyomega.pac

convert-china-list -s gfwlist -i gfwlist.txt -o gfwlist.shadowrocket.conf -c 0
convert-china-list -s gfwlist -i gfwlist.txt -o gfwlist.bind.unblockYouku.conf -c 1 -d 158.69.209.100
convert-china-list -s gfwlist -i gfwlist.txt -o gfwlist.bind.conf -c 1
convert-china-list -s gfwlist -i gfwlist.txt -o gfwlist.pac.txt -c 2
convert-china-list -s gfwlist -i gfwlist.txt -o gfwlist.switchyomega.pac -c 3

```

### For "--proxy"

i.e.

``SOCKS5 127.0.0.1:1080; SOCKS 127.0.0.1:1080`` -- This is for shadowsocks' default port.


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

<h1>最後に、東條希最高!</h1>
