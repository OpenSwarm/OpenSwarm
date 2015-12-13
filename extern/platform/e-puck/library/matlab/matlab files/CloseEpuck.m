%! \brief Close the communication with the e-puck
function CloseEpuck()
global EpuckPort;
fclose(EpuckPort);
clear EpuckPort;
clear global EpuckPort;
end