function [ePic] = updateDef(ePic, propName, value)
% Set/clear the update bit for the different sensors. The update will be
% done by the 'ePicKernel.update' method
%
% 
% [ePic] = updateDef(ePic, propName, value)
%
% Results :
%   ePic            :   updated ePicKernel object
%
% Parameters :
%   ePic            :   ePicKernel object
%   value           :   update : 0=disable, 1=enable, 2=enable once
%   propName        :   
%       'accel'           :   set accelerometer update flag value
%       'proxi'           :   set proximity update flag value
%       'light'           :   set light update flag value
%       'micro'           :   set micro update flag value
%       'speed'           :   set motor speed update flag value
%       'pos'             :   set wheel encoder update flag value
%       'floor'           :   set floor sensor update flag value
%       'external'        :   set external sensor update flag value
%       'odom'            :   set odometry update flag value.
%                             odometry update is performed using
%                             ePicKernel.updateOdometry methode
%       'image'           :   set camera image update flag value
%                             image update is performed using
%                             ePicKernel.updateImage methode
%       'custom'          :   custom command

switch propName
case 'accel'
   ePic.update.accel = value;
case 'proxi'
   ePic.update.proxi = value;
case 'light'
   ePic.update.light = value;
case 'micro'
   ePic.update.micro = value;
case 'speed'
   ePic.update.speed = value;
case 'pos'
   ePic.update.pos = value;
case 'odom'
   ePic.update.odom = value;
case 'floor'     
   ePic.update.floor = value;
case 'external'
   ePic.update.exter = value;
case 'image'
   ePic.update.image = value;
case 'custom'
    ePic.update.custom = value;
otherwise
   error([propName,' Is not a valid asset property'])
end