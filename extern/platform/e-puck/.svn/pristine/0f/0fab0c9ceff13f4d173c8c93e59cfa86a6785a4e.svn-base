function ePic = set(ePic,varargin)
% SET Set asset properties and return the updated object. The information
% will be send to ePuck during the next update cycle.
% 
% ePic = set(ePic,propName,value)
%
% Results :
%   ePic            :   updated ePicKernel object
%
% Parameters :
%   ePic            :   ePicKernel object
%   value           :   value to set
%   propName        :   
%       'speed'           :   set motor speed values
%       'ledON'           :   switch the led number (value) on
%       'ledOFF'          :   switch the led number (value) off
%       'odom'            :   set odometry values for current position and
%                             set the initialization flag to restart
%                             odometry
%       'camMode'         :   set camera mode (0:grayscale, 1:color)
%       'camSize'         :   set camera image size [width, height]
%       'camZoom'         :   set camera zoom factor (1,4,8)
%       'external'        :   select the external sensor to update. For the
%                             list of sensor, please refer to the 
%                             ePicKernel.update file
%       'ledIR'           :   select the IR led for the external 5 led 
%                             range sensor
%       'custom'          :   defines a custom command to send to the
%                             e-puck. It takes as argument a vector
%                             containing [command, size of the data]

propertyArgIn = varargin;
while length(propertyArgIn) >= 2,
   prop = propertyArgIn{1};
   val = propertyArgIn{2};
   propertyArgIn = propertyArgIn(3:end);
   switch prop
   case 'speed'
       ePic.set.speed = val;
       % Verify max speed
       if ePic.set.speed(1) < -1024
           ePic.set.speed(1) = -1024;
       elseif ePic.set.speed(1) > 1024
           ePic.set.speed(1) = 1024;
       end
       if ePic.set.speed(2) < -1024
           ePic.set.speed(2) = -1024;
       elseif ePic.set.speed(2) > 1024
           ePic.set.speed(2) = 1024;
       end
       
   case 'ledON'
       if (val > 9 || val < 0)
           for i=1:10 
            ePic.set.led(1,i) = 1;
           end
       else
            ePic.set.led(1,val+1) = 1;
       end
   case 'ledOFF'
       if (val > 9|| val < 0)
           for i=1:10
            ePic.clear.led(1,i) = 1;
           end
       else
            ePic.clear.led(1,val+1) = 1;
       end
   case 'odom'     
        ePic.value.odom = val;
        ePic.param.odomIni = 0;
   case 'camMode'
       ePic.param.imgMode = val;
       ePic.param.imgMod = 1;
   case 'camSize'
        ePic.param.imgSize = val;
        ePic.param.imgMod = 1;
   case 'camZoom'
        ePic.param.imgZoom = val;
        ePic.param.imgMod = 1;
   case 'ledIR'      
        ePic.param.ledIR = val;
   case 'external'
       ePic.param.extSel = val;
   case 'custom'
       ePic.param.customCommand = val(1:size(val,2)-1);
       ePic.param.customSize = val(size(val,2));
   otherwise
      error('Asset properties: Descriptor, Date, CurrentValue')
   end
end