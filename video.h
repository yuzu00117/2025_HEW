//-----------------------------------------------------------------------------------------------------
// #name video.h
// #description ����Đ�
// #make 2024/12/11     ���j�S
// #update 2024/12/11
// #comment �ǉ��E�C���\��
//          �E
//           
//----------------------------------------------------------------------------------------------------

#pragma once
#ifndef VIDEO_H
#define VIDEO_H
#include <mfapi.h>
#include <mfidl.h>
#include <combaseapi.h>


enum VideoState
{
    PLAYING = 0,    //�Đ���
    PAUSED,         //�ꎞ��~��
    CLOSED,         //����
};
class Video :public  IMFAsyncCallback
{
public:
    void	Initialize();
    void	Finalize();
    void	Update(wchar_t* URL);

    //  ������Đ�����
    VideoState  Start(wchar_t* URL);
    //  ��������
    void    Close();
    //  ������ꎞ��~������
    VideoState  Pause();
    //  ������ĊJ������
    VideoState  Resume();

    HRESULT Invoke(IMFAsyncResult* pAsyncResult);
    //STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult, VideoState state);
    virtual HRESULT OnPresentationEnded(IMFMediaEvent* pMediaEvent);
    // �ǉ��̃Z�b�V�����C�x���g��ߑ����邽�߂̃I�[�o�[���C�h
    virtual HRESULT OnSessionEvent(IMFMediaEvent*, MediaEventType)
    {
        return S_OK;
    }



    // IUnknown ���\�b�h
    // �I�u�W�F�N�g�̐������Ԃ��Q�ƃJ�E���g (m_nRefCount) �Ő��䂷��
    STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
    // IMFAsyncCallback ���\�b�h
    STDMETHODIMP GetParameters(DWORD*, DWORD*){ return E_NOTIMPL; }

private:
    //  MediaResolver �� MediaSource�̍쐬
    HRESULT CreateMediaSource(PCWSTR sURL, IMFMediaSource** ppSource);


    //  Topology�̍쐬
    HRESULT CreatePlaybackTopology(
        IMFMediaSource* pSource,          // Media source.
        IMFPresentationDescriptor* pPD,   // Presentation descriptor.
        HWND hVideoWnd,                   // Video window.
        IMFTopology** ppTopology);        // Receives a pointer to the topology.
    //  MediaSink�ւ�stream�̐ڑ�
    HRESULT AddBranchToPartialTopology(
        IMFTopology* pTopology,         // Topology.
        IMFMediaSource* pSource,        // Media source.
        IMFPresentationDescriptor* pPD, // Presentation descriptor.
        DWORD iStream,                  // Stream index.
        HWND hVideoWnd);                 // Window for video playback.
    //  MediaSink�̍쐬
    HRESULT CreateMediaSinkActivate(
        IMFStreamDescriptor* pSourceSD,     // Pointer to the stream descriptor.
        HWND hVideoWindow,                  // Handle to the video clipping window.
        IMFActivate** ppActivate);
    // Source Node�̍쐬
    HRESULT AddSourceNode(
        IMFTopology* pTopology,           // Topology.
        IMFMediaSource* pSource,          // Media source.
        IMFPresentationDescriptor* pPD,   // Presentation descriptor.
        IMFStreamDescriptor* pSD,         // Stream descriptor.
        IMFTopologyNode** ppNode);         // Receives the node pointer.

    //  �o��Node�̍쐬
    HRESULT AddOutputNode(
        IMFTopology* pTopology,     // Topology.
        IMFActivate* pActivate,     // Media sink activation object.
        DWORD dwId,                 // Identifier of the stream sink.
        IMFTopologyNode** ppNode);   // Receives the node pointer.


    //�����o�[�ϐ�
    //---------------------------------------------------------------------------------------------

    HRESULT	m_hr;

    PCWSTR m_URL = nullptr;	//�����URL
    IMFMediaSession* m_p_Session = nullptr;	// MediaSession
    IMFMediaSource* m_p_Source = nullptr;	// MediaSource Interface
    IMFPresentationDescriptor* m_p_PD = nullptr;	// Presentation Descriptor
    IMFTopology* m_p_Topology = nullptr;	//	Topology
    PROPVARIANT m_varStartPosition;		//�@�Đ��J�n�̏ꏊ
    IMFMediaEvent* m_p_Event = NULL;
    HWND* M_p_Video_hWnd = nullptr;			// Video�̃n���h����ۑ����Ă������߂̕ϐ�
    HWND M_Windows_hWnd;		// Windows�̃n���h����ۑ����Ă������߂̕ϐ�

    VideoState	m_state = CLOSED;    //���̓���̍Đ����

    long m_nRefCount = 1; // �Q�ƃJ�E���g

};


template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

#endif // !VIDEO_H


