### 1st Portscanner in C 
Simple TCP scan with connect() works.
![image](https://github.com/user-attachments/assets/6af0d7c2-807a-4956-8d1d-51f242d6dd8d)

Was not able to make UDP work atm, it looks like 
you have to craft custom packages for that. 

NMAP sends these when using -sU flag.
```
Internet Protocol Version 4, Src: 100.121.248.28, Dst: 100.89.159.80
    0100 .... = Version: 4
    .... 0101 = Header Length: 20 bytes (5)
    Differentiated Services Field: 0x00 (DSCP: CS0, ECN: Not-ECT)
        0000 00.. = Differentiated Services Codepoint: Default (0)
        .... ..00 = Explicit Congestion Notification: Not ECN-Capable Transport (0)
    Total Length: 28
    Identification: 0x4bdf (19423)
    000. .... = Flags: 0x0
    ...0 0000 0000 0000 = Fragment Offset: 0
    Time to Live: 49
    Protocol: ICMP (1)
    Header Checksum: 0xddc2 [validation disabled]
    [Header checksum status: Unverified]
    Source Address: 100.121.248.28
    Destination Address: 100.89.159.80
    [Stream index: 0]

Internet Control Message Protocol
    Type: 8 (Echo (ping) request)
    Code: 0
    Checksum: 0x10d8 [correct]
    [Checksum Status: Good]
    Identifier (BE): 59175 (0xe727)
    Identifier (LE): 10215 (0x27e7)
    Sequence Number (BE): 0 (0x0000)
    Sequence Number (LE): 0 (0x0000)
    [Response frame: 3319]
```
This is something for another session. 


