//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



// MainDlg.h : ͷ�ļ�
//

#pragma once
#include "Resource.h"
#include "IOCPModel.h"
#include <sstream>

// CMainDlg �Ի���
class CMainDlg : public CDialog
{
	// ����
public:
	CMainDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_PIGGYIOCPSERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	// ��ʼ����
	afx_msg void OnBnClickedOk();
	// ֹͣ����
	afx_msg void OnBnClickedStop();
	// "�˳�"��ť
	afx_msg void OnBnClickedCancel();
	///////////////////////////////////////////////////////////////////////
	//	ϵͳ�˳���ʱ��Ϊȷ����Դ�ͷţ�ֹͣ���������Socket���
	afx_msg void OnDestroy();
	//afx_msg LRESULT OnNewMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:

	// ��ʼ��Socket���Լ�������Ϣ
	void Init();

	// ��ʼ��List�ؼ�
	void InitListCtrl();

	inline void ChageConnectInfo()
	{
		SetDlgItemInt( IDC_CONNECTS, m_Connects );
	}

public:

	// ��ǰ�ͻ���������Ϣ������ʱ��������������ʾ�µ�������Ϣ(����CIOCPModel�е���)
	// Ϊ�˼��ٽ�������Ч�ʵ�Ӱ�죬�˴�ʹ��������
	inline void AddInformation(const CString strInfo)
	{
		CString str = CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S ");
		str += strInfo;
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
		pList->InsertItem(0,str);
	}
	inline void Connects()
	{
		m_Connects++;
		this->ChageConnectInfo();
	}
	inline void Disconnect(int num)
	{
		m_Connects-=num;
		this->ChageConnectInfo();
	}

private:

	CIOCPModel m_IOCP;                         // ��Ҫ������ɶ˿�ģ��

	int			m_Connects;

};
