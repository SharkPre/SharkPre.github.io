///
/// echo server using select model
/// @author Kevin Lynx
/// @date 5.16.2008
///
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment( lib, "ws2_32.lib" )

#define SEND_DEF_BUF 1024
#define RECV_DEF_BUF 1024
#define MAX_SESSION_COUNT 100

///
/// win_fd_set to wrap the fd_set
///
struct win_fd_set
{
	fd_set *_set;
	unsigned int array_size;
};

///
/// a session represents a peer-to-peer connection
///
struct session
{
	SOCKET _socket;
	sockaddr_in _addr;
	char *_send_buf;
	/// the _send_buf size
	int _send_buf_size;
	//// the position where should write data
	int _send_pos;
	/// the data size that has been already sended.
	int _send_size;
};

///
/// server
///
struct server
{
	SOCKET _socket;
	sockaddr_in _addr;

	session _clients[MAX_SESSION_COUNT];
	int _add_pos;

	/// fd set to poll
	win_fd_set _read_set;
	win_fd_set _write_set;
};

///
/// socket error
///
void socket_error( const char *error_desc, int error_code )
{
	fprintf( stderr, "socket error : %s . error code : %d.\n", error_desc, error_code );
}

///
/// format a string
///
const char *FORMAT_STR( const char *format, ... )
{
	static char buf[512];
	va_list list;
	va_start( list, format );
	vsprintf( buf, format, list );
	va_end( list );
	return buf;
}

///
/// create a win_fd_set 
///
win_fd_set fs_create( unsigned int max_count )
{
	max_count = max_count < FD_SETSIZE ? FD_SETSIZE : max_count;
	win_fd_set fs;
	fs.array_size = max_count;
	fs._set = (fd_set*) malloc( sizeof( fd_set ) + sizeof( SOCKET ) * ( max_count - FD_SETSIZE ) );

	return fs;
}

///
/// delete a win_fd_set
///
void fs_delete( win_fd_set *fs )
{
	fs->array_size = 0;
	free( fs->_set );
}

///
/// macro to clear the win_fd_set
///
#define WINFD_ZERO( set ) FD_ZERO( ((win_fd_set*)set)->_set )

///
/// macro to remove a fd from the win_fd_set
///
#define WINFD_CLR( fd, set ) FD_CLR( fd, set )

///
/// macro to add a fd to the win_fd_set
///
#define WINFD_SET( fd, set ) do { \
	unsigned int __i ; \
	for( __i = 0; __i < ((win_fd_set*)set)->_set->fd_count; ++ __i ) { \
		if( ((win_fd_set*)set)->_set->fd_array[__i] == (fd) ) { \
			break; \
		} \
	} \
	if( __i == ((win_fd_set*)set)->_set->fd_count ) { \
		if( ((win_fd_set*)set)->_set->fd_count < ((win_fd_set*)set)->array_size ) { \
			((win_fd_set*)set)->_set->fd_array[((win_fd_set*)set)->_set->fd_count++] = (fd); \
		} \
	} \
}while(0) 

///
/// macro to check whether the fd is in the set
///
#define WINFD_ISSET( fd, set ) FD_ISSET( fd, ((win_fd_set*)set)->_set )

///
/// create a session when a new connection comes
///
session session_create( const server *host )
{
	session new_session;
	int size = sizeof( new_session._addr );
	new_session._socket = accept( host->_socket, (sockaddr*) &new_session._addr, &size );
	new_session._send_buf_size = 0;
	new_session._send_size = 0;
	new_session._send_buf = (char*) malloc( SEND_DEF_BUF );
	new_session._send_pos = 0;
	return new_session;
}

///
/// destroy a session when the connection is closed
///
void session_destroy( session *client )
{
	closesocket( client->_socket );
	client->_socket = INVALID_SOCKET;
	free( client->_send_buf );
}

///
/// check whether a session is valid
///
bool session_valid( session *client )
{
	return client->_socket != INVALID_SOCKET;
}

///
/// add data to the session, and send it later
///
void session_adddata( session *client, const char *buf, int size )
{
	if( size + client->_send_pos >= client->_send_buf_size )
	{
		// the buffer is not enough, reallocate
		int add_size = size > SEND_DEF_BUF ? size : SEND_DEF_BUF ;
		client->_send_buf_size += add_size ;
		client->_send_buf = (char*) realloc( client->_send_buf, client->_send_buf_size );
	}

	// enough memory, copy the data
	memcpy( &client->_send_buf[client->_send_pos], buf, size );
	client->_send_pos += size;
}

///
/// reset a session especially for its send buffer
///
void session_reset( session *client )
{
	client->_send_pos = 0;
	client->_send_size = 0;
}

///
/// send data to a client
///
int session_send( session *client )
{
	if( client->_send_pos <= client->_send_size )
	{
		// no data to send
		return 0;
	}

	//int send_size = send( client->_socket, &client->_send_buf[client->_send_size], client->_send_buf_size - client->_send_size, 0 );
	int send_size = send( client->_socket, &client->_send_buf[client->_send_size], client->_send_pos - client->_send_size, 0 );
	if( send_size == SOCKET_ERROR )
	{
		socket_error( FORMAT_STR( "send data to %s-%d failed.", inet_ntoa( client->_addr.sin_addr ), 
			ntohs( client->_addr.sin_port ) ), WSAGetLastError() );
	}
	else
	{
		printf( "send %d bytes to client %s-%d.\n", send_size, inet_ntoa( client->_addr.sin_addr ), ntohs( client->_addr.sin_port ) );
		client->_send_size += send_size;
		if( client->_send_size >= client->_send_pos )
		{
			session_reset( client );
		}
	}

	return send_size;
}

///
/// host a server
/// 
bool server_host( server *host, unsigned int max_fd_count, const char *host_name, unsigned short port )
{
	host->_socket = socket( AF_INET, SOCK_STREAM, 0 );

	memset( &host->_addr, 0, sizeof( host->_addr ) );
	host->_addr.sin_family = AF_INET;
	host->_addr.sin_addr.s_addr = inet_addr( host_name );
	host->_addr.sin_port = htons( port );

	host->_read_set = fs_create( max_fd_count );
	host->_write_set = fs_create( max_fd_count );

	int ret = bind( host->_socket, (sockaddr*) &host->_addr, sizeof( host->_addr ) );
	if( ret == SOCKET_ERROR )
	{
		socket_error( FORMAT_STR( "host server %s-%d failed.", inet_ntoa( host->_addr.sin_addr ),
			ntohs( host->_addr.sin_port ) ), WSAGetLastError() );
		return false;
	}

	ret = listen( host->_socket, 5 );
	if( ret == SOCKET_ERROR )
	{
		socket_error( FORMAT_STR( "listen on %d failed.", ntohs( host->_addr.sin_port ) ), WSAGetLastError() );
		return false;
	}
	
	host->_add_pos = 0;
	return true;
}

///
/// shutdown a server
///
void server_shutdown( server *host )
{
	for( int i = 0; i < host->_add_pos; ++ i )
	{
		session_destroy( &host->_clients[i] );
	}
	closesocket( host->_socket );
	host->_add_pos = 0;

	fs_delete( &host->_read_set );
	fs_delete( &host->_write_set );
}

///
/// set the fd set
///
void server_fdset( server *host )
{
	WINFD_ZERO( &host->_read_set );
	WINFD_ZERO( &host->_write_set );

	// the listen socket
	WINFD_SET( host->_socket, &host->_read_set );
	// all client sockets
	for( int i = 0; i < host->_add_pos; ++ i )
	{
		WINFD_SET( host->_clients[i]._socket, &host->_read_set );
		WINFD_SET( host->_clients[i]._socket, &host->_write_set );
	}
}

/// 
/// add a session to the server
///
void server_add_session( server *host, const session *client )
{
	if( host->_add_pos >= MAX_SESSION_COUNT )
	{
		socket_error( FORMAT_STR( "the server %s-%d cannot add more clients.", inet_ntoa( host->_addr.sin_addr ), 
			ntohs( host->_addr.sin_port ) ), WSAGetLastError() );
		return ;
	}

	host->_clients[host->_add_pos] = *client; 
	host->_add_pos ++;
}

///
/// realign clients
///
void server_realign( server *host )
{
	for( int i = 0; i < host->_add_pos; ++ i )
	{
		session *client = &host->_clients[i];
		if( client->_socket == INVALID_SOCKET )
		{
			for( int j = i; j < host->_add_pos - 1; ++ j )
			{
				host->_clients[j] = host->_clients[j+1];  
			}

			-- host->_add_pos ;
		}
	}
}

///
/// poll clients
///
void server_poll( server *host, timeval timeout )
{
	static char recv_buf[RECV_DEF_BUF];

	server_fdset( host );
	
	int ret = select( 0, host->_read_set._set, host->_write_set._set, 0, &timeout );
	if( ret == SOCKET_ERROR )
	{
		socket_error( "poll failed.", WSAGetLastError() );
		return ;
	}

	if( ret > 0 )
	{
		// can accept new connection ?
		if( WINFD_ISSET( host->_socket, &host->_read_set ) )
		{
			session client = session_create( host );
			if( client._socket == INVALID_SOCKET )
			{
				socket_error( "accept new client failed.", WSAGetLastError() );
			}
			else
			{
				server_add_session( host, &client );
				printf( "a new client %s-%d connected.\n", inet_ntoa( client._addr.sin_addr ), 
					ntohs( client._addr.sin_port ) );
			}
		}

		// poll all the clients
		for( int i = 0; i < host->_add_pos; ++ i )
		{
			session *client = &host->_clients[i]; 
			// can read ?
			if( WINFD_ISSET( client->_socket, &host->_read_set ) )
			{
				int recv_bytes = recv( client->_socket, recv_buf, sizeof( recv_buf ), 0 );
				
				// should close?
				if( recv_bytes == 0 )
				{
					session_destroy( client );
					printf( "a client %s-%d disconnect.\n", inet_ntoa( client->_addr.sin_addr ),
						ntohs( client->_addr.sin_port ) );
					continue;
				}
				else if( recv_bytes == SOCKET_ERROR )
				{
					socket_error( FORMAT_STR( "recv on %s-%d failed.disconnect it.", inet_ntoa( client->_addr.sin_addr ),
						ntohs( client->_addr.sin_port ) ), WSAGetLastError() );
					session_destroy( client );
					continue;
				}

				// as an echo server, it should send the data which received.
				session_adddata( client, recv_buf, recv_bytes );	
				printf( "recv %d bytes from %s-%d.\n", recv_bytes, inet_ntoa( client->_addr.sin_addr ), ntohs( client->_addr.sin_port ) );
			}
			// can write?
			if( WINFD_ISSET( client->_socket, &host->_write_set ) )
			{
				session_send( client );
			}
		}

		// realign all the clients
		server_realign( host );
	}
}

///
/// exit flag
///
volatile static bool is_exit = false;

///
/// ctrl+c routine
///
BOOL WINAPI Handler( DWORD ctrl_event )
{
	switch( ctrl_event )
	{
	case CTRL_CLOSE_EVENT:
	case CTRL_C_EVENT:
		is_exit = true;
		return TRUE;
	}

	return FALSE;
}

///
/// program entry
///
int main( int argc, char **argv )
{
	if( argc != 3 )
	{
		fprintf( stderr, "usage : %s local_ip listen_port.\n", argv[0] );
		return -1;
	}

	if( !SetConsoleCtrlHandler( Handler, TRUE ) )
	{
		fprintf( stderr, "error setting event handler.\n" );
		return -1;
	}

	// startup the winsock
	WSADATA wd;
	WSAStartup( MAKEWORD( 2, 2 ), &wd );

	server host;
	
	if( !server_host( &host, 100, argv[1], (unsigned short) atoi( argv[2] ) ) )
	{
		server_shutdown( &host );
		return -1;
	}
	
	printf( "echo server starts.ctrl+c to exit.\n" );
	while( !is_exit )
	{
		timeval timeout = { 0, 0 };
		server_poll( &host, timeout );
	}
	
	server_shutdown( &host );
	printf( "echo server ends.\n" );

	WSACleanup();
	return 0;
}