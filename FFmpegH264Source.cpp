#include "FFmpegH264Source.h"

#if defined(_WIN32)
#include <chrono>

int gettimeofday(struct timeval *tp, struct timezone *tzp)
{
	namespace sc = std::chrono;
	sc::system_clock::duration d = sc::system_clock::now().time_since_epoch();
	sc::seconds s = sc::duration_cast<sc::seconds>(d);
	tp->tv_sec = s.count();
	tp->tv_usec = sc::duration_cast<sc::microseconds>(d - s).count();

	return 0;
}

#endif // _WIN32

namespace MESAI
{
	FFmpegH264Source *FFmpegH264Source::createNew(UsageEnvironment &env, FFmpegH264Encoder *E_Source)
	{
		return new FFmpegH264Source(env, E_Source);
	}

	FFmpegH264Source::FFmpegH264Source(UsageEnvironment &env, FFmpegH264Encoder *E_Source) : FramedSource(env), Encoding_Source(E_Source)
	{
		m_eventTriggerId = envir().taskScheduler().createEventTrigger(FFmpegH264Source::deliverFrameStub);
		std::function<void()> callback1 = std::bind(&FFmpegH264Source::onFrame, this);
		Encoding_Source->setCallbackFunctionFrameIsReady(callback1);
	}

	FFmpegH264Source::~FFmpegH264Source()
	{
	}

	void FFmpegH264Source::doStopGettingFrames()
	{
		FramedSource::doStopGettingFrames();
	}

	void FFmpegH264Source::onFrame()
	{
		envir().taskScheduler().triggerEvent(m_eventTriggerId, this);
	}

	void FFmpegH264Source::doGetNextFrame()
	{
		deliverFrame();
	}

	void FFmpegH264Source::deliverFrame()
	{
		if (!isCurrentlyAwaitingData())
			return; // we're not ready for the data yet

		static uint8_t *newFrameDataStart;
		static unsigned newFrameSize = 0;

		/* get the data frame from the Encoding thread.. */
		if (Encoding_Source->GetFrame(&newFrameDataStart, &newFrameSize))
		{
			if (newFrameDataStart != NULL)
			{
				/* This should never happen, but check anyway.. */
				if (newFrameSize > fMaxSize)
				{
					fFrameSize = fMaxSize;
					fNumTruncatedBytes = newFrameSize - fMaxSize;
				}
				else
				{
					fFrameSize = newFrameSize;
				}

				gettimeofday(&fPresentationTime, NULL);
				memcpy(fTo, newFrameDataStart, fFrameSize);

				//delete newFrameDataStart;
				//newFrameSize = 0;

				Encoding_Source->ReleaseFrame();
			}
			else
			{
				fFrameSize = 0;
				fTo = NULL;
				handleClosure(this);
			}
		}
		else
		{
			fFrameSize = 0;
		}

		if (fFrameSize > 0)
			FramedSource::afterGetting(this);
	}
}