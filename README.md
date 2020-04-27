# netwhale
Netwhale is a Linux tool to capture packets and process it to audio sound. It sounds like whale talking in the ocean of network.

## Backgroud
Once upon a time, I read the《Expert C Programming: Deep C Secrets》. Like those curious adventurers in those old tales, I was attracted by this sentence: "solaris has a tool to listen the sound of network packets", I didn't find it in my virtual machine, so I implement netwhale.

## Example
ping from local machine
<audio id="audio" controls="" preload="none">
      <source id="mp3" src="https://github.com/Heersin/netwhale/example/ping_AAC.mp4">
</audio>

## Requirement
Before install this project, the following libirary is required.
- libpcap
- libao

you can run the command to test if libao is succesfully installed.
```
make makeaudio && make clean
./little_star
```
it will play a short music.

## Install
```
git clone 
make && make clean 
```

## Usage
use -h to show the Usage.
-  -f [bpf statement] : Use Filter in packet capturing, it's optional,default capture all packets.

- -i [interface name] : Use which card, you can find your network interface by using 'ifconfig', it's optional but should work with -t

- -t [link layer type] : If you know the type of your link layer, set this, default as Ethernet, The Type code as Follows:<br>
    [1] Ethernet -- 0x00<br>
    [2] WLAN 802.11 -- 0x01<br>

- -h : this help page

- -lx : contains l2, l3, l4, l5, design to filter the audio, such as '-l3' only listen to network layer, not implement yet.

## Architecture
![Arch](https://github.com/Heersin/netwhale/example/netwhale.jpg)
## Project Structure
```
netwhale
    |------dispatch.c : handle diffrent layer and send packet to parse.
    |------main.c : Main Logic
    |------parse.c : handle protocol header and proc to sound
    |------utils.c : some utils in this project
    |------little_star.c : libao demo
    |------makefile : make this project
    |------makeaudio : make test for libao
    |------....(header files)
```

## Contribution
Welcome to write new rules to convert packet to music.