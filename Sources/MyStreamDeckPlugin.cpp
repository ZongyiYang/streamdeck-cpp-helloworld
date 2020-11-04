//==============================================================================
/**
@file       MyStreamDeckPlugin.cpp

@brief      Demo Hello World

@copyright  (c) 2020, Zongyi Yang

**/
//==============================================================================

#include "MyStreamDeckPlugin.h"
#include <atomic>

#include "Windows/HelloWorldHelper.h"

#include "Common/ESDConnectionManager.h"

class CallBackTimer
{
public:
    CallBackTimer() :_execute(false) { }

    ~CallBackTimer()
    {
        if( _execute.load(std::memory_order_acquire) )
        {
            stop();
        };
    }

    void stop()
    {
        _execute.store(false, std::memory_order_release);
        if(_thd.joinable())
            _thd.join();
    }

    void start(int interval, std::function<void(void)> func)
    {
        if(_execute.load(std::memory_order_acquire))
        {
            stop();
        };
        _execute.store(true, std::memory_order_release);
        _thd = std::thread([this, interval, func]()
        {
            while (_execute.load(std::memory_order_acquire))
            {
                func();
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            }
        });
    }

    bool is_running() const noexcept
    {
        return (_execute.load(std::memory_order_acquire) && _thd.joinable());
    }

private:
    std::atomic<bool> _execute;
    std::thread _thd;
};

MyStreamDeckPlugin::MyStreamDeckPlugin()
{
	mTimer->start(500, [this]()
	{
		this->UpdateTimer();
	});
}

MyStreamDeckPlugin::~MyStreamDeckPlugin()
{
	if(mTimer != nullptr)
	{
		mTimer->stop();
	}
}

void MyStreamDeckPlugin::UpdateTimer()
{
	//
	// Warning: UpdateTimer() is running in the timer thread
	//
	if(mConnectionManager != nullptr)
	{
		mVisibleContextsMutex.lock();
		for (auto & context : mVisibleContexts)
		{
			// change the key's text based on the context's message data that we retreived.
			mConnectionManager->SetTitle(mHelloWorld->genText(context.second.message, context.second.index), context.first, kESDSDKTarget_HardwareAndSoftware);

			// cycle text to look like it's typed
			context.second.index++;
			if (context.second.index > context.second.message.length())
				context.second.index = 0;
		}
		mVisibleContextsMutex.unlock();
	}
}

void MyStreamDeckPlugin::KeyDownForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID)
{
	// Nothing to do
}

void MyStreamDeckPlugin::KeyUpForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID)
{
	// Nothing to do
}

void MyStreamDeckPlugin::WillAppearForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID)
{
	// On key appearing, remember the context and store the settings
	mVisibleContextsMutex.lock();
	contextData_t data{};
	if (inPayload.find("settings") != inPayload.end() && inPayload["settings"].find("message") != inPayload["settings"].end())
		data.message = inPayload["settings"]["message"].get<std::string>();
	mVisibleContexts.insert({ inContext, data });
	mVisibleContextsMutex.unlock();
}

void MyStreamDeckPlugin::WillDisappearForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID)
{
	// Remove the context
	mVisibleContextsMutex.lock();
	mVisibleContexts.erase(inContext);
	mVisibleContextsMutex.unlock();
}

void MyStreamDeckPlugin::DeviceDidConnect(const std::string& inDeviceID, const json &inDeviceInfo)
{
	// Nothing to do
}

void MyStreamDeckPlugin::DeviceDidDisconnect(const std::string& inDeviceID)
{
	// Nothing to do
}

void MyStreamDeckPlugin::SendToPlugin(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID)
{
	// on settings change, store the new setting
	mVisibleContextsMutex.lock();
	if (mVisibleContexts.find(inContext) != mVisibleContexts.end())
	{
		contextData_t data = mVisibleContexts.at(inContext);
		if (inPayload.find("message") != inPayload.end())
			data.message = inPayload["message"].get<std::string>();
		mVisibleContexts.at(inContext) = data;
	}
	mVisibleContextsMutex.unlock();
}