function [output] = readBin(ePic)
%   readBin allow you to read the buffer in binary mode.
%   Datas are in raw format
%   
%   [output] = readBin(ePic)
%
% Results :
%   output          :   recieved data in raw format
%
% Parameters :
%   ePic            :   ePicKernel object

try
    output =  fread(ePic.param.comPort, ser_Port.BytesAvailable, 'uint8');
catch
    output = 0;
end