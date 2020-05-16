u1=udp('127.0.0.1','RemotePort',6970,'LocalPort',6969);
u2=udp('127.0.0.1','RemotePort',6980,'LocalPort',6979);

fopen(u1);
fopen(u2);
QS=[];
KS=[];

while(1)
    data=fscanf(u1);%主机2接收文本消息
    if(isempty(data))
        continue;
    end
    tf = strcmp(data,'SimulationOVER');
    if tf==1
        break;
    end
    eval(strcat("input=[",data,"];"))
    a=input(1);
    b=input(2);
    g=input(3);
    t=input(4);
    mu=input(5);
    q=input(6);
    QS=[QS,q];
    n=input(7);
    p=input(8);
    
    fun=@(K)(a*(K/q/mu)+b*(K/mu/q*q*p)+g*(K^(-1*n)));
    K=fmincon(fun,0.9*q,(1/(mu*q)),(0.1-t))
    KS=[KS,K];
    
    

    fwrite(u2,'Hello Client 500');
    
end

fclose(u1);
fclose(u2);


% delete(u1);
% clear u1


