# LEDStripVisualiser
Software to visualise music with an arduino and a msgeq7 on an LED strip
Also runs on ESP8266 so that LED strip can be controlled over the internet via a web app or through TCP GET.

Currently 9 music visulisation programs.

When first turned on, through use of Wifimanager the ESP with create a wifi network to connect to where the user can input thier SSID and Password.

"hostname" should be changed in the code.

<br/>
Also whole strip can be used as a white light with variable brightness through the same method.

<b>Dependencies</b>
  <p>FastLED Library</p>
  <p>WifiManager</p>
  <p>ESP8266Wifi Arduino Library</p>
  <p>MSGEQ7 Library</p>
  
  
  
  <b>Schematic</b>
  ![alt tag](https://github.com/robmckay/LEDStripVisualiser/blob/master/schematic1.jpg)
  
  <br/>
  <b>PCB</b><br/>
  ![alt tag](https://github.com/robmckay/LEDStripVisualiser/blob/master/PCB.png)
  

