u1=udp('127.0.0.1','RemotePort',6970,'LocalPort',6969);
u2=udp('127.0.0.1','RemotePort',6980,'LocalPort',6979);

fopen(u1);
fopen(u2);

while(1)
    data=fscanf(u1)%主机2接收文本消息
    tf = strcmp(data,'SimulationOVER');
    if tf==1
        break;
    end
    fwrite(u2,'Hello Client 500');
        
    
end

fclose(u1);
fclose(u2);
% delete(u1);
% clear u1


