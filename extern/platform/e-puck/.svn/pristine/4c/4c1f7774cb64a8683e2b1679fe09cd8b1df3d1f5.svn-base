function [] = EpuckFlush()
%EPUCKFLUSH Flush the input and output buffer of Matlab
%   This function is useful each time you do a send / receive cylce with
%   the e-puck. In fact, Matlab probably send old data if you don't do this
%   flush, so the e-puck could crash...

global EpuckPort;

flushinput(EpuckPort);
flushoutput(EpuckPort);
