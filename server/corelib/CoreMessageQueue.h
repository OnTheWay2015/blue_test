#pragma once

template<class M>
class CoreMessageQueue
{
public:
    CoreMessageQueue() {};
    ~CoreMessageQueue() {};
private:
	CEasyCriticalSection m_EasyCriticalSection;
    std::queue< CSmartPtr<M>> m_Messages;
public:
    virtual void ProcessQueueMessageHandler(CSmartPtr<M> msg)=0;

public:
    void UpdateProcessMessage(int processCount);
    void PushMessage(CSmartPtr<M> msg);
};
	
template<class M>
void CoreMessageQueue<M>::UpdateProcessMessage(int processCount)
{
    CAutoLock lock(&m_EasyCriticalSection); //todo safe deque
    auto count = 0;
    while (count < processCount && !m_Messages.empty())
    {
        count++;
        auto& msg = m_Messages.front();
        ProcessQueueMessageHandler(msg);
        m_Messages.pop();
    }

}
template<class M>
void CoreMessageQueue<M>::PushMessage(CSmartPtr<M> msg)
{
    CAutoLock lock(&m_EasyCriticalSection); //todo safe deque
    m_Messages.push(msg);
}


