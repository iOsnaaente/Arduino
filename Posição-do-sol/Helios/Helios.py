from math import * 
#include <math.h>

from datetime import datetime


class Helios:

    EarthMeanRadius  = 6371.01   # Em Km 
    AstronomicalUnit = 149597890 # Em Km
    
    year  = 0
    month = 0
    day   = 0

    hours     = 0.0
    minutes   = 0.0
    seconds   = 0.0

    longitude = 0.0
    latitude  = 0.0

    ElapsedJulianDays         = 0.0 
    DecimalHours              = 0.0 
    EclipticLongitude         = 0.0 
    EclipticObliquity         = 0.0 
    RightAscension            = 0.0 
    Declination               = 0.0 
    Sin_EclipticLongitude     = 0.0 
    MeanLongitude             = 0.0 
    MeanAnomaly               = 0.0 
    Omega                     = 0.0 
    GreenwichMeanSiderealTime = 0.0 
    LocalMeanSiderealTime     = 0.0 
    Longitude                 = 0.0 
    LatitudeInRadians         = 0.0 
    HourAngle                 = 0.0 
    Cos_Latitude              = 0.0 
    Sin_Latitude              = 0.0 
    Cos_HourAngle             = 0.0 
    Parallax                  = 0.0 
    Azimuth                   = 0.0 
    ZenithAngle               = 0.0 
    Elevation                 = 0.0 


    def __init__(self, latitude : float, longitude : float  ):

        self.latitude = latitude
        self.longitude = longitude

        self.now()
        self.compute()


    def now( self ):
        data    = datetime.now()
        
        self.year    = data.year
        self.month   = data.month
        self.day     = data.day
        self.hours   = data.hour
        self.minutes = data.minute
        self.seconds = data.second


    def compute( self):

        radians = pi/180 

        # Atualizar o horário 
        self.now()

        # Converte tudo para um grande numero decimal
        self.DecimalHours = self.hours + (self.minutes + self.seconds / 60.0 ) / 60.0

        liAux1 = ( self.month - 14 ) / 12
        liAux2 = ( 1461 * ( self.year + 4800 + liAux1 ) ) / 4 + \
                 ( 367 * ( self.month - 2 - 12 * liAux1 ) ) / 12 - \
                 ( 3 * ( ( self.year + 4900 + liAux1 ) / 100 ) ) / 4 + \
                 self.day - 32075
        
        self.JulianDate = liAux2 -0.5 + self.DecimalHours / 24.0
        
        self.ElapsedJulianDays = self.JulianDate - 2451545.0
        
        self.Omega = 2.1429 - 0.0010394594 * self.ElapsedJulianDays
        
        self.MeanLongitude = 4.8950630+ 0.017202791698 * self.ElapsedJulianDays
        
        self.MeanAnomaly = 6.2400600+ 0.0172019699 * self.ElapsedJulianDays
        
        self.EclipticLongitude = self.MeanLongitude + 0.03341607 * sin( self.MeanAnomaly )  \
                                +0.00034894 * sin( 2 * self.MeanAnomaly ) -0.0001134        \
                                -0.0000203 * sin( self.Omega )

        self.EclipticObliquity = 0.4090928 - 6.2140e-9 * self.ElapsedJulianDays + 0.0000396 * cos( self.Omega )

        self.Sin_EclipticLongitude= sin( self.EclipticLongitude )
        dY1 = cos( self.EclipticObliquity ) * self.Sin_EclipticLongitude
        dX1 = cos( self.EclipticLongitude )
        
        self.RightAscension = atan2( dY1,dX1 )
        if self.RightAscension < 0.0 :
            self.RightAscension = self.RightAscension + 2*pi

        self.Declination = asin( sin( self.EclipticObliquity ) * self.Sin_EclipticLongitude )


        # Calculate local coordinates ( azimuth and zenith angle ) in degrees
        self.GreenwichMeanSiderealTime = 6.6974243242 + 0.0657098283 * self.ElapsedJulianDays + self.DecimalHours
                        
        self.LocalMeanSiderealTime = ( self.GreenwichMeanSiderealTime * 15 + self.longitude ) * radians
        self.HourAngle = self.LocalMeanSiderealTime - self.RightAscension
        self.LatitudeInRadians = self.latitude * radians
        self.Cos_Latitude = cos( self.LatitudeInRadians )
        self.Sin_Latitude = sin( self.LatitudeInRadians )
        self.Cos_HourAngle= cos( self.HourAngle )
        self.ZenithAngle = (acos( self.Cos_Latitude * self.Cos_HourAngle * cos(self.Declination) + \
                            sin( self.Declination ) * self.Sin_Latitude))
        
        dY = -sin( self.HourAngle )
        dX = tan( self.Declination ) * self.Cos_Latitude - self.Sin_Latitude * self.Cos_HourAngle
        
        self.Azimuth = atan2( dY, dX )

        if ( self.Azimuth < 0.0 ):
            self.Azimuth = self.Azimuth + 2*pi
        
        self.Azimuth = self.Azimuth / radians	

        #// Parallax Correction		
        self.Parallax    = (self.EarthMeanRadius / self.AstronomicalUnit) * sin( self.ZenithAngle )
        self.ZenithAngle = ( self.ZenithAngle + self.Parallax) / radians
        self.Elevation   = 90 - self.ZenithAngle



if __name__ == '__main__':

    lat = -29.16530 
    lon = -54.89832

    data = Helios( lat, lon )

    print("Year : ", data.year, ", Month : ", data.month, " : Day ", data.day )
    print("Hours : ", data.hours, ", Minutes : ", data.minutes, ", Seconds : ", data.seconds )
    print("Azimute : ", data.Azimuth, ", Zenite : ", data.ZenithAngle, ", Elevation : ", data.Elevation )

