import requests
import urllib.request
import urllib.error
import time
import sys
import os
import json
import math
import numpy as np
from skyfield.api import Topos,load,EarthSatellite
url="https://isstracker.spaceflight.esa.int/tledata.txt?ts=1717787338481"
url2="https://spaceflight.nasa.gov/realdata/sightings/SSapplications/Post/JavaSSOP/orbit/ISS/SVPOST.html"
api="ejRNGboT6MHF36M5JBFN6orgWNa1NPkC376NytQe"
timestamp=1717787338481
params={"unique":timestamp}
def downloadDate(timestamp):
    response=requests.get(url,params)
    if response.status_code==200:
        return response.json()
def get_bearing(lat1,long1,lat2,long2):
    lat1=math.radians(lat1)
    lat2=math.radians(lat2)
    long1=math.radians(long1)
    long2=math.radians(long2)
    bearing=math.atan2(math.sin(long2-long1)*math.cos(lat2),math.cos(lat1)*math.sin(lat2)-math.sin(lat1)*math.cos(lat2)*math.cos(long2-long1))
    bearing=math.degrees(bearing)
    return bearing
if __name__=="__main__":
    #req=urllib.request(url)
    response=urllib.request.urlopen(url)
    data=response.read()
    #data=downloadDate(timestamp)
    print(data)
    tle_lines=data.decode('utf-8').splitlines()
    print(tle_lines[0],tle_lines[1],tle_lines[2])
    ts=load.timescale()
    data2=EarthSatellite(tle_lines[1],tle_lines[2],tle_lines[0],ts)
    print(data2)
    t=ts.now()
    geocentric=data2.at(t)
    subpoint=geocentric.subpoint()
    lat=subpoint.latitude.degrees
    long=subpoint.longitude.degrees
    altitude=subpoint.elevation.km 
    print(lat)
    print(long)
    print(altitude)
    lat1=35.07821247945041
    long1= -106.65145440859672
    lat2=35.078774406429616
    long2=-106.65009184645479
    bearing=get_bearing(lat1,long1,lat2,long2)
    print(bearing)

