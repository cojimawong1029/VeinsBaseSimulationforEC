u1=udp('127.0.0.1','RemotePort',6970,'LocalPort',6980);


fopen(u1);


while(1)
    data=fscanf(u1)%主机2接收文本消息
    tf = strcmp(data,'SimulationOVER');
    if tf==1
        break;
    end

    
end

fclose(u1);

% delete(u1);
% clear u1


