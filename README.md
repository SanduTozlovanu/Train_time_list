# Train's time list
</br>
<img src="https://imgur.com/4eWiUkx.png"></img>

A Server-Client application for displaying Train's time list
Built using POSIX C and C++, utilizing [Pugixml](https://pugixml.org/) XML library.
## Table of Contents
* [Starting up](#starting-up)
* [Using the Station Menu](#Using-the-Station-Menu)
  * [Commands](#commands)
  * [Details](#details)
* [Using the Train Menu](#Using-the-train-Menu)
  * [Commands](#commands)
  * [Details](#details)
## Starting up
1.Download the [Train's_timeline.zip](https://drive.google.com/uc?export=download&id=1AO4BpEUP8okBTI4fgFpRzQ7GXOHy_Szz) <br>
2.Unzip the downloaded file. <br>
3.Enter in the downloaded directory in the LINUX terminal. <br>
4.Start the Server by entering the Server file and typing ./Server <br>
<br><br><img src="https://media.giphy.com/media/cSmxhb2jeSIIRTaarR/giphy.gif"></img> <br>
## Using the Station Menu
Enter the "Simple_Client" folder and enter the following command: ./Simple_Client (Server's IP adress) (Server's port) (Station's ID) <br>
Example: ./Simple_Client 127.0.0.1 2024 10017 <br>
<br><br><img src="https://media.giphy.com/media/ENXOLofUMgNrsO1SGh/giphy.gif"></img> <br>
### Commands<br>
1: Get the next incoming trains real-time - Starts showing all the incoming trains , along with their estimated time arrival. <br>
2 : Get all the trains that come/came today - Shows the full list of trains that arrive/arrived today at the station. <br>
3 : Get the name of the station - Shows the name of station. <br>
4 : Get current time - Displays the current time. <br>
5 : Exit - Exits the Client. <br>
<br><br><img src="https://media.giphy.com/media/ZG9ChAYpOEw8mp1H5v/giphy.gif"></img> <br>
### Details<br>
Get the next incoming trains real-time command will update the incoming train's list real-time , displaying all the specific details. To stop the update , press "CTRL+C". <br>
## Using the Train Menu<br>
Enter the "Special_Client" folder and enter the following command: ./Special_Client (Server's IP adress) (Server's port) (Train's ID) <br>
Example: ./Special_Client 127.0.0.1 2024 4340 <br>
<br><br><img src="https://media.giphy.com/media/TNzaaoBLaQFCubwNmm/giphy.gif"></img> <br>
### Commands<br>
1: get_my_stations - Shows all the stations's name and arrival time from the route. <br>
2: report_sooner (number of minutes) - Reports that the train will come later in the all the next stations. <br>
3: report_delay (number of minutes) - Reports that the train will come earlier in all the next stations. <br>
4: get_current_time - Displays the current time.  <br>
5: exit - Exits the Client. <br>
<br><br><img src="https://media.giphy.com/media/NopJcxJsx0tstIHwNN/giphy.gif"></img>
### Details<br>
Report Sooner Command will change the arrival time of ALL the station where ARRIVAL TIME is later than the CURRENT TIME.<br>
Example: report_sooner 20         ( sooner with 20 mins) <br>
Report Delay Command will change the arrival time of ALL the station where ARRIVAL TIME is later than the CURRENT TIME. <br>
Example: report_delay 20         ( delay with 20 mins) <br>
