function [stat, up] = status(ePic, propName)
% GET the update status of the different sensors
% 
% [stat, up] = status(ePic, propName)
%
% results :
%   stat          :   status of sensors
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
%       'connectionState' : get the connection flag (1:connected, 0:disconnected)


switch propName
% Access to ePic values
case 'accel'
   stat = ePic.update.accel;
   up = ePic.updated.accel;
case 'proxi'
   stat = ePic.update.proxi;
   up = ePic.updated.proxi;
case 'light'
   stat = ePic.update.light;
   up = ePic.updated.light;
case 'micro'
   stat = ePic.update.micro;
   up = ePic.updated.micro;
case 'speed'
   stat = ePic.update.speed;
   up = ePic.updated.speed;
case 'pos'
   stat = ePic.update.pos;
   up = ePic.updated.pos;
case 'odom'     
   stat = ePic.update.odom;
   up = ePic.updated.odom;
case 'floor'     
   stat = ePic.update.floor;
   up = ePic.updated.floor;
case 'external'
    stat = ePic.update.exter;
    up = ePic.updated.exter;
case 'image'
   stat = ePic.update.image;
   up = ePic.updated.image;
case 'custom'
   stat = ePic.update.custom;
   up = ePic.updated.custom;
% Access to ePic parameters
case 'connectionState'
    stat = ePic.param.connected;
otherwise
   error([propName,' Is not a valid asset property'])
end