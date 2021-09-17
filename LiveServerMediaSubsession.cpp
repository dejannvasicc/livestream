#include "LiveServerMediaSubsession.h"

namespace MESAI
{
	LiveServerMediaSubsession * LiveServerMediaSubsession::createNew(UsageEnvironment& env, StreamReplicator* replicator)
	{ 
		return new LiveServerMediaSubsession(env,replicator);
	}
					
	FramedSource* LiveServerMediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate)
	{
		FramedSource* source = m_replicator->createStreamReplica();
		return H264VideoStreamDiscreteFramer::createNew(envir(), source);
	}
		
	RTPSink* LiveServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock,  unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource)
	{
		return H264VideoRTPSink::createNew(envir(), rtpGroupsock,rtpPayloadTypeIfDynamic);
	}

}
