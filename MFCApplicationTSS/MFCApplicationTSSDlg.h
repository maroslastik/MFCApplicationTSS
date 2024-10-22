
// MFCApplicationTSSDlg.h : header file
//

#pragma once
#include <vector>
#include <array>
#include <algorithm>

struct IMAGE 
{
	CString path;
	CString name;   
	Gdiplus::Image* gdiImage;

	bool histogramCalculated = false;
	std::array<uint32_t, 256> histogramRed = { 0 };
	std::array<uint32_t, 256> histogramGreen = { 0 };
	std::array<uint32_t, 256> histogramBlue = { 0 };
};

enum
{
	WM_DRAW_IMAGE = WM_USER + 1,
	WM_DRAW_HISTOGRAM
};

class CStaticImage : public CStatic
{
public:
	Gdiplus::Image* m_gdiImage = nullptr;
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
};

class CStaticHistogram : public CStatic
{
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
};


// CMFCApplicationTSSDlg dialog
class CMFCApplicationTSSDlg : public CDialogEx
{
public:
	CMFCApplicationTSSDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATIONTSS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);


protected:
	HICON m_hIcon;

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

	std::vector<IMAGE> m_ImageVector;

	int m_selectedColor;

	afx_msg void OnFileOpen32771();
	afx_msg void OnFileClose32772();
	afx_msg void OnLvnItemchangedFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnDrawImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawHistogram(WPARAM wParam, LPARAM lParam);

	void CalculateHistogram(int imgIndex);
	void DrawHistogramCurve(int colorIndex);
	afx_msg void OnHistogramR();
	afx_msg void OnHistogramG();
	afx_msg void OnHistogramB();

	// helper functions
	bool IsDuplicate(const IMAGE& img) const;
	
};