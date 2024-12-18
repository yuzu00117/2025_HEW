//-----------------------------------------------------------------------------------------------------
// #name video.cpp
// #description ����Đ�
// #make 2024/12/11			���j�S
// #update 2024/12/11
// #comment �ǉ��E�C���\��
//          �EUpdate���̕ϐ�pressed_pause�̏����l��false�ɂ�����
//           
//----------------------------------------------------------------------------------------------------

#include "video.h"
#include "main.h"
#include <shlwapi.h>
#include"Xinput_controller.h"



#pragma comment(lib, "Mfplat.lib")	//MFStartup�֐��AMFCreateSourceResolver�֐��K�v
#pragma comment(lib, "MF.lib")		//MFCreateMediaSession�֐��K�v
#pragma comment(lib, "Mfuuid.lib")	//SetTopology��Start���\�b�h�K�v
#pragma comment(lib, "Shlwapi.lib")	






void  Video::Initialize()
{
	// COM���C�u����������������
	//m_hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	//if (FAILED(m_hr))
	//{
	//	return -1;
	//}
}

void Video::Finalize()
{

	// COM�̏I������
	//CoUninitialize();
}

void Video::Update(wchar_t* URL)
{
	//�R���g���[���[�̓��͂̎󂯎��
	ControllerState controller;
	controller = GetControllerInput();
	//�g���K�[����p�ϐ�
	bool	pressed_pause = true;	//���̃V�[���ɑJ�ڂ��Ă��鎞�A�Ȃ����{�^���iA�j�̓��͗����������Ȃ��āA�Ƃ肠�����J�ڂ����炢���Ȃ��~���Ȃ��悤true�ɂ���
	bool	pressed_close = false;

	//������Đ�
	Start(URL);

	//���悪�܂��I����ĂȂ����胋�[�v����
	while (m_state != CLOSED)
	{
		controller = GetControllerInput();

		//�{�^�������ꂽ�ꍇ
		if (!pressed_pause && controller.buttonA)
		{
			//����Đ����Ȃ�
			if (m_state == PLAYING)
			{
				//����ꎞ��~
				Pause();
			}
			//���悪�ꎞ��~���Ȃ�
			else if (m_state == PAUSED)
			{
				//������ĊJ����
				Resume();
			}
		}
		pressed_pause = controller.buttonA;

		if (!pressed_close && controller.buttonB)
		{
			m_state = CLOSED;
		}
		pressed_close = controller.buttonB;
	}

	//��������
	Close();

}

VideoState Video::Start(wchar_t* URL)
{
	//Media Foundation �v���b�g�t�H�[����������
	m_hr = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
	if (m_hr != S_OK)
	{
		return m_state;
	}

	
	


	// MediaSession �̍쐬
	m_hr = MFCreateMediaSession(NULL, &m_p_Session);
	//���s�����ꍇ���������
	if (FAILED(m_hr))
	{
		return m_state;
	}
	// ����Session�C�x���g�����������Ƃ��ɁAMedia Session��IMFAsyncCallback::Invoke���Ăяo���B
	m_hr = m_p_Session->BeginGetEvent((IMFAsyncCallback*)this, NULL);

	if (FAILED(m_hr)) return m_state;

	//	SourceResolver�@�Ɓ@MediaSource�@�̍쐬
	m_hr = CreateMediaSource(URL, &m_p_Source);
	if (m_hr != S_OK)
	{
		return m_state;
	}

	//PresentationDescriptor�̍쐬
	m_hr = m_p_Source->CreatePresentationDescriptor(&m_p_PD);

	//	main����Windows�̃n���h���������Ă��Ȃ��Ƃ����Ȃ��iWindows�̃n���h������Video�̃n���h���ɐ؂�ւ��邽�߂Ɂj
	M_p_Video_hWnd = GetWindowsHandle();
	M_Windows_hWnd = *M_p_Video_hWnd;

	//�@Topology�@�̍쐬
	m_hr = CreatePlaybackTopology(m_p_Source, m_p_PD, *M_p_Video_hWnd, &m_p_Topology);
	if (m_hr != S_OK)
	{
		return m_state;
	}

	//	MesiaSession�@�ł����������Topology�̐ݒ�����
	m_hr = m_p_Session->SetTopology(0, m_p_Topology);
	if (FAILED(m_hr))
	{
		return m_state;
	}


	//�@�Đ�
	PropVariantInit(&m_varStartPosition);

	m_hr = m_p_Session->Start(&GUID_NULL, &m_varStartPosition);
	if (FAILED(m_hr))
	{
		return m_state;
	}

	m_state = PLAYING;
	return PLAYING;
}

void Video::Close()
{
	PropVariantClear(&m_varStartPosition);

	// MediaSession�����
	m_p_Session->Close();
	// MediaSource���V���b�g�_�E��
	m_p_Source->Shutdown();
	// MediaSession�@���V���b�g�_�E��
	m_p_Session->Shutdown();
	//Media Foundation Platform���I��������
	MFShutdown();

	//�n���h����Windows�ɖ߂�
	SetWindowsHandle(M_Windows_hWnd);
}


VideoState Video::Pause()
{
	if (m_state == PLAYING)
	{
		m_hr = m_p_Session->Pause();
		if (SUCCEEDED(m_hr))
		{
			m_state = PAUSED;
			return PAUSED;
		}
	}
	return m_state;
}

VideoState Video::Resume()
{
	if (m_state == PAUSED)
	{
		m_hr = m_p_Session->Start(&GUID_NULL, &m_varStartPosition);
		if (SUCCEEDED(m_hr))
		{
			m_state = PLAYING;
			return PLAYING;
		}
	}
	return m_state;
}



// �񓯊���BeginGetEvent���\�b�h�p�̃R�[���o�b�N
HRESULT Video::Invoke(IMFAsyncResult* pAsyncResult)
{
	// ���̃��\�b�h�̓��[�J�[�X���b�h�ŌĂ΂��B

	MediaEventType mediaEventType = MEUnknown;
	IMFMediaEvent* pMediaEvent = NULL;

	// �C�x���g �L���[����C�x���g���擾����
	HRESULT m_hr = m_p_Session->EndGetEvent(pAsyncResult, &pMediaEvent);
	if (FAILED(m_hr)) goto done;

	// �C�x���g�̎�ނ��擾����
	m_hr = pMediaEvent->GetType(&mediaEventType);
	if (FAILED(m_hr)) goto done;

	if (mediaEventType == MESessionEnded) //�@�Đ��I�����
	{
		m_state = CLOSED;
	}

	if (mediaEventType == MESessionClosed) // �Z�b�V�����͕���ꂽ�B
	{

	}
	else
	{
		// ���ׂĂ̑��̃C�x���g�̂��߂ɁA�L���[�̎��̃C�x���g���擾����
		m_hr = m_p_Session->BeginGetEvent(this, NULL);
		if (FAILED(m_hr)) goto done;
	}


done:
	SafeRelease(&pMediaEvent);
	return S_OK;
}

HRESULT Video::OnPresentationEnded(IMFMediaEvent* pMediaEvent)
{
	// �Z�b�V�����͎����I�Ɏ��g���A��~��Ԃɂ���
	return S_OK;
}




STDMETHODIMP Video::QueryInterface(REFIID riid, void** ppv) {
	static const QITAB qit[] =
	{
		QITABENT(Video, IMFAsyncCallback),
		{ 0 }
	};
	return QISearch(this, qit, riid, ppv);
}

ULONG Video::AddRef()
{
	return InterlockedIncrement(&m_nRefCount);
}

ULONG Video::Release()
{
	ULONG uCount = InterlockedDecrement(&m_nRefCount);
	if (uCount == 0)
	{
		delete this;
	}
	return uCount;
}




/// private
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT Video::CreateMediaSource(PCWSTR m_URL, IMFMediaSource** ppSource)
{
	MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;

	IMFSourceResolver* pSourceResolver = NULL;
	IUnknown* m_p_Source = NULL;

	//	SourceResolver	�̃C���X�^���X�쐬
	HRESULT m_hr = MFCreateSourceResolver(&pSourceResolver);
	//���s�����ꍇ���������
	if (FAILED(m_hr))
	{
		goto done;
	}

	//�@(MediaSource)�@����̃I�u�W�F�N�g�𐶐��iDVD���Ǝv���Ƃ��ā[�j
	m_hr = pSourceResolver->CreateObjectFromURL(
		m_URL,                       // URL of the source.
		MF_RESOLUTION_MEDIASOURCE,  // Create a source object.
		NULL,                       // Optional property store.
		&ObjectType,        // Receives the created object type. 
		&m_p_Source            // Receives a pointer to the media source.
	);
	//���s�����ꍇ���������
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Get the IMFMediaSource interface from the media source.
	m_hr = m_p_Source->QueryInterface(IID_PPV_ARGS(ppSource));

done:
	SafeRelease(&pSourceResolver);
	SafeRelease(&m_p_Source);
	return m_hr;
}



HRESULT Video::CreatePlaybackTopology(
	IMFMediaSource* m_p_Source,          // Media source.
	IMFPresentationDescriptor* m_p_PD,   // Presentation descriptor.
	HWND M_p_Video_hWnd,                   // Video window.
	IMFTopology** ppTopology)		  // Receives a pointer to the topology.
{
	IMFTopology* m_p_Topology = NULL;
	DWORD cSourceStreams = 0;

	// Create a new topology.
	HRESULT m_hr = MFCreateTopology(&m_p_Topology);
	//���s�����ꍇ���������
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Get the number of streams in the media source.
	m_hr = m_p_PD->GetStreamDescriptorCount(&cSourceStreams);
	//���s�����ꍇ���������
	if (FAILED(m_hr))
	{
		goto done;
	}

	// For each stream, create the topology nodes and add them to the topology.
	for (DWORD i = 0; i < cSourceStreams; i++)
	{
		m_hr = AddBranchToPartialTopology(m_p_Topology, m_p_Source, m_p_PD, i, M_p_Video_hWnd);
		//���s�����ꍇ���������
		if (FAILED(m_hr))
		{
			goto done;
		}
	}

	// Return the IMFTopology pointer to the caller.
	*ppTopology = m_p_Topology;
	(*ppTopology)->AddRef();

done:
	SafeRelease(&m_p_Topology);
	return m_hr;

}

HRESULT Video::AddBranchToPartialTopology(
	IMFTopology* m_p_Topology,         // Topology.
	IMFMediaSource* m_p_Source,        // Media source.
	IMFPresentationDescriptor* m_p_PD, // Presentation descriptor.
	DWORD iStream,                  // Stream index.
	HWND M_p_Video_hWnd)                 // Window for video playback.
{
	IMFStreamDescriptor* pSD = NULL;
	IMFActivate* pSinkActivate = NULL;
	IMFTopologyNode* pSourceNode = NULL;
	IMFTopologyNode* pOutputNode = NULL;

	BOOL fSelected = FALSE;

	HRESULT m_hr = m_p_PD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSD);
	//���s�����ꍇ���������
	if (FAILED(m_hr))
	{
		goto done;
	}

	if (fSelected)
	{
		// Create the media sink activation object.
		m_hr = CreateMediaSinkActivate(pSD, M_p_Video_hWnd, &pSinkActivate);
		//���s�����ꍇ���������
		if (FAILED(m_hr))
		{
			goto done;
		}

		// Add a source node for this stream.
		m_hr = AddSourceNode(m_p_Topology, m_p_Source, m_p_PD, pSD, &pSourceNode);
		//���s�����ꍇ���������
		if (FAILED(m_hr))
		{
			goto done;
		}

		// Create the output node for the renderer.
		m_hr = AddOutputNode(m_p_Topology, pSinkActivate, 0, &pOutputNode);
		//���s�����ꍇ���������
		if (FAILED(m_hr))
		{
			goto done;
		}

		// Connect the source node to the output node.
		m_hr = pSourceNode->ConnectOutput(0, pOutputNode, 0);
	}
	// else: If not selected, don't add the branch. 

done:
	SafeRelease(&pSD);
	SafeRelease(&pSinkActivate);
	SafeRelease(&pSourceNode);
	SafeRelease(&pOutputNode);
	return m_hr;
}


HRESULT Video::CreateMediaSinkActivate(
	IMFStreamDescriptor* pSourceSD,     // Pointer to the stream descriptor.
	HWND hVideoWindow,                  // Handle to the video clipping window.
	IMFActivate** ppActivate
)
{
	IMFMediaTypeHandler* pHandler = NULL;
	IMFActivate* pActivate = NULL;

	// Get the media type handler for the stream.
	HRESULT m_hr = pSourceSD->GetMediaTypeHandler(&pHandler);
	//���s�����ꍇ���������
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Get the major media type.
	GUID guidMajorType;
	m_hr = pHandler->GetMajorType(&guidMajorType);
	//���s�����ꍇ���������
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Create an IMFActivate object for the renderer, based on the media type.
	if (MFMediaType_Audio == guidMajorType)
	{
		// Create the audio renderer.
		m_hr = MFCreateAudioRendererActivate(&pActivate);
	}
	else if (MFMediaType_Video == guidMajorType)
	{
		// Create the video renderer.
		m_hr = MFCreateVideoRendererActivate(hVideoWindow, &pActivate);
	}
	else
	{
		// Unknown stream type. 
		m_hr = E_FAIL;
		// Optionally, you could deselect this stream instead of failing.
	}
	//���s�����ꍇ���������
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Return IMFActivate pointer to caller.
	*ppActivate = pActivate;
	(*ppActivate)->AddRef();

done:
	SafeRelease(&pHandler);
	SafeRelease(&pActivate);
	return m_hr;
}

// Add an output node to a topology.
HRESULT Video::AddOutputNode(
	IMFTopology* m_p_Topology,     // Topology.
	IMFActivate* pActivate,     // Media sink activation object.
	DWORD dwId,                 // Identifier of the stream sink.
	IMFTopologyNode** ppNode)   // Receives the node pointer.
{
	IMFTopologyNode* pNode = NULL;

	// Create the node.
	HRESULT m_hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode);
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Set the object pointer.
	m_hr = pNode->SetObject(pActivate);
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Set the stream sink ID attribute.
	m_hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId);
	if (FAILED(m_hr))
	{
		goto done;
	}

	m_hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Add the node to the topology.
	m_hr = m_p_Topology->AddNode(pNode);
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(&pNode);
	return m_hr;
}


HRESULT Video::AddSourceNode(
	IMFTopology* m_p_Topology,           // Topology.
	IMFMediaSource* m_p_Source,          // Media source.
	IMFPresentationDescriptor* m_p_PD,   // Presentation descriptor.
	IMFStreamDescriptor* pSD,         // Stream descriptor.
	IMFTopologyNode** ppNode)         // Receives the node pointer.
{
	IMFTopologyNode* pNode = NULL;

	// Create the node.
	HRESULT m_hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode);
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Set the attributes.
	m_hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, m_p_Source);
	if (FAILED(m_hr))
	{
		goto done;
	}

	m_hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, m_p_PD);
	if (FAILED(m_hr))
	{
		goto done;
	}

	m_hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Add the node to the topology.
	m_hr = m_p_Topology->AddNode(pNode);
	if (FAILED(m_hr))
	{
		goto done;
	}

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(&pNode);
	return m_hr;
}