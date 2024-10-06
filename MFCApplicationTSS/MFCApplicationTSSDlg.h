
// MFCApplicationTSSDlg.h : header file
//

#pragma once
#include <vector>

struct IMAGE 
{
	CString path;
	CString name;   
};

enum
{
	WM_DRAW_IMAGE = WM_USER + 1,
	WM_DRAW_HISTOGRAM
};

class CStaticImage : public CStatic
{
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
};

class CStaticHistogram : public CStatic
{
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
};


// CMFCApplicationTSSDlg dialog
class CMFCApplicationTSSDlg : public CDialogEx
{
	// Construction
public:
	CMFCApplicationTSSDlg(CWnd* pParent = nullptr);	// standard constructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATIONTSS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CRect m_rect;

	CListCtrl m_fileList;
	CRect m_rectFileList;

	CStaticHistogram m_staticHistogram;
	CRect m_rectStaticHistogram;

	CStaticImage m_staticImage;
	CRect m_rectStaticImage;

	std::vector<IMAGE> ImageVector;

	afx_msg void OnFileOpen32771();
	afx_msg void OnFileClose32772();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnDrawImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawHistogram(WPARAM wParam, LPARAM lParam);

	// helper functions
	bool IsDuplicate(const IMAGE& img) const;
};