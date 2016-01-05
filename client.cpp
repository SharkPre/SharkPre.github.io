///
/// client
///
#include <winsock2.h>
#include <stdio.h>
#include <string>

#pragma comment( lib, "ws2_32.lib" )

int main( int argc, char **argv )
{
	if( argc != 4 )
	{
		fprintf( stderr, "usage : %s host_ip host_port package_count.\n", argv[0] );
		exit( 1 );
	}

	WSADATA wd;
	WSAStartup( MAKEWORD( 2, 2 ), &wd );

	struct sockaddr_in addr = { 0 };
	addr.sin_addr.s_addr = inet_addr( argv[1] );
	addr.sin_port = htons( (unsigned short) atoi( argv[2] ) );
	addr.sin_family = AF_INET;

	SOCKET s = socket( AF_INET, SOCK_STREAM, 0 );

	printf( "connect to %s-%d...\n", argv[1], atoi( argv[2] ) );

	int ret = connect( s, (sockaddr*) &addr, sizeof( addr ) );
	if( ret == SOCKET_ERROR )
	{
		fprintf( stderr, "connect failed [%d].\n", WSAGetLastError() );
		closesocket( s );
		exit( 1 );
	}

	printf( "connect ok.\n" );

	int pc = atoi( argv[3] );
	for( int i = 0; i < pc; ++ i )
	{
		char buf[512] = {0};
		sprintf( buf, "packet %d", i + 1 );
		int send_bytes = send( s, buf, strlen( buf ) + 1, 0 );
		printf( "send data : %s [%d bytes].\n", buf, send_bytes );

		Sleep( 100 );
		int bytes = recv( s, buf, sizeof( buf ), 0 );
		if( bytes > 0 )
		{
			printf( "receive data : %s.[%d bytes]\n", buf, bytes );
		}

		Sleep( 100 );
	}

	getchar();
	closesocket( s );

	WSACleanup();
	return 0;
}
