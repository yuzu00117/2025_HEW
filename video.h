//-----------------------------------------------------------------------------------------------------
// #name video.h
// #description 動画再生
// #make 2024/12/11     王泳心
// #update 2024/12/11
// #comment 追加・修正予定
//          ・
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
    PLAYING = 0,    //再生中
    PAUSED,         //一時停止中
    CLOSED,         //閉じた
};
class Video :public  IMFAsyncCallback
{
public:
    void	Initialize();
    void	Finalize();
    void	Update(wchar_t* URL);

    //  動画を再生する
    VideoState  Start(wchar_t* URL);
    //  動画を閉じる
    void    Close();
    //  動画を一時停止させる
    VideoState  Pause();
    //  動画を再開させる
    VideoState  Resume();

    HRESULT Invoke(IMFAsyncResult* pAsyncResult);
    //STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult, VideoState state);
    virtual HRESULT OnPresentationEnded(IMFMediaEvent* pMediaEvent);
    // 追加のセッションイベントを捕捉するためのオーバーライド
    virtual HRESULT OnSessionEvent(IMFMediaEvent*, MediaEventType)
    {
        return S_OK;
    }



    // IUnknown メソッド
    // オブジェクトの生存期間を参照カウント (m_nRefCount) で制御する
    STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
    // IMFAsyncCallback メソッド
    STDMETHODIMP GetParameters(DWORD*, DWORD*){ return E_NOTIMPL; }

private:
    //  MediaResolver と MediaSourceの作成
    HRESULT CreateMediaSource(PCWSTR sURL, IMFMediaSource** ppSource);


    //  Topologyの作成
    HRESULT CreatePlaybackTopology(
        IMFMediaSource* pSource,          // Media source.
        IMFPresentationDescriptor* pPD,   // Presentation descriptor.
        HWND hVideoWnd,                   // Video window.
        IMFTopology** ppTopology);        // Receives a pointer to the topology.
    //  MediaSinkへのstreamの接続
    HRESULT AddBranchToPartialTopology(
        IMFTopology* pTopology,         // Topology.
        IMFMediaSource* pSource,        // Media source.
        IMFPresentationDescriptor* pPD, // Presentation descriptor.
        DWORD iStream,                  // Stream index.
        HWND hVideoWnd);                 // Window for video playback.
    //  MediaSinkの作成
    HRESULT CreateMediaSinkActivate(
        IMFStreamDescriptor* pSourceSD,     // Pointer to the stream descriptor.
        HWND hVideoWindow,                  // Handle to the video clipping window.
        IMFActivate** ppActivate);
    // Source Nodeの作成
    HRESULT AddSourceNode(
        IMFTopology* pTopology,           // Topology.
        IMFMediaSource* pSource,          // Media source.
        IMFPresentationDescriptor* pPD,   // Presentation descriptor.
        IMFStreamDescriptor* pSD,         // Stream descriptor.
        IMFTopologyNode** ppNode);         // Receives the node pointer.

    //  出力Nodeの作成
    HRESULT AddOutputNode(
        IMFTopology* pTopology,     // Topology.
        IMFActivate* pActivate,     // Media sink activation object.
        DWORD dwId,                 // Identifier of the stream sink.
        IMFTopologyNode** ppNode);   // Receives the node pointer.


    //メンバー変数
    //---------------------------------------------------------------------------------------------

    HRESULT	m_hr;

    PCWSTR m_URL = nullptr;	//動画のURL
    IMFMediaSession* m_p_Session = nullptr;	// MediaSession
    IMFMediaSource* m_p_Source = nullptr;	// MediaSource Interface
    IMFPresentationDescriptor* m_p_PD = nullptr;	// Presentation Descriptor
    IMFTopology* m_p_Topology = nullptr;	//	Topology
    PROPVARIANT m_varStartPosition;		//　再生開始の場所
    IMFMediaEvent* m_p_Event = NULL;
    HWND* M_p_Video_hWnd = nullptr;			// Videoのハンドルを保存しておくための変数
    HWND M_Windows_hWnd;		// Windowsのハンドルを保存しておくための変数

    VideoState	m_state = CLOSED;    //今の動画の再生状態

    long m_nRefCount = 1; // 参照カウント

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


