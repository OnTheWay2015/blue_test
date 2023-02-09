#pragma once
 
class blue_logskin :public std::enable_shared_from_this<blue_logskin>
{
public:
	blue_logskin(std::string filename);
	~blue_logskin();
 
public:
	HANDLE m_hfile;
private:
	std::string m_logfile;
};

 
blue_logskin& operator <<(blue_logskin& log, const char* s);
