/*
 Copyright (c) 2007-2008 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#include <atlcomcli.h>
#include <atlconv.h>
#include <comutil.h>
#include <windows.h>
#include "BaseCom.h"
#include "BootEncryption.h"
#include "Dlgcode.h"
#include "Format.h"
#include "Progress.h"
#include "TcFormat.h"
#include "FormatCom.h"
#include "FormatCom_h.h"
#include "FormatCom_i.c"

using namespace YourProduct;

static volatile LONG ObjectCount = 0;

class YourProductFormatCom : public IYourProductFormatCom
{

public:
	YourProductFormatCom (DWORD messageThreadId) : RefCount (0),
		MessageThreadId (messageThreadId),
		CallBack (NULL)
	{
		InterlockedIncrement (&ObjectCount);
	}

	~YourProductFormatCom ()
	{
		if (InterlockedDecrement (&ObjectCount) == 0)
			PostThreadMessage (MessageThreadId, WM_APP, 0, 0);
	}

	virtual ULONG STDMETHODCALLTYPE AddRef ()
	{
		return InterlockedIncrement (&RefCount);
	}

	virtual ULONG STDMETHODCALLTYPE Release ()
	{
		if (!InterlockedDecrement (&RefCount))
		{
			delete this;
			return 0;
		}

		return RefCount;
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface (REFIID riid, void **ppvObject)
	{
		if (riid == IID_IUnknown || riid == IID_IYourProductFormatCom)
			*ppvObject = this;
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}

		AddRef ();
		return S_OK;
	}
	
	virtual DWORD STDMETHODCALLTYPE CallDriver (DWORD ioctl, BSTR input, BSTR *output)
	{
		return BaseCom::CallDriver (ioctl, input, output);
	}

	virtual DWORD STDMETHODCALLTYPE CopyFile (BSTR sourceFile, BSTR destinationFile)
	{
		return BaseCom::CopyFile (sourceFile, destinationFile);
	}

	virtual DWORD STDMETHODCALLTYPE DeleteFile (BSTR file)
	{
		return BaseCom::DeleteFile (file);
	}

	virtual BOOL STDMETHODCALLTYPE FormatNtfs (int driveNo, int clusterSize)
	{
		return ::FormatNtfs (driveNo, clusterSize);
	}

	virtual int STDMETHODCALLTYPE AnalyzeHiddenVolumeHost (
		LONG_PTR hwndDlg, int *driveNo, __int64 hiddenVolHostSize, int *realClusterSize, __int64 *nbrFreeClusters)
	{
		return ::AnalyzeHiddenVolumeHost (
			(HWND) hwndDlg, driveNo, hiddenVolHostSize, realClusterSize, nbrFreeClusters);
	}

	virtual DWORD STDMETHODCALLTYPE ReadWriteFile (BOOL write, BOOL device, BSTR filePath, BSTR *bufferBstr, unsigned __int64 offset, unsigned __int32 size, DWORD *sizeDone)
	{
		return BaseCom::ReadWriteFile (write, device, filePath, bufferBstr, offset, size, sizeDone);
	}

	virtual DWORD STDMETHODCALLTYPE RegisterFilterDriver (BOOL registerDriver, int filterType)
	{
		return BaseCom::RegisterFilterDriver (registerDriver, filterType);
	}

	virtual DWORD STDMETHODCALLTYPE RegisterSystemFavoritesService (BOOL registerService)
	{
		return BaseCom::RegisterSystemFavoritesService (registerService);
	}

	virtual DWORD STDMETHODCALLTYPE SetDriverServiceStartType (DWORD startType)
	{
		return BaseCom::SetDriverServiceStartType (startType);
	}

	virtual BOOL STDMETHODCALLTYPE IsPagingFileActive (BOOL checkNonWindowsPartitionsOnly)
	{
		return BaseCom::IsPagingFileActive (checkNonWindowsPartitionsOnly);
	}

	virtual DWORD STDMETHODCALLTYPE WriteLocalMachineRegistryDwordValue (BSTR keyPath, BSTR valueName, DWORD value)
	{
		return BaseCom::WriteLocalMachineRegistryDwordValue (keyPath, valueName, value);
	}

protected:
	DWORD MessageThreadId;
	LONG RefCount;
	IYourProductFormatCom *CallBack;
};


extern "C" BOOL ComServerFormat ()
{
	SetProcessShutdownParameters (0x100, 0);

	YourProductFactory<YourProductFormatCom> factory (GetCurrentThreadId ());
	DWORD cookie;

	if (IsUacSupported ())
		UacElevated = TRUE;

	if (CoRegisterClassObject (CLSID_YourProductFormatCom, (LPUNKNOWN) &factory,
		CLSCTX_LOCAL_SERVER, REGCLS_SINGLEUSE, &cookie) != S_OK)
		return FALSE;

	MSG msg;
	while (int r = GetMessage (&msg, NULL, 0, 0))
	{
		if (r == -1)
			return FALSE;

		TranslateMessage (&msg);
		DispatchMessage (&msg);

		if (msg.message == WM_APP
			&& ObjectCount < 1
			&& !factory.IsServerLocked ())
			break;
	}
	CoRevokeClassObject (cookie);

	return TRUE;
}


static BOOL ComGetInstance (HWND hWnd, IYourProductFormatCom **tcServer)
{
	return ComGetInstanceBase (hWnd, CLSID_YourProductFormatCom, IID_IYourProductFormatCom, (void **) tcServer);
}


IYourProductFormatCom *GetElevatedInstance (HWND parent)
{
	IYourProductFormatCom *instance;

	if (!ComGetInstance (parent, &instance))
		throw UserAbort (SRC_POS);

	return instance;
}


extern "C" int UacFormatNtfs (HWND hWnd, int driveNo, int clusterSize)
{
	CComPtr<IYourProductFormatCom> tc;
	int r;

	CoInitialize (NULL);

	if (ComGetInstance (hWnd, &tc))
		r = tc->FormatNtfs (driveNo, clusterSize);
	else
		r = 0;

	CoUninitialize ();

	return r;
}


extern "C" int UacAnalyzeHiddenVolumeHost (HWND hwndDlg, int *driveNo, __int64 hiddenVolHostSize, int *realClusterSize, __int64 *nbrFreeClusters)
{
	CComPtr<IYourProductFormatCom> tc;
	int r;

	CoInitialize (NULL);

	if (ComGetInstance (hwndDlg, &tc))
		r = tc->AnalyzeHiddenVolumeHost ((LONG_PTR) hwndDlg, driveNo, hiddenVolHostSize, realClusterSize, nbrFreeClusters);
	else
		r = 0;

	CoUninitialize ();

	return r;
}
