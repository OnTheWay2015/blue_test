#include <stdio.h>
#include <string>
#include <vector>
#include <memory>

#include "curl/curl.h"



CURLM* m_CURLMHandle;
int m_HaveRequestRunning;	


enum REQUEST_TYPE
{
    REQUEST_TYPE_POST,
    REQUEST_TYPE_POST_BIN,
    REQUEST_TYPE_GET,
};



struct HTTP_REQUEST_OPTION
{
	std::string UserAgent;
	std::vector<std::string> HTTPHeaderList;
};
struct REQUEST_INFO
{
    UINT						RequestID;
    int							RequestType;
    std::string RequestURL;
    std::vector<char> RequestContent;
    std::string		UserAgent;
    CURL* CURLHandle;
    std::vector<char> RecvBuffer;
    curl_slist* pHTTPHeaderList;
    void* Param;

};


size_t ReadCallback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    REQUEST_INFO* pRequestInfo = (REQUEST_INFO*)userp;

    size = size * nmemb;
    char* bb = (char*)(ptr);

    //pRequestInfo->RecvBuffer.reserve(pRequestInfo->RecvBuffer.size()+ size);

    std::vector<char> RecvBuffer;
    for (int i = 0; i < size; i++)
    {
		char c= *bb;
        RecvBuffer.push_back(c);
		bb++;
    }

    std::vector<char> b;
	b.reserve(RecvBuffer.size() + pRequestInfo->RecvBuffer.size());
	
	if (pRequestInfo->RecvBuffer.size()>0)
	{
		b.insert(b.end(),pRequestInfo->RecvBuffer.begin(),pRequestInfo->RecvBuffer.end());

	}
	if (RecvBuffer.size() >0 )
	{
		b.insert(b.end(),RecvBuffer.begin(),RecvBuffer.end());
	}
	
	pRequestInfo->RecvBuffer = b;
	return size;
}

bool SendRequest(REQUEST_INFO* pRequestInfo)
{	
	int Result;
	Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_URL, (LPCTSTR)pRequestInfo->RequestURL.c_str());
	if(Result!=CURLE_OK)
	{
		return false;
	}

	//Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_CONNECTTIMEOUT, CMainConfig::GetInstance()->GetHTTPRequesterConfig().ConnectTimeout);
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_CONNECTTIMEOUT, 30*1000); 
	if (Result != CURLE_OK)
	{
		printf("AddPostRequest::SendRequest:设置连接超时失败%d\n", Result);
		return false;
	}
	//Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_TIMEOUT, CMainConfig::GetInstance()->GetHTTPRequesterConfig().RequestTimeout);
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_TIMEOUT, 30*1000);
	if (Result != CURLE_OK)
	{
		printf("AddPostRequest::SendRequest:设置请求超时失败%d\n", Result);
		return false;
	}
	if(pRequestInfo->RequestType==REQUEST_TYPE_POST||pRequestInfo->RequestType==REQUEST_TYPE_POST_BIN)
	{
		//Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_POSTFIELDS, pRequestInfo->RequestContent.GetBuffer());
		Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_POSTFIELDS, &pRequestInfo->RequestContent[0]);
		if(Result!=CURLE_OK)
		{
			printf("AddPostRequest::SendRequest:设置Post内容失败%d\n",Result);
			return false;
		}
		Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_POSTFIELDSIZE, pRequestInfo->RequestContent.size());
		if(Result!=CURLE_OK)
		{
			printf("AddPostRequest::SendRequest:设置Post内容长度失败%d\n",Result);
			return false;
		}
		Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_POST, 1L);
		if(Result!=CURLE_OK)
		{
			printf("AddPostRequest::SendRequest:设置Post开关失败%d\n",Result);
			return false;
		}
	}
	Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_WRITEFUNCTION, ReadCallback);
	if(Result!=CURLE_OK)
	{
		printf("AddPostRequest::SendRequest:设置回调函数失败%d\n",Result);
		return false;
	}
	Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_WRITEDATA, pRequestInfo);
	if(Result!=CURLE_OK)
	{
		printf("AddPostRequest::SendRequest:设置回调参数失败%d\n",Result);
		return false;
	}
	Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_PRIVATE, pRequestInfo);
		/*
		对应取数据
				Result = curl_easy_getinfo(pMsg->easy_handle, CURLINFO_PRIVATE, &pRequestInfo);
		*/
	if(Result!=CURLE_OK)
	{
		printf("AddPostRequest::SendRequest:设置参数失败%d\n",Result);
		return false;
	}	
	Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_SSL_VERIFYPEER, 0); 
	if(Result!=CURLE_OK)
	{
		printf("AddPostRequest::SendRequest:设置SSL参数失败%d\n",Result);
		return false;
	}
	if(!pRequestInfo->UserAgent.empty())
	{
		Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_USERAGENT, (LPCTSTR)pRequestInfo->UserAgent.c_str()); 
		if(Result!=CURLE_OK)
		{
			printf("AddPostRequest::SendRequest:设置UserAgent失败%d\n",Result);
			return false;
		}
	}
	if (pRequestInfo->pHTTPHeaderList)
	{
		Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_HTTPHEADER, pRequestInfo->pHTTPHeaderList);
		if (Result != CURLE_OK)
		{
			printf("AddPostRequest::SendRequest:设置HTTPHeader失败%d\n", Result);
			return false;
		}
	}
	//Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_USERPWD, "hjmlcs:f4fd5df438e8fd32b3b3468bdf08dc78"); 
	//Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_ISSUERCERT,"curl-ca-bundle.cer"); 
	int MResult;
	MResult=curl_multi_add_handle(m_CURLMHandle, pRequestInfo->CURLHandle);
	if(MResult!=CURLM_OK)
	{
		printf("AddPostRequest::SendRequest:添加请求失败%d\n",MResult);
		return false;
	}
	MResult=curl_multi_perform(m_CURLMHandle, &m_HaveRequestRunning);
	if(MResult!=CURLM_OK&&MResult!=CURLM_CALL_MULTI_PERFORM)
	{
		printf("AddPostRequest::SendRequest:处理请求失败%d\n",MResult);
		return false;
	}
	switch (pRequestInfo->RequestType)
	{
	case REQUEST_TYPE_POST:
	case REQUEST_TYPE_POST_BIN:
		printf("(%u)RequestURL=%s  RequestContent=%s\n",
			pRequestInfo->RequestID, (LPCTSTR)pRequestInfo->RequestURL.c_str(), (LPCTSTR)&(pRequestInfo->RequestContent[0]) );
		break;
	default:
		printf("(%u)RequestURL=%s\n",
			pRequestInfo->RequestID, (LPCTSTR)pRequestInfo->RequestURL.c_str());
		break;
	}	
	
	return true;
}



int RunCURL()
{
	int ProcessCount=0;

	fd_set fdread;
	fd_set fdwrite;
	fd_set fdexcep;
	int maxfd = -1;
	int rc;

	FD_ZERO(&fdread);
	FD_ZERO(&fdwrite);
	FD_ZERO(&fdexcep);
	
	/* get file descriptors from the transfers */
	int Result=curl_multi_fdset(m_CURLMHandle, &fdread, &fdwrite, &fdexcep, &maxfd);



	long TimeOut = -1;
	timeval	SelectTimeOut;
	Result = curl_multi_timeout(m_CURLMHandle, &TimeOut);
	if (TimeOut >= 0)
	{
		SelectTimeOut.tv_sec = TimeOut / 1000;
		if (SelectTimeOut.tv_sec > 0)
		{
			SelectTimeOut.tv_sec = 1;
			SelectTimeOut.tv_usec = 0;
		}			
		else
		{
			SelectTimeOut.tv_usec = (TimeOut % 1000) * 1000;
		}			
	}
	else
	{
		SelectTimeOut.tv_sec = 0;
		SelectTimeOut.tv_usec = 0;
	}
	
	/* In a real-world program you OF COURSE check the return code of the
	 function calls.  On success, the value of maxfd is guaranteed to be
	 greater or equal than -1.  We call select(maxfd + 1, ...), specially in
	 case of (maxfd == -1), we call select(0, ...), which is basically equal
	 to sleep. */
	if(maxfd>=0)
	{
		rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &SelectTimeOut);

		switch(rc) 
		{
		case -1:
			/* select error */
			{
#ifdef WIN32
				int ErrCode=WSAGetLastError ();
#else
				int ErrCode = errno;
#endif
				printf("SelectError=%d\n",ErrCode);
			}
			break;
		case 0:
			curl_multi_perform(m_CURLMHandle, &m_HaveRequestRunning);
			break;
		default:
			/* timeout or readable/writable sockets */
			curl_multi_perform(m_CURLMHandle, &m_HaveRequestRunning);
			ProcessCount++;
			break;
		}
	}
	else
	{
		curl_multi_perform(m_CURLMHandle, &m_HaveRequestRunning);
		//ProcessCount++;
	}
	return ProcessCount;
}


void FreeRequest(REQUEST_INFO* pRequestInfo)
{


    if ( pRequestInfo->pHTTPHeaderList)
    {

        curl_slist* pList = pRequestInfo->pHTTPHeaderList;
		while(pList)
		{
			auto p = pList->next;
			delete pList;
			pList = p;
		}
	
    }
    delete pRequestInfo;
}

void ProcessRequestEnd(REQUEST_INFO * pRequestInfo,CURLcode Result,int ResponseCode)
{
	printf("(%u)Result=%s,ResponseCode=%d\n",
		pRequestInfo->RequestID,
		curl_easy_strerror(Result),ResponseCode);


	if(Result==CURLE_OK&&ResponseCode==200)
	{
		printf("OK RequestID[%u] RequestType[%d]\n",
					pRequestInfo->RequestID, pRequestInfo->RequestType);
	
		auto sz = pRequestInfo->RecvBuffer.size() + 1;
		
		//char* buf = (char*)malloc(sz);
		//buf[sz-1]='\0';
		//memcpy(buf,	&pRequestInfo->RecvBuffer[0],sz);
		//printf("RecvBuffer[%s]\n",buf);
		//free(buf);
		
		auto buf = std::make_shared<std::vector<char>>();
		*buf = pRequestInfo->RecvBuffer;	
		

	}
	else
	{
		printf("error RequestID[%u] RequestType[%d]\n",
					pRequestInfo->RequestID, pRequestInfo->RequestType);
	}
	//pRequestInfo->Clear(m_CURLMHandle);


	FreeRequest(pRequestInfo); //free
}





void FinishCURL()
{
	int MsgLeft=0;
	CURLMsg * pMsg=curl_multi_info_read(m_CURLMHandle, &MsgLeft);
	if(pMsg)
	{
		if(pMsg->msg==CURLMSG_DONE)
		{
			long ResponseCode = 0;
			int Result = curl_easy_getinfo(pMsg->easy_handle, CURLINFO_RESPONSE_CODE, &ResponseCode);			
			if (Result == CURLE_OK)
			{
				REQUEST_INFO * pRequestInfo = NULL;
				Result = curl_easy_getinfo(pMsg->easy_handle, CURLINFO_PRIVATE, &pRequestInfo);
				if(Result==CURLE_OK)
				{
					if(pRequestInfo)
					{
						ProcessRequestEnd(pRequestInfo,pMsg->data.result,ResponseCode);
					}
					else
					{
						printf("无法找到请求数据\n");
					}
				}
				else
				{
					printf("获取参数2失败%d\n",Result);
				}
			}
			else
			{
				printf("获取参数1失败%d\n",Result);
			}
		}
	}
}

int Update()
{
    int ProcessCount = 0;

    if (m_HaveRequestRunning)
    {
        RunCURL();
    }
    FinishCURL();

    return ProcessCount;
}


void AddGetRequest(const std::string szURL, HTTP_REQUEST_OPTION* pOptions)
{
    REQUEST_INFO* pRequestInfo = new REQUEST_INFO;
	memset(pRequestInfo,0,sizeof(REQUEST_INFO));
    //pRequestInfo->Clear(m_CURLMHandle);
    pRequestInfo->RequestURL = szURL;
    pRequestInfo->RequestType = REQUEST_TYPE_GET;
    pRequestInfo->UserAgent = pOptions->UserAgent;
    if (pOptions->HTTPHeaderList.size())
    {
        curl_slist* pCurList = NULL;
        for (UINT i = 0; i < pOptions->HTTPHeaderList.size(); i++)
        {
            curl_slist* pList = new curl_slist();
            pList->data = (char*)malloc( pOptions->HTTPHeaderList[i].size()+1);
            strcpy_s(pList->data, pOptions->HTTPHeaderList[i].size() + 1, pOptions->HTTPHeaderList[i].c_str());
            pList->next = NULL;
            if (pCurList)
            {
                pCurList->next = pList;
            }
            else
            {
                pRequestInfo->pHTTPHeaderList = pList;
            }
            pCurList = pList;
        }
    }
    pRequestInfo->CURLHandle = curl_easy_init();
    if (pRequestInfo->CURLHandle == NULL)
    {
        printf("AddGetRequest::Init:curl_easy_init失败\n");
        return ;
    }

    SendRequest(pRequestInfo);

}

struct aa {
	int a;
};
void use_qsort1() {
    aa arr[10] = { 1, 3, 5, 2, 4, 6, 0, 9, 7, 8 };
    qsort(arr, 10, sizeof(int), [](const void* a, const void* b) -> int {return *(int*)a - *(int*)b; });
    for (int i = 0; i < 10; i++) {
        printf("%d",arr[i].a);
    }
}

void use_qsort() {
    int arr[] = { 1, 3, 5, 2, 4, 6, 0, 9, 7, 8 };
    qsort(arr, 10, sizeof(int), [](const void* a, const void* b) -> int {return *(int*)a - *(int*)b; });
    for (int i = 0; i < 10; i++) {
        printf("%d",arr[i]);
    }
}

int main()
{

	

	m_CURLMHandle=curl_multi_init();
	if(m_CURLMHandle==NULL)
	{
		printf("curl_multi_init失败\n");
		return 1;
    }
    HTTP_REQUEST_OPTION Options;
    AddGetRequest("https://www.baidu.com", &Options);


	while (true)
	{
		Update();
		Sleep(10);
	}


    return 0;
}

