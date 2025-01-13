#include <Geode/Geode.hpp>
#ifdef GEODE_IS_WINDOWS
#include <windows.h>
#include <ole2.h>
#include <shlobj.h>
#include "DragDrop.hpp"

// Helper function to convert TCHAR string to UTF-8
std::string ConvertToUTF8(const TCHAR* input) {
#ifdef UNICODE
    // If UNICODE is defined, TCHAR is wchar_t and we need to convert from UTF-16 to UTF-8
    int utf8Length = WideCharToMultiByte(CP_UTF8, 0, input, -1, nullptr, 0, nullptr, nullptr);
    if (utf8Length <= 0) return "";
    
    std::string result(utf8Length, 0);
    WideCharToMultiByte(CP_UTF8, 0, input, -1, &result[0], utf8Length, nullptr, nullptr);
    if (!result.empty() && result.back() == 0) { // Remove null terminator if present
        result.pop_back();
    }
    return result;
#else
    // If UNICODE is not defined, TCHAR is char and we assume it's already in the correct encoding
    return std::string(input);
#endif
}

class CDropTarget : public IDropTarget {
private:
    LONG m_refCount;
    HWND m_hWnd;
    bool m_allowDrop;

public:
    CDropTarget(HWND hWnd) : m_refCount(1), m_hWnd(hWnd), m_allowDrop(false) {}

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) {
        if (riid == IID_IUnknown || riid == IID_IDropTarget) {
            *ppvObject = static_cast<IDropTarget*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    STDMETHODIMP_(ULONG) AddRef() {
        return InterlockedIncrement(&m_refCount);
    }

    STDMETHODIMP_(ULONG) Release() {
        LONG count = InterlockedDecrement(&m_refCount);
        if (count == 0) {
            delete this;
        }
        return count;
    }

    // IDropTarget methods
    STDMETHODIMP DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
        FORMATETC fmte = { CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        
        if (FAILED(pDataObj->QueryGetData(&fmte))) {
            *pdwEffect = DROPEFFECT_NONE;
            return S_OK;
        }

        STGMEDIUM stgm;
        if (SUCCEEDED(pDataObj->GetData(&fmte, &stgm))) {
            HDROP hDrop = static_cast<HDROP>(stgm.hGlobal);
            TCHAR filePath[MAX_PATH];
            
            if (DragQueryFile(hDrop, 0, filePath, MAX_PATH)) {
                // Convert the path to UTF-8 using our helper function
                std::string utf8Path = ConvertToUTF8(filePath);
                
                // Post the Drag event and check the result
                auto result = DragDropEvent(utf8Path, DragDropType::Drag).post();
                m_allowDrop = (result != ListenerResult::Propagate);
            }
            
            ReleaseStgMedium(&stgm);
        }

        *pdwEffect = m_allowDrop ? (*pdwEffect & DROPEFFECT_COPY) : DROPEFFECT_NONE;
        return S_OK;
    }

    STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
        *pdwEffect = m_allowDrop ? (*pdwEffect & DROPEFFECT_COPY) : DROPEFFECT_NONE;
        return S_OK;
    }

    STDMETHODIMP DragLeave() {
        m_allowDrop = false;
        return S_OK;
    }

    STDMETHODIMP Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
        if (!m_allowDrop) {
            *pdwEffect = DROPEFFECT_NONE;
            return S_OK;
        }

        FORMATETC fmte = { CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        STGMEDIUM stgm;
        
        if (SUCCEEDED(pDataObj->GetData(&fmte, &stgm))) {
            HDROP hDrop = static_cast<HDROP>(stgm.hGlobal);
            UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
            
            for (UINT i = 0; i < fileCount; i++) {
                TCHAR filePath[MAX_PATH];
                if (DragQueryFile(hDrop, i, filePath, MAX_PATH)) {
                    // Convert the path to UTF-8 using our helper function
                    std::string utf8Path = ConvertToUTF8(filePath);
                    
                    // Post the Drop event
                    DragDropEvent(utf8Path, DragDropType::Drop).post();
                }
            }
            
            ReleaseStgMedium(&stgm);
        }

        *pdwEffect = DROPEFFECT_COPY;
        m_allowDrop = false;
        return S_OK;
    }
};


__attribute__((constructor)) void initialize() {
	OleInitialize(NULL);
    auto wnd = wglGetCurrentDC();
    static auto target = new CDropTarget(wnd);
    RegisterDragDrop(wnd, (IDropTarget*)(target));
}
#endif