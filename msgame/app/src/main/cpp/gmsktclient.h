#ifndef __GMSKTCLIENT_H__
#define __GMSKTCLIENT_H__

#define  TCP_CONNECT_NULL    -1
#define  TCP_CONNECT_SUCCESS    0
#define  TCP_CONNECT_KEEPING    1
#define  TCP_CONNECT_FAILED    3
#define  TCP_CONNECT_STARTREAD_SUCCESS 4
#define  TCP_CONNECT_CANCEL    5
#define  TCP_CONNECT_RETRYING   6
#define  TCP_CONNECT_RETRY_FAILED  7

class CGmSktClient
{
public:
	CGmSktClient();
	~CGmSktClient();
public:
	int		InitGmSktClient();
	int		UnInitGmSktClient();

	int		Connect(const char *peer_ip, int peer_port);

	int		Disconnect();

	int		SendStream(const char* pData, int nLen);

	int		SendStream(int nDataType, const char* pData, int nLen);
public:
	int		SetInSndCardBufMs(int nMs);
	int		GetInSndCardBufMs();

	int		JoinAuTeam(const char *pJoinKey);
	int		QuitAuTeam();

protected:
	virtual int OnGmConnect(int nResult) { return 0; }
	virtual int OnGmClose(int nResult) { return 0; }
	virtual int OnGmRecv(const char *pData, int nLen) { return 0; }
	virtual int OnGmSend(int nResule, int nLen) { return 0; }
	virtual int OnGmStream(int nDataType, const char *pData, int nLen) { return 0; }
protected:

private:
	static  void	func_streampkt_cb(void *pUserData, void *ptagStream_UnPkt);
	static  void	func_pktinfo_cb(void *pUserData, void *pTCP_PktInfo);

	void			process_streampkt(void *ptagStream_UnPkt);
	void			process_pktinfo(void *pTCP_PktInfo);
private:
	void  *m_pTcpClient;
};


#endif

