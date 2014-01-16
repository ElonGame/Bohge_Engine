//////////////////////////////////////
//			������������		   //
/////////////////////////////////////

#pragma once
#include "IMessageHandle.h"
#include "Utility.h"
#include "json.h"


#include <string>
#include <map>
#include <list>


class CrackedUrl;
class ReceiptHandle : public IMessageHandle
{
private:
	struct ReslutData
	{
		SmartPtr<ClientContext>		m_Client;
		std::string			m_UDID;
		bool				m_isSucess;//�������Ƿ񷵻�����
		bool				m_isRealReceipt;//�Ƿ�����ȷ�ĸ���Ķ���
		Json::Value			m_ReslutJson;
	};
	struct ReceiptFragment//�ڲ�����
	{
		ushort							m_PacketCount;//�ְ�����
		std::map<ushort, std::string>	m_Fragment;//�ְ�
	};
	struct ReceiptVerifyData
	{
		CrackedUrl*		m_pURL;
		std::string		m_Receipt;
		ReslutData*		m_pReslut;
	};
private:
	typedef std::map<std::string, ReceiptFragment> ReceiptMap;
	typedef std::list< ReslutData* > Reslutlist;
private:
	CrackedUrl*			m_pUrlResolve;
	ReceiptMap			Receipts;
	Reslutlist			m_Reslutlist;
	CRITICAL_SECTION	m_csReslutlist;// ����Worker�߳�ͬ���Ļ�����
	HANDLE				m_ProcessEvent;//���������ݵ��¼�
	HANDLE				m_ReslutThreads;
public:
	ReceiptHandle(void);
	~ReceiptHandle(void);
	virtual bool HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );
	static DWORD WINAPI HttpReceiptVerifyData(LPVOID lpParam);
	static DWORD WINAPI PorcessReslut(LPVOID lpParam);
	void InsertReslut( ReslutData* data );
	ReslutData* GetReslutData();
	inline HANDLE GetEvent()
	{
		return m_ProcessEvent;
	}
	inline CRITICAL_SECTION GetMutex()
	{
		return m_csReslutlist;
	}
};
