clear all
s = tcpip('127.0.0.1', 6969, 'NetworkRole', 'server');
set(s, 'InputBufferSize', 3000); 
set(s, 'outputBufferSize', 3000); 
set(s,'Timeout',1);
fopen(s); 
dataALL=[];
AnsALL=[];

while 1
    
if get(s,'BytesAvailable') > 1
    data=strcat(fread(s, s.BytesAvailable, 'uint8')')
    tf = strcmp(data,'SimulationOVER');
    if tf==1
        break;
    end
     eval(strcat('dataM=[',data,'];'));
     dataALL=[dataALL;dataM];
     speed=milp(dataM);
     AnsALL=[AnsALL;speed(1:8)'];
    fwrite(s,num2str(speed(1:8)' )); 
end

end

fclose(s);