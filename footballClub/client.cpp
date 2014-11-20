//Le programme client 
#include<iostream>
#include<iomanip>
#include<winsock.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
    WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	//1.Commencer la bibliothèque "SOCKET", la version est 2.0
	wVersionRequested = MAKEWORD(2,0);
	err = WSAStartup(wVersionRequested,&wsaData);
	if(err != 0) //Tester si le Socket est initialisé
	{
	    cout<<"Socket2.0初始化失败,Exit!";
		return 0;
	}
	//Tester si la version de Socket est 2.0
	if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=0)
	{
	    WSACleanup();  //Quitter l'environnement du socket
		return 0;
	}
	//2.Créer socket
	SOCKET sock_client = socket(AF_INET,SOCK_STREAM,0);
	if(sock_client == INVALID_SOCKET)              
	{
	    cout<<"Socket 创建失败, Exit!";
		return 0;
	}
	//Connecter le serveur
	SOCKADDR_IN addr_ser;
	addr_ser.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr_ser.sin_family = AF_INET;
	addr_ser.sin_port = htons(6000);
    connect(sock_client,(SOCKADDR*)&addr_ser,sizeof(SOCKADDR));
	//envoyer et recevoir les messages 
	int bytes_sent;
	int bytes_rece = SOCKET_ERROR;
	char send_buf[5000], rece_buf[5000];

	while(true)
	{
	    recv(sock_client,rece_buf,5000,0);
		cout<<rece_buf;
		if(rece_buf[0] != '#') //Si le premier caractère du message re?u est '#'，le client n'ai pas
		{                        //besoin de répondre ce message  
		    cin>>send_buf;
		    send(sock_client,send_buf,strlen(send_buf)+1,0); 
		}
	}	

	system("pause");
	return 0;
}
