# Emoncms_light.py
# We have a lightsensor that sends values to emoncms.org. Theese values are stored as a feed with id 103273.
# This script will fetch the current value of that feed. 
# To check the value in a browser, use the following url:
# http://emoncms.org/feed/value.json?apikey=<your api>&id=103273

import httplib

id = "103273"
readapi = "8133697b1b562f52689bd680b330cb4d"
remotedomain="emoncms.org"
remoteconn = httplib.HTTPConnection(remotedomain)

remoteconn.request("GET", "/feed/value.json?apikey="+readapi+"&id="+id)
response = remoteconn.getresponse()
answer =  response.read()
# Remove quotation marks
answer = answer.replace('"', '').strip()

print "Received value: "+ answer

# Convert to long
longAns=long(answer)
# Compare
if longAns < 500000:
  print "It's daytime"
else:
  print "Good night"
  