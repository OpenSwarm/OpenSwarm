function [ePic, result] = connect(ePic, port)
% function who open serial connection to the e-puck.
%
% [ePic, result] = connect(ePic, port)
%
% Results :
%   ePic            :   updated ePicKernel object
%   result          :   connection result (1:ok, 0:error)
%
% Parameters :
%   ePic            :   ePicKernel object
%   port            :   communication port name 'COMxy'

ePic.param.comPort = serial(port,'BaudRate', 115200,'inputBuffersize',65536,'OutputBufferSize',4096,'ByteOrder','littleendian');
try
    % open port
    fopen(ePic.param.comPort);
    flush(ePic);
    write(ePic,'P,0,0');
    read(ePic);
    write(ePic,'P,0,0');
    read(ePic);
    disp 'ePic successfully connected'
    result= 1;
    ePic.param.connected = 1;
catch
    % could not open the port
    disp 'Error, Could not open serial port'
    clear ePic.param.comPort;
    result = 0;
end