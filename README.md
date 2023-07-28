# stomp-protocol-server-client-communication

In oreder to execute the client - server project :
1. Get inside the client folder and do "make"
2. Get inside the server folder and write in the terminal "mvn compile"
3. Inside the serever write in the terminal -->  mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="7777 tpc"
4. Get inside the client folder and then get inside the bin folder  
5. Inside the bin folder write to the terminal --> ./StompWCIClient 172.28.134.170 7777
6. Now the client and the server are connected and you can start the commuincaite between them
 while using the stomp comand : login ,subscribe...
ים'
