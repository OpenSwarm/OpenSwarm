%! \brief Open the communication with the e-puck
 % \params port The port in wich the e-puck is paired to.
 % it must be a string like that "COM11" if e-puck is paired
 % on COM 11.
 %/
function OpenEpuck(port)
global EpuckPort;
EpuckPort = serial(port,'BaudRate', 115200,'inputBuffersize',4096,'OutputBufferSize',4096,'ByteOrder','littleendian');
try
    fopen(EpuckPort);
catch
    % could not open the port, delete the global variable
    clear EpuckPort
    clear global EpuckPort;
    disp 'Error, Could not open serial port'
    return;
end
return