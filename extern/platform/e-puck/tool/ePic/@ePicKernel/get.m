function [val, up] = get(ePic, propName)
% GET Get asset properties from the specified object
% and return the value
% 
% [val, up] = get(ePic, propName)
%
% results :
%   val             :   value of sensors
%   up              :   flag that defines if asked values has been updated on
%                        the last update (0:not updated, 1: updated)
% Parameters :
%   ePic            :   ePicKernel object
%   propName        :   
%       'accel'           :   get accelerometer values
%       'proxi'           :   get proximity sensors values
%       'light'           :   get light sensors values
%       'micro'           :   get micro values
%       'speed'           :   get motor speed values
%       'pos'             :   get wheel encoder values
%       'odom'            :   get odometry position
%       'floor'           :   get floor sensor values
%       'external'        :   get external sensor value
%       'image'           :   get camera image
%       'custom'          :   get the raw values requested by the custom command
%       'connectionState' :   get the connection flag (1:connected, 0:disconnected)


switch propName
% Access to ePic values
case 'accel'
   val = ePic.value.accel;
   up = ePic.updated.accel;
case 'proxi'
   val = ePic.value.proxi;
   up = ePic.updated.proxi;
case 'light'
   val = ePic.value.light;
   up = ePic.updated.light;
case 'micro'
   val = ePic.value.micro;
   up = ePic.updated.micro;
case 'speed'
   val = ePic.value.speed;
   up = ePic.updated.speed;
case 'pos'
   val = ePic.value.pos;
   up = ePic.updated.pos;
case 'odom'     
   val = ePic.value.odom;
   up = ePic.updated.odom;
case 'floor'     
   val = ePic.value.floor;
   up = ePic.updated.floor;
case 'external'
    val = ePic.value.exter;
    up = ePic.updated.exter;
case 'image'
   val = ePic.value.image;
   up = ePic.updated.image;
case 'custom'
   val = ePic.value.custom;
   up = ePic.updated.custom;
% Access to ePic parameters
case 'connectionState'
    val = ePic.param.connected;
otherwise
   error([propName,' Is not a valid asset property'])
end