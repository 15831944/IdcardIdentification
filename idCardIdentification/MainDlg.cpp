// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "pinyindlg.h"
#include "convertchinesegbdlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	//����Ϊ�������
	m_editIdcard = m_birthDate = m_sex = m_address = m_idcard = L"";

	DoDataExchange(FALSE);

	// Add your code
	if(LRunSql::InitConnectPtr()==false)		//��ʼ��COM�������������ݿ�����
		return false;


	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
//	if (LRunSql::m_database != NULL)
	{
		LRunSql::Close();		//�Ͽ����ݿ�����
	}

	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	Clear();

	/************************************************************************/
	/* ת����λ�� �� 4379 �� 3220 �� 1788                                                                      */
	/************************************************************************/
	CString name = L"������";
	GetSpell(name);
	char *name2 = "��";
// 	GetGBFromCString(L"������");
// 	GetStringFromGB(L"437932201788");

	CString name3 = L"����˭";
	CString py ;
//	GetChineseSpell(name3, py);

	DoDataExchange(TRUE);
	if (m_editIdcard.GetLength() != 18)
	{
		GetDlgItem(IDC_STATIC_INFO).SetWindowText(L"������18λ���֤���룡");
		//AtlMessageBox(m_hWnd, L"������18λ���֤��");
		return 1;
	}

	if ( iso7064(m_editIdcard) == false)
	{
		GetDlgItem(IDC_STATIC_INFO).SetWindowText(L"������Ч�����֤���룡");
		return 1;
	}

	//��ѯ�Ա�
	if (m_editIdcard.GetAt(16) % 2 == 1)
		m_sex = L"��";
	else 
		m_sex = L"Ů";
// 	else
// 		m_sex = L"δ֪";

	//��������
	m_birthDate.Format(L"%s��%s��%s��", m_editIdcard.Mid(6, 4), m_editIdcard.Mid(10, 2), m_editIdcard.Mid(12, 2));

	//��ѯ����
	CString ch = m_editIdcard.Mid(0, 5);
	sql.Format(L"select * from City where sz_code='%s'", m_editIdcard.Mid(0, 6));
	if (m_runsql.CheckSQLResult(sql))
	{
		LPCTSTR str = (LPCTSTR)(_bstr_t)m_runsql.m_recordset->GetCollect("id");
		m_address.Format(L"%s %s", (LPCTSTR)(_bstr_t)m_runsql.m_recordset->GetCollect("state"),
			(LPCTSTR)(_bstr_t)m_runsql.m_recordset->GetCollect("city"));
	}


	//
	m_idcard = m_editIdcard;
	
	DoDataExchange(FALSE);

	//CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedAppPysx(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CPinyinDlg dlg;

	dlg.DoModal();

	return 0;
}

LRESULT CMainDlg::OnBnClickedAppConvert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CConvertGBDlg dlg;
	dlg.DoModal();

	return 0;
}
