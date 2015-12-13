function [ePic, result] = disconnect(ePic)
% function who close the serial connection with e-puck
%
% [ePic, result] = disconnect(ePic)
%
% Results :
%   ePic            :   updated ePicKernel object
%   result          :   disconnection result (1:ok, 0:error)
%
% Parameters :
%   ePic            :   ePicKernel object

try
    flush(ePic);
    fclose(ePic.param.comPort);
    clear ePic.param.comPort;
    disp 'ePic successfully disconnected'
    result = 1;
    ePic.param.connected = 0;
catch
    disp 'Erreur while closing serial port'
    result = 0;
end