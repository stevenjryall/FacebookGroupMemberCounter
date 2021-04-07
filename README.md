# FacebookGroupMemberCounter

In celebration of my Facebook group 'FTIJPI' reaching 100k members, I built this member counter using 'Facebook graph' API, an esp8266 board, a cheap OLED display and an enclosure printed with my Prusa Mini+!

Every 30 seconds it updates with the current number of members, and it's been super reliable so far! 

<img src="https://github.com/stevenjryall/FacebookGroupMemberCounter/blob/main/ftijpi_member_counter.jpeg" width="350" title="hover text">

The 'group_member_counter.ino' program is built specifically to query the number of members in a group
on Facebook. It will only send over the
number if the request contains an access token generated by an admin
of the group in question (as far as I'm aware, at the time of writing). 

Credit goes to "sjsd" over at "https://www.instructables.com/Facebook-Fan-Count/" 
for the original code! I simply tweaked it for my needs & spruced it up for the 
FTIJPI community! His process of doing this is expired now since Facebook
changed it's API's and Developer App setup, so this instructional guide should serve as an updated method!

# Updates needed:

. A menu system utilising buttons to view different data (maybe from different groups?)
and toggle debug mode! 

. A screensaver since OLED screens can suffer from burn-in 

. Create a server/AP so the user can connect and change credentials from their phone 

. Perhaps a way to auto-reconnect to WiFi if we lose connection if that's not already built into the libraries...

# Hardware...

The specific items I used are:

. ESP8266 ESP-12N WIFI Development Board

. 0.96" I2C IIC SPI 128X64 OLED Display Module for Arduino

I grabbed some prototype board and laid everything out how I liked it. Because of the way the displays pins were behind the WiFi board, I soldered the display first and used the top pins to make my connections.

I hooked up SDA on the display to the D3 pin, and SCL on the display to the D5 pin on the WiFi board. I powered the display using the 3.3v and GND pins on the WiFi board. I haven't hooked up the buttons yet, but there's plenty of room to do so in the future. 

The front:

<img src="https://github.com/stevenjryall/FacebookGroupMemberCounter/blob/main/group_member_counter_images/board_front.jpeg" width="350" title="hover text">

The back:

<img src="https://github.com/stevenjryall/FacebookGroupMemberCounter/blob/main/group_member_counter_images/board_back.jpeg" width="350" title="hover text">

Next I printed my enclosure. The model I used was from an Octoprint monitor project I did a while ago with this same board. I used the 'Pancake method' to create the 'FTIJPI' text on the front. More about that here "https://www.youtube.com/watch?v=KV2AjyowXX4". I've included my .f3d file for anybody who wants to alter it! 

<img src="https://github.com/stevenjryall/FacebookGroupMemberCounter/blob/main/group_member_counter_images/enclosure_image.png" width="350" title="hover text">

# Facebook Graph...

Now I had the hardware ready, I had to setup the Facebook Graph API. It's pretty simple...

. Head over to "https://developers.facebook.com/" and create a developers account. 

. Create an app! The name of it doesn't matter, you just need an app in place so you can use the 'Facebook Graph' API. When doing this, ensure to disable your Adblock if you have one running - it'll run a security prompt but it won't show the actual prompt if you have Adblock activated...

. Head over to Facebook Graph via Tools > Graph API Explorer.

<img src="https://github.com/stevenjryall/FacebookGroupMemberCounter/blob/main/group_member_counter_images/tools_to_graph.png" width="350" title="hover text">

. On the right you'll see various options. You want to set 'Facebook App' as the App you've just created, 'User or page' as 'User token', and create a Permission called 'groups_access_member_info'. Perhaps alternate configurations would also work but this is just how I have it setup. 

. Hit 'Generate access token' and it will produce one in the box. This is the access token you need to put in the URL in the code where it says 'YOUR-ACCESS-TOKEN'. This token is a 'short lived' token and will only work for one hour. If you'd like one that works for longer, Facebook will extend it to a couple months if you head over to Tools > Access Token Debugger, paste in your new token, press 'Debug' and scroll until you see 'Extend Acess Token'. Hit that and it should extend it for two months! Be prepared to have to re-fresh this token after the alloted time though...

# Facebook fingerprint...

I only know how to find Facebooks fingerprint on Chrome, but what I did was: 

. Go to Facebook, and hit the lock icon in the address bar next to the website URL. A menu should pop up, click on 'Certificate'. 

<img src="https://github.com/stevenjryall/FacebookGroupMemberCounter/blob/main/group_member_counter_images/chrome_lock_menu.jpeg" width="350" title="hover text">

. Another box should pop up, and you should see 'Details' listed under the certificate for Facebook. Click on this and scroll all the way to the bottom, where you should see 'Fingerprint' followed by two fingerprints - One in SHA-256 and one in SHA-1 - you want SHA-1.

. Copy and paste the fingerprint into the code where it says 'FINGERPRINT-OF-FACEBOOK-VIA-YOUR-BROWSER'. 

# Uploading...

Now with everything hooked up and your credentials sorted, you can upload the code to the device and watch the magic happen! Here's my settings in Arduino for the board I used (ensure to find your board under the 'Port' option):

<img src="https://github.com/stevenjryall/FacebookGroupMemberCounter/blob/main/group_member_counter_images/arduino_settings.jpeg" width="350" title="hover text">

Before you do this though, ensure you have the correct libraries installed! You can hit Sketch > Include Libraries > Manage Libraries in the Arduino IDE and a box will pop up. Search for and install the following libraries: 

. Arduino_JSON

. ESP8266 and ESP32 OLED driver - I use the one by Thingpulse, version 4.1.0, though I think Adafruits libraries are very similar if you'd prefer to use them. Your mileage may vary! 

The libraries ESP8266WiFi and WiFiClientSecure will be in your arsenal if you load "http://arduino.esp8266.com/stable/package_esp8266com_index.json" into the address bar found in Arduino > Preferences > Additional Boards Manager URLs and restart the Arduino IDE. More information about that can be found here "https://arduino-esp8266.readthedocs.io/en/latest/installing.html" if you're having trouble. 

<img src="https://github.com/stevenjryall/FacebookGroupMemberCounter/blob/main/group_member_counter_images/esp_libraries.jpeg" width="350" title="hover text">

If you see Googles dinosaur after you've uploaded, you know it's worked! ;) 

# Thanks!

A special thanks to all FTIJPI members from the FTIJPI team on Facebook for hanging out, creating a great community with us and sharing your amazing work. You all inspire me everyday to learn and create! 
