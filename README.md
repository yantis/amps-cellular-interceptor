# AMPS Cellular Interceptor

## What is it?

This code is for an old project I made over twenty years ago in 1997. 

In the 80s & 90s, Cellular phones were mostly analog meaning you could listen in on phone calls. (1983 to 2008)
Most cell phones were in peoples cars, and as they drove around they would jump from cell tower to cell tower 
You can read more about (AMPS) Advanced Mobile Phone System here
https://en.wikipedia.org/wiki/Advanced_Mobile_Phone_System

What this tool did was follow people that were flagged interesting and record the calls based on the flagged "alert level" even as they jumped from cell tower to cell tower.
It would always prioritize recording of calls based on the alert/interesting level. So if someone of high interested started a call, it would record them over someone of a lower alert level.
It also maintained a database of the call history etc.

Then it would store a WAV audio file with a snippet of data about that call (most of it was manually added over time from listening to the calls).
A hypothetical situation would be something like a border agent taking a bribe would be interesting so you would raise the alert level to flag that users calls.
(I do not have any of this history or audio files. I just kept the source code. It has been so long I don't even remember much about this project)


## Type:
Window MFC C++ Application

## Requires specific hardware:
- Scanner Radio Shack Realistic Pro-2005 or Pro-2006 (https://wiki.radioreference.com/index.php/Pro-2006)
  - Famous for its cellular telephone monitoring modification which allowed one to cut a diode inside the scanner to receive the old analog cell phone range)
  - more info here: (http://cd.textfiles.com/thegreatunsorted/texts/cell_nfo/cellfrek.txt)
- OptoScan456 Interface:
  - The OptoScan456 provides a serial computer interface, as well as built-in circuitry to decode Continuous Tone-Controlled Squelch System (CTCSS) sub-audible tones, Digitally-Coded Squelch (DCS) codes, and Dual-Tone Multi-Frequency (DTMF) digits. (http://www.monitoringpost.com/pro2006/os456.html)

## Date:
1997

## Status
Historical only so no updates ever. Not sure what you would do with it but I do know the one in a million chance someone might find something of use here is better than it sitting in a zip file somewhere.

# License
MIT




## The output text file was in this format for calls with attached WAV file (everything is also stored in the database):
```
TIME       : 03-01-98 07:11:39pm
CALL TIME  : 58 Seconds
NUMBER     : (526) 620-3338
AREA       : Tijuana, BC Mexico   
NAME       : 
NAME #2    : 
OCCUPATION : 
SUBJECT    : 
GENDER     : 
LANGUAGE   : Spanish
DTMF DIGITS: 
INFO #1    : 
INFO #2    : 
INFO #3    : 
LINKED FILE: 
ESN        : 
SCM        : 
LAST CALL  : 03-01-98 07:11p
TOTAL CALLS: 1
MODEM      : 
FAX        : 
ALERT LEVEL: 
RECORD CALL: 
SYSTEM TYPE: A
```