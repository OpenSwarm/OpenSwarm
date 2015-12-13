function display(ePic)
% display sensor informations
%
% display(ePic)
%
% Results :
%   ePic            :   updated ePicKernel object
%
% Parameters :
%   ePic            :   ePicKernel object

disp '------------- ePic Kernel v2.1 -------------';
disp ' ';
disp '-----> sensors informations :'
disp 'Accelerometers : '
disp (ePic.value.accel);
disp 'Proximity sensors : '
disp (ePic.value.proxi);
disp 'Light sensors : '
disp (ePic.value.light);
disp 'Microphones : '
disp (ePic.value.micro);
disp 'Motor speed : '
disp (ePic.value.speed);
disp 'Motor position : '
disp (ePic.value.pos);
if (ePic.update.floor > 0)
    disp 'Floor Sensors : '
    disp (ePic.value.floor);
end
if (ePic.update.exter > 0)
    disp 'External sensors : '
    disp (ePic.value.exter);
end