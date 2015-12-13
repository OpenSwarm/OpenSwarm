function [output] = readBinSized(ePic, n )
%   readBin allow you to read n byte in the buffer in binary mode.
%   Datas are in raw format
%   
%   [output] = readBinSized(ePic, n )
%
% Results :
%   output          :   recieved data in raw format
%
% Parameters :
%   ePic            :   ePicKernel object
%   n               :   number of byte to read in the buffer

try
    output =  fread(ePic.param.comPort, n, 'uint8');
catch
    output = 0;
end