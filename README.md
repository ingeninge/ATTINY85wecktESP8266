# ATTINY85wecktESP8266
ATTiny85 wakes up from deep sleep on key press and resets a ESP8266, latching the triggering key



ATTINY85 zum Tastenabfragen und Wecken des ESP8266 aus dem DeepSleep. 
Zwei Taster lösen einen PCI (Pin Change Interrupt) am ATTiny85 aus. 
Es wird abgefragt, welcher Pin Change den Interrupt ausgelöst hat und eine 
kosrrespondierende Ausgangsleitung parallel zur Reset Leitung zum ESP, wird auf Low gelegt. 
Kurz darauf geht die Reset Leitung auf High und der ESP wacht aus dem DeepSleep auf, 
schaut welcher ATTiny85-Pin betätigt wurde und schickt eine MQTT Nachricht an den Broker. 
Bei mir wird damit der Wasserhahn auf oder zu gemacht wird. Man kann im Code vom ESP natürlich auch andere Aktionen folgen lassen.

Ich habe nach dieser Lösung gesucht, weil der ESP8266 nur auf einen einzelnen Taster aus dem DeepSleep gewckt werden kann. 
Ich brauche aber 2 bzw. vielleicht auch mal mehr Tasten oder Ereignisse, die einen ESP8266 aufwecken sollen.

Der ATTiny85 geht nach verrichteter Arbeit übrigens auch in den DeepSleep und die ganze Schaltung kann mit 2 AAA Batterien, 
längere Zeit betrieben werden. Wie lange weiß ich noch nicht, 
aber ein zeitgesteuertes Polling von Tasten mit dem ESP8266 geht nicht lange gut. 
Dafür verbraucht der WLAN-Chip einfach zu viel Strom.
