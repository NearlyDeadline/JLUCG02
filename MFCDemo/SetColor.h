#pragma once


// SetColor �Ի���

class SetColor : public CDialog
{
	DECLARE_DYNAMIC(SetColor)

public:
	SetColor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetColor();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_r;
	int m_g;
	int m_b;
};
