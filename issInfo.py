import requests
import urllib.request
import urllib.error
import time
import sys
import os
import json
url="https://isstracker.spaceflight.esa.int/tledata.txt?ts=1717787338481"
url2="https://spaceflight.nasa.gov/realdata/sightings/SSapplications/Post/JavaSSOP/orbit/ISS/SVPOST.html"
api="ejRNGboT6MHF36M5JBFN6orgWNa1NPkC376NytQe"
timestamp=1717787338481
params={"unique":timestamp}
def downloadDate(timestamp):
    response=requests.get(url,params)
    if response.status_code==200:
        return response.json()

if __name__=="__main__":
    #req=urllib.request(url)
    response=urllib.request.urlopen(url)
    data=response.read()
    #data=downloadDate(timestamp)
    print(data)
