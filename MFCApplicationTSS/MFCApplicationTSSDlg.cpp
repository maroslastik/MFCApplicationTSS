﻿// MFCApplicationTSSDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCApplicationTSS.h"
#include "MFCApplicationTSSDlg.h"
#include "afxdialogex.h"
#include "HistogramCalculator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMFCApplicationTSSDlg dialog

CMFCApplicationTSSDlg::CMFCApplicationTSSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATIONTSS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationTSSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_fileList);
	DDX_Control(pDX, IDC_STATIC_HISTOGRAM, m_staticHistogram);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_staticImage);
}

BEGIN_MESSAGE_MAP(CMFCApplicationTSSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DRAW_IMAGE, OnDrawImage)
	ON_MESSAGE(WM_DRAW_HISTOGRAM, OnDrawHistogram)
	ON_MESSAGE(WM_HISTOGRAM_CALCULATED, &CMFCApplicationTSSDlg::OnHistogramCalculated)
	ON_MESSAGE(WM_IMAGE_FLIP_CALCULATED, &CMFCApplicationTSSDlg::OnImageFlipped)

	ON_COMMAND(ID_FILE_OPEN32771, &CMFCApplicationTSSDlg::OnFileOpen32771)
	ON_COMMAND(ID_FILE_CLOSE32772, &CMFCApplicationTSSDlg::OnFileClose32772)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, &CMFCApplicationTSSDlg::OnLvnItemchangedFileList)
	ON_COMMAND(ID_HISTOGRAM_R, &CMFCApplicationTSSDlg::OnHistogramR)
	ON_COMMAND(ID_HISTOGRAM_G, &CMFCApplicationTSSDlg::OnHistogramG)
	ON_COMMAND(ID_HISTOGRAM_B, &CMFCApplicationTSSDlg::OnHistogramB)

	ON_COMMAND(ID_IMAGE_HORIZONTAL, &CMFCApplicationTSSDlg::OnHorizontalF)
	ON_COMMAND(ID_IMAGE_VERTICAL, &CMFCApplicationTSSDlg::OnVerticalF)
END_MESSAGE_MAP()


// CMFCApplicationTSSDlg message handlers

BOOL CMFCApplicationTSSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// histogram initialization
	// 0 for R, 1 for G, 2 for B, -1 for none
	CheckMenuRadioItem(
		GetMenu()->m_hMenu,
		ID_HISTOGRAM_R,
		ID_HISTOGRAM_G,
		ID_HISTOGRAM_B,
		MF_BYCOMMAND);

	GetWindowRect(&m_rect);
	m_fileList.GetWindowRect(&m_rectFileList);
	m_staticHistogram.GetWindowRect(&m_rectStaticHistogram);
	m_staticImage.GetWindowRect(&m_rectStaticImage);
	m_histogramChecked = { false, false, false };

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplicationTSSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplicationTSSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplicationTSSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplicationTSSDlg::OnFileOpen32771()
{
	CFileDialog fileDialog(TRUE, _T("bmp"), NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,
		_T("Image Files (*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|All Files (*.*)|*.*||"));


	if (fileDialog.DoModal() == IDOK)
	{
		POSITION pos = fileDialog.GetStartPosition();
		while (pos != NULL)
		{
			CString filePath = fileDialog.GetNextPathName(pos);

			IMAGE img;
			img.path = filePath;

			int pos = max(filePath.ReverseFind(_T('\\')), filePath.ReverseFind(_T('/')));
			if (pos != -1)
				img.name = filePath.Mid(pos + 1);
			else
				img.name = filePath;

			if (!IsDuplicate(img))
			{
				img.gdiImage = Gdiplus::Image::FromFile(img.path);
				if (img.gdiImage != nullptr && img.gdiImage->GetLastStatus() == Gdiplus::Ok)
				{
					m_ImageVector.push_back(img);
					m_fileList.InsertItem(m_fileList.GetItemCount(), img.name);
				}
				else
				{
					AfxMessageBox(_T("Failed to load image: ") + img.name);
					delete img.gdiImage;
					img.gdiImage = nullptr;
				}
			}
			else
			{
				AfxMessageBox(_T("Duplicate file not added: ") + img.name);
			}
		}
	}
}

void CMFCApplicationTSSDlg::OnFileClose32772()
{
	int selectedIndex = m_fileList.GetSelectionMark();

	if (selectedIndex == -1)
	{
		AfxMessageBox(_T("No file selected to remove."));
		return;
	}

	CString fileNameToRemove = m_fileList.GetItemText(selectedIndex, 0);

	if (AfxMessageBox(_T("Are you sure you want to remove this file?"), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		for (auto it = m_ImageVector.begin(); it != m_ImageVector.end(); ++it)
		{
			if (it->name == fileNameToRemove)
			{
				m_ImageVector.erase(it);
				break;
			}
		}

		m_fileList.DeleteItem(selectedIndex);

		if (!m_ImageVector.empty())
		{
			m_staticImage.m_gdiImage = m_ImageVector[0].gdiImage;
			m_staticImage.Invalidate();
			m_staticImage.UpdateWindow();

			m_fileList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_fileList.EnsureVisible(0, FALSE);

		}
		else
		{
			m_staticImage.m_gdiImage = nullptr;
			m_staticImage.Invalidate();
			m_staticImage.UpdateWindow();
		}

		AfxMessageBox(_T("File removed successfully."), MB_OK | MB_ICONINFORMATION);
	}
}

void CMFCApplicationTSSDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	int nDiffX = cx - m_rect.Width();
	int nDiffY = cy - m_rect.Height();

	if (::IsWindow(m_staticImage.m_hWnd) && ::IsWindow(m_staticHistogram.m_hWnd) && ::IsWindow(m_fileList.m_hWnd))
	{
		// histogram
		m_staticHistogram.GetWindowRect(&m_rectStaticHistogram);
		ScreenToClient(&m_rectStaticHistogram);
		int histogramTop = cy - m_rectStaticHistogram.Height();
		m_staticHistogram.SetWindowPos(NULL, m_rectStaticHistogram.left, histogramTop, m_rectStaticHistogram.Width(), m_rectStaticHistogram.Height(), SWP_NOZORDER);

		// file list
		m_fileList.GetWindowRect(&m_rectFileList);
		ScreenToClient(&m_rectFileList);
		int newFileListHeight = histogramTop - m_rectFileList.top;
		m_fileList.SetWindowPos(NULL, m_rectFileList.left, m_rectFileList.top, m_rectFileList.Width(), newFileListHeight, SWP_NOZORDER);

		// image
		int fileListRightEdge = m_rectFileList.left + m_rectFileList.Width();
		m_staticImage.GetWindowRect(&m_rectStaticImage);
		ScreenToClient(&m_rectStaticImage);
		int newImageWidth = cx - fileListRightEdge;
		int newImageHeight = newFileListHeight + m_rectStaticHistogram.Height();
		m_staticImage.SetWindowPos(NULL, fileListRightEdge, m_rectStaticImage.top, newImageWidth, newImageHeight, SWP_NOZORDER);

		m_staticImage.Invalidate();
		m_staticImage.UpdateWindow();
	}
}

LRESULT CMFCApplicationTSSDlg::OnDrawImage(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)wParam;

	Gdiplus::Graphics graphics(pDIS->hDC);

	if (m_ImageVector.empty())
	{
		CRect rect = pDIS->rcItem;
		Gdiplus::SolidBrush backgroundBrush(Gdiplus::Color(255, 255, 255));
		graphics.FillRectangle(&backgroundBrush, rect.left, rect.top, rect.Width(), rect.Height());
	}
	else
	{
		int imgIndex = m_fileList.GetNextItem(-1, LVNI_SELECTED);
		if (imgIndex < 0 && imgIndex >= m_ImageVector.size()) return S_OK;
		
		IMAGE& selectedImage = m_ImageVector[imgIndex];

		int varMode = selectedImage.VarMode;
		if (varMode < 0 && varMode > 3) return S_OK;

		Gdiplus::Image* imageToDraw = nullptr;

		if (varMode == 3)
		{
			imageToDraw = selectedImage.gdiImage;
		}
		else
		{
			if (!selectedImage.m_ImgVarsCalculated[varMode])
			{
				CalculateFlip(imgIndex, m_ImageVector[imgIndex].VarMode);
			}

			if (!selectedImage.m_ImgVarsRunning[0] && !selectedImage.m_ImgVarsRunning[1] && !selectedImage.m_ImgVarsRunning[2])
			{
				imageToDraw = selectedImage.m_ImgVars[varMode];
			}
			else
			{
				return S_OK;
			}
		}

		if (imageToDraw == nullptr)
		{
			CRect rect = pDIS->rcItem;
			Gdiplus::SolidBrush backgroundBrush(Gdiplus::Color(255, 255, 255));
			graphics.FillRectangle(&backgroundBrush, rect.left, rect.top, rect.Width(), rect.Height());
		}
		else
		{
			UINT imageWidth = imageToDraw->GetWidth();
			UINT imageHeight = imageToDraw->GetHeight();

			CRect rect = pDIS->rcItem;

			float scaleX = static_cast<float>(rect.Width()) / imageWidth;
			float scaleY = static_cast<float>(rect.Height()) / imageHeight;
			float scale = min(scaleX, scaleY);

			int scaledWidth = static_cast<int>(imageWidth * scale);
			int scaledHeight = static_cast<int>(imageHeight * scale);

			int imageX = rect.left + (rect.Width() - scaledWidth) / 2;
			int imageY = rect.top + (rect.Height() - scaledHeight) / 2;

			Gdiplus::SolidBrush backgroundBrush(Gdiplus::Color(255, 255, 255));
			graphics.FillRectangle(&backgroundBrush, rect.left, rect.top, rect.Width(), rect.Height());

			graphics.DrawImage(imageToDraw, imageX, imageY, scaledWidth, scaledHeight);
		}
	}

	return S_OK;
}

LRESULT CMFCApplicationTSSDlg::OnDrawHistogram(WPARAM wParam, LPARAM lParam)
{
	if (!m_histogramChecked[0] && !m_histogramChecked[1] && !m_histogramChecked[2])
		return S_OK;
	LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)wParam;
	CDC* pDC = CDC::FromHandle(pDIS->hDC);

	Gdiplus::Graphics graphics(pDIS->hDC);
	CRect rect = pDIS->rcItem;
	Gdiplus::SolidBrush backgroundBrush(Gdiplus::Color(255, 255, 255));
	graphics.FillRectangle(&backgroundBrush, rect.left, rect.top, rect.Width(), rect.Height());

	int imgIndex = m_fileList.GetNextItem(-1, LVNI_SELECTED);
	if (imgIndex >= 0 && imgIndex < m_ImageVector.size())
	{
		IMAGE& img = m_ImageVector[imgIndex];

		if (img.histogramCalculated)
		{
			if (m_histogramChecked[0]) DrawHistogramForColor(pDC, 0);
			if (m_histogramChecked[1]) DrawHistogramForColor(pDC, 1);
			if (m_histogramChecked[2]) DrawHistogramForColor(pDC, 2);
			return S_OK;
		}
		else
		{
			if (!img.histogramRunning)
			{
				CalculateHistogram(imgIndex);
				return S_OK;
			}
			else
			{
				return S_OK;
			}
		}
	}
	
	return S_OK;
}

LRESULT CMFCApplicationTSSDlg::OnHistogramCalculated(WPARAM wParam, LPARAM lParam)
{
	m_staticHistogram.Invalidate(true);
	return LRESULT();
}

LRESULT CMFCApplicationTSSDlg::OnImageFlipped(WPARAM wParam, LPARAM lParam)
{
	m_staticImage.Invalidate(true);
	return LRESULT();
}

bool CMFCApplicationTSSDlg::IsDuplicate(const IMAGE& img) const
{
	for (const auto& existingImg : m_ImageVector)
	{
		if (existingImg.name == img.name)
		{
			if (existingImg.path == img.path)
			{
				return true;
			}
		}
	}
	return false;
}

void CStaticImage::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	GetParent()->SendMessage(WM_DRAW_IMAGE, (WPARAM)lpDrawItemStruct);
}

void CStaticHistogram::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	GetParent()->SendMessage(WM_DRAW_HISTOGRAM, (WPARAM)lpDrawItemStruct);
}

void CMFCApplicationTSSDlg::OnLvnItemchangedFileList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED))
	{
		int nItem = pNMLV->iItem;
		if (nItem >= 0 && nItem < m_ImageVector.size())
		{
			IMAGE& img = m_ImageVector[nItem];

			m_staticImage.m_gdiImage = img.gdiImage;
			CMenu* pMenu = GetMenu();
			CMenu* pImageMenu = pMenu->GetSubMenu(2);
			pImageMenu->CheckMenuItem(ID_IMAGE_HORIZONTAL, MF_UNCHECKED);
			pImageMenu->CheckMenuItem(ID_IMAGE_VERTICAL, MF_UNCHECKED);
			m_ImageVector[m_fileList.GetNextItem(-1, LVNI_SELECTED)].VarMode = 3;

			m_staticImage.Invalidate();
			m_staticImage.UpdateWindow();
			m_staticHistogram.Invalidate();
		}
	}
	*pResult = 0;
}

void CMFCApplicationTSSDlg::DrawHistogramForColor(CDC* pDC, int colorIndex)
{
	const std::array<uint32_t, 256>* histogram = nullptr;

	switch (colorIndex)
	{
	case 0:
		histogram = &m_ImageVector[m_fileList.GetNextItem(-1, LVNI_SELECTED)].histogramRed;
		break;
	case 1:
		histogram = &m_ImageVector[m_fileList.GetNextItem(-1, LVNI_SELECTED)].histogramGreen;
		break;
	case 2:
		histogram = &m_ImageVector[m_fileList.GetNextItem(-1, LVNI_SELECTED)].histogramBlue;
		break;
	default:
		return;
	}

	CPen colorPen(PS_SOLID, 2, RGB(255 * (colorIndex == 0), 255 * (colorIndex == 1), 255 * (colorIndex == 2)));
	CPen* pOldPen = pDC->SelectObject(&colorPen);

	int width = m_rectStaticHistogram.Width();
	int height = m_rectStaticHistogram.Height();

	uint32_t maxVal = *std::max_element(histogram->begin(), histogram->end());

	for (int i = 0; i < 255; ++i)
	{
		int x1 = (i * width) / 255;
		int y1 = height - (histogram->at(i) * height) / maxVal; // error
		int x2 = ((i + 1) * width) / 255;
		int y2 = height - (histogram->at(i + 1) * height) / maxVal;

		pDC->MoveTo(x1, y1);
		pDC->LineTo(x2, y2);
	}

	pDC->SelectObject(pOldPen);
}

void CMFCApplicationTSSDlg::OnHistogramR()
{
	CMenu* pMenu = GetMenu();
	if (pMenu)
	{
		CMenu* pHistogramMenu = pMenu->GetSubMenu(1);
		int imgIndex = m_fileList.GetNextItem(-1, LVNI_SELECTED);

		if (imgIndex < 0 && imgIndex >= m_ImageVector.size())
		{
			AfxMessageBox(_T("Please select an image from the list to display the histogram."));
			return;
		}

		if (pHistogramMenu->GetMenuState(ID_HISTOGRAM_R, MF_BYCOMMAND) & MF_CHECKED)
		{
			pHistogramMenu->CheckMenuItem(ID_HISTOGRAM_R, MF_UNCHECKED);
			m_histogramChecked[0] = false;
		}
		else
		{
			pHistogramMenu->CheckMenuItem(ID_HISTOGRAM_R, MF_CHECKED);
			m_histogramChecked[0] = true;
		}
	}
	m_staticHistogram.Invalidate();
}

void CMFCApplicationTSSDlg::OnHistogramG()
{
	CMenu* pMenu = GetMenu();
	if (pMenu)
	{
		CMenu* pHistogramMenu = pMenu->GetSubMenu(1);
		int imgIndex = m_fileList.GetNextItem(-1, LVNI_SELECTED);

		if (imgIndex < 0 && imgIndex >= m_ImageVector.size())
		{
			AfxMessageBox(_T("Please select an image from the list to display the histogram."));
			return;
		}

		if (pHistogramMenu->GetMenuState(ID_HISTOGRAM_G, MF_BYCOMMAND) & MF_CHECKED)
		{
			pHistogramMenu->CheckMenuItem(ID_HISTOGRAM_G, MF_UNCHECKED);
			m_histogramChecked[1] = false;
		}
		else
		{
			pHistogramMenu->CheckMenuItem(ID_HISTOGRAM_G, MF_CHECKED);
			m_histogramChecked[1] = true;
		}
	}
	m_staticHistogram.Invalidate();
}

void CMFCApplicationTSSDlg::OnHistogramB()
{
	CMenu* pMenu = GetMenu();
	if (pMenu)
	{
		CMenu* pHistogramMenu = pMenu->GetSubMenu(1);
		int imgIndex = m_fileList.GetNextItem(-1, LVNI_SELECTED);

		if (imgIndex < 0 && imgIndex >= m_ImageVector.size())
		{
			AfxMessageBox(_T("Please select an image from the list to display the histogram."));
			return;
		}

		if (pHistogramMenu->GetMenuState(ID_HISTOGRAM_B, MF_BYCOMMAND) & MF_CHECKED)
		{
			pHistogramMenu->CheckMenuItem(ID_HISTOGRAM_B, MF_UNCHECKED);
			m_histogramChecked[2] = false;
		}
		else
		{
			pHistogramMenu->CheckMenuItem(ID_HISTOGRAM_B, MF_CHECKED);
			m_histogramChecked[2] = true;
		}
	}
	m_staticHistogram.Invalidate();
}

void CMFCApplicationTSSDlg::OnHorizontalF()
{
	CMenu* pMenu = GetMenu();
	if (pMenu)
	{
		CMenu* pImageMenu = pMenu->GetSubMenu(2);
		int imgIndex = m_fileList.GetNextItem(-1, LVNI_SELECTED);

		if (imgIndex < 0 && imgIndex >= m_ImageVector.size())
		{
			AfxMessageBox(_T("Please select an image from the list to do a flip."));
			return;
		}

		if (pImageMenu->GetMenuState(ID_IMAGE_HORIZONTAL, MF_BYCOMMAND) & MF_CHECKED) // deselect
		{
			pImageMenu->CheckMenuItem(ID_IMAGE_HORIZONTAL, MF_UNCHECKED);
			if (pImageMenu->GetMenuState(ID_IMAGE_VERTICAL, MF_BYCOMMAND) & MF_CHECKED)
			{
				m_ImageVector[imgIndex].VarMode = 1; // vertical
			}
			else
			{
				m_ImageVector[imgIndex].VarMode = 3; // original
			}
		}
		else // select
		{
			pImageMenu->CheckMenuItem(ID_IMAGE_HORIZONTAL, MF_CHECKED);
			if (pImageMenu->GetMenuState(ID_IMAGE_VERTICAL, MF_BYCOMMAND) & MF_CHECKED)
			{
				m_ImageVector[imgIndex].VarMode = 0; // vertical + horizontal
			}
			else
			{
				m_ImageVector[imgIndex].VarMode = 2; // horizontal
			}
		}

		int varMode = m_ImageVector[imgIndex].VarMode;

		if (varMode < 0 && varMode > 3) return;

		if (varMode == 3)
		{
			m_staticImage.Invalidate(true);
			return;
		}

		if (!m_ImageVector[imgIndex].m_ImgVarsCalculated[varMode]) // ak nie je vypocitany flip - vypocitaj ho
		{
			CalculateFlip(imgIndex, m_ImageVector[imgIndex].VarMode);
		}
		else
		{
			m_staticImage.Invalidate(true);
		}
	}
}

void CMFCApplicationTSSDlg::OnVerticalF()
{
	CMenu* pMenu = GetMenu();
	if (pMenu)
	{
		CMenu* pImageMenu = pMenu->GetSubMenu(2);
		int imgIndex = m_fileList.GetNextItem(-1, LVNI_SELECTED);

		if (imgIndex < 0 && imgIndex >= m_ImageVector.size())
		{
			AfxMessageBox(_T("Please select an image from the list to do a flip."));
			return;
		}

		if (pImageMenu->GetMenuState(ID_IMAGE_VERTICAL, MF_BYCOMMAND) & MF_CHECKED) // deselect
		{
			pImageMenu->CheckMenuItem(ID_IMAGE_VERTICAL, MF_UNCHECKED);
			if (pImageMenu->GetMenuState(ID_IMAGE_HORIZONTAL, MF_BYCOMMAND) & MF_CHECKED)
			{
				m_ImageVector[imgIndex].VarMode = 2; // horizontal
			}
			else
			{
				m_ImageVector[imgIndex].VarMode = 3; // original
			}
		}
		else // select
		{
			pImageMenu->CheckMenuItem(ID_IMAGE_VERTICAL, MF_CHECKED);
			if (pImageMenu->GetMenuState(ID_IMAGE_HORIZONTAL, MF_BYCOMMAND) & MF_CHECKED)
			{
				m_ImageVector[imgIndex].VarMode = 0; // vertical + horizontal
			}
			else
			{
				m_ImageVector[imgIndex].VarMode = 1; // vertical
			}
		}

		int varMode = m_ImageVector[imgIndex].VarMode;

		if (varMode < 0 && varMode > 3) return;

		if (varMode == 3)
		{
			m_staticImage.Invalidate(true);
			return;
		}

		if (!m_ImageVector[imgIndex].m_ImgVarsCalculated[varMode]) // ak nie je vypocitany flip - vypocitaj ho
		{
			CalculateFlip(imgIndex, m_ImageVector[imgIndex].VarMode);
		}
		else
		{
			m_staticImage.Invalidate(true);
		}
	}
}

void CMFCApplicationTSSDlg::CalculateHistogram(int imgIndex)
{
	if (imgIndex < 0 && imgIndex >= m_ImageVector.size()) return;
	IMAGE& img = m_ImageVector[imgIndex];

	Gdiplus::Bitmap* bitmap = static_cast<Gdiplus::Bitmap*>(img.gdiImage);
	Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());

	Gdiplus::BitmapData bitmapData;
	bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);

	std::thread([this, &img, bitmap, bitmapData]()
	{
		img.histogramRunning = true;
		CalculateHistogramFromPixels(
			static_cast<BYTE*>(bitmapData.Scan0),
			bitmap->GetWidth(),
			bitmap->GetHeight(),
			bitmapData.Stride,
			img.histogramRed,
			img.histogramGreen,
			img.histogramBlue
		);

		img.histogramCalculated = true;
		img.histogramRunning = false;
		PostMessage(WM_HISTOGRAM_CALCULATED);
	}).detach();

	bitmap->UnlockBits(&bitmapData);
}

void CMFCApplicationTSSDlg::CalculateFlip(int imgIndex, int imgVar)
{
	if (imgIndex < 0 || imgIndex >= m_ImageVector.size()) return;

	IMAGE& img = m_ImageVector[imgIndex];
	if (imgVar < 0 || imgVar > 3) return;

	switch (imgVar)
	{
	case 3: // No flip
	{
		PostMessage(WM_IMAGE_FLIP_CALCULATED, 0, 0);
		return;
	}
	case 2: // Horizontal
	{

		Gdiplus::Bitmap* bitmap = static_cast<Gdiplus::Bitmap*>(img.gdiImage->Clone());
		if (!bitmap) return;

		Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());

		Gdiplus::BitmapData bitmapData;
		bitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

		if (!img.m_ImgVarsRunning[0] && !img.m_ImgVarsRunning[1] && !img.m_ImgVarsRunning[2])
		{
			img.m_ImgVarsRunning[imgVar] = true;

			std::thread([this, &img, rect, imgVar, bitmapData]()
				{
					Sleep(5000);
					FlipHorizontal(bitmapData);
					PostMessage(WM_IMAGE_FLIP_CALCULATED, 0, 0);
					img.m_ImgVarsCalculated[imgVar] = true;
					img.m_ImgVarsRunning[imgVar] = false;
				}).detach();

				bitmap->UnlockBits(&bitmapData);
				img.m_ImgVars[imgVar] = bitmap;
		}
		return;
	}
	case 1: // Vertical
	{

		Gdiplus::Bitmap* bitmap = static_cast<Gdiplus::Bitmap*>(img.gdiImage->Clone());
		if (!bitmap) return;

		Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());

		Gdiplus::BitmapData bitmapData;
		bitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

		if (!img.m_ImgVarsRunning[0] && !img.m_ImgVarsRunning[1] && !img.m_ImgVarsRunning[2])
		{
			img.m_ImgVarsRunning[imgVar] = true;

			std::thread([this, &img, rect, imgVar, bitmapData]()
				{
					Sleep(5000);
					FlipVertical(bitmapData);
					PostMessage(WM_IMAGE_FLIP_CALCULATED, 0, 0);
					img.m_ImgVarsCalculated[imgVar] = true;
					img.m_ImgVarsRunning[imgVar] = false;
				}).detach();
		}
		bitmap->UnlockBits(&bitmapData);
		img.m_ImgVars[imgVar] = bitmap;
		return;
	}
	case 0: // Horizontal + Vertical
	{
		if (img.m_ImgVarsCalculated[2] && !img.m_ImgVarsCalculated[1]) // Horizontal done, flip it vertically
		{
			Gdiplus::Bitmap* bitmap = static_cast<Gdiplus::Bitmap*>(img.m_ImgVars[2]->Clone());
			if (!bitmap) return;

			Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());

			Gdiplus::BitmapData bitmapData;
			bitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

			if (!img.m_ImgVarsRunning[0] && !img.m_ImgVarsRunning[1] && !img.m_ImgVarsRunning[2])
			{
				img.m_ImgVarsRunning[imgVar] = true;

				std::thread([this, &img, rect, imgVar, bitmapData]()
					{
						Sleep(5000);
						FlipVertical(bitmapData);
						PostMessage(WM_IMAGE_FLIP_CALCULATED, 0, 0);
						img.m_ImgVarsCalculated[imgVar] = true;
						img.m_ImgVarsRunning[imgVar] = false;
					}).detach();
			}
			bitmap->UnlockBits(&bitmapData);
			img.m_ImgVars[imgVar] = bitmap;
			return;
		}
		else if (img.m_ImgVarsCalculated[1] && !img.m_ImgVarsCalculated[2]) // Vertical done, flip it horizontally
		{
			Gdiplus::Bitmap* bitmap = static_cast<Gdiplus::Bitmap*>(img.m_ImgVars[1]->Clone());
			if (!bitmap) return;

			Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());

			Gdiplus::BitmapData bitmapData;
			bitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

			if (!img.m_ImgVarsRunning[0] && !img.m_ImgVarsRunning[1] && !img.m_ImgVarsRunning[2])
			{
				img.m_ImgVarsRunning[imgVar] = true;
				std::thread([this, &img, rect, imgVar, bitmapData]()
					{
						Sleep(5000);
						FlipHorizontal(bitmapData);
						PostMessage(WM_IMAGE_FLIP_CALCULATED, 0, 0);
						img.m_ImgVarsCalculated[imgVar] = true;
						img.m_ImgVarsRunning[imgVar] = false;
					}).detach();
			}
			bitmap->UnlockBits(&bitmapData);
			img.m_ImgVars[imgVar] = bitmap;
			return;
		}
	}
	default:
		return;
	}
}

void CMFCApplicationTSSDlg::FlipHorizontal(Gdiplus::BitmapData bitmapData)
{
	unsigned int width = bitmapData.Width;
	unsigned int height = bitmapData.Height;
	BYTE* pixels = static_cast<BYTE*>(bitmapData.Scan0);
	unsigned int stride = bitmapData.Stride;

	unsigned int start = 0;
	unsigned int end = height - 1;

	while (start < end)
	{
		BYTE* rowStart = pixels + start * stride;
		BYTE* rowEnd = pixels + end * stride;

		for (unsigned int x = 0; x < width * 4; ++x)
		{
			std::swap(rowStart[x], rowEnd[x]);
		}

		++start;
		--end;
	}
}

void CMFCApplicationTSSDlg::FlipVertical(Gdiplus::BitmapData bitmapData)
{
	unsigned int width = bitmapData.Width;
	unsigned int height = bitmapData.Height;
	BYTE* pixels = static_cast<BYTE*>(bitmapData.Scan0);
	unsigned int stride = bitmapData.Stride;

	for (unsigned int y = 0; y < height; ++y)
	{
		BYTE* row = pixels + y * stride;

		unsigned int start = 0;
		unsigned int end = (width - 1) * 4;

		while (start < end)
		{
			for (unsigned int i = 0; i < 4; ++i)
			{
				std::swap(row[start + i], row[end + i]);
			}

			start += 4;
			end -= 4;
		}
	}
}
