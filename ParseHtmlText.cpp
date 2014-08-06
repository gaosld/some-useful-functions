#include <mshtml.h>

/*------------------------------------------
 * str_html: the input html strings
 * return the cleantext in the html body.
 -------------------------------------------*/
CString ParseHtmlText(CString str_html)
{
	OLECHAR *olechar_html = str_html.GetBuffer();
	CString str_result;
	IHTMLDocument2 *pDoc = NULL;  
	CoInitialize(NULL);  
	CoCreateInstance(CLSID_HTMLDocument,  
		NULL,  
		CLSCTX_INPROC_SERVER,  
		IID_IHTMLDocument2,  
		(LPVOID *) &pDoc);  
	if (pDoc)  
	{  
		IPersistStreamInit *pPersist = NULL;  
		pDoc->QueryInterface(IID_IPersistStreamInit,  
			(LPVOID *) &pPersist);  
		if (pPersist)  
		{  
			IMarkupServices *pMS = NULL;  
			pPersist->InitNew();  
			pPersist->Release();  
			pDoc->QueryInterface(IID_IMarkupServices,  
				(LPVOID *) &pMS);  
			if (pMS)  
			{  
				IMarkupContainer *pMC = NULL;  
				IMarkupPointer *pMkStart = NULL;  
				IMarkupPointer *pMkFinish = NULL;  
				pMS->CreateMarkupPointer(&pMkStart);  
				pMS->CreateMarkupPointer(&pMkFinish);  
				pMS->ParseString(olechar_html,  
					0,  
					&pMC,  
					pMkStart,  
					pMkFinish);  
				if (pMC)  
				{  
					IHTMLDocument2 *pNewDoc = NULL;  
					pMC->QueryInterface(IID_IHTMLDocument,  
						(LPVOID *) &pNewDoc);  
					if (pNewDoc)  
					{  
						// do anything with pNewDoc, in this case  
						// get the body innerText.  
						IHTMLElement *pBody;  
						pNewDoc->get_body(&pBody);  
						if (pBody)  
						{  
							BSTR strText;  
							pBody->get_innerText(&strText);  
							pBody->Release();
							str_result = strText;
							SysFreeString(strText);  
						}  
						pNewDoc->Release();  
					}  
					pMC->Release();  
				}  
				if (pMkStart)  
					pMkStart->Release();  
				if (pMkFinish)  
					pMkFinish->Release();  
				pMS->Release();  
			}  
		}  
		pDoc->Release();  
	}  
	CoUninitialize(); 
	str_html.ReleaseBuffer(); 
	return str_result;
}
