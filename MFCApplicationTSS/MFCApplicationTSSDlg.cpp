
// MFCApplicationTSSDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCApplicationTSS.h"
#include "MFCApplicationTSSDlg.h"
#include "afxdialogex.h"

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
	ON_COMMAND(ID_FILE_OPEN32771, &CMFCApplicationTSSDlg::OnFileOpen32771)
	ON_COMMAND(ID_FILE_CLOSE32772, &CMFCApplicationTSSDlg::OnFileClose32772)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, &CMFCApplicationTSSDlg::OnLvnItemchangedFileList)
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

	GetWindowRect(&m_rect);
	m_fileList.GetWindowRect(&m_rectFileList);
	m_staticHistogram.GetWindowRect(&m_rectStaticHistogram);
	m_staticImage.GetWindowRect(&m_rectStaticImage);

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
		m_staticImage.GetWindowRect(&m_rectStaticImage);
		ScreenToClient(&m_rectStaticImage);
		int newImageWidth = cx - m_rectFileList.Width();
		m_staticImage.SetWindowPos(NULL, m_rectFileList.right, m_rectStaticImage.top, newImageWidth, newFileListHeight + m_rectStaticHistogram.Height(), SWP_NOZORDER);

		m_staticImage.Invalidate();
		m_staticImage.UpdateWindow();
	}
}

LRESULT CMFCApplicationTSSDlg::OnDrawImage(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)wParam;

	Gdiplus::Graphics graphics(pDIS->hDC);

	if (m_ImageVector.empty() || m_staticImage.m_gdiImage == nullptr)
	{
		CRect rect = pDIS->rcItem;
		Gdiplus::SolidBrush backgroundBrush(Gdiplus::Color(255, 255, 255));
		graphics.FillRectangle(&backgroundBrush, rect.left, rect.top, rect.Width(), rect.Height());
	}
	else
	{
		UINT imageWidth = m_staticImage.m_gdiImage->GetWidth();
		UINT imageHeight = m_staticImage.m_gdiImage->GetHeight();

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

		graphics.DrawImage(m_staticImage.m_gdiImage, imageX, imageY, scaledWidth, scaledHeight);
	}

	return S_OK;
}

LRESULT CMFCApplicationTSSDlg::OnDrawHistogram(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
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

			m_staticImage.Invalidate();
			m_staticImage.UpdateWindow();
		}
	}
	*pResult = 0;
}
