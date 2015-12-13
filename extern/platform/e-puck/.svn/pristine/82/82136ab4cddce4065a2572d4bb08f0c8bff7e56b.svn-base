function [ePic] = reset(ePic, propName)
% RESET - reset a ePic counter or value
%   
% [ePic] = reset(ePic, propName)
%
% Results :
%   ePic            :   updated ePicKernel object
%
% Parameters :
%   ePic            :   ePicKernel object
%   propName        :   
%       'odom'           :  reset odometry


switch propName
% Access to ePic values
case 'odom'     
   ePic.value.odom = zeros(1,3);
   ePic.updated.odom = 0;
   ePic.param.odomIni = 0;
    
otherwise
   error([propName,' Is not a valid asset property'])
end