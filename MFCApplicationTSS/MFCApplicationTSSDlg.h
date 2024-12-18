// MFCApplicationTSSDlg.h : header file
//

#pragma once
#include <vector>
#include <array>
#include <algorithm>
#include <thread>


struct IMAGE
{
	CString path;
	CString name;
	Gdiplus::Image* gdiImage;

	bool histogramCalculated = false;
	bool histogramRunning = false;

	std::array<uint32_t, 256> histogramRed = { 0 };
	std::array<uint32_t, 256> histogramGreen = { 0 };
	std::array<uint32_t, 256> histogramBlue = { 0 };

	int VarMode = 3;
	std::array<bool, 3> m_ImgVarsCalculated = {false, false, false};
	std::array<bool, 3> m_ImgVarsRunning = { false, false, false };
	std::array<Gdiplus::Image*, 3> m_ImgVars;
};

enum
{
	WM_DRAW_IMAGE = WM_USER + 1,
	WM_DRAW_HISTOGRAM, 
	WM_HISTOGRAM_CALCULATED,
	WM_IMAGE_FLIP_CALCULATED
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
	std::array<bool, 3> m_histogramChecked= { false, false, false };

	afx_msg void OnFileOpen32771();
	afx_msg void OnFileClose32772();
	afx_msg void OnLvnItemchangedFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnDrawImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawHistogram(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHistogramCalculated(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImageFlipped(WPARAM wParam, LPARAM lParam);

	void CalculateHistogram(int imgIndex);
	void CalculateFlip(int imgIndex, int imgVar);
	void DrawHistogramForColor(CDC* pDC, int colorIndex);
	void FlipHorizontal(Gdiplus::BitmapData bitmapData);
	void FlipVertical(Gdiplus::BitmapData bitmapData);

	afx_msg void OnHistogramR();
	afx_msg void OnHistogramG();
	afx_msg void OnHistogramB();

	afx_msg void OnHorizontalF();
	afx_msg void OnVerticalF();

	// helper functions
	bool IsDuplicate(const IMAGE& img) const;
};