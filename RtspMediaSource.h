#ifndef RTSPMEDIASOURCE_H_
#define RTSPMEDIASOURCE_H_

#include "mindroid/os/Handler.h"
#include "mindroid/os/Thread.h"
#include "mindroid/os/Lock.h"
#include "mindroid/lang/String.h"
#include "mindroid/util/List.h"
#include "RtspSocket.h"
#include <map>

namespace mindroid {
class Message;
class Buffer;
}

using mindroid::sp;

class RtspMediaSource :
	public mindroid::Handler {
public:
	static const uint32_t START_VIDEO_TRACK = 0;
	static const uint32_t START_AUDIO_TRACK = 1;
	static const uint32_t DESCRIBE_MEDIA_SOURCE = 2;
	static const uint32_t SETUP_AUDIO_TRACK = 3;
	static const uint32_t PLAY_AUDIO_TRACK = 4;
	static const uint32_t SETUP_VIDEO_TRACK = 5;
	static const uint32_t PLAY_VIDEO_TRACK = 6;

	RtspMediaSource(const sp<mindroid::Handler>& netHandler);
	virtual ~RtspMediaSource();

	bool start(const mindroid::String& url);
	void stop();

	virtual void handleMessage(const sp<mindroid::Message>& message);

private:
	class NetReceiver :
		public mindroid::Thread
	{
	public:
		NetReceiver(const sp<RtspMediaSource>& mediaSource);
		virtual void run();
		void stop();

	private:
		sp<RtspMediaSource> mMediaSource;
		sp<RtspSocket> mSocket;

		NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(NetReceiver)
	};

	sp<RtspSocket> getSocket() { return mSocket; }
	void describeMediaSource();
	void onDescribeMediaSource(const sp<mindroid::Buffer>& desc);
	void setupAudioTrack(uint16_t port);
	void playAudioTrack();
	void setupVideoTrack(uint16_t port);
	void playVideoTrack();
	void setPendingRequest(uint32_t id, const sp<mindroid::Message>& message);
	sp<mindroid::Message> getPendingRequest(uint32_t id);
	sp<mindroid::Message> removePendingRequest(uint32_t id);

	sp<mindroid::Handler> mNetHandler;
	sp<NetReceiver> mNetReceiver;
	sp<RtspSocket> mSocket;
	mindroid::String mHost;
	mindroid::String mPort;
	mindroid::String mSdpFile;
	mindroid::String mAudioMediaSource;
	mindroid::String mAudioSessionId;
	mindroid::String mVideoMediaSource;
	mindroid::String mVideoSessionId;
	uint32_t mCSeq;

	mindroid::Lock mLock;
	std::map< uint32_t, sp<mindroid::Message> > mPendingRequests;

	friend class NetReceiver;
};

#endif /* RTSPMEDIASOURCE_H_ */
