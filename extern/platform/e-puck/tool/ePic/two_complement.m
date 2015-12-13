function value=two_complement(rawdata)

if (mod(max(size(rawdata)),2) == 1)
    error('The data to be converted must be 16 bits and the vector does not contain pairs of numbers')
end

value=zeros(1,max(size(rawdata))/2);
j=1;
for i=1:2:max(size(rawdata))
    if (bitget(rawdata(i+1),8)==1)   % Negatif number -> two'complement
        value(j)=-(invert_bin(rawdata(i))+bitshift(invert_bin(rawdata(i+1)),8)+1);
    else
        value(j)=rawdata(i)+bitshift(rawdata(i+1),8);
    end
    j=j+1;
end


function value=invert_bin(value)
for i=1:8
    value=bitset(value,i,mod(bitget(value,i)+1,2));
end
